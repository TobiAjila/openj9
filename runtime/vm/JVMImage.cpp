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

#include <sys/mman.h>

/* TODO: reallocation will fail so initial heap size is large (Should be PAGE_SIZE aligned) */
/* TODO: currently only works because JVMImageHeader is 8 byte aligned */
const UDATA JVMImage::INITIAL_IMAGE_SIZE = 100 * 1024 * 1024;


JVMImage::JVMImage(J9JavaVM *javaVM) :
	_vm(javaVM),
	_jvmImageHeader(NULL),
	_heap(NULL)
{
	_dumpFileName = javaVM->ramStateFilePath;
}

JVMImage::~JVMImage()
{
	PORT_ACCESS_FROM_JAVAVM(_vm);

	j9mem_free_memory((void *)_jvmImageHeader->imageAddress);
}

bool
JVMImage::initializeMonitor(void)
{
	if (omrthread_monitor_init_with_name(&_jvmImageMonitor, 0, "JVM Image Heap Monitor") != 0) {
		return false;
	}

	return true;
}

void
JVMImage::destroyMonitor(void)
{
	omrthread_monitor_destroy(_jvmImageMonitor);
}

JVMImage *
JVMImage::createInstance(J9JavaVM *vm)
{
	PORT_ACCESS_FROM_JAVAVM(vm);

	JVMImage *jvmInstance = (JVMImage *)j9mem_allocate_memory(sizeof(JVMImage), J9MEM_CATEGORY_CLASSES);
	if (NULL != jvmInstance) {
		new(jvmInstance) JVMImage(vm);
	}

	return jvmInstance;
}

/* TODO: This will be expanded once warm run set up is finished */
ImageRC
JVMImage::setupWarmRun(void)
{
	if (!readImageFromFile()) {
		return IMAGE_ERROR;
	}

	return IMAGE_OK;
}

ImageRC
JVMImage::setupColdRun(void)
{
	if (NULL == allocateImageMemory(INITIAL_IMAGE_SIZE)) {
		return IMAGE_ERROR;
	}

	if (NULL == initializeHeap()) {
		return IMAGE_ERROR;
	}

	if (!initializeMonitor()) {
		return IMAGE_ERROR;
	}

	if (!allocateImageTableHeaders()) {
		return IMAGE_ERROR;
	}

	if ((NULL == allocateTable(getClassLoaderTable(), INITIAL_CLASSLOADER_TABLE_SIZE))
		|| (NULL == allocateTable(getClassTable(), INITIAL_CLASS_TABLE_SIZE))
		|| (NULL == allocateTable(getClassPathEntryTable(), INITIAL_CLASSPATH_TABLE_SIZE))) {
		return IMAGE_ERROR;
	}

	_invalidTable = (J9ITable *) subAllocateMemory(sizeof(J9ITable));
	if (NULL == _invalidTable) {
		return IMAGE_ERROR;
	}

	return IMAGE_OK;
}

void *
JVMImage::allocateImageMemory(UDATA size)
{
	PORT_ACCESS_FROM_JAVAVM(_vm);

	void *imageAddress = j9mem_allocate_memory(size + PAGE_SIZE, J9MEM_CATEGORY_CLASSES); //TODO: change category
	if (NULL == imageAddress) {
		return NULL;
	}

	_jvmImageHeader = (JVMImageHeader *) PAGE_SIZE_ALIGNED_ADDRESS(imageAddress);
	_jvmImageHeader->imageAddress = (uintptr_t)imageAddress;
	_jvmImageHeader->imageAlignedAddress = (uintptr_t)_jvmImageHeader;
	_jvmImageHeader->imageSize = size;

	return _jvmImageHeader;
}

/* TODO: Currently reallocating image memory is broken since all references to memory inside of heap will fail (i.e. vm->classLoadingPool) */
void *
JVMImage::reallocateImageMemory(UDATA size)
{
	return NULL;
}

void * 
JVMImage::initializeHeap(void)
{
	PORT_ACCESS_FROM_JAVAVM(_vm);
	
	_heap = j9heap_create((J9Heap *)(_jvmImageHeader + 1), JVMImage::INITIAL_IMAGE_SIZE - sizeof(_jvmImageHeader), 0);
	if (NULL == _heap) {
		return NULL;
	}

	return _heap;
}

