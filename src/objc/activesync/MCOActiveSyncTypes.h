#ifndef MAILCORE_MCOACTIVESYNCTYPES_H

#define MAILCORE_MCOACTIVESYNCTYPES_H

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, MCOActiveSyncBodyType) {
    MCOActiveSyncBodyTypeUnknown = 0,
    MCOActiveSyncBodyTypePlainText = 1,
    MCOActiveSyncBodyTypeHTML = 2,
    MCOActiveSyncBodyTypeRTF = 3,
    MCOActiveSyncBodyTypeMIME = 4,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncAttachmentMethod) {
    MCOActiveSyncAttachmentMethodUnknown = 0,
    MCOActiveSyncAttachmentMethodNormal = 1,
    MCOActiveSyncAttachmentMethodEmbeddedMessage = 5,
    MCOActiveSyncAttachmentMethodOLE = 6,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncFilterType) {
    MCOActiveSyncFilterTypeNone = 0,
    MCOActiveSyncFilterTypeOneDay = 1,
    MCOActiveSyncFilterTypeThreeDays = 2,
    MCOActiveSyncFilterTypeOneWeek = 3,
    MCOActiveSyncFilterTypeTwoWeeks = 4,
    MCOActiveSyncFilterTypeOneMonth = 5,
    MCOActiveSyncFilterTypeThreeMonths = 6,
    MCOActiveSyncFilterTypeSixMonths = 7,
    MCOActiveSyncFilterTypeIncompleteTasks = 8,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncFolderSyncStatus) {
    MCOActiveSyncFolderSyncStatusUnknown = 0,
    MCOActiveSyncFolderSyncStatusSuccess = 1,
    MCOActiveSyncFolderSyncStatusServerError = 6,
    MCOActiveSyncFolderSyncStatusInvalidSyncKey = 9,
    MCOActiveSyncFolderSyncStatusMalformedRequest = 10,
    MCOActiveSyncFolderSyncStatusUnknownError = 11,
    MCOActiveSyncFolderSyncStatusCodeUnknown = 12,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncSyncStatus) {
    MCOActiveSyncSyncStatusUnknown = 0,
    MCOActiveSyncSyncStatusSuccess = 1,
    MCOActiveSyncSyncStatusInvalidSyncKey = 3,
    MCOActiveSyncSyncStatusProtocolError = 4,
    MCOActiveSyncSyncStatusServerError = 5,
    MCOActiveSyncSyncStatusConversionError = 6,
    MCOActiveSyncSyncStatusConflictResolved = 7,
    MCOActiveSyncSyncStatusObjectNotFound = 8,
    MCOActiveSyncSyncStatusMoreAvailable = 9,
    MCOActiveSyncSyncStatusAccountResyncRequired = 12,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncProvisionStatus) {
    MCOActiveSyncProvisionStatusUnknown = 0,
    MCOActiveSyncProvisionStatusSuccess = 1,
    MCOActiveSyncProvisionStatusProtocolError = 2,
    MCOActiveSyncProvisionStatusServerError = 3,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncProvisionPolicyStatus) {
    MCOActiveSyncProvisionPolicyStatusUnknown = 0,
    MCOActiveSyncProvisionPolicyStatusSuccess = 1,
    MCOActiveSyncProvisionPolicyStatusNoPolicy = 2,
    MCOActiveSyncProvisionPolicyStatusUnknownPolicyType = 3,
    MCOActiveSyncProvisionPolicyStatusPolicyDataCorrupt = 4,
    MCOActiveSyncProvisionPolicyStatusPolicyKeyMismatch = 5,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncSettingsStatus) {
    MCOActiveSyncSettingsStatusUnknown = 0,
    MCOActiveSyncSettingsStatusSuccess = 1,
    MCOActiveSyncSettingsStatusProtocolError = 2,
    MCOActiveSyncSettingsStatusAccessDenied = 3,
    MCOActiveSyncSettingsStatusServerUnavailable = 4,
    MCOActiveSyncSettingsStatusInvalidArguments = 5,
    MCOActiveSyncSettingsStatusConflictingArguments = 6,
    MCOActiveSyncSettingsStatusDeniedByPolicy = 7,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncSettingsDeviceInformationStatus) {
    MCOActiveSyncSettingsDeviceInformationStatusUnknown = 0,
    MCOActiveSyncSettingsDeviceInformationStatusSuccess = 1,
    MCOActiveSyncSettingsDeviceInformationStatusProtocolError = 2,
    MCOActiveSyncSettingsDeviceInformationStatusAccessDenied = 3,
    MCOActiveSyncSettingsDeviceInformationStatusServerUnavailable = 4,
    MCOActiveSyncSettingsDeviceInformationStatusInvalidArguments = 5,
    MCOActiveSyncSettingsDeviceInformationStatusConflictingArguments = 6,
    MCOActiveSyncSettingsDeviceInformationStatusDeniedByPolicy = 7,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncItemEstimateStatus) {
    MCOActiveSyncItemEstimateStatusUnknown = 0,
    MCOActiveSyncItemEstimateStatusSuccess = 1,
    MCOActiveSyncItemEstimateStatusInvalidCollection = 2,
    MCOActiveSyncItemEstimateStatusSyncStateNotPrimed = 3,
    MCOActiveSyncItemEstimateStatusInvalidSyncKey = 4,
};

typedef NS_ENUM(NSInteger, MCOActiveSyncPingStatus) {
    MCOActiveSyncPingStatusUnknown = 0,
    MCOActiveSyncPingStatusExpired = 1,
    MCOActiveSyncPingStatusChanges = 2,
    MCOActiveSyncPingStatusMissingParameters = 3,
    MCOActiveSyncPingStatusSyntaxError = 4,
    MCOActiveSyncPingStatusInvalidHeartbeatInterval = 5,
    MCOActiveSyncPingStatusTooManyFolders = 6,
    MCOActiveSyncPingStatusFolderSyncRequired = 7,
    MCOActiveSyncPingStatusServerError = 8,
};

#endif
