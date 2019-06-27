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

#include "j9protos.h"

#if defined(J9VM_OPT_VM_LOCAL_STORAGE)

#include "j9vmls.h"

J9VMLSFunctionTable J9VMLSFunctions = {
	J9VMLSAllocKeys,
	J9VMLSFreeKeys,
	J9VMLSGet,
	J9VMLSSet,
};

#endif /* J9VM_OPT_VM_LOCAL_STORAGE */

J9InternalVMFunctions J9InternalFunctions = {
	NULL /* reserved field reserved0 initialized to zero */,
	NULL /* reserved field reserved1 initialized to zero */,
	NULL /* reserved field reserved2 initialized to zero */,
	DestroyJavaVM,
	AttachCurrentThread,
	DetachCurrentThread,
	GetEnv,
	AttachCurrentThreadAsDaemon,
	addSystemProperty,
	getSystemProperty,
	setSystemProperty,
	findDllLoadInfo,
	internalExceptionDescribe,
	internalFindClassUTF8,
	internalFindClassInModule,
	internalFindClassString,
	hashClassTableAt,
	hashClassTableAtPut,
	hashClassTableDelete,
	hashClassTableReplace,
	monitorTableAt,
	allocateVMThread,
	deallocateVMThread,
	allocateMemorySegment,
	javaThreadProc,
	copyStringToUTF8WithMemAlloc,
	copyStringToJ9UTF8WithMemAlloc,
	internalAcquireVMAccess,
	internalAcquireVMAccessWithMask,
	internalAcquireVMAccessNoMutexWithMask,
	internalReleaseVMAccessSetStatus,
	instanceFieldOffset,
	staticFieldAddress,
	internalFindKnownClass,
	resolveKnownClass,
	computeHashForUTF8,
	getStringUTF8Length,
	acquireExclusiveVMAccess,
	releaseExclusiveVMAccess,
	internalReleaseVMAccess,
	sendInit,
	internalAcquireVMAccessNoMutex,
	internalCreateArrayClass,
	attachSystemDaemonThread,
	internalAcquireVMAccessClearStatus,
#if defined(J9VM_OPT_REFLECT)
	helperMultiANewArray,
#endif /* J9VM_OPT_REFLECT */
	javaLookupMethod,
	javaLookupMethodImpl,
	setCurrentException,
	setCurrentExceptionUTF,
	setCurrentExceptionNLS,
	setCurrentExceptionNLSWithArgs,
	setCurrentExceptionWithCause,
	objectMonitorEnter,
	objectMonitorExit,
	resolveStaticMethodRef,
	resolveStaticSplitMethodRef,
	resolveSpecialMethodRef,
	resolveSpecialSplitMethodRef,
	resolveStaticFieldRef,
	resolveInstanceFieldRef,
	allocateClassLoader,
	internalSendExceptionConstructor,
	instanceOfOrCheckCast,
	internalCreateRAMClassFromROMClass,
	resolveStringRef,
	exitJavaVM,
	internalRunPreInitInstructions,
	resolveClassRef,
	currentVMThread,
	freeMemorySegment,
	jniPopFrame,
	resolveVirtualMethodRef,
	resolveInterfaceMethodRef,
	getVTableOffsetForMethod,
	checkVisibility,
	sendClinit,
	freeStackWalkCaches,
	genericStackDumpIterator,
	exceptionHandlerSearch,
	internalCreateBaseTypePrimitiveAndArrayClasses,
	isExceptionTypeCaughtByHandler,
#if defined(J9VM_IVE_ROM_IMAGE_HELPERS) || (defined(J9VM_OPT_DYNAMIC_LOAD_SUPPORT) && defined(J9VM_OPT_ROM_IMAGE_SUPPORT))
	romImageNewSegment,
#endif /* J9VM_IVE_ROM_IMAGE_HELPERS || (J9VM_OPT_DYNAMIC_LOAD_SUPPORT && J9VM_OPT_ROM_IMAGE_SUPPORT) */
	runCallInMethod,
	catUtfToString4,
	allocateMemorySegmentList,
	allocateMemorySegmentListWithFlags,
	freeMemorySegmentList,
	allocateMemorySegmentInList,
	allocateVirtualMemorySegmentInList,
	allocateMemorySegmentListEntry,
	allocateClassMemorySegment,
#if defined(J9VM_GC_FINALIZATION)
	jniResetStackReferences,
#endif /* J9VM_GC_FINALIZATION */
	freeClassLoader,
	j9jni_createLocalRef,
	j9jni_deleteLocalRef,
	j9jni_createGlobalRef,
	j9jni_deleteGlobalRef,
	javaCheckAsyncMessages,
	getJNIFieldID,
	getJNIMethodID,
	initializeMethodRunAddress,
	growJavaStack,
	freeStacks,
#if defined(J9VM_INTERP_SIG_QUIT_THREAD) || defined(J9VM_RAS_DUMP_AGENTS)
	printThreadInfo,
#endif /* J9VM_INTERP_SIG_QUIT_THREAD || J9VM_RAS_DUMP_AGENTS */
	initializeAttachedThread,
	initializeMethodRunAddressNoHook,
	sidecarInvokeReflectMethod,
	sidecarInvokeReflectConstructor,
	allocateMemorySegmentListWithSize,
	freeMemorySegmentListEntry,
	acquireExclusiveVMAccessFromExternalThread,
	releaseExclusiveVMAccessFromExternalThread,
#if defined(J9VM_GC_REALTIME)
	requestExclusiveVMAccessMetronome,
	waitForExclusiveVMAccessMetronome,
	releaseExclusiveVMAccessMetronome,
	requestExclusiveVMAccessMetronomeTemp,
	waitForExclusiveVMAccessMetronomeTemp,
#endif /* J9VM_GC_REALTIME */
#if defined(J9VM_GC_JNI_ARRAY_CACHE)
	cleanupVMThreadJniArrayCache,
#endif /* J9VM_GC_JNI_ARRAY_CACHE */
	objectMonitorInflate,
	objectMonitorEnterNonBlocking,
	objectMonitorEnterBlocking,
	fillJITVTableSlot,
	findArgInVMArgs,
	optionValueOperations,
	dumpStackTrace,
	loadJ9DLL,
	runJVMOnLoad,
	checkRomClassForError,
	setExceptionForErroredRomClass,
	jniVersionIsValid,
	allClassesStartDo,
	allClassesNextDo,
	allClassesEndDo,
	allLiveClassesStartDo,
	allLiveClassesNextDo,
	allLiveClassesEndDo,
	allClassLoadersStartDo,
	allClassLoadersNextDo,
	allClassLoadersEndDo,
#if defined(J9VM_OPT_ROM_IMAGE_SUPPORT) || defined(J9VM_IVE_ROM_IMAGE_HELPERS)
	romClassLoadFromCookie,
#endif /* J9VM_OPT_ROM_IMAGE_SUPPORT || J9VM_IVE_ROM_IMAGE_HELPERS */
#if defined(J9VM_GC_DYNAMIC_CLASS_UNLOADING)
	cleanUpClassLoader,
#endif /* J9VM_GC_DYNAMIC_CLASS_UNLOADING */
	iterateStackTrace,
	internalReleaseVMAccessNoMutex,
	getVMHookInterface,
	internalAttachCurrentThread,
	setHaltFlag,
	returnFromJNI,
	j9stackmap_StackBitsForPC,
	getJITHookInterface,
	haltThreadForInspection,
	resumeThreadForInspection,
	threadCleanup,
	walkStackForExceptionThrow,
	postInitLoadJ9DLL,
	annotationElementIteratorNext,
	annotationElementIteratorStart,
	elementArrayIteratorNext,
	elementArrayIteratorStart,
	getAllAnnotationsFromAnnotationInfo,
	getAnnotationDefaultsForAnnotation,
	getAnnotationDefaultsForNamedAnnotation,
	getAnnotationInfoFromClass,
	getAnnotationsFromAnnotationInfo,
	getAnnotationFromAnnotationInfo,
	getNamedElementFromAnnotation,
	registerNativeLibrary,
	registerBootstrapLibrary,
	startJavaThread,
	createCachedOutOfMemoryError,
	internalTryAcquireVMAccess,
	internalTryAcquireVMAccessWithMask,
	structuredSignalHandler,
	structuredSignalHandlerVM,
	addHiddenInstanceField,
	fieldOffsetsStartDo,
	fieldOffsetsNextDo,
	fullTraversalFieldOffsetsStartDo,
	fullTraversalFieldOffsetsNextDo,
	setClassCastException,
	compareStrings,
	compareStringToUTF8,
	prepareForExceptionThrow,
	copyUTF8ToUnicode,
	verifyQualifiedName,
	copyStringToUTF8Helper,
	sendCompleteInitialization,
	J9RegisterAsyncEvent,
	J9UnregisterAsyncEvent,
	J9SignalAsyncEvent,
	J9SignalAsyncEventWithoutInterrupt,
	J9CancelAsyncEvent,
	hashClassTableStartDo,
	hashClassTableNextDo,
	hashPkgTableAt,
	hashPkgTableStartDo,
	hashPkgTableNextDo,
	ensureJNIIDTable,
	initializeMethodID,
	objectMonitorDestroy,
	objectMonitorDestroyComplete,
	buildNativeFunctionNames,
	resolveInstanceFieldRefInto,
	resolveInterfaceMethodRefInto,
	resolveSpecialMethodRefInto,
	resolveStaticFieldRefInto,
	resolveStaticMethodRefInto,
	resolveVirtualMethodRefInto,
	findObjectDeadlockedThreads,
	findROMClassFromPC,
	j9localmap_LocalBitsForPC,
	fillInDgRasInterface,
	rasStartDeferredThreads,
	initJVMRI,
	shutdownJVMRI,
	getOwnedObjectMonitors,
	fixUnsafeMethods,
#if !defined(J9VM_SIZE_SMALL_CODE)
	fieldIndexTableRemove,
#endif /* J9VM_SIZE_SMALL_CODE */
	getJavaThreadPriority,
	atomicOrIntoConstantPool,
	atomicAndIntoConstantPool,
	setNativeOutOfMemoryError,
	illegalAccessMessage,
	setThreadForkOutOfMemoryError,
	initializeNativeLibrary,
	addStatistic,
	getStatistic,
	setVMThreadNameFromString,
	findJNIMethod,
	getJ9VMVersionString,
	resolveMethodTypeRef,
	sendFromMethodDescriptorString,
	addToBootstrapClassLoaderSearch,
	addToSystemClassLoaderSearch,
	queryLogOptions,
	setLogOptions,
	exitJavaThread,
#if defined(J9VM_THR_LOCK_NURSERY)
	cacheObjectMonitorForLookup,
#endif /* J9VM_THR_LOCK_NURSERY */
	jniArrayAllocateMemoryFromThread,
	jniArrayFreeMemoryFromThread,
	sendForGenericInvoke,
	jitFillOSRBuffer,
	sendResolveMethodHandle,
	resolveConstantDynamic,
	resolveInvokeDynamic,
	sendResolveConstantDynamic,
	sendResolveInvokeDynamic,
	resolveMethodHandleRef,
	resolveNativeAddress,
	clearHaltFlag,
	setHeapOutOfMemoryError,
	initializeHeapOOMMessage,
	copyUTF8ToCompressedUnicode,
	threadAboutToStart,
	mustHaveVMAccess,
#if defined(J9VM_PORT_ZOS_CEEHDLRSUPPORT)
	javaAndCStacksMustBeInSync,
#endif /* J9VM_PORT_ZOS_CEEHDLRSUPPORT */
	findFieldSignatureClass,
	walkBytecodeFrameSlots,
	jniNativeMethodProperties,
	invalidJITReturnAddress,
	internalAllocateClassLoader,
	initializeClass,
	threadParkImpl,
	threadUnparkImpl,
	monitorWaitImpl,
	threadSleepImpl,
	getMonitorForWait,
	jvmPhaseChange,
	prepareClass,
	buildMethodTypeFrame,
	fatalRecursiveStackOverflow,
	setIllegalAccessErrorNonPublicInvokeInterface,
	createThreadWithCategory,
	attachThreadWithCategory,
	searchClassForMethod,
#if defined(J9VM_INTERP_ATOMIC_FREE_JNI)
	internalEnterVMFromJNI,
	internalExitVMToJNI,
#endif /* J9VM_INTERP_ATOMIC_FREE_JNI */
	hashModuleNameTableNew,
	hashModulePointerTableNew,
	hashPackageTableNew,
	hashModuleExtraInfoTableNew,
	hashClassLocationTableNew,
	findModuleForPackageUTF8,
	findModuleForPackage,
	findModuleInfoForModule,
	findClassLocationForClass,
	getJimModules,
	initializeClassPath,
	initializeClassPathEntry,
	setBootLoaderModulePatchPaths,
	isAnyClassLoadedFromPackage,
	freeJ9Module,
	acquireSafePointVMAccess,
	releaseSafePointVMAccess,
	setIllegalAccessErrorReceiverNotSameOrSubtypeOfCurrentClass,
	getVMRuntimeState,
	updateVMRuntimeState,
	getVMMinIdleWaitTime,
	registerCPEntry,
#if defined(J9VM_RAS_EYECATCHERS)
	j9rasSetServiceLevel,
#endif /* J9VM_RAS_EYECATCHERS */
#if defined(J9VM_INTERP_ATOMIC_FREE_JNI_USES_FLUSH)
	flushProcessWriteBuffers,
#endif /* J9VM_INTERP_ATOMIC_FREE_JNI_USES_FLUSH */
	registerPredefinedHandler,
	registerOSHandler,
	throwNativeOOMError,
	throwNewJavaIoIOException,
#if defined(J9VM_OPT_VALHALLA_NESTMATES)
	loadAndVerifyNestHost,
	setNestmatesError,
#endif
	areValueTypesEnabled,
};
