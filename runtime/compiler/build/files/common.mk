# Copyright (c) 2000, 2019 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception

JIT_PRODUCT_BACKEND_SOURCES+=\
    compiler/control/J9Recompilation.cpp \
    compiler/env/CHTable.cpp \
    compiler/env/PersistentCHTable.cpp \
    compiler/optimizer/AllocationSinking.cpp \
    compiler/optimizer/DataAccessAccelerator.cpp \
    compiler/optimizer/DynamicLiteralPool.cpp \
    compiler/optimizer/EscapeAnalysis.cpp \
    compiler/optimizer/PreEscapeAnalysis.cpp \
    compiler/optimizer/PostEscapeAnalysis.cpp \
    compiler/optimizer/FearPointAnalysis.cpp \
    compiler/optimizer/HCRGuardAnalysis.cpp \
    compiler/optimizer/IdiomRecognition.cpp \
    compiler/optimizer/IdiomRecognitionUtils.cpp \
    compiler/optimizer/IdiomTransformations.cpp \
    compiler/optimizer/J9CFGSimplifier.cpp \
    compiler/optimizer/J9LocalCSE.cpp \
    compiler/optimizer/J9OptimizationManager.cpp \
    compiler/optimizer/J9Optimizer.cpp \
    compiler/optimizer/J9RecognizedCallTransformer.cpp \
    compiler/optimizer/J9Simplifier.cpp \
    compiler/optimizer/J9SimplifierHandlers.cpp \
    compiler/optimizer/J9SimplifierHelpers.cpp \
    compiler/optimizer/J9TransformUtil.cpp \
    compiler/optimizer/J9ValuePropagation.cpp \
    compiler/optimizer/JitProfiler.cpp \
    compiler/optimizer/JProfilingBlock.cpp \
    compiler/optimizer/JProfilingValue.cpp \
    compiler/optimizer/JProfilingRecompLoopTest.cpp \
    compiler/optimizer/LiveVariablesForGC.cpp \
    compiler/optimizer/LoopAliasRefiner.cpp \
    compiler/optimizer/MonitorElimination.cpp \
    compiler/optimizer/NewInitialization.cpp \
    compiler/optimizer/OSRGuardAnalysis.cpp \
    compiler/optimizer/OSRGuardInsertion.cpp \
    compiler/optimizer/OSRGuardRemoval.cpp \
    compiler/optimizer/ProfileGenerator.cpp \
    compiler/optimizer/SequentialStoreSimplifier.cpp \
    compiler/optimizer/SignExtendLoads.cpp \
    compiler/optimizer/SPMDParallelizer.cpp \
    compiler/optimizer/SPMDPreCheck.cpp \
    compiler/optimizer/StaticFinalFieldFolding.cpp \
    compiler/optimizer/StringBuilderTransformer.cpp \
    compiler/optimizer/StringPeepholes.cpp \
    compiler/optimizer/UnsafeFastPath.cpp \
    compiler/optimizer/VarHandleTransformer.cpp \
    compiler/optimizer/VPBCDConstraint.cpp \
    omr/compiler/codegen/Analyser.cpp \
    omr/compiler/codegen/CodeGenGC.cpp \
    omr/compiler/codegen/CodeGenPrep.cpp \
    omr/compiler/codegen/CodeGenRA.cpp \
    omr/compiler/codegen/ELFGenerator.cpp \
    omr/compiler/codegen/FrontEnd.cpp \
    omr/compiler/codegen/LiveRegister.cpp \
    omr/compiler/codegen/NodeEvaluation.cpp \
    omr/compiler/codegen/OMRAheadOfTimeCompile.cpp \
    omr/compiler/codegen/OMRCodeGenerator.cpp \
    omr/compiler/codegen/OMRCodeGenPhase.cpp \
    omr/compiler/codegen/OMRELFRelocationResolver.cpp \
    omr/compiler/codegen/OMRGCRegisterMap.cpp \
    omr/compiler/codegen/OMRGCStackAtlas.cpp \
    omr/compiler/codegen/OMRInstruction.cpp \
    omr/compiler/codegen/OMRLinkage.cpp \
    omr/compiler/codegen/OMRMachine.cpp \
    omr/compiler/codegen/OMRMemoryReference.cpp \
    omr/compiler/codegen/OMRRealRegister.cpp \
    omr/compiler/codegen/OMRRegister.cpp \
    omr/compiler/codegen/OMRRegisterPair.cpp \
    omr/compiler/codegen/OMRSnippet.cpp \
    omr/compiler/codegen/OMRSnippetGCMap.cpp \
    omr/compiler/codegen/OMRTreeEvaluator.cpp \
    omr/compiler/codegen/OMRUnresolvedDataSnippet.cpp \
    omr/compiler/codegen/OutOfLineCodeSection.cpp \
    omr/compiler/codegen/PreInstructionSelection.cpp \
    omr/compiler/codegen/RegisterIterator.cpp \
    omr/compiler/codegen/Relocation.cpp \
    omr/compiler/codegen/ScratchRegisterManager.cpp \
    omr/compiler/codegen/StorageInfo.cpp \
    omr/compiler/compile/OMRAliasBuilder.cpp \
    omr/compiler/compile/OMRCompilation.cpp \
    omr/compiler/compile/OMRMethod.cpp \
    omr/compiler/compile/OMRSymbolReferenceTable.cpp \
    omr/compiler/compile/OSRData.cpp \
    omr/compiler/compile/ResolvedMethod.cpp \
    omr/compiler/compile/TLSCompilationManager.cpp \
    omr/compiler/compile/VirtualGuard.cpp \
    omr/compiler/control/OMROptions.cpp \
    omr/compiler/control/OMRRecompilation.cpp \
    omr/compiler/control/OptimizationPlan.cpp \
    omr/compiler/env/DebugSegmentProvider.cpp \
    omr/compiler/env/ExceptionTable.cpp \
    omr/compiler/env/OMRArithEnv.cpp \
    omr/compiler/env/OMRCPU.cpp \
    omr/compiler/env/OMRClassEnv.cpp \
    omr/compiler/env/OMRDebugEnv.cpp \
    omr/compiler/env/OMRObjectModel.cpp \
    omr/compiler/env/OMRPersistentInfo.cpp \
    omr/compiler/env/OMRVMEnv.cpp \
    omr/compiler/env/Region.cpp \
    omr/compiler/env/SegmentAllocator.cpp \
    omr/compiler/env/SegmentProvider.cpp \
    omr/compiler/env/StackMemoryRegion.cpp \
    omr/compiler/env/TRMemory.cpp \
    omr/compiler/env/TRPersistentMemory.cpp \
    omr/compiler/env/VerboseLog.cpp \
    omr/compiler/il/Aliases.cpp \
    omr/compiler/il/NodePool.cpp \
    omr/compiler/il/NodeUtils.cpp \
    omr/compiler/il/OMRBlock.cpp \
    omr/compiler/il/OMRDataTypes.cpp \
    omr/compiler/il/OMRIL.cpp \
    omr/compiler/il/OMRILOps.cpp \
    omr/compiler/il/OMRNode.cpp \
    omr/compiler/il/OMRSymbolReference.cpp \
    omr/compiler/il/OMRTreeTop.cpp \
    omr/compiler/il/symbol/OMRAutomaticSymbol.cpp \
    omr/compiler/il/symbol/OMRLabelSymbol.cpp \
    omr/compiler/il/symbol/OMRMethodSymbol.cpp \
    omr/compiler/il/symbol/OMRParameterSymbol.cpp \
    omr/compiler/il/symbol/OMRRegisterMappedSymbol.cpp \
    omr/compiler/il/symbol/OMRResolvedMethodSymbol.cpp \
    omr/compiler/il/symbol/OMRStaticSymbol.cpp \
    omr/compiler/il/symbol/OMRSymbol.cpp \
    omr/compiler/ilgen/IlGenRequest.cpp \
    omr/compiler/infra/Assert.cpp \
    omr/compiler/infra/BitVector.cpp \
    omr/compiler/infra/Checklist.cpp \
    omr/compiler/infra/HashTab.cpp \
    omr/compiler/infra/IGBase.cpp \
    omr/compiler/infra/IGNode.cpp \
    omr/compiler/infra/ILWalk.cpp \
    omr/compiler/infra/InterferenceGraph.cpp \
    omr/compiler/infra/OMRCfg.cpp \
    omr/compiler/infra/OMRMonitorTable.cpp \
    omr/compiler/infra/Random.cpp \
    omr/compiler/infra/SimpleRegex.cpp \
    omr/compiler/infra/Timer.cpp \
    omr/compiler/infra/TreeServices.cpp \
    omr/compiler/optimizer/AsyncCheckInsertion.cpp \
    omr/compiler/optimizer/BackwardBitVectorAnalysis.cpp \
    omr/compiler/optimizer/BackwardIntersectionBitVectorAnalysis.cpp \
    omr/compiler/optimizer/BackwardUnionBitVectorAnalysis.cpp \
    omr/compiler/optimizer/BitVectorAnalysis.cpp \
    omr/compiler/optimizer/CatchBlockRemover.cpp \
    omr/compiler/optimizer/OMRCFGSimplifier.cpp \
    omr/compiler/optimizer/CompactLocals.cpp \
    omr/compiler/optimizer/CopyPropagation.cpp \
    omr/compiler/optimizer/DataFlowAnalysis.cpp \
    omr/compiler/optimizer/DeadStoreElimination.cpp \
    omr/compiler/optimizer/DeadTreesElimination.cpp \
    omr/compiler/optimizer/DebuggingCounters.cpp \
    omr/compiler/optimizer/Delayedness.cpp \
    omr/compiler/optimizer/Dominators.cpp \
    omr/compiler/optimizer/DominatorVerifier.cpp \
    omr/compiler/optimizer/DominatorsChk.cpp \
    omr/compiler/optimizer/Earliestness.cpp \
    omr/compiler/optimizer/ExpressionsSimplification.cpp \
    omr/compiler/optimizer/FieldPrivatizer.cpp \
    omr/compiler/optimizer/GeneralLoopUnroller.cpp \
    omr/compiler/optimizer/GlobalAnticipatability.cpp \
    omr/compiler/optimizer/GlobalRegisterAllocator.cpp \
    omr/compiler/optimizer/InductionVariable.cpp \
    omr/compiler/optimizer/Inliner.cpp \
    omr/compiler/optimizer/IntersectionBitVectorAnalysis.cpp \
    omr/compiler/optimizer/IsolatedStoreElimination.cpp \
    omr/compiler/optimizer/Isolatedness.cpp \
    omr/compiler/optimizer/Latestness.cpp \
    omr/compiler/optimizer/LiveOnAllPaths.cpp \
    omr/compiler/optimizer/LiveVariableInformation.cpp \
    omr/compiler/optimizer/Liveness.cpp \
    omr/compiler/optimizer/LoadExtensions.cpp \
    omr/compiler/optimizer/LocalAnalysis.cpp \
    omr/compiler/optimizer/LocalAnticipatability.cpp \
    omr/compiler/optimizer/LocalDeadStoreElimination.cpp \
    omr/compiler/optimizer/LocalLiveRangeReducer.cpp \
    omr/compiler/optimizer/LocalOpts.cpp \
    omr/compiler/optimizer/LocalReordering.cpp \
    omr/compiler/optimizer/LocalTransparency.cpp \
    omr/compiler/optimizer/LoopCanonicalizer.cpp \
    omr/compiler/optimizer/LoopReducer.cpp \
    omr/compiler/optimizer/LoopReplicator.cpp \
    omr/compiler/optimizer/LoopVersioner.cpp \
    omr/compiler/optimizer/OMRLocalCSE.cpp \
    omr/compiler/optimizer/OMROptimization.cpp \
    omr/compiler/optimizer/OMROptimizationManager.cpp \
    omr/compiler/optimizer/OMROptimizer.cpp \
    omr/compiler/optimizer/OMRRecognizedCallTransformer.cpp \
    omr/compiler/optimizer/OMRSimplifier.cpp \
    omr/compiler/optimizer/OMRSimplifierHandlers.cpp \
    omr/compiler/optimizer/OMRSimplifierHelpers.cpp \
    omr/compiler/optimizer/OMRTransformUtil.cpp \
    omr/compiler/optimizer/OMRValuePropagation.cpp \
    omr/compiler/optimizer/OrderBlocks.cpp \
    omr/compiler/optimizer/OSRDefAnalysis.cpp \
    omr/compiler/optimizer/PartialRedundancy.cpp \
    omr/compiler/optimizer/PreExistence.cpp \
    omr/compiler/optimizer/PrefetchInsertion.cpp \
    omr/compiler/optimizer/Reachability.cpp \
    omr/compiler/optimizer/ReachingDefinitions.cpp \
    omr/compiler/optimizer/RedundantAsyncCheckRemoval.cpp \
    omr/compiler/optimizer/RegDepCopyRemoval.cpp \
    omr/compiler/optimizer/RegisterCandidate.cpp \
    omr/compiler/optimizer/RematTools.cpp \
    omr/compiler/optimizer/ReorderIndexExpr.cpp \
    omr/compiler/optimizer/SinkStores.cpp \
    omr/compiler/optimizer/StripMiner.cpp \
    omr/compiler/optimizer/StructuralAnalysis.cpp \
    omr/compiler/optimizer/Structure.cpp \
    omr/compiler/optimizer/SwitchAnalyzer.cpp \
    omr/compiler/optimizer/TranslateTable.cpp \
    omr/compiler/optimizer/TrivialDeadBlockRemover.cpp \
    omr/compiler/optimizer/UnionBitVectorAnalysis.cpp \
    omr/compiler/optimizer/UseDefInfo.cpp \
    omr/compiler/optimizer/ValueNumberInfo.cpp \
    omr/compiler/optimizer/ValuePropagationCommon.cpp \
    omr/compiler/optimizer/VirtualGuardCoalescer.cpp \
    omr/compiler/optimizer/VirtualGuardHeadMerger.cpp \
    omr/compiler/optimizer/VPConstraint.cpp \
    omr/compiler/optimizer/VPHandlers.cpp \
    omr/compiler/optimizer/VPHandlersCommon.cpp \
    omr/compiler/ras/CallStack.cpp \
    omr/compiler/ras/CFGChecker.cpp \
    omr/compiler/ras/Debug.cpp \
    omr/compiler/ras/DebugCounter.cpp \
    omr/compiler/ras/IgnoreLocale.cpp \
    omr/compiler/ras/ILValidationRules.cpp \
    omr/compiler/ras/ILValidationUtils.cpp \
    omr/compiler/ras/ILValidator.cpp \
    omr/compiler/ras/LimitFile.cpp \
    omr/compiler/ras/LogTracer.cpp \
    omr/compiler/ras/OptionsDebug.cpp \
    omr/compiler/ras/Tree.cpp

