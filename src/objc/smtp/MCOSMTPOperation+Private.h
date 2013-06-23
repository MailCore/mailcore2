//
//  MCOSMTPOperation+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOSMTPOPERATION_PRIVATE_H_
#define __MAILCORE_MCOSMTPOPERATION_PRIVATE_H_

@class MCOSMTPSession;

@interface MCOSMTPOperation (Private)

@property (nonatomic, retain) MCOSMTPSession * session;

@end

#endif
