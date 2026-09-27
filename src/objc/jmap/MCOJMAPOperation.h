//
//  MCOJMAPOperation.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPOPERATION_H

#define MAILCORE_MCOJMAPOPERATION_H

#import <MailCore/MCOOperation.h>

@interface MCOJMAPOperation : MCOOperation

- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
