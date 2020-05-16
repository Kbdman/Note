# TEB PEB
Thread Environment Block  线程环境信息块
Process Environment Block 进程环境信息块
TIB Thread Information Block是TEB的前面一部分

## TEB的位置
### 32位
FS指向TEB
FS[0] ExceptionList
FS[0x18] TEB地址
FS[0x30] PEB

### 64位
GS指向TEB
#### PEB
+ 0x000 InheritedAddressSpace : UChar
+ 0x001 ReadImageFileExecOptions : UChar
+ 0x002 BeingDebugged    : UChar
+ 0x003 BitField         : UChar
+ 0x003 ImageUsesLargePages : Pos 0, 1 Bit
+ 0x003 IsProtectedProcess : Pos 1, 1 Bit
+ 0x003 IsImageDynamicallyRelocated : Pos 2, 1 Bit
+ 0x003 SkipPatchingUser32Forwarders : Pos 3, 1 Bit
+ 0x003 IsPackagedProcess : Pos 4, 1 Bit
+ 0x003 IsAppContainer   : Pos 5, 1 Bit
+ 0x003 IsProtectedProcessLight : Pos 6, 1 Bit
+ 0x003 IsLongPathAwareProcess : Pos 7, 1 Bit
+ 0x004 Padding0         : [4] UChar
+ 0x008 Mutant           : Ptr64 Void
+ 0x010 ImageBaseAddress : Ptr64 Void
+ 0x018 Ldr              : Ptr64 _PEB_LDR_DATA
+ 0x020 ProcessParameters : Ptr64 _RTL_USER_PROCESS_PARAMETERS
+ 0x028 SubSystemData    : Ptr64 Void
+ 0x030 ProcessHeap      : Ptr64 Void
+ 0x038 FastPebLock      : Ptr64 _RTL_CRITICAL_SECTION
+ 0x040 AtlThunkSListPtr : Ptr64 _SLIST_HEADER
+ 0x048 IFEOKey          : Ptr64 Void
+ 0x050 CrossProcessFlags : Uint4B
+ 0x050 ProcessInJob     : Pos 0, 1 Bit
+ 0x050 ProcessInitializing : Pos 1, 1 Bit
+ 0x050 ProcessUsingVEH  : Pos 2, 1 Bit
+ 0x050 ProcessUsingVCH  : Pos 3, 1 Bit
+ 0x050 ProcessUsingFTH  : Pos 4, 1 Bit
+ 0x050 ProcessPreviouslyThrottled : Pos 5, 1 Bit
+ 0x050 ProcessCurrentlyThrottled : Pos 6, 1 Bit
+ 0x050 ReservedBits0    : Pos 7, 25 Bits
+ 0x054 Padding1         : [4] UChar
+ 0x058 KernelCallbackTable : Ptr64 Void
+ 0x058 UserSharedInfoPtr : Ptr64 Void
+ 0x060 SystemReserved   : [1] Uint4B
+ 0x064 AtlThunkSListPtr32 : Uint4B
+ 0x068 ApiSetMap        : Ptr64 Void
+ 0x070 TlsExpansionCounter : Uint4B
+ 0x074 Padding2         : [4] UChar
+ 0x078 TlsBitmap        : Ptr64 Void
+ 0x080 TlsBitmapBits    : [2] Uint4B
+ 0x088 ReadOnlySharedMemoryBase : Ptr64 Void
+ 0x090 SharedData       : Ptr64 Void
+ 0x098 ReadOnlyStaticServerData : Ptr64 Ptr64 Void
+ 0x0a0 AnsiCodePageData : Ptr64 Void
+ 0x0a8 OemCodePageData  : Ptr64 Void
+ 0x0b0 UnicodeCaseTableData : Ptr64 Void
+ 0x0b8 NumberOfProcessors : Uint4B
+ 0x0bc NtGlobalFlag     : Uint4B
+ 0x0c0 CriticalSectionTimeout : _LARGE_INTEGER
+ 0x0c8 HeapSegmentReserve : Uint8B
+ 0x0d0 HeapSegmentCommit : Uint8B
+ 0x0d8 HeapDeCommitTotalFreeThreshold : Uint8B
+ 0x0e0 HeapDeCommitFreeBlockThreshold : Uint8B
+ 0x0e8 NumberOfHeaps    : Uint4B
+ 0x0ec MaximumNumberOfHeaps : Uint4B
+ 0x0f0 ProcessHeaps     : Ptr64 Ptr64 Void
+ 0x0f8 GdiSharedHandleTable : Ptr64 Void
+ 0x100 ProcessStarterHelper : Ptr64 Void
+ 0x108 GdiDCAttributeList : Uint4B
+ 0x10c Padding3         : [4] UChar
+ 0x110 LoaderLock       : Ptr64 _RTL_CRITICAL_SECTION
+ 0x118 OSMajorVersion   : Uint4B
+ 0x11c OSMinorVersion   : Uint4B
+ 0x120 OSBuildNumber    : Uint2B
+ 0x122 OSCSDVersion     : Uint2B
+ 0x124 OSPlatformId     : Uint4B
+ 0x128 ImageSubsystem   : Uint4B
+ 0x12c ImageSubsystemMajorVersion : Uint4B
+ 0x130 ImageSubsystemMinorVersion : Uint4B
+ 0x134 Padding4         : [4] UChar
+ 0x138 ActiveProcessAffinityMask : Uint8B
+ 0x140 GdiHandleBuffer  : [60] Uint4B
+ 0x230 PostProcessInitRoutine : Ptr64     void 
+ 0x238 TlsExpansionBitmap : Ptr64 Void
+ 0x240 TlsExpansionBitmapBits : [32] Uint4B
+ 0x2c0 SessionId        : Uint4B
+ 0x2c4 Padding5         : [4] UChar
+ 0x2c8 AppCompatFlags   : _ULARGE_INTEGER
+ 0x2d0 AppCompatFlagsUser : _ULARGE_INTEGER
+ 0x2d8 pShimData        : Ptr64 Void
+ 0x2e0 AppCompatInfo    : Ptr64 Void
+ 0x2e8 CSDVersion       : _UNICODE_STRING
+ 0x2f8 ActivationContextData : Ptr64 _ACTIVATION_CONTEXT_DATA
+ 0x300 ProcessAssemblyStorageMap : Ptr64 _ASSEMBLY_STORAGE_MAP
+ 0x308 SystemDefaultActivationContextData : Ptr64 _ACTIVATION_CONTEXT_DATA
+ 0x310 SystemAssemblyStorageMap : Ptr64 _ASSEMBLY_STORAGE_MAP
+ 0x318 MinimumStackCommit : Uint8B
+ 0x320 FlsCallback      : Ptr64 _FLS_CALLBACK_INFO
+ 0x328 FlsListHead      : _LIST_ENTRY
+ 0x338 FlsBitmap        : Ptr64 Void
+ 0x340 FlsBitmapBits    : [4] Uint4B
+ 0x350 FlsHighIndex     : Uint4B
+ 0x358 WerRegistrationData : Ptr64 Void
+ 0x360 WerShipAssertPtr : Ptr64 Void
+ 0x368 pUnused          : Ptr64 Void
+ 0x370 pImageHeaderHash : Ptr64 Void
+ 0x378 TracingFlags     : Uint4B
+ 0x378 HeapTracingEnabled : Pos 0, 1 Bit
+ 0x378 CritSecTracingEnabled : Pos 1, 1 Bit
+ 0x378 LibLoaderTracingEnabled : Pos 2, 1 Bit
+ 0x378 SpareTracingBits : Pos 3, 29 Bits
+ 0x37c Padding6         : [4] UChar
+ 0x380 CsrServerReadOnlySharedMemoryBase : Uint8B
+ 0x388 TppWorkerpListLock : Uint8B
+ 0x390 TppWorkerpList   : _LIST_ENTRY
+ 0x3a0 WaitOnAddressHashTable : [128] Ptr64 Void
1:k d> dt _TEB
ntl l!_TEB
+ 0x000 NtTib            : _NT_TIB
+ 0x038 EnvironmentPointer : Ptr64 Void
+ 0x040 ClientId         : _CLIENT_ID
+ 0x050 ActiveRpcHandle  : Ptr64 Void
+ 0x058 ThreadLocalStoragePointer : Ptr64 Void
+ 0x060 ProcessEnvironmentBlock : Ptr64 _PEB
+ 0x068 LastErrorValue   : Uint4B
+ 0x06c CountOfOwnedCriticalSections : Uint4B
+ 0x070 CsrClientThread  : Ptr64 Void
+ 0x078 Win32ThreadInfo  : Ptr64 Void
+ 0x080 User32Reserved   : [26] Uint4B
+ 0x0e8 UserReserved     : [5] Uint4B
+ 0x100 WOW32Reserved    : Ptr64 Void
+ 0x108 CurrentLocale    : Uint4B
+ 0x10c FpSoftwareStatusRegister : Uint4B
+ 0x110 ReservedForDebuggerInstrumentation : [16] Ptr64 Void
+ 0x190 SystemReserved1  : [32] Ptr64 Void
+ 0x290 _ActivationStack : _ACTIVATION_CONTEXT_STACK
+ 0x2b8 WorkingOnBehalfTicket : [8] UChar
+ 0x2c0 ExceptionCode    : Int4B
+ 0x2c4 Padding0         : [4] UChar
+ 0x2c8 ActivationContextStackPointer : Ptr64 _ACTIVATION_CONTEXT_STACK
+ 0x2d0 InstrumentationCallbackSp : Uint8B
+ 0x2d8 InstrumentationCallbackPreviousPc : Uint8B
+ 0x2e0 InstrumentationCallbackPreviousSp : Uint8B
+ 0x2e8 TxFsContext      : Uint4B
+ 0x2ec InstrumentationCallbackDisabled : UChar
+ 0x2ed Padding1         : [3] UChar
+ 0x2f0 GdiTebBatch      : _GDI_TEB_BATCH
+ 0x7d8 RealClientId     : _CLIENT_ID
+ 0x7e8 GdiCachedProcessHandle : Ptr64 Void
+ 0x7f0 GdiClientPID     : Uint4B
+ 0x7f4 GdiClientTID     : Uint4B
+ 0x7f8 GdiThreadLocalInfo : Ptr64 Void
+ 0x800 Win32ClientInfo  : [62] Uint8B
+ 0x9f0 glDispatchTable  : [233] Ptr64 Void
+ 0x1138 glReserved1      : [29] Uint8B
+ 0x1220 glReserved2      : Ptr64 Void
+ 0x1228 glSectionInfo    : Ptr64 Void
+ 0x1230 glSection        : Ptr64 Void
+ 0x1238 glTable          : Ptr64 Void
+ 0x1240 glCurrentRC      : Ptr64 Void
+ 0x1248 glContext        : Ptr64 Void
+ 0x1250 LastStatusValue  : Uint4B
+ 0x1254 Padding2         : [4] UChar
+ 0x1258 StaticUnicodeString : _UNICODE_STRING
+ 0x1268 StaticUnicodeBuffer : [261] Wchar
+ 0x1472 Padding3         : [6] UChar
+ 0x1478 DeallocationStack : Ptr64 Void
+ 0x1480 TlsSlots         : [64] Ptr64 Void
+ 0x1680 TlsLinks         : _LIST_ENTRY
+ 0x1690 Vdm              : Ptr64 Void
+ 0x1698 ReservedForNtRpc : Ptr64 Void
+ 0x16a0 DbgSsReserved    : [2] Ptr64 Void
+ 0x16b0 HardErrorMode    : Uint4B
+ 0x16b4 Padding4         : [4] UChar
+ 0x16b8 Instrumentation  : [11] Ptr64 Void
+ 0x1710 ActivityId       : _GUID
+ 0x1720 SubProcessTag    : Ptr64 Void
+ 0x1728 PerflibData      : Ptr64 Void
+ 0x1730 EtwTraceData     : Ptr64 Void
+ 0x1738 WinSockData      : Ptr64 Void
+ 0x1740 GdiBatchCount    : Uint4B
+ 0x1744 CurrentIdealProcessor : _PROCESSOR_NUMBER
+ 0x1744 IdealProcessorValue : Uint4B
+ 0x1744 ReservedPad0     : UChar
+ 0x1745 ReservedPad1     : UChar
+ 0x1746 ReservedPad2     : UChar
+ 0x1747 IdealProcessor   : UChar
+ 0x1748 GuaranteedStackBytes : Uint4B
+ 0x174c Padding5         : [4] UChar
+ 0x1750 ReservedForPerf  : Ptr64 Void
+ 0x1758 ReservedForOle   : Ptr64 Void
+ 0x1760 WaitingOnLoaderLock : Uint4B
+ 0x1764 Padding6         : [4] UChar
+ 0x1768 SavedPriorityState : Ptr64 Void
+ 0x1770 ReservedForCodeCoverage : Uint8B
+ 0x1778 ThreadPoolData   : Ptr64 Void
+ 0x1780 TlsExpansionSlots : Ptr64 Ptr64 Void
+ 0x1788 DeallocationBStore : Ptr64 Void
+ 0x1790 BStoreLimit      : Ptr64 Void
+ 0x1798 MuiGeneration    : Uint4B
+ 0x179c IsImpersonating  : Uint4B
+ 0x17a0 NlsCache         : Ptr64 Void
+ 0x17a8 pShimData        : Ptr64 Void
+ 0x17b0 HeapVirtualAffinity : Uint2B
+ 0x17b2 LowFragHeapDataSlot : Uint2B
+ 0x17b4 Padding7         : [4] UChar
+ 0x17b8 CurrentTransactionHandle : Ptr64 Void
+ 0x17c0 ActiveFrame      : Ptr64 _TEB_ACTIVE_FRAME
+ 0x17c8 FlsData          : Ptr64 Void
+ 0x17d0 PreferredLanguages : Ptr64 Void
+ 0x17d8 UserPrefLanguages : Ptr64 Void
+ 0x17e0 MergedPrefLanguages : Ptr64 Void
+ 0x17e8 MuiImpersonation : Uint4B
+ 0x17ec CrossTebFlags    : Uint2B
+ 0x17ec SpareCrossTebBits : Pos 0, 16 Bits
+ 0x17ee SameTebFlags     : Uint2B
+ 0x17ee SafeThunkCall    : Pos 0, 1 Bit
+ 0x17ee InDebugPrint     : Pos 1, 1 Bit
+ 0x17ee HasFiberData     : Pos 2, 1 Bit
+ 0x17ee SkipThreadAttach : Pos 3, 1 Bit
+ 0x17ee WerInShipAssertCode : Pos 4, 1 Bit
+ 0x17ee RanProcessInit   : Pos 5, 1 Bit
+ 0x17ee ClonedThread     : Pos 6, 1 Bit
+ 0x17ee SuppressDebugMsg : Pos 7, 1 Bit
+ 0x17ee DisableUserStackWalk : Pos 8, 1 Bit
+ 0x17ee RtlExceptionAttached : Pos 9, 1 Bit
+ 0x17ee InitialThread    : Pos 10, 1 Bit
+ 0x17ee SessionAware     : Pos 11, 1 Bit
+ 0x17ee LoadOwner        : Pos 12, 1 Bit
+ 0x17ee LoaderWorker     : Pos 13, 1 Bit
+ 0x17ee SkipLoaderInit   : Pos 14, 1 Bit
+ 0x17ee SpareSameTebBits : Pos 15, 1 Bit
+ 0x17f0 TxnScopeEnterCallback : Ptr64 Void
+ 0x17f8 TxnScopeExitCallback : Ptr64 Void
+ 0x1800 TxnScopeContext  : Ptr64 Void
+ 0x1808 LockCount        : Uint4B
+ 0x180c WowTebOffset     : Int4B
+ 0x1810 ResourceRetValue : Ptr64 Void
+ 0x1818 ReservedForWdf   : Ptr64 Void
+ 0x1820 ReservedForCrt   : Uint8B
+ 0x1828 EffectiveContainerId : _GUID