bool
JVMImage::allocateImageTableHeaders(void)
{
	WSRP_SET(_jvmImageHeader->classLoaderTable, subAllocateMemory(sizeof(ImageTableHeader)));
	WSRP_SET(_jvmImageHeader->classTable, subAllocateMemory(sizeof(ImageTableHeader)));
	WSRP_SET(_jvmImageHeader->classPathEntryTable, subAllocateMemory(sizeof(ImageTableHeader)));

	if ((0 == _jvmImageHeader->classLoaderTable)
		|| (0 == _jvmImageHeader->classTable)
		|| (0 == _jvmImageHeader->classPathEntryTable)) {
		return false;
	}

	return true;
}

void * 
JVMImage::allocateTable(ImageTableHeader *table, uintptr_t tableSize)
{
	void *firstSlot = subAllocateMemory(tableSize * sizeof(UDATA));
	if (NULL == firstSlot) {
		return NULL;
	}

	table->tableSize = tableSize;
	table->currentSize = 0;
	WSRP_SET(table->tableHead, firstSlot);
	WSRP_SET(table->tableTail, firstSlot);
	
	return firstSlot;
}

void *
JVMImage::reallocateTable(ImageTableHeader *table, uintptr_t tableSize)
{
	UDATA *firstSlot = WSRP_GET(table->tableHead, UDATA*);
	firstSlot = (UDATA *)reallocateMemory((void *)firstSlot, tableSize * sizeof(UDATA));
	if (NULL == firstSlot) {
		return NULL;
	}

	table->tableSize = tableSize;
	WSRP_SET(table->tableHead, firstSlot);
	WSRP_SET(table->tableTail, firstSlot + table->currentSize - 1);

	return firstSlot;
}

void *
JVMImage::subAllocateMemory(uintptr_t byteAmount)
{
	Trc_VM_SubAllocateImageMemory_Entry(_jvmImageHeader, byteAmount);

	omrthread_monitor_enter(_jvmImageMonitor);

	OMRPortLibrary *portLibrary = IMAGE_OMRPORT_FROM_JAVAVM(_vm);
	void *memStart = portLibrary->heap_allocate(portLibrary, _heap, byteAmount);	
	/* image memory is not large enough and needs to be reallocated */
	if (NULL == memStart) {
		reallocateImageMemory(_jvmImageHeader->imageSize * 2 + byteAmount);
		memStart = portLibrary->heap_allocate(portLibrary, _heap, byteAmount);
	}

	omrthread_monitor_exit(_jvmImageMonitor);

	Trc_VM_SubAllocateImageMemory_Exit(memStart);

	return memStart;
}

void*
JVMImage::reallocateMemory(void *address, uintptr_t byteAmount)
{
	omrthread_monitor_enter(_jvmImageMonitor);

	OMRPortLibrary *portLibrary = IMAGE_OMRPORT_FROM_JAVAVM(_vm);
	void *memStart = portLibrary->heap_reallocate(portLibrary, _heap, address, byteAmount);
	/* image memory is not large enough and needs to be reallocated */
	if (NULL == memStart) {
		reallocateImageMemory(_jvmImageHeader->imageSize * 2 + byteAmount);
		memStart = portLibrary->heap_reallocate(portLibrary, _heap, address, byteAmount);
	}

	omrthread_monitor_exit(_jvmImageMonitor);

	return memStart;
}

void
JVMImage::freeSubAllocatedMemory(void *address)
{
	omrthread_monitor_enter(_jvmImageMonitor);

	OMRPortLibrary *portLibrary = IMAGE_OMRPORT_FROM_JAVAVM(_vm);
	portLibrary->heap_free(portLibrary, _heap, address);

	omrthread_monitor_exit(_jvmImageMonitor);
}

