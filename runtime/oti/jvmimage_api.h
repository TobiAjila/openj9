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
* Allocate memory in image heap
* 
* @param portLibrary[in] the default port library
* @param byteAmount[in] size to allocate
* @param callSite[in] location memory allocation is called from
* @param category[in] category of memory allocation
*
* @return pointer to allocated memory on success, NULL on failure 
*/
void* image_mem_allocate_memory(struct OMRPortLibrary *portLibrary, uintptr_t byteAmount, const char *callSite, uint32_t category);

/*
* Free memory in heap image
*
* @param portLibrary[in] the default port library
* @param memoryPointer[in] pointer of address to free
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
* @param classLoaderCategory[in] category to identify the type of classloader. see @ref jvmimage.h
*
* TODO: Allow users to register user defined class loader
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
* @param clazz[in] J9Class to deregister
*/
void deregisterClass(J9JavaVM *javaVM, J9Class *clazz);

/*
* Deregisters class path entry from table
*
* @param javaVM[in] the java vm
* @param cpEntry[in] J9ClassPathEntry to deregister
*/
void deregisterCPEntry(J9JavaVM *javaVM, J9ClassPathEntry *cpEntry);

/*
* Initialized table for the iterator pattern and returns the first element in table.
*
* @param table[in] the table to iterate. Table contains UDATA ptrs. see @ref jvmimage.h
* 
* @return the starting element in the table. NULL if table empty
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
* @return pointer to entry in table. NULL if entry not in table
*/
UDATA* findEntryLocationInTable(ImageTableHeader *table, UDATA entry);

/*
* Finding class loader based on category
*
* @param javaVM[in] the java vm
* @param classLoaderCategory[in] the category of class loaders. see @ref jvmimage.h
*
* TODO: Allow users to register user defined class loader.
*/
J9ClassLoader* findClassLoader(J9JavaVM *javaVM, uint32_t classLoaderCategory);

/*
* Initializes class object. Mimics behaviour of internalCreateRAMClass
*
* @param javaVM[in] the java vm
* @param classLoader the class loader loading the class
* @param clazz the class being loader
*
* @return class object if object allocation passes otherwise null
*/
J9Class* initializeImageClassObject(J9VMThread *vmThread, J9ClassLoader *classLoader, J9Class *clazz);

/*
* Initializes class loader object. Mimics behaviour of internalAllocateClassLoader
* 
* @param javaVM[in] the java vm
* @param classLoader[in] the J9ClassLoader struct 
* @param classLoaderObject[in] unwrapped class loader object ref
*/
void initializeImageClassLoaderObject(J9JavaVM *javaVM, J9ClassLoader *classLoader, j9object_t classLoaderObject);

/*
* Shut down sequence of JVMImage
* Frees memory of heap variables and jvmimage instance
* Does not destroy jvmimageheap monitor
*
* @param javaVM[in] the java vm
*/
void shutdownJVMImage(J9JavaVM *vm);

/*
* Called on cold run to perform fixup of the image heap memory
* Fixup of J9Class, J9ClassLoader, and J9CPEntry performed
*
* @param javaVM[in] the java vm
*/
void teardownJVMImage(J9JavaVM *javaVM);

/*
* Stores the JavaVM initial methods in jvmimageheader. Done for cold runs.
*
* @param javaVM[in] the java vm
* @param cInitialStaticMethod[in] the initial static method
* @param cInitialSpecialMethod[in] the initial special method
* @param cInitialVirtualMethod[in] the initial virtual method
*/
void storeInitialVMMethods(J9JavaVM *javaVM, J9Method *cInitialStaticMethod, J9Method *cInitialSpecialMethod, J9Method *cInitialVirtualMethod);

/*
* Sets JavaVM initial methods to address stored in jvmimageheader. Needed for warm runs. 
*
*
* @param javaVM[in] the java vm
* @param cInitialStaticMethod[in] the initial static method
* @param cInitialSpecialMethod[in] the initial special method
* @param cInitialVirtualMethod[in] the initial virtual method
*/
void setInitialVMMethods(J9JavaVM *javaVM, J9Method **cInitialStaticMethod, J9Method **cInitialSpecialMethod, J9Method **cInitialVirtualMethod);

#ifdef __cplusplus
}
#endif

#endif /* jvmimage_api_h */