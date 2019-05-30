/*
	Copyright (c) Microsoft Corporation.  All rights reserved.
*/

#include "sdv_prefixer.h"


#define FILE_AUTOGENERATED_DEVICE_NAME  128
#define FILE_DEVICE_DISK_FILE_SYSTEM    8
#define FILE_DEVICE_NETWORK_FILE_SYSTEM 20
#define FILE_DEVICE_SECURE_OPEN         256
#define FILE_DEVICE_TAPE_FILE_SYSTEM    32

#define IRP_MJ_CREATE 0
#define IRP_MJ_CREATE_NAMED_PIPE 1
#define IRP_MJ_CLOSE 2
#define IRP_MJ_READ 3
#define IRP_MJ_WRITE 4
#define IRP_MJ_DEVICE_CONTROL 14
#define IRP_MJ_INTERNAL_DEVICE_CONTROL 15
#define IRP_MJ_POWER 22


#define IRP_MN_REMOVE_DEVICE 2
#define IRP_MN_START_DEVICE 0
#define IRP_MN_SURPRISE_REMOVAL 23

#define IRP_MN_CANCEL_STOP_DEVICE 6
#define IRP_MN_CANCEL_REMOVE_DEVICE 3


#define IRP_MN_QUERY_STOP_DEVICE 5
#define IRP_MN_QUERY_REMOVE_DEVICE 1
#define IRP_MN_QUERY_DEVICE_RELATIONS 7
#define IRP_MN_QUERY_INTERFACE 8
#define IRP_MN_QUERY_CAPABILITIES 9
#define IRP_MN_QUERY_RESOURCES 10
#define IRP_MN_QUERY_RESOURCE_REQUIREMENTS 11
#define IRP_MN_QUERY_DEVICE_TEXT 12
#define IRP_MN_QUERY_ID 19
#define IRP_MN_QUERY_PNP_DEVICE_STATE 20
#define IRP_MN_QUERY_BUS_INFORMATION 21
#define IRP_MN_QUERY_LEGACY_BUS_INFORMATION 24

#define IO_TYPE_ADAPTER                 1
#define IO_TYPE_CONTROLLER              2
#define IO_TYPE_DEVICE                  3
#define IO_TYPE_DRIVER                  4
#define IO_TYPE_FILE                    5
#define IO_TYPE_IRP                     6
#define IO_TYPE_MASTER_ADAPTER          7
#define IO_TYPE_OPEN_PACKET             8
#define IO_TYPE_TIMER                   9
#define IO_TYPE_VPB                     10
#define IO_TYPE_ERROR_LOG               11
#define IO_TYPE_ERROR_MESSAGE           12
#define IO_TYPE_DEVICE_OBJECT_EXTENSION 13

#define NT_SUCCESS(Status) (Status >= 0)
#define NULL 0

#define PowerDeviceD3 4
#define PowerDeviceD2 3
#define PowerDeviceD1 2
#define PowerDeviceD0 1

#define STATUS_NOT_SUPPORTED 3221225659
#define STATUS_PENDING 259
#define STATUS_SUCCESS 0

#define IRP_MJ_PNP 27

#define WMIREG_ACTION_REGISTER      1
#define WMIREG_ACTION_DEREGISTER    2

#define KernelMode 0
#define UserMode 1
#define MaximumMode 2

#define BusRelations 0
#define EjectionRelations 1
#define PowerRelations 2
#define RemovalRelations 3
#define TargetDeviceRelation 4
#define SingleBusRelations 5


#define NonPagedPool 0
#define PagedPool 1
#define NonPagedPoolMustSucceed 2
#define DontUseThisType 3
#define NonPagedPoolCacheAligned 4
#define PagedPoolCacheAligned 5
#define NonPagedPoolCacheAlignedMustS 6


#define OBJ_KERNEL_HANDLE 512


