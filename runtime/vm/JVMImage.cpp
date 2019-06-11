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
	_heap(NULL),
	_currentImageSize(0),
	_isImageAllocated(false),
	_dumpFileName(NULL)
{
	PORT_ACCESS_FROM_JAVAVM(javaVM);
	_portLibrary = (OMRPortLibrary *) j9mem_allocate_memory(sizeof(OMRPortLibrary), OMRMEM_CATEGORY_PORT_LIBRARY);

	OMRPORT_ACCESS_FROM_J9PORT(javaVM->portLibrary);
	memset(_portLibrary, 0, sizeof(OMRPortLibrary));
	memcpy(_portLibrary, privateOmrPortLibrary, sizeof(OMRPortLibrary));
	_portLibrary->mem_allocate_memory = image_mem_allocate_memory;
	_portLibrary->mem_free_memory = image_mem_free_memory;
}

JVMImage::~JVMImage()
{
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

void
JVMImage::allocateImageMemory(J9JavaVM *vm, UDATA size)
{
	PORT_ACCESS_FROM_JAVAVM(vm);

	J9Heap *allocPtr = (J9Heap*)j9mem_allocate_memory(size, J9MEM_CATEGORY_CLASSES);
	if (allocPtr == NULL) {
		// Memory allocation failed
		return;
	}

	_heap = j9heap_create(allocPtr, size, 0);
	if (_heap == NULL) {
		// Heap creation failed
		j9mem_free_memory((void *) allocPtr);
		return;
	}

	_currentImageSize = size;
	_isImageAllocated = true;
	initializeMonitor();
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

void
JVMImage::readImageFromFile()
{
	OMRPORT_ACCESS_FROM_OMRPORT(getPortLibrary());

	// TODO: The size will need to be very large or dynamically based on whatever is written to the dump
	char imageBuffer[JVMImage::INITIAL_IMAGE_SIZE];
	memset(imageBuffer, 0, sizeof(imageBuffer));

	intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenRead, 0444);
	if (fileDescriptor == -1) {
		// Failure to open file
	}

	intptr_t bytesRead = omrfile_read(fileDescriptor, imageBuffer, sizeof(imageBuffer));
	if (bytesRead == -1) {
		// Failure to read the image
	}

	if (omrfile_close(fileDescriptor) != 0) {
		// Failure to close
	}

	// TODO: Finally load the JVMImage using the data we read from the image
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

extern "C" void
create_and_allocate_jvm_image(J9JavaVM *vm)
{
	JVMImage *jvmImage = JVMImage::createInstance(vm);
	jvmImage->allocateImageMemory(vm, JVMImage::INITIAL_IMAGE_SIZE);
}

extern "C" void *
image_mem_allocate_memory(struct OMRPortLibrary *portLibrary, uintptr_t byteAmount, const char *callSite, uint32_t category)
{
	void *pointer = NULL;
	
	JVMImage *jvmImage = JVMImage::getInstance();
	pointer = jvmImage->subAllocateMemory(byteAmount);
	return pointer;
}

extern "C" void *
mem_allocate_memory(uintptr_t byteAmount)
{
    JVMImage *jvmImage = JVMImage::getInstance();
    return image_mem_allocate_memory(jvmImage->getPortLibrary(), byteAmount, J9_GET_CALLSITE(), J9MEM_CATEGORY_CLASSES);
}

extern "C" void
image_mem_free_memory(struct OMRPortLibrary *portLibrary, void *memoryPointer)
{
	JVMImage *jvmImage = JVMImage::getInstance();
	jvmImage->freeSubAllocatedMemory(memoryPointer);
}

extern "C" void
mem_free_memory(void *memoryPointer)
{
    JVMImage *jvmImage = JVMImage::getInstance();
    image_mem_free_memory(jvmImage->getPortLibrary(), memoryPointer);
}

extern "C" UDATA
addClassPathEntries(J9JavaVM *vm, const char *filename)
{
	J9ClassLoader *classLoader = vm->systemClassLoader;
	UDATA newCount = 0;
	U_32 entryIndex = 0;
	J9ClassPathEntry *newEntries = NULL;

	UDATA jarPathSize = strlen(filename);
	J9ClassPathEntry *oldEntries = classLoader->classPathEntries;
	UDATA entryCount = classLoader->classPathEntryCount;
    
	UDATA classPathLength = jarPathSize + 1; /* add space for a terminating null character */
	for (entryIndex = 0; entryIndex < entryCount; ++entryIndex) {
		classPathLength += oldEntries[entryIndex].pathLength + 1;	/* add 1 for a null character */
	}

	newEntries = (J9ClassPathEntry*) mem_allocate_memory(sizeof(J9ClassPathEntry) * (entryCount + 1) + classPathLength);
	if (NULL != newEntries) {
		J9ClassPathEntry *cpEntry = &newEntries[entryCount];
		U_8 *stringCursor = (U_8 *)(cpEntry + 1);
		memcpy(newEntries, oldEntries, sizeof(J9ClassPathEntry) * entryCount);
		/* copy the old entries */
		for (entryIndex = 0; entryIndex < entryCount; ++entryIndex) {
			memcpy(stringCursor, newEntries[entryIndex].path, newEntries[entryIndex].pathLength);
			newEntries[entryIndex].path = stringCursor;
			newEntries[entryIndex].path[newEntries[entryIndex].pathLength] = 0;	/* null character terminated */
			stringCursor += newEntries[entryIndex].pathLength + 1;
		}
		/* Create the new entry */
		memcpy(stringCursor, filename, jarPathSize);
		cpEntry->pathLength = (U_32) jarPathSize;
		cpEntry->path = stringCursor;
		cpEntry->path[cpEntry->pathLength] = 0;	/* null character terminated */
		cpEntry->extraInfo = NULL;
		cpEntry->type = CPE_TYPE_UNKNOWN;
		cpEntry->flags = CPE_FLAG_BOOTSTRAP;

#if defined(J9VM_OPT_SHARED_CLASSES)
		if (J9_ARE_ALL_BITS_SET(classLoader->flags, J9CLASSLOADER_SHARED_CLASSES_ENABLED)) {
			/* 
			 * Warm up the classpath entry so that the Classpath stored in the cache has the correct info.
			 * This is required because when we are finding classes in the cache, initializeClassPathEntry is not called 
			 * */
			if (vm->internalVMFunctions->initializeClassPathEntry(vm, cpEntry) != CPE_TYPE_JAR) {
				goto done;
			}
		}
#endif
		/* Everything OK, install the new array and discard the old one */
		TRIGGER_J9HOOK_VM_CLASS_LOADER_CLASSPATH_ENTRY_ADDED(vm->hookInterface, vm, classLoader, cpEntry);
		newCount = entryCount + 1;
		classLoader->classPathEntries = newEntries;
		classLoader->classPathEntryCount = newCount;
		mem_free_memory(oldEntries);
	}
done:
	/* If any error occurred, discard any allocated memory and throw OutOfMemoryError */
	if (0 == newCount) {
		mem_free_memory(newEntries);
	}
	return newCount;
}