void
JVMImage::registerEntryInTable(ImageTableHeader *table, UDATA entry)
{
	Trc_VM_RegisterInTable_Entry(table, table->currentSize, *(WSRP_GET(table->tableTail, UDATA*)), entry);

	/* table is not large enough and needs to be reallocated */
	if (table->currentSize >= table->tableSize) {
		reallocateTable(table, table->tableSize * 2); /*TODO: Introduce error handling for table reallocation */
	}

	UDATA *tail = WSRP_GET(table->tableTail, UDATA*);

	/* initial state of every table */
	if (0 != table->currentSize) {
		tail += 1;
		WSRP_SET(table->tableTail, tail);
	}

	*(tail) = entry;
	table->currentSize += 1;

	Trc_VM_RegisterInTable_Exit(table, table->currentSize, *(WSRP_GET(table->tableTail, UDATA*)));
}

void
JVMImage::deregisterEntryInTable(ImageTableHeader *table, UDATA entry)
{
	Trc_VM_DeregisterInTable_Entry(table, table->currentSize, *(WSRP_GET(table->tableTail, UDATA*)), entry);

	UDATA *tail = WSRP_GET(table->tableTail, UDATA*);
	UDATA *temp = findEntryLocationInTable(table, entry);

	if (NULL != temp) {
		while (temp != tail) {
			*temp = *(temp + 1);
			temp += 1;
		}
		*temp = 0;

		/*tail should point to first element when empty*/
		if (table->currentSize != 1) {
			tail -= 1;
		}
		table->currentSize -= 1;
	}
	
	WSRP_SET(table->tableTail, tail);

	Trc_VM_DeregisterInTable_Exit(table, table->currentSize, *(WSRP_GET(table->tableTail, UDATA*)));
}

void
JVMImage::setClassLoader(J9ClassLoader *classLoader, uint32_t classLoaderCategory)
{
	/* TODO: Function will change when hash table is created and there would be no need for setting specific class loaders */
	if (IS_SYSTEM_CLASSLOADER_CATEGORY(classLoaderCategory)) {
		WSRP_SET(_jvmImageHeader->systemClassLoader, classLoader);
	}

	if (IS_APP_CLASSLOADER_CATEGORY(classLoaderCategory)) {
		WSRP_SET(_jvmImageHeader->appClassLoader, classLoader);
	}

	if (IS_EXTENSION_CLASSLOADER_CATEGORY(classLoaderCategory)) {
		WSRP_SET(_jvmImageHeader->extensionClassLoader, classLoader);
	}
}

void
JVMImage::fixupClassLoaders(void)
{
	J9ClassLoader *currentClassLoader = (J9ClassLoader *) imageTableStartDo(getClassLoaderTable());
	
	while (NULL != currentClassLoader) {
		currentClassLoader->sharedLibraries = NULL;
		currentClassLoader->classLoaderObject = NULL;
		currentClassLoader->unloadLink = NULL;
		currentClassLoader->gcLinkNext = NULL;
		currentClassLoader->gcLinkPrevious = NULL;
		currentClassLoader->gcFlags = 0;
		currentClassLoader->gcRememberedSet = 0;
		currentClassLoader->gcThreadNotification = NULL;
		currentClassLoader->jniIDs = NULL;
		currentClassLoader->jniRedirectionBlocks = NULL;
		currentClassLoader->gcRememberedSet = 0;

		currentClassLoader = (J9ClassLoader *) imageTableNextDo(getClassLoaderTable());
	}
}

void
JVMImage::fixupClasses(void)
{
	J9Class *currentClass = (J9Class *) imageTableStartDo(getClassTable());

	while (NULL != currentClass) {
		currentClass->classObject = NULL;
		currentClass->initializeStatus = J9ClassInitNotInitialized;
		internalRunPreInitInstructions(currentClass, _vm->mainThread);
		currentClass->jniIDs = NULL;
		currentClass->replacedClass = NULL;
		currentClass->callSites = NULL;
		currentClass->methodTypes = NULL;
		currentClass->varHandleMethodTypes = NULL;
		currentClass->gcLink = NULL;

		/* Fixup the last ITable */
		currentClass->lastITable = (J9ITable *) currentClass->iTable;
		if (NULL == currentClass->lastITable) {
			currentClass->lastITable = JVMImage::getInvalidTable();
		}

		currentClass = (J9Class *) imageTableNextDo(getClassTable());
	}
}