/*
  IRQL

  The SDV Rules specifies IRQL requirements for various DDIs.

  The IRQLs are positive ordered numbers starting from 0.  The actual
  numbers are processer specific, on X86 ranging from 0 to 31 and on
  AMD64 and IA64 ranging from 0 to 15.  For all three platforms
  PASSIVE_LEVEL, APC_LEVEL and DISPATCH_LEVEL are respectively 0, 1
  and 2.

  SDV rules only distinguish between the concrete values
  PASSIVE_LEVEL, APC_LEVEL, DISPATCH_LEVEL and the predicate
  SDV_IRQL_ELEVATED_LEVEL for 'everything else' (3-31 on X86, 3-15 on
  AMD64 or IA64).  The SDV rules are unable to distinguish between
  DIRQL, CMC_LEVEL, HIGH_LEVEL, etc.  The OS header files provide the
  equivalent macro definitions except for the SDV_IRQL_ELEVATED_LEVEL
  predicate.

  The SDV OS Model declares the SDV_DIRQL macro for elevating to an
  arbitrary Device IRQL.  The definition of SDV_DIRQL and the
  definition of SDV_IRQL_ELEVATED_LEVEL implies that
  SDV_IRQL_ELEVATED_LEVEL(SDV_DIRQL)==TRUE.

  See the article "Scheduling, Thread Context, and IRQL" for more
  details.
*/

#define PASSIVE_LEVEL 0 
#define APC_LEVEL 1 
#define DISPATCH_LEVEL 2 

#define HIGH_LEVEL 31
#define POWER_LEVEL 30
#define IPI_LEVEL 29
#define PROFILE_LEVEL 27

#define SDV_IRQL_ELEVATED_LEVEL(irql) ((irql)!=PASSIVE_LEVEL && (irql)!=APC_LEVEL && (irql)!=DISPATCH_LEVEL)


#define STACKL Tail.Overlay.CurrentStackLocation

#define WdfSynchronizationScopeInvalid 0
#define WdfSynchronizationScopeInheritFromParent 1
#define WdfSynchronizationScopeDevice 2
#define WdfSynchronizationScopeQueue 3
#define WdfSynchronizationScopeNone 4

#define WdfExecutionLevelInvalid 0
#define WdfExecutionLevelInheritFromParent 1
#define WdfExecutionLevelPassive 2
#define WdfExecutionLevelDispatch 3



#define WdfIoQueueDispatchInvalid 0
#define WdfIoQueueDispatchSequential 1 
#define WdfIoQueueDispatchParallel 2
#define WdfIoQueueDispatchManual 3
#define WdfIoQueueDispatchMax 4


