//
//  Created by mronge on 1/31/13.
//


#import <Foundation/Foundation.h>

#import <MailCore/MCMessageConstants.h>

@interface NSError (MCO)
#ifdef __cplusplus
+ (NSError *)mco_errorWithErrorCode:(mailcore::ErrorCode)code;
#endif
@end