void
JVMImage::fixupClassPathEntries(void)
{
	J9ClassPathEntry *currentCPEntry = (J9ClassPathEntry *) imageTableStartDo(getClassPathEntryTable());

	while (NULL != currentCPEntry) {
		currentCPEntry->type = CPE_TYPE_UNKNOWN;
		currentCPEntry->status = 0;

		currentCPEntry = (J9ClassPathEntry *) imageTableNextDo(getClassPathEntryTable());
	}
}

bool
JVMImage::readImageFromFile(void)
{
	Trc_VM_ReadImageFromFile_Entry(_heap, _dumpFileName);

	OMRPortLibrary *portLibrary = IMAGE_OMRPORT_FROM_JAVAVM(_vm);
	OMRPORT_ACCESS_FROM_OMRPORT(portLibrary);

	intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenRead | EsOpenWrite, 0444);
	if (-1 == fileDescriptor) {
		return false;
	}

	/* Read image header then mmap the rest of the image (heap) into memory */
	/* TODO: Should only read imageAddress and size because that is the only data we need for mmap */
	JVMImageHeader imageHeaderBuffer;
	omrfile_read(fileDescriptor, (void *)&imageHeaderBuffer, sizeof(JVMImageHeader));
	uint64_t fileSize = omrfile_flength(fileDescriptor);
	if (imageHeaderBuffer.imageSize != fileSize) {
		return false;
	}

	_jvmImageHeader = (JVMImageHeader *)mmap(
		(void *)imageHeaderBuffer.imageAlignedAddress,
		imageHeaderBuffer.imageSize,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fileDescriptor, 0);
	_heap = (J9Heap *)(_jvmImageHeader + 1);

	omrfile_close(fileDescriptor);

	Trc_VM_ReadImageFromFile_Exit();

	return true;
}

bool
JVMImage::writeImageToFile(void)
{
	Trc_VM_WriteImageToFile_Entry(_heap, _dumpFileName);

	OMRPortLibrary *portLibrary = IMAGE_OMRPORT_FROM_JAVAVM(_vm);
	OMRPORT_ACCESS_FROM_OMRPORT(portLibrary);

	intptr_t fileDescriptor = omrfile_open(_dumpFileName, EsOpenCreate | EsOpenCreateAlways | EsOpenWrite, 0666);
	if (-1 == fileDescriptor) {
		return false;
	}

	/* Write header followed by the heap */
	if ((intptr_t)_jvmImageHeader->imageSize != omrfile_write(fileDescriptor, (void *)_jvmImageHeader, _jvmImageHeader->imageSize)) {
		return false;
	}

	omrfile_close(fileDescriptor);

	Trc_VM_WriteImageToFile_Exit();

	return true;
}

void
JVMImage::teardownImage(void)
{
	fixupClassLoaders();
	fixupClasses();
	fixupClassPathEntries();
	writeImageToFile();
}

JVMImagePortLibrary * 
setupJVMImagePortLibrary(J9JavaVM *javaVM)
{
	PORT_ACCESS_FROM_JAVAVM(javaVM);
	JVMImagePortLibrary *jvmImagePortLibrary = (JVMImagePortLibrary*)j9mem_allocate_memory(sizeof(JVMImagePortLibrary), OMRMEM_CATEGORY_PORT_LIBRARY);

	OMRPORT_ACCESS_FROM_J9PORT(javaVM->portLibrary);
	memcpy(&(jvmImagePortLibrary->portLibrary), privateOmrPortLibrary, sizeof(OMRPortLibrary));
	jvmImagePortLibrary->portLibrary.mem_allocate_memory = image_mem_allocate_memory;
	jvmImagePortLibrary->portLibrary.mem_free_memory = image_mem_free_memory;
	jvmImagePortLibrary->jvmImage = NULL;

	return jvmImagePortLibrary;
}