/*#define	WdfDevStatePowerInvalid 	161
#define WdfDevStatePowerObjectCreated 	162
#define WdfDevStatePowerCheckDeviceType 	163
#define WdfDevStatePowerCheckDeviceTypeNP 	164
#define WdfDevStatePowerCheckParentState 	165
#define WdfDevStatePowerCheckParentStateNP 	166
#define WdfDevStatePowerEnablingWakeAtBus 	167
#define WdfDevStatePowerEnablingWakeAtBusNP 	168
#define WdfDevStatePowerD0 	169
#define WdfDevStatePowerD0NP 	2576
#define WdfDevStatePowerD0BusWakeOwner 	2577
#define WdfDevStatePowerD0BusWakeOwnerNP 	2578
#define WdfDevStatePowerD0ArmedForWake 	2579
#define WdfDevStatePowerD0ArmedForWakeNP 	2580
#define WdfDevStatePowerD0DisarmingWakeAtBus 	2581
#define WdfDevStatePowerD0DisarmingWakeAtBusNP 	2582
#define WdfDevStatePowerD0Starting 	2583
#define WdfDevStatePowerD0StartingConnectInterrupt 	2584
#define WdfDevStatePowerD0StartingDmaEnable 	2585
#define WdfDevStatePowerD0StartingStartSelfManagedIo 	2592
#define WdfDevStatePowerDecideD0State 	2593
#define WdfDevStatePowerGotoD3Stopped 	2594
#define WdfDevStatePowerStopped 	2595
#define WdfDevStatePowerStartingCheckDeviceType 	2596
#define WdfDevStatePowerStartingChild 	2597
#define WdfDevStatePowerDxDisablingWakeAtBus 	2598
#define WdfDevStatePowerDxDisablingWakeAtBusNP 	2599
#define WdfDevStatePowerGotoDx 	2600
#define WdfDevStatePowerGotoDxNP 	2601
#define WdfDevStatePowerGotoDxIoStopped 	2608
#define WdfDevStatePowerGotoDxIoStoppedNP 	2609
#define WdfDevStatePowerGotoDxNPFailed 	2610
#define WdfDevStatePowerDx 	2611
#define WdfDevStatePowerDxNP 	2612
#define WdfDevStatePowerGotoDxArmedForWake 	2613
#define WdfDevStatePowerGotoDxArmedForWakeNP 	2614
#define WdfDevStatePowerGotoDxIoStoppedArmedForWake 	2615
#define WdfDevStatePowerGotoDxIoStoppedArmedForWakeNP 	2616
#define WdfDevStatePowerDxArmedForWake 	2617
#define WdfDevStatePowerDxArmedForWakeNP 	2624
#define WdfDevStatePowerCheckParentStateArmedForWake 	2625
#define WdfDevStatePowerCheckParentStateArmedForWakeNP 	2626
#define WdfDevStatePowerWaitForParentArmedForWake 	2627
#define WdfDevStatePowerWaitForParentArmedForWakeNP 	2628
#define WdfDevStatePowerStartSelfManagedIo 	2629
#define WdfDevStatePowerStartSelfManagedIoNP 	2630
#define WdfDevStatePowerStartSelfManagedIoFailed 	2631
#define WdfDevStatePowerStartSelfManagedIoFailedNP 	2632
#define WdfDevStatePowerWaitForParent 	2633
#define WdfDevStatePowerWaitForParentNP 	2640
#define WdfDevStatePowerWakePending 	2641
#define WdfDevStatePowerWakePendingNP 	2642
#define WdfDevStatePowerWaking 	2643
#define WdfDevStatePowerWakingNP 	2644
#define WdfDevStatePowerWakingConnectInterrupt 	2645
#define WdfDevStatePowerWakingConnectInterruptNP 	2646
#define WdfDevStatePowerWakingConnectInterruptFailed 	2647
#define WdfDevStatePowerWakingConnectInterruptFailedNP 	2648
#define WdfDevStatePowerWakingDmaEnable 	2649
#define WdfDevStatePowerWakingDmaEnableNP 	2656
#define WdfDevStatePowerWakingDmaEnableFailed 	2657
#define WdfDevStatePowerWakingDmaEnableFailedNP 	2658
#define WdfDevStatePowerReportPowerUpFailedDerefParent 	2659
#define WdfDevStatePowerReportPowerUpFailed 	2660
#define WdfDevStatePowerPowerFailedPowerDown 	2661
#define WdfDevStatePowerReportPowerDownFailed 	2662
#define WdfDevStatePowerInitialConnectInterruptFailed 	2663
#define WdfDevStatePowerInitialDmaEnableFailed 	2664
#define WdfDevStatePowerInitialSelfManagedIoFailed 	2665
#define WdfDevStatePowerInitialPowerUpFailedDerefParent 	2672
#define WdfDevStatePowerInitialPowerUpFailed 	2673
#define WdfDevStatePowerDxStoppedDisarmWake 	2674
#define WdfDevStatePowerDxStoppedDisarmWakeNP 	2675
#define WdfDevStatePowerGotoDxStoppedDisableInterruptNP 	2676
#define WdfDevStatePowerGotoDxStopped 	2677
#define WdfDevStatePowerDxStopped 	2678
#define WdfDevStatePowerGotoStopped 	2679
#define WdfDevStatePowerStoppedCompleteDx 	2680
#define WdfDevStatePowerDxStoppedDecideDxState 	2681
#define WdfDevStatePowerDxStoppedArmForWake 	2688
#define WdfDevStatePowerDxStoppedArmForWakeNP 	2689
#define WdfDevStatePowerFinalPowerDownFailed 	2690
#define WdfDevStatePowerFinal 	2691
#define WdfDevStatePowerNull 	2692*/