JIT_PRODUCT_SOURCE_FILES+=\
    compiler/codegen/CodeGenGC.cpp \
    compiler/codegen/CodeGenGPU.cpp \
    compiler/codegen/CodeGenRA.cpp \
    compiler/codegen/J9AheadOfTimeCompile.cpp \
    compiler/codegen/J9CodeGenPhase.cpp \
    compiler/codegen/J9CodeGenerator.cpp \
    compiler/codegen/J9GCStackAtlas.cpp \
    compiler/codegen/J9Instruction.cpp \
    compiler/codegen/J9Snippet.cpp \
    compiler/codegen/J9TreeEvaluator.cpp \
    compiler/codegen/J9WatchedInstanceFieldSnippet.cpp \
    compiler/codegen/J9WatchedStaticFieldSnippet.cpp \
    compiler/codegen/MonitorState.cpp \
    compiler/compile/J9AliasBuilder.cpp \
    compiler/compile/J9Compilation.cpp \
    compiler/compile/J9Method.cpp \
    compiler/compile/J9SymbolReferenceTable.cpp \
    compiler/control/CompilationController.cpp \
    compiler/control/CompilationThread.cpp \
    compiler/control/DLLMain.cpp \
    compiler/control/HookedByTheJit.cpp \
    compiler/control/J9Options.cpp \
    compiler/control/MethodToBeCompiled.cpp \
    compiler/control/rossa.cpp \
    compiler/env/ClassLoaderTable.cpp \
    compiler/env/CpuUtilization.cpp \
    compiler/env/FilePointer.cpp \
    compiler/env/J2IThunk.cpp \
    compiler/env/J9ArithEnv.cpp \
    compiler/env/J9ClassEnv.cpp \
    compiler/env/J9CompilerEnv.cpp \
    compiler/env/J9CPU.cpp \
    compiler/env/J9DebugEnv.cpp \
    compiler/env/J9IO.cpp \
    compiler/env/J9JitMemory.cpp \
    compiler/env/J9KnownObjectTable.cpp \
    compiler/env/J9ObjectModel.cpp \
    compiler/env/J9SegmentAllocator.cpp \
    compiler/env/J9SegmentCache.cpp \
    compiler/env/J9SegmentProvider.cpp \
    compiler/env/J9SharedCache.cpp \
    compiler/env/J9VMEnv.cpp \
    compiler/env/J9VMMethodEnv.cpp \
    compiler/env/PersistentAllocator.cpp \
    compiler/env/ProcessorDetection.cpp \
    compiler/env/Startup.cpp \
    compiler/env/SystemSegmentProvider.cpp \
    compiler/env/VMJ9.cpp \
    compiler/env/annotations/AnnotationBase.cpp \
    compiler/env/annotations/Annotations.cpp \
    compiler/env/annotations/GPUAnnotation.cpp \
    compiler/env/annotations/TRNoSideFXAnnotation.cpp \
    compiler/env/annotations/TROptAnnotation.cpp \
    compiler/env/annotations/TestAnnotation.cpp \
    compiler/env/annotations/VMJ9Annotations.cpp \
    compiler/env/j9fieldsInfo.cpp \
    compiler/env/j9method.cpp \
    compiler/env/jitsupport.cpp \
    compiler/env/ut_j9jit.c \
    compiler/il/J9DataTypes.cpp \
    compiler/il/J9IL.cpp \
    compiler/il/J9Node.cpp \
    compiler/il/J9SymbolReference.cpp \
    compiler/il/symbol/J9MethodSymbol.cpp \
    compiler/il/symbol/J9ResolvedMethodSymbol.cpp \
    compiler/il/symbol/J9StaticSymbol.cpp \
    compiler/il/symbol/J9Symbol.cpp \
    compiler/ilgen/ClassLookahead.cpp \
    compiler/ilgen/IlGenerator.cpp \
    compiler/ilgen/J9ByteCodeIterator.cpp \
    compiler/ilgen/J9IlGeneratorMethodDetails.cpp \
    compiler/ilgen/Walker.cpp \
    compiler/infra/J9Cfg.cpp \
    compiler/infra/J9Monitor.cpp \
    compiler/infra/J9MonitorTable.cpp \
    compiler/infra/RWMonitor.cpp \
    compiler/optimizer/BoolArrayStoreTransformer.cpp \
    compiler/optimizer/EstimateCodeSize.cpp \
    compiler/optimizer/InlinerTempForJ9.cpp \
    compiler/optimizer/InterProceduralAnalyzer.cpp \
    compiler/optimizer/J9EstimateCodeSize.cpp \
    compiler/optimizer/J9Inliner.cpp \
    compiler/ras/DebugExt.cpp \
    compiler/ras/DebugExtSegmentProvider.cpp \
    compiler/ras/HashTable.cpp \
    compiler/ras/InternalFunctions.cpp \
    compiler/ras/kca_offsets_generator.cpp \
    compiler/runtime/ArtifactManager.cpp \
    compiler/runtime/ClassUnloadAssumption.cpp \
    compiler/runtime/CRuntimeImpl.cpp \
    compiler/runtime/DataCache.cpp \
    compiler/runtime/GPUHelpers.cpp \
    compiler/runtime/HWProfiler.cpp \
    compiler/runtime/HookHelpers.cpp \
    compiler/runtime/IProfiler.cpp \
    compiler/runtime/J9CodeCache.cpp \
    compiler/runtime/J9CodeCacheManager.cpp \
    compiler/runtime/J9CodeCacheMemorySegment.cpp \
    compiler/runtime/J9JitPersistentMemory.cpp \
    compiler/runtime/J9Profiler.cpp \
    compiler/runtime/JitRuntime.cpp \
    compiler/runtime/LMGuardedStorage.cpp \
    compiler/runtime/MetaData.cpp \
    compiler/runtime/MetaDataDebug.cpp \
    compiler/runtime/MethodMetaData.c \
    compiler/runtime/RelocationRecord.cpp \
    compiler/runtime/RelocationRuntime.cpp \
    compiler/runtime/RelocationRuntimeLogger.cpp \
    compiler/runtime/RelocationTarget.cpp \
    compiler/runtime/Runtime.cpp \
    compiler/runtime/RuntimeAssumptions.cpp \
    compiler/runtime/SignalHandler.c \
    compiler/runtime/SymbolValidationManager.cpp \
    compiler/runtime/Trampoline.cpp \
    compiler/runtime/ValueProfiler.cpp \
    compiler/runtime/codertinit.cpp \
    compiler/runtime/emfloat.c \
    omr/compiler/env/Globals.cpp \
    omr/compiler/env/OMRCompilerEnv.cpp \
    omr/compiler/env/OMRIO.cpp \
    omr/compiler/env/OMRKnownObjectTable.cpp \
    omr/compiler/runtime/Alignment.cpp \
    omr/compiler/runtime/CodeCacheTypes.cpp \
    omr/compiler/runtime/OMRCodeCache.cpp \
    omr/compiler/runtime/OMRCodeCacheConfig.cpp \
    omr/compiler/runtime/OMRCodeCacheManager.cpp \
    omr/compiler/runtime/OMRCodeCacheMemorySegment.cpp \
    omr/compiler/runtime/OMRRuntimeAssumptions.cpp

