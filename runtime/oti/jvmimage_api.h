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

#ifndef jvmimage_api_h
#define jvmimage_api_h

#include "j9.h"
#include "j9comp.h"
#include "jvmimage.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Allocate memory in heap image
* 
* @param portLibrary[in] the default port library
* @param byteAmount[in] size to allocate
* @param callSite[in] location memory alloc is called from
* @param category[in] category of memory alloc
*
* @return pointer to allocated memory on success, NULL on failure 
*/
void* image_mem_allocate_memory(struct OMRPortLibrary *portLibrary, uintptr_t byteAmount, const char *callSite, uint32_t category);

/*
* Free memory in heap image
*
* @param portLibrary[in] the default port library
* @param memoryPointer[in] pointer to address for free
*/
void image_mem_free_memory(struct OMRPortLibrary *portLibrary, void *memoryPointer);

/*
* Creates and allocates the jvm image and its' heap
*
* @param javaVM[in] the java vm 
*
* @return 0 on fail, 1 on success
*/
UDATA initializeJVMImage(J9JavaVM *javaVM);

/*
* Registers class loader to one of the three defined class loaders
*
* @param javaVM[in] the java vm
* @param classLoader[in] J9ClassLoader to register
* @param classLoaderCategory[in] category to identify the type of classloader
*/
void registerClassLoader(J9JavaVM *javaVM, J9ClassLoader *classLoader, uint32_t classLoaderCategory);

/*
* Registers class in table
*
* @param javaVM[in] the java vm
* @param clazz[in] J9Class to register
*/
void registerClass(J9JavaVM *javaVM, J9Class *clazz);

/*
* Registers class path entry in table
*
* @param javaVM[in] the java vm
* @param cpEntry[in] J9ClassPathEntry to register
*/
void registerCPEntry(J9JavaVM *javaVM, J9ClassPathEntry *cpEntry);

/*
* Deregisters class from table
*
* @param javaVM[in] the java vm
* @param clazz[in] J9Class to register
*/
void deregisterClass(J9JavaVM *javaVM, J9Class *clazz);

/*
* Deregisters class path entry from table
*
* @param javaVM[in] the java vm
* @param cpEntry[in] J9ClassPathEntry to register
*/
void deregisterCPEntry(J9JavaVM *javaVM, J9ClassPathEntry *cpEntry);

/*
* Initializes for iterator pattern 
*
* @param table[in] the table to iterate
* 
* @return the starting element in the table. NULL if empty
*/
void* imageTableStartDo(ImageTableHeader *table);

/*
* Next element in table
*
* @param table[in] the table to iterate
*
* @return the next element in the table. NULL if no next element
*/
void* imageTableNextDo(ImageTableHeader *table);

/*
* Checks table for entry location
*
* @param table[in] the table to iterate
* @param entry[in] the entry to find
*
* @return location of entry in table. NULL if entry not in table
*/
UDATA* findEntryLocationInTable(ImageTableHeader *table, UDATA entry);

/*
* Finding class loader based on category
*
* @param javaVM[in] the java vm
* @param classLoaderCategory[in] the category of class loaders
*/
J9ClassLoader* findClassLoader(J9JavaVM *javaVM, uint32_t classLoaderCategory);

/*
* Shut down sequence of JVMImage
* Frees memory of heap variables and jvmimage instance
*
* @param javaVM[in] the java vm
*/
void shutdownJVMImage(J9JavaVM *vm);

/*
* Frees the memory associated with the JVMImage
* If it is a cold run it will perform fixup and write the image to file
*
* @param javaVM[in] the java vm
*/
void teardownJVMImage(J9JavaVM *javaVM);

/*
* Store the initial methods in jvmimageheader
*
* @param javaVM[in] the java vm
* @param cInitialStaticMethod[in] the initial static method
* @param cInitialSpecialMethod[in] the initial special method
* @param cInitialVirtualMethod[in] the initial virtual method
* @param cInvokePrivateMethod[in] the invoke private method
*/
void store_initial_methods(J9JavaVM *javaVM, J9Method *cInitialStaticMethod, J9Method *cInitialSpecialMethod, J9Method *cInitialVirtualMethod);

/*
* Set initial methods to address stored in jvmimageheader 
*
* @param javaVM[in] the java vm
* @param cInitialStaticMethod[in] the initial static method
* @param cInitialSpecialMethod[in] the initial special method
* @param cInitialVirtualMethod[in] the initial virtual method
* @param cInvokePrivateMethod[in] the invoke private method
*/
void set_initial_methods(J9JavaVM *javaVM, J9Method **cInitialStaticMethod, J9Method **cInitialSpecialMethod, J9Method **cInitialVirtualMethod);

#ifdef __cplusplus
}
#endif

#endif /* jvmimage_api_h */