### TEB
+ 0x000 NtTib            : _NT_TIB
+ 0x038 EnvironmentPointer : Ptr64 Void
+ 0x040 ClientId         : _CLIENT_ID
+ 0x050 ActiveRpcHandle  : Ptr64 Void
+ 0x058 ThreadLocalStoragePointer : Ptr64 Void
+ 0x060 ProcessEnvironmentBlock : Ptr64 _PEB
+ 0x068 LastErrorValue   : Uint4B
+ 0x06c CountOfOwnedCriticalSections : Uint4B
+ 0x070 CsrClientThread  : Ptr64 Void
+ 0x078 Win32ThreadInfo  : Ptr64 Void
+ 0x080 User32Reserved   : [26] Uint4B
+ 0x0e8 UserReserved     : [5] Uint4B
+ 0x100 WOW32Reserved    : Ptr64 Void
+ 0x108 CurrentLocale    : Uint4B
+ 0x10c FpSoftwareStatusRegister : Uint4B
+ 0x110 ReservedForDebuggerInstrumentation : [16] Ptr64 Void
+ 0x190 SystemReserved1  : [32] Ptr64 Void
+ 0x290 _ActivationStack : _ACTIVATION_CONTEXT_STACK
+ 0x2b8 WorkingOnBehalfTicket : [8] UChar
+ 0x2c0 ExceptionCode    : Int4B
+ 0x2c4 Padding0         : [4] UChar
+ 0x2c8 ActivationContextStackPointer : Ptr64 _ACTIVATION_CONTEXT_STACK
+ 0x2d0 InstrumentationCallbackSp : Uint8B
+ 0x2d8 InstrumentationCallbackPreviousPc : Uint8B
+ 0x2e0 InstrumentationCallbackPreviousSp : Uint8B
+ 0x2e8 TxFsContext      : Uint4B
+ 0x2ec InstrumentationCallbackDisabled : UChar
+ 0x2ed Padding1         : [3] UChar
+ 0x2f0 GdiTebBatch      : _GDI_TEB_BATCH
+ 0x7d8 RealClientId     : _CLIENT_ID
+ 0x7e8 GdiCachedProcessHandle : Ptr64 Void
+ 0x7f0 GdiClientPID     : Uint4B
+ 0x7f4 GdiClientTID     : Uint4B
+ 0x7f8 GdiThreadLocalInfo : Ptr64 Void
+ 0x800 Win32ClientInfo  : [62] Uint8B
+ 0x9f0 glDispatchTable  : [233] Ptr64 Void
+ 0x1138 glReserved1      : [29] Uint8B
+ 0x1220 glReserved2      : Ptr64 Void
+ 0x1228 glSectionInfo    : Ptr64 Void
+ 0x1230 glSection        : Ptr64 Void
+ 0x1238 glTable          : Ptr64 Void
+ 0x1240 glCurrentRC      : Ptr64 Void
+ 0x1248 glContext        : Ptr64 Void
+ 0x1250 LastStatusValue  : Uint4B
+ 0x1254 Padding2         : [4] UChar
+ 0x1258 StaticUnicodeString : _UNICODE_STRING
+ 0x1268 StaticUnicodeBuffer : [261] Wchar
+ 0x1472 Padding3         : [6] UChar
+ 0x1478 DeallocationStack : Ptr64 Void
+ 0x1480 TlsSlots         : [64] Ptr64 Void
+ 0x1680 TlsLinks         : _LIST_ENTRY
+ 0x1690 Vdm              : Ptr64 Void
+ 0x1698 ReservedForNtRpc : Ptr64 Void
+ 0x16a0 DbgSsReserved    : [2] Ptr64 Void
+ 0x16b0 HardErrorMode    : Uint4B
+ 0x16b4 Padding4         : [4] UChar
+ 0x16b8 Instrumentation  : [11] Ptr64 Void
+ 0x1710 ActivityId       : _GUID
+ 0x1720 SubProcessTag    : Ptr64 Void
+ 0x1728 PerflibData      : Ptr64 Void
+ 0x1730 EtwTraceData     : Ptr64 Void
+ 0x1738 WinSockData      : Ptr64 Void
+ 0x1740 GdiBatchCount    : Uint4B
+ 0x1744 CurrentIdealProcessor : _PROCESSOR_NUMBER
+ 0x1744 IdealProcessorValue : Uint4B
+ 0x1744 ReservedPad0     : UChar
+ 0x1745 ReservedPad1     : UChar
+ 0x1746 ReservedPad2     : UChar
+ 0x1747 IdealProcessor   : UChar
+ 0x1748 GuaranteedStackBytes : Uint4B
+ 0x174c Padding5         : [4] UChar
+ 0x1750 ReservedForPerf  : Ptr64 Void
+ 0x1758 ReservedForOle   : Ptr64 Void
+ 0x1760 WaitingOnLoaderLock : Uint4B
+ 0x1764 Padding6         : [4] UChar
+ 0x1768 SavedPriorityState : Ptr64 Void
+ 0x1770 ReservedForCodeCoverage : Uint8B
+ 0x1778 ThreadPoolData   : Ptr64 Void
+ 0x1780 TlsExpansionSlots : Ptr64 Ptr64 Void
+ 0x1788 DeallocationBStore : Ptr64 Void
+ 0x1790 BStoreLimit      : Ptr64 Void
+ 0x1798 MuiGeneration    : Uint4B
+ 0x179c IsImpersonating  : Uint4B
+ 0x17a0 NlsCache         : Ptr64 Void
+ 0x17a8 pShimData        : Ptr64 Void
+ 0x17b0 HeapVirtualAffinity : Uint2B
+ 0x17b2 LowFragHeapDataSlot : Uint2B
+ 0x17b4 Padding7         : [4] UChar
+ 0x17b8 CurrentTransactionHandle : Ptr64 Void
+ 0x17c0 ActiveFrame      : Ptr64 _TEB_ACTIVE_FRAME
+ 0x17c8 FlsData          : Ptr64 Void
+ 0x17d0 PreferredLanguages : Ptr64 Void
+ 0x17d8 UserPrefLanguages : Ptr64 Void
+ 0x17e0 MergedPrefLanguages : Ptr64 Void
+ 0x17e8 MuiImpersonation : Uint4B
+ 0x17ec CrossTebFlags    : Uint2B
+ 0x17ec SpareCrossTebBits : Pos 0, 16 Bits
+ 0x17ee SameTebFlags     : Uint2B
+ 0x17ee SafeThunkCall    : Pos 0, 1 Bit
+ 0x17ee InDebugPrint     : Pos 1, 1 Bit
+ 0x17ee HasFiberData     : Pos 2, 1 Bit
+ 0x17ee SkipThreadAttach : Pos 3, 1 Bit
+ 0x17ee WerInShipAssertCode : Pos 4, 1 Bit
+ 0x17ee RanProcessInit   : Pos 5, 1 Bit
+ 0x17ee ClonedThread     : Pos 6, 1 Bit
+ 0x17ee SuppressDebugMsg : Pos 7, 1 Bit
+ 0x17ee DisableUserStackWalk : Pos 8, 1 Bit
+ 0x17ee RtlExceptionAttached : Pos 9, 1 Bit
+ 0x17ee InitialThread    : Pos 10, 1 Bit
+ 0x17ee SessionAware     : Pos 11, 1 Bit
+ 0x17ee LoadOwner        : Pos 12, 1 Bit
+ 0x17ee LoaderWorker     : Pos 13, 1 Bit
+ 0x17ee SkipLoaderInit   : Pos 14, 1 Bit
+ 0x17ee SpareSameTebBits : Pos 15, 1 Bit
+ 0x17f0 TxnScopeEnterCallback : Ptr64 Void
+ 0x17f8 TxnScopeExitCallback : Ptr64 Void
+ 0x1800 TxnScopeContext  : Ptr64 Void
+ 0x1808 LockCount        : Uint4B
+ 0x180c WowTebOffset     : Int4B
+ 0x1810 ResourceRetValue : Ptr64 Void
+ 0x1818 ReservedForWdf   : Ptr64 Void
+ 0x1820 ReservedForCrt   : Uint8B
+ 0x1828 EffectiveContainerId : _GUID
1: kd> dt _NT_TIB
ntdll!_NT_TIB
+ 0x000 ExceptionList    : Ptr64 _EXCEPTION_REGISTRATION_RECORD
+ 0x008 StackBase        : Ptr64 Void
+ 0x010 StackLimit       : Ptr64 Void
+ 0x018 SubSystemTib     : Ptr64 Void
+ 0x020 FiberData        : Ptr64 Void
+ 0x020 Version          : Uint4B
+ 0x028 ArbitraryUserPointer : Ptr64 Void
+ 0x030 Self             : Ptr64 _NT_TIB
