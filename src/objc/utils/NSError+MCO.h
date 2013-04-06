//
//  Created by mronge on 1/31/13.
//

#ifndef __MAILCORE_NSERROR_MCO_H_

#define __MAILCORE_NSERROR_MCO_H_

#import <Foundation/Foundation.h>

#import <MailCore/MCMessageConstants.h>

@interface NSError (MCO)
#ifdef __cplusplus
+ (NSError *) mco_errorWithErrorCode:(mailcore::ErrorCode)code;
#endif
@end

#endif
