#ifndef MAILCORE_MCOACTIVESYNCSETTINGSRESULT_H

#define MAILCORE_MCOACTIVESYNCSETTINGSRESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncSettingsResult : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncSettingsStatus status;
@property (nonatomic, assign) MCOActiveSyncSettingsDeviceInformationStatus deviceInformationStatus;
@end

#endif
