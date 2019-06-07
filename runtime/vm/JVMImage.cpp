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

#define OMRPORT_FROM_IMAGE JVMImage::getInstance()->getPortLibrary();

JVMImage *JVMImage::_jvmInstance = NULL;
const char *JVMImage::_dumpFileName = "jvm_image.data";
const UDATA JVMImage::_imageSize = 1024;

JVMImage::JVMImage(J9JavaVM *javaVM) :
    _heap(NULL),
	_size(0),
	_isImageAllocated(false)
{
	OMRPORT_ACCESS_FROM_J9PORT(javaVM->portLibrary);
	memset(&_portLibrary, 0, sizeof(OMRPortLibrary));
	memcpy(&_portLibrary, privateOmrPortLibrary, sizeof(OMRPortLibrary));
	_portLibrary.mem_allocate_memory = image_mem_allocate_memory;
	_portLibrary.mem_free_memory = image_mem_free_memory;
}

JVMImage::~JVMImage()
{
    delete _jvmInstance;
}

//should be called once the image memory is allocated
bool
JVMImage::initializeMonitor()
{
	if (omrthread_monitor_init_with_name(&_jvmImageMonitor, 0, "JVM Image Heap Monitor") != 0) {
		return false;
	}
	return true;
}

JVMImage *
JVMImage::createInstance(J9JavaVM *vm)
{
    if (_jvmInstance != NULL)
        _jvmInstance = new JVMImage(vm);

    return _jvmInstance;
}

JVMImage *
JVMImage::getInstance()
{
    return _jvmInstance;
}

void
JVMImage::allocateImageMemory(J9JavaVM *vm)
{
    PORT_ACCESS_FROM_JAVAVM(vm);
    
    J9Heap *allocPtr = (J9Heap*)j9mem_allocate_memory(_imageSize, J9MEM_CATEGORY_CLASSES);
    if (allocPtr == NULL) {
        // Memory allocation failed
        return;
    }

    _size = _imageSize;
    _isImageAllocated = true;

    _heap = j9heap_create(allocPtr, _size, 0);
    if (_heap == NULL) {
        // Heap creation failed
        return;
    }
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

	void *memStart = _portLibrary.heap_allocate(&_portLibrary, _heap, byteAmount);	
	// image memory is not large enough and needs to be reallocated
	if (memStart == NULL) {
		reallocateImageMemory(_size * 2 + byteAmount);
		memStart = _portLibrary.heap_allocate(&_portLibrary, _heap, byteAmount);
	}

	omrthread_monitor_exit(_jvmImageMonitor);

	return memStart;
}

void
JVMImage::freeSubAllocatedMemory(void* address)
{
	omrthread_monitor_enter(_jvmImageMonitor);

	_portLibrary.heap_free(&_portLibrary, _heap, address);

	omrthread_monitor_exit(_jvmImageMonitor);
}

void
JVMImage::readImageFromFile()
{
    OMRPORT_ACCESS_FROM_OMRPORT(getPortLibrary());

    char imageBuffer[_imageSize];
    memset(imageBuffer, 0, sizeof(imageBuffer));

    intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenRead, 0666);
    if (fileDescriptor == -1) {
        // Failure to open file
    }

    intptr_t bytesRead = omrfile_read(fileDescriptor, imageBuffer, sizeof(imageBuffer));
    if ((bytesRead == 0) || (bytesRead == -1)) {
        // Failure to read the image
    }

    if (omrfile_close(fileDescriptor) != 0) {
        // Failure to close
    }

    // Finally load the JVMImage using the data we read from the image
}

void
JVMImage::storeImageInFile()
{
    OMRPORT_ACCESS_FROM_OMRPORT(getPortLibrary());

    intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenCreate | EsOpenWrite | EsOpenTruncate, 0666);
    if (fileDescriptor == -1) {
        // Failure to open file
    }

    if (omrfile_write(fileDescriptor, (void*)_heap, _size) != (intptr_t)_size) {
        // Failure to write to the file
    }

    if (omrfile_close(fileDescriptor) != 0) {
        // Failure to close
    }
}

extern "C" void
create_and_allocate_jvm_image(J9JavaVM *vm)
{
    JVMImage *jvmImage = JVMImage::createInstance(vm);
    jvmImage->allocateImageMemory(vm);
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
