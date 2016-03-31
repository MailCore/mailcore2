//
//  Created by mronge on 1/31/13.
//


#import "NSError+MCO.h"

#include "MCBaseTypes.h"
#include "MCErrorMessage.h"

#import "MCOConstants.h"
#import "NSObject+MCO.h"

using namespace mailcore;

@implementation NSError (MCO)
+ (NSError *)mco_errorWithErrorCode:(mailcore::ErrorCode)code {
    return [self mco_errorWithErrorCode:code userInfo:nil];
}

+ (NSError *)mco_errorWithErrorCode:(mailcore::ErrorCode)code userInfo:(NSDictionary *)userInfoOverride {
    if (code == mailcore::ErrorNone) {
        return nil;
    }
    
    NSMutableDictionary *userInfo = [[NSMutableDictionary alloc] init];
    if ((NSInteger) code < MCOErrorCodeCount) {
        String * errorMessage = errorMessageWithErrorCode(code);
        if (errorMessage != NULL) {
            NSString * localizedString = NSLocalizedStringFromTable(MCO_TO_OBJC(errorMessage), @"description of errors of mailcore", @"MailCore");
            [userInfo setObject:localizedString forKey:NSLocalizedDescriptionKey];
        }
    }
    
    if (userInfoOverride) {
        [userInfo addEntriesFromDictionary:userInfoOverride];
    }
    NSError *error = [NSError errorWithDomain:MCOErrorDomain
                                         code:(int)code
                                     userInfo:userInfo];
    [userInfo release];
    return error;
}
@end
