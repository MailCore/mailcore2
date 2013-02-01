//
//  Created by mronge on 1/31/13.
//


#import "NSError+MCO.h"

@implementation NSError (MCO)
+ (NSError *)mco_errorWithErrorCode:(mailcore::ErrorCode)code {
    if (code == mailcore::ErrorNone) {
        return nil;
    }

    NSError *error = [NSError errorWithDomain:@"MailCore" code:(int)code
                                     userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Unknown error: %d", (int)code]}];
    return error;
}
@end