//
//  Created by mronge on 1/31/13.
//


#import "NSError+MCO.h"

#import "MCOConstants.h"

@implementation NSError (MCO)
+ (NSError *)mco_errorWithErrorCode:(mailcore::ErrorCode)code {
    if (code == mailcore::ErrorNone) {
        return nil;
    }

    NSError *error = [NSError errorWithDomain:MCOErrorDomain
                                         code:(int)code
                                     userInfo:nil];
    return error;
}
@end