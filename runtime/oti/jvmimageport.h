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

#ifndef jvmimageport_h
#define jvmimageport_h

/* Macros for jvmimage port access */
#define JVMIMAGEPORT_FROM_PORT(_portLibrary) ((JVMImagePortLibrary *)(_portLibrary))
#define JVMIMAGEPORT_FROM_JAVAVM(javaVM) ((JVMImagePortLibrary *)(javaVM->jvmImagePortLibrary))
#define JVMIMAGEPORT_ACCESS_FROM_JAVAVM(javaVM) JVMImagePortLibrary *privateImagePortLibrary = JVMIMAGEPORT_FROM_JAVAVM(javaVM)

/* Macros for omr port access */
#define IMAGE_OMRPORT_FROM_JAVAVM(javaVM) ((OMRPortLibrary *)(javaVM->jvmImagePortLibrary))
#define IMAGE_OMRPORT_FROM_JVMIMAGEPORT(jvmImagePortLibrary) ((OMRPortLibrary *)jvmImagePortLibrary)

/* Macros for image access */
#define IMAGE_ACCESS_FROM_PORT(_portLibrary) JVMImage *jvmImage = (JVMImage *)(JVMIMAGEPORT_FROM_PORT(_portLibrary)->jvmImage)
#define IMAGE_ACCESS_FROM_JAVAVM(javaVM) JVMImage *jvmImage = (JVMImage *)(JVMIMAGEPORT_FROM_JAVAVM(javaVM)->jvmImage)

/*
* OMR port library wrapper
* JVMImage instance to access image functions
*/
typedef struct JVMImagePortLibrary {
	/* portLibrary, must be the first member of J9PortLibrary */
	OMRPortLibrary portLibrary;
	void *jvmImage;
} JVMImagePortLibrary;

/* Standard allocation and free functions for image heap suballocator */
#define imem_allocate_memory(param1, category) IMAGE_OMRPORT_FROM_JVMIMAGEPORT(privateImagePortLibrary)->mem_allocate_memory(IMAGE_OMRPORT_FROM_JVMIMAGEPORT(privateImagePortLibrary),param1, J9_GET_CALLSITE(), category)
#define imem_free_memory(param1) IMAGE_OMRPORT_FROM_JVMIMAGEPORT(privateImagePortLibrary)->mem_free_memory(IMAGE_OMRPORT_FROM_JVMIMAGEPORT(privateImagePortLibrary),param1)

#endif