ifneq ($(JITSERVER_SUPPORT),)
JIT_PRODUCT_SOURCE_FILES+=\
    compiler/control/JITServerCompilationThread.cpp \
    compiler/control/JITServerHelpers.cpp \
    compiler/env/j9methodServer.cpp \
    compiler/env/JITServerCHTable.cpp \
    compiler/env/JITServerPersistentCHTable.cpp \
    compiler/net/ClientStream.cpp \
    compiler/net/CommunicationStream.cpp \
    compiler/net/ProtobufTypeConvert.cpp \
    compiler/net/ServerStream.cpp \
    compiler/runtime/CompileService.cpp \
    compiler/runtime/JITClientSession.cpp \
    compiler/runtime/JITServerIProfiler.cpp \
    compiler/runtime/JITServerStatisticsThread.cpp \
    compiler/runtime/Listener.cpp
endif

-include $(JIT_MAKE_DIR)/files/extra.mk
include $(JIT_MAKE_DIR)/files/host/$(HOST_ARCH).mk
include $(JIT_MAKE_DIR)/files/target/$(TARGET_ARCH).mk
-include $(JIT_MAKE_DIR)/files/host/$(HOST_ARCH)-extra.mk
-include $(JIT_MAKE_DIR)/files/target/$(TARGET_ARCH)-extra.mk
ifneq ($(JITSERVER_SUPPORT),)
include $(JIT_MAKE_DIR)/files/net.mk
endif
