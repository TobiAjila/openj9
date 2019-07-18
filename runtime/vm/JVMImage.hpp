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
/*
* JVMImage.hpp
*/

#ifndef JVMIMAGE_HPP_
#define JVMIMAGE_HPP_

#include "j9.h"
#include "jvmimageport.h"
#include "j9comp.h"
#include "j9protos.h"
#include "ut_j9vm.h"
#include "objhelp.h"

class JVMImage
{
	/*
	 * Data Members
	 */
private:
	J9JavaVM *_vm;

	JVMImageHeader *_jvmImageHeader;
	J9Heap *_heap;
	J9ITable *_invalidTable;

	omrthread_monitor_t _jvmImageMonitor;

	char *_dumpFileName;
protected:
public:
	static const UDATA INITIAL_IMAGE_SIZE;
	
	/*
	 * Function Members
	 */
private:
	bool initializeMonitor(void);
	bool initializeInvalidITable(void);

	void* allocateImageMemory(UDATA size);
	void* reallocateImageMemory(UDATA size);
	void* initializeHeap(void);

	bool allocateImageTableHeaders(void);
	void* allocateTable(ImageTableHeader *table, uintptr_t tableSize);
	void* reallocateTable(ImageTableHeader *table, uintptr_t tableSize);

	bool readImageFromFile(void);
	bool writeImageToFile(void);

	void fixupClassLoaders(void);
	void fixupClasses(void);
	void fixupClass(J9Class *clazz);
	void fixupArrayClass(J9ArrayClass *clazz);
	void fixupMethodRunAddresses(J9Class *ramClass);
	void fixupConstantPool(J9Class *ramClass);
	void fixupClassPathEntries(void);
protected:
	void *operator new(size_t size, void *memoryPointer) { return memoryPointer; }
public:
	JVMImage(J9JavaVM *vm);
	~JVMImage();

	static JVMImage* createInstance(J9JavaVM *javaVM);

	ImageRC setupWarmRun(void);
	ImageRC setupColdRun(void);

	void teardownImage(void);

	void* subAllocateMemory(uintptr_t byteAmount);
	void* reallocateMemory(void *address, uintptr_t byteAmount); /* TODO: Extension functions for heap (not used currently) */
	void freeSubAllocatedMemory(void *memStart); /* TODO: Extension functions for heap (not used currently) */

	void registerEntryInTable(ImageTableHeader *table, UDATA entry);
	void deregisterEntryInTable(ImageTableHeader *table, UDATA entry);

	void destroyMonitor(void);

	/* TODO: currently the three major class loaders have their own accessors/mutators. This could be ported to hash table once user defined class works */
	void setClassLoader(J9ClassLoader *classLoader, uint32_t classLoaderCategory);
	J9ClassLoader* getSystemClassLoader(void) { return WSRP_GET(_jvmImageHeader->systemClassLoader, J9ClassLoader*); }
	J9ClassLoader* getApplicationClassLoader(void) { return WSRP_GET(_jvmImageHeader->appClassLoader, J9ClassLoader*); }
	J9ClassLoader* getExtensionClassLoader(void) { return WSRP_GET(_jvmImageHeader->extensionClassLoader, J9ClassLoader*); }
	
	ImageTableHeader* getClassLoaderTable(void) { return WSRP_GET(_jvmImageHeader->classLoaderTable, ImageTableHeader*); }
	ImageTableHeader* getClassTable(void) { return WSRP_GET(_jvmImageHeader->classTable, ImageTableHeader*); }
	ImageTableHeader* getClassPathEntryTable(void) { return WSRP_GET(_jvmImageHeader->classPathEntryTable, ImageTableHeader*); }

	J9ITable* getInvalidTable(void) { return _invalidTable; }
};

#endif /* JVMIMAGE_H_ */
