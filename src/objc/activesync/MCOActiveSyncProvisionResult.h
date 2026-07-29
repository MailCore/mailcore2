#ifndef MAILCORE_MCOACTIVESYNCPROVISIONRESULT_H

#define MAILCORE_MCOACTIVESYNCPROVISIONRESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncProvisionResult : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncProvisionStatus status;
@property (nonatomic, assign) MCOActiveSyncProvisionPolicyStatus policyStatus;
@property (nonatomic, copy) NSString * policyKey;
@end

#endif
