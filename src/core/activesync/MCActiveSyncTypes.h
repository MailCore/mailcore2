#ifndef MAILCORE_MCACTIVESYNCTYPES_H

#define MAILCORE_MCACTIVESYNCTYPES_H

#ifdef __cplusplus

namespace mailcore {

    enum ActiveSyncBodyType {
        ActiveSyncBodyTypeUnknown = 0,
        ActiveSyncBodyTypePlainText = 1,
        ActiveSyncBodyTypeHTML = 2,
        ActiveSyncBodyTypeRTF = 3,
        ActiveSyncBodyTypeMIME = 4,
    };

    enum ActiveSyncAttachmentMethod {
        ActiveSyncAttachmentMethodUnknown = 0,
        ActiveSyncAttachmentMethodNormal = 1,
        ActiveSyncAttachmentMethodEmbeddedMessage = 5,
        ActiveSyncAttachmentMethodOLE = 6,
    };

    enum ActiveSyncFilterType {
        ActiveSyncFilterTypeNone = 0,
        ActiveSyncFilterTypeOneDay = 1,
        ActiveSyncFilterTypeThreeDays = 2,
        ActiveSyncFilterTypeOneWeek = 3,
        ActiveSyncFilterTypeTwoWeeks = 4,
        ActiveSyncFilterTypeOneMonth = 5,
        ActiveSyncFilterTypeThreeMonths = 6,
        ActiveSyncFilterTypeSixMonths = 7,
        ActiveSyncFilterTypeIncompleteTasks = 8,
    };

    enum ActiveSyncFolderSyncStatus {
        ActiveSyncFolderSyncStatusUnknown = 0,
        ActiveSyncFolderSyncStatusSuccess = 1,
        ActiveSyncFolderSyncStatusServerError = 6,
        ActiveSyncFolderSyncStatusInvalidSyncKey = 9,
        ActiveSyncFolderSyncStatusMalformedRequest = 10,
        ActiveSyncFolderSyncStatusUnknownError = 11,
        ActiveSyncFolderSyncStatusCodeUnknown = 12,
    };

    enum ActiveSyncSyncStatus {
        ActiveSyncSyncStatusUnknown = 0,
        ActiveSyncSyncStatusSuccess = 1,
        ActiveSyncSyncStatusInvalidSyncKey = 3,
        ActiveSyncSyncStatusProtocolError = 4,
        ActiveSyncSyncStatusServerError = 5,
        ActiveSyncSyncStatusConversionError = 6,
        ActiveSyncSyncStatusConflictResolved = 7,
        ActiveSyncSyncStatusObjectNotFound = 8,
        ActiveSyncSyncStatusMoreAvailable = 9,
        ActiveSyncSyncStatusAccountResyncRequired = 12,
    };

    enum ActiveSyncProvisionStatus {
        ActiveSyncProvisionStatusUnknown = 0,
        ActiveSyncProvisionStatusSuccess = 1,
        ActiveSyncProvisionStatusProtocolError = 2,
        ActiveSyncProvisionStatusServerError = 3,
    };

    enum ActiveSyncProvisionPolicyStatus {
        ActiveSyncProvisionPolicyStatusUnknown = 0,
        ActiveSyncProvisionPolicyStatusSuccess = 1,
        ActiveSyncProvisionPolicyStatusNoPolicy = 2,
        ActiveSyncProvisionPolicyStatusUnknownPolicyType = 3,
        ActiveSyncProvisionPolicyStatusPolicyDataCorrupt = 4,
        ActiveSyncProvisionPolicyStatusPolicyKeyMismatch = 5,
    };

    enum ActiveSyncSettingsStatus {
        ActiveSyncSettingsStatusUnknown = 0,
        ActiveSyncSettingsStatusSuccess = 1,
        ActiveSyncSettingsStatusProtocolError = 2,
        ActiveSyncSettingsStatusAccessDenied = 3,
        ActiveSyncSettingsStatusServerUnavailable = 4,
        ActiveSyncSettingsStatusInvalidArguments = 5,
        ActiveSyncSettingsStatusConflictingArguments = 6,
        ActiveSyncSettingsStatusDeniedByPolicy = 7,
    };

    enum ActiveSyncSettingsDeviceInformationStatus {
        ActiveSyncSettingsDeviceInformationStatusUnknown = 0,
        ActiveSyncSettingsDeviceInformationStatusSuccess = 1,
        ActiveSyncSettingsDeviceInformationStatusProtocolError = 2,
        ActiveSyncSettingsDeviceInformationStatusAccessDenied = 3,
        ActiveSyncSettingsDeviceInformationStatusServerUnavailable = 4,
        ActiveSyncSettingsDeviceInformationStatusInvalidArguments = 5,
        ActiveSyncSettingsDeviceInformationStatusConflictingArguments = 6,
        ActiveSyncSettingsDeviceInformationStatusDeniedByPolicy = 7,
    };

    enum ActiveSyncItemEstimateStatus {
        ActiveSyncItemEstimateStatusUnknown = 0,
        ActiveSyncItemEstimateStatusSuccess = 1,
        ActiveSyncItemEstimateStatusInvalidCollection = 2,
        ActiveSyncItemEstimateStatusSyncStateNotPrimed = 3,
        ActiveSyncItemEstimateStatusInvalidSyncKey = 4,
    };

    enum ActiveSyncPingStatus {
        ActiveSyncPingStatusUnknown = 0,
        ActiveSyncPingStatusExpired = 1,
        ActiveSyncPingStatusChanges = 2,
        ActiveSyncPingStatusMissingParameters = 3,
        ActiveSyncPingStatusSyntaxError = 4,
        ActiveSyncPingStatusInvalidHeartbeatInterval = 5,
        ActiveSyncPingStatusTooManyFolders = 6,
        ActiveSyncPingStatusFolderSyncRequired = 7,
        ActiveSyncPingStatusServerError = 8,
    };

    enum ActiveSyncFolderType {
        ActiveSyncFolderTypeUnknown = 0,
        ActiveSyncFolderTypeUserCreatedGeneric = 1,
        ActiveSyncFolderTypeDefaultInbox = 2,
        ActiveSyncFolderTypeDefaultDrafts = 3,
        ActiveSyncFolderTypeDefaultDeletedItems = 4,
        ActiveSyncFolderTypeDefaultSentItems = 5,
        ActiveSyncFolderTypeDefaultOutbox = 6,
        ActiveSyncFolderTypeDefaultTasks = 7,
        ActiveSyncFolderTypeDefaultCalendar = 8,
        ActiveSyncFolderTypeDefaultContacts = 9,
        ActiveSyncFolderTypeDefaultNotes = 10,
        ActiveSyncFolderTypeDefaultJournal = 11,
        ActiveSyncFolderTypeUserCreatedMail = 12,
        ActiveSyncFolderTypeUserCreatedCalendar = 13,
        ActiveSyncFolderTypeUserCreatedContacts = 14,
        ActiveSyncFolderTypeUserCreatedTasks = 15,
        ActiveSyncFolderTypeUserCreatedJournal = 16,
        ActiveSyncFolderTypeUserCreatedNotes = 17,
        ActiveSyncFolderTypeServerUnknown = 18,
        ActiveSyncFolderTypeRecipientInformationCache = 19,
    };

}

#endif

#endif
