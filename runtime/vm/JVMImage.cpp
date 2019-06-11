/*******************************************************************************
 * Copyright (c) 2001, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "JVMImage.hpp"

JVMImage *JVMImage::_jvmInstance = NULL;
const UDATA JVMImage::INITIAL_IMAGE_SIZE = 1024;

JVMImage::JVMImage(J9JavaVM *javaVM) :
	_vm(javaVM),
	_heapBase(NULL),
    _heap(NULL),
	_currentImageSize(0),
	_isImageAllocated(false)
{
	PORT_ACCESS_FROM_JAVAVM(javaVM);
	_portLibrary = (OMRPortLibrary *) j9mem_allocate_memory(sizeof(OMRPortLibrary), OMRMEM_CATEGORY_PORT_LIBRARY);

	OMRPORT_ACCESS_FROM_J9PORT(javaVM->portLibrary);
	memset(_portLibrary, 0, sizeof(OMRPortLibrary));
	memcpy(_portLibrary, privateOmrPortLibrary, sizeof(OMRPortLibrary));
	_portLibrary->mem_allocate_memory = image_mem_allocate_memory;
	_portLibrary->mem_free_memory = image_mem_free_memory;

	_dumpFileName = javaVM->ramStateFilePath;
	_isWarmRun = J9_ARE_ALL_BITS_SET(javaVM->extendedRuntimeFlags2, J9_EXTENDED_RUNTIME2_RAMSTATE_WARM_RUN);
}

JVMImage::~JVMImage()
{
	PORT_ACCESS_FROM_JAVAVM(_vm);

	j9mem_free_memory((void*)_heapBase);
	j9mem_free_memory((void*)_portLibrary);
	_jvmInstance = NULL;
}

bool
JVMImage::initializeMonitor()
{
	if (omrthread_monitor_init_with_name(&_jvmImageMonitor, 0, "JVM Image Heap Monitor") != 0) {
		return false;
	}
	return true;
}

void
JVMImage::destroyMonitor()
{
	omrthread_monitor_destroy(_jvmImageMonitor);
}

JVMImage *
JVMImage::createInstance(J9JavaVM *vm)
{
	PORT_ACCESS_FROM_JAVAVM(vm);

	_jvmInstance = (JVMImage *)j9mem_allocate_memory(sizeof(JVMImage), J9MEM_CATEGORY_CLASSES);
	if (_jvmInstance != NULL) {
		new(_jvmInstance) JVMImage(vm);   
	}

	return _jvmInstance;
}

JVMImage *
JVMImage::getInstance()
{
	return _jvmInstance;
}

void * 
JVMImage::allocateImageMemory(UDATA size)
{
	PORT_ACCESS_FROM_JAVAVM(_vm);

	_heapBase = j9mem_allocate_memory(size, J9MEM_CATEGORY_CLASSES);
	if (_heapBase == NULL) {
		return NULL;
	}

	_heap = j9heap_create((J9Heap*)_heapBase, size, 0);
	if (_heap == NULL) {
		return NULL;
	}

	_currentImageSize = size;
	_isImageAllocated = true;

	return _heap;
}

void
JVMImage::reallocateImageMemory(UDATA size)
{
	return;
}

void *
JVMImage::subAllocateMemory(uintptr_t byteAmount)
{
	omrthread_monitor_enter(_jvmImageMonitor);

	void *memStart = _portLibrary->heap_allocate(_portLibrary, _heap, byteAmount);	
	// image memory is not large enough and needs to be reallocated
	if (memStart == NULL) {
		reallocateImageMemory(_currentImageSize * 2 + byteAmount);
		memStart = _portLibrary->heap_allocate(_portLibrary, _heap, byteAmount);
	}

	omrthread_monitor_exit(_jvmImageMonitor);

	return memStart;
}

void
JVMImage::freeSubAllocatedMemory(void* address)
{
	omrthread_monitor_enter(_jvmImageMonitor);

	_portLibrary->heap_free(_portLibrary, _heap, address);

	omrthread_monitor_exit(_jvmImageMonitor);
}

bool
JVMImage::readImageFromFile()
{
	OMRPORT_ACCESS_FROM_OMRPORT(getPortLibrary());

	// TODO: The size will need to be very large or dynamically based on whatever is written to the dump
	// TODO: Fetch the initial size of the jvm image
    char imageBuffer[JVMImage::INITIAL_IMAGE_SIZE];
    memset(imageBuffer, 0, sizeof(imageBuffer));

	intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenRead, 0444);
	if (fileDescriptor == -1) {
		return false;
	}

	intptr_t bytesRead = omrfile_read(fileDescriptor, imageBuffer, sizeof(imageBuffer));
	if (bytesRead == -1) {
		return false;
	}

	if (omrfile_close(fileDescriptor) != 0) {
		return false;
	}

	// TODO: Finally load the JVMImage using the data we read from the image

	return true;
}

void
JVMImage::storeImageInFile()
{
	OMRPORT_ACCESS_FROM_OMRPORT(getPortLibrary());

	if (!_isImageAllocated) {
		// Nothing to dump
	}

	intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenCreate | EsOpenWrite | EsOpenTruncate, 0666);
	if (fileDescriptor == -1) {
		// Failure to open file
	}

	if (omrfile_write(fileDescriptor, (void*)_heap, _currentImageSize) != (intptr_t)_currentImageSize) {
		// Failure to write to the file
	}

	if (omrfile_close(fileDescriptor) != 0) {
		// Failure to close
	}
}

extern "C" UDATA
initializeJVMImage(J9JavaVM *vm)
{
    JVMImage *jvmImage = JVMImage::createInstance(vm);
	
	if (jvmImage->isWarmRun()
		&& jvmImage->allocateImageMemory(JVMImage::INITIAL_IMAGE_SIZE) == NULL
		&& jvmImage->readImageFromFile() == 0) {
		goto _error;
	}

	if (jvmImage->initializeMonitor() == 0) {
		goto _error;
	}

	return 1;

_error:
	shutdownJVMImage(vm);
	return 0;
}

extern "C" void
shutdownJVMImage(J9JavaVM *vm)
{
	JVMImage *jvmImage = JVMImage::getInstance();

	if (jvmImage != NULL) {
		PORT_ACCESS_FROM_JAVAVM(vm);

		jvmImage->destroyMonitor();
		jvmImage->~JVMImage();
		j9mem_free_memory(jvmImage);
	}
}

extern "C" void *
image_mem_allocate_memory(struct OMRPortLibrary* portLibrary, uintptr_t byteAmount, const char* callSite, uint32_t category)
{
	void *pointer = NULL;
	
	JVMImage *jvmImage = JVMImage::getInstance();
	pointer = jvmImage->subAllocateMemory(byteAmount);
	return pointer;
}

extern "C" void
image_mem_free_memory(struct OMRPortLibrary* portLibrary, void* memoryPointer)
{
	JVMImage *jvmImage = JVMImage::getInstance();
	jvmImage->freeSubAllocatedMemory(memoryPointer);
} 