extern "C" UDATA
initializeJVMImage(J9JavaVM *javaVM)
{
	JVMImagePortLibrary *jvmImagePortLibrary = setupJVMImagePortLibrary(javaVM);
	JVMImage *jvmImage = JVMImage::createInstance(javaVM);
	if (NULL == jvmImagePortLibrary || NULL == jvmImage) {
		goto _error;
	}
	javaVM->jvmImagePortLibrary = jvmImagePortLibrary;
	jvmImagePortLibrary->jvmImage = jvmImage;

	if (IS_COLD_RUN(javaVM)
		&& (IMAGE_ERROR == jvmImage->setupColdRun())) {
		goto _error;
	}

	if(IS_WARM_RUN(javaVM)
		&& (IMAGE_ERROR == jvmImage->setupWarmRun())) {
		goto _error;
	}

	return 1;

_error:
	shutdownJVMImage(javaVM);
	jvmImage->destroyMonitor();
	return 0;
}

extern "C" void
registerClassLoader(J9JavaVM *javaVM, J9ClassLoader *classLoader, uint32_t classLoaderCategory)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	Assert_VM_notNull(jvmImage);

	jvmImage->registerEntryInTable(jvmImage->getClassLoaderTable(), (UDATA)classLoader);
	/* TODO: Currently only three class loaders are stored */
	jvmImage->setClassLoader(classLoader, classLoaderCategory);
}

extern "C" void
registerClass(J9JavaVM *javaVM, J9Class *clazz)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	Assert_VM_notNull(jvmImage);

	jvmImage->registerEntryInTable(jvmImage->getClassTable(), (UDATA)clazz);
}

extern "C" void
registerCPEntry(J9JavaVM *javaVM, J9ClassPathEntry *cpEntry)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	Assert_VM_notNull(jvmImage);
	
	jvmImage->registerEntryInTable(jvmImage->getClassPathEntryTable(), (UDATA)cpEntry);
}

extern "C" void
deregisterClass(J9JavaVM *javaVM, J9Class *clazz)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	Assert_VM_notNull(jvmImage);

	jvmImage->deregisterEntryInTable(jvmImage->getClassTable(), (UDATA)clazz);
}

extern "C" void
deregisterCPEntry(J9JavaVM *javaVM, J9ClassPathEntry *cpEntry)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	Assert_VM_notNull(jvmImage);

	jvmImage->deregisterEntryInTable(jvmImage->getClassPathEntryTable(), (UDATA)cpEntry);
}

extern "C" J9ClassLoader *
findClassLoader(J9JavaVM *javaVM, uint32_t classLoaderCategory)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	/* TODO: Function will change when hash table is created and there would be no need for accessing specific class loaders (hardcoded) */
	if (IS_SYSTEM_CLASSLOADER_CATEGORY(classLoaderCategory)) {
		return jvmImage->getSystemClassLoader();
	}

	if (IS_APP_CLASSLOADER_CATEGORY(classLoaderCategory)) {
		return jvmImage->getApplicationClassLoader();
	}

	if (IS_EXTENSION_CLASSLOADER_CATEGORY(classLoaderCategory)) {
		return jvmImage->getExtensionClassLoader();
	}

	/* find should never get here */
	return NULL;
}

extern "C" void
shutdownJVMImage(J9JavaVM *javaVM)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	if (NULL != jvmImage) {
		PORT_ACCESS_FROM_JAVAVM(javaVM);

		jvmImage->~JVMImage();
		j9mem_free_memory(jvmImage);
		j9mem_free_memory(JVMIMAGEPORT_FROM_JAVAVM(javaVM));
		javaVM->jvmImagePortLibrary = NULL;
	}
}

extern "C" void
teardownAndShutdownJVMImage(J9JavaVM *javaVM)
{
	IMAGE_ACCESS_FROM_JAVAVM(javaVM);

	if (IS_COLD_RUN(javaVM)) {
		jvmImage->teardownImage();
	}

	shutdownJVMImage(javaVM);
}

extern "C" void *
image_mem_allocate_memory(struct OMRPortLibrary *portLibrary, uintptr_t byteAmount, const char *callSite, uint32_t category)
{
	IMAGE_ACCESS_FROM_PORT(portLibrary);

	void *pointer = jvmImage->subAllocateMemory(byteAmount);
	return pointer;
}

extern "C" void
image_mem_free_memory(struct OMRPortLibrary *portLibrary, void *memoryPointer)
{
	IMAGE_ACCESS_FROM_PORT(portLibrary);

	jvmImage->freeSubAllocatedMemory(memoryPointer);
}
