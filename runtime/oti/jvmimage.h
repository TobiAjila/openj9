/*******************************************************************************
 * Copyright (c) 1991, 2019 IBM Corp. and others
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

#ifndef jvmimage_h
#define jvmimage_h

/* Feature status macros */
#define IS_WARM_RUN(javaVM) J9_ARE_ALL_BITS_SET(javaVM->extendedRuntimeFlags2, J9_EXTENDED_RUNTIME2_RAMSTATE_WARM_RUN)
#define IS_COLD_RUN(javaVM) J9_ARE_ALL_BITS_SET(javaVM->extendedRuntimeFlags2, J9_EXTENDED_RUNTIME2_RAMSTATE_COLD_RUN)
#define IS_RAM_CACHE_ON(javaVM) J9_ARE_ANY_BITS_SET(javaVM->extendedRuntimeFlags2, J9_EXTENDED_RUNTIME2_RAMSTATE_COLD_RUN | J9_EXTENDED_RUNTIME2_RAMSTATE_WARM_RUN)

/* Page size allocation macros needed for MMAP MAP_FIXED to work. see @ref JVMImage::readImageFromFile */
/* TODO: remove once MMAP is not used anymore (random memory loading) */
#define PAGE_SIZE 4096 /* PAGE Size only defined for certain architectures (4KB default) */
#define PAGE_SIZE_MASK (PAGE_SIZE - 1)
#define PAGE_SIZE_ALIGNED_ADDRESS(address) ((void *) (((uintptr_t) (address) + PAGE_SIZE_MASK) & (~PAGE_SIZE_MASK)))

/* Class loader categories */
/* TODO: Categories will change when a better identifier for classloaders is used (also would allow user defined classloaders) */
#define IMAGE_CATEGORY_SYSTEM_CLASSLOADER 0
#define IMAGE_CATEGORY_APP_CLASSLOADER 1
#define IMAGE_CATEGORY_EXTENSION_CLASSLOADER 2

#define IS_SYSTEM_CLASSLOADER_CATEGORY(_category) (IMAGE_CATEGORY_SYSTEM_CLASSLOADER == _category)
#define IS_APP_CLASSLOADER_CATEGORY(_category) (IMAGE_CATEGORY_APP_CLASSLOADER == _category)
#define IS_EXTENSION_CLASSLOADER_CATEGORY(_category) (IMAGE_CATEGORY_EXTENSION_CLASSLOADER == _category)

/* Initial sizes for table based on estimations */
/* TODO: Find exactly number of classes/CPEntries for startup runs */
#define INITIAL_CLASSLOADER_TABLE_SIZE 3
#define INITIAL_CLASS_TABLE_SIZE 70
#define INITIAL_CLASSPATH_TABLE_SIZE 4

enum ImageRC {
	IMAGE_OK = BCT_ERR_NO_ERROR,
	IMAGE_ERROR = BCT_ERR_GENERIC_ERROR,
	Image_EnsureWideEnum = 0x1000000 /* force 4-byte enum */
};

/* forward struct declarations */
struct ImageTableHeader;
struct JVMImageHeader;
struct JVMImageSizeAndLocation;

/*
 * Struct containing data about image heap and quick access variables
 *
 * Allows us to dump this struct into file and reload easier
 * Allocated space for image data is longer than sizeof(JVMImageHeader) and includes heap. see @ref JVMImage::allocateImageMemory
 * All quick access variables are J9WSRP to allow reallocation in future. TODO: Allow heap reallocation 
 */
typedef struct JVMImageHeader {
	UDATA imageSize; /* image size in bytes */
	uintptr_t imageAddress;
	uintptr_t imageAlignedAddress; /* TODO: Will be removed once PAGE alignment is not needed anymore */
	/* TODO: only three main class loaders stored for prototype and quick access. Need to allow user defined classloaders */
	J9WSRP systemClassLoader;
	J9WSRP appClassLoader;
	J9WSRP extensionClassLoader;
	J9WSRP classLoaderTable;
	J9WSRP classTable;
	J9WSRP classPathEntryTable;
	J9WSRP cInitialStaticMethod;
	J9WSRP cInitialSpecialMethod;
	J9WSRP cInitialVirtualMethod;
} JVMImageHeader;

/*
 * Struct containing first three elements of JVMImageHeader
 * 
 * Used for initial read from file. see @ref JVMImage::readImageFromFile
 */
typedef struct JVMImageSizeAndLocation {
	UDATA imageSize;
	uintptr_t imageAddress;
	uintptr_t imageAlignedAddress;
} JVMImageSizeAndLocation;

/* Table structure to walk thorugh registered entries for J9Class, J9ClassLoader, and CPEntries
 *
 * Needed for fixup during teardown sequence
 */
typedef struct ImageTableHeader {
	J9WSRP tableHead;
	J9WSRP tableTail; /* tail optimization for O(1) append */
	UDATA *iteratorPtr; /* pointer to implement iterator pattern for table */
	UDATA tableSize;
	UDATA currentSize; /* Number of registered entries */
} ImageTableHeader;

#endif
