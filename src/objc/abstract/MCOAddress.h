//
//  MCOAddress.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MCOAddress : NSObject <NSCopying>

+ (MCOAddress *) addressWithDisplayName:(NSString *)displayName
                                mailbox:(NSString *)mailbox;

+ (MCOAddress *) addressWithMailbox:(NSString *)mailbox;

+ (MCOAddress *) addressWithRFC822String:(NSString *)RFC822String;

+ (MCOAddress *) addressWithNonEncodedRFC822String:(NSString *)nonEncodedRFC822String;

@property (nonatomic, copy) NSString * displayName;
@property (nonatomic, copy) NSString * mailbox;

- (NSString *) RFC822String;
- (NSString *) nonEncodedRFC822String;

@end
