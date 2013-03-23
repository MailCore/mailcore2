//
//  MCOMessageHeader+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOMESSAGEHEADER_PRIVATE_H_

#define __MAILCORE_MCOMESSAGEHEADER_PRIVATE_H_

#ifdef __cplusplus
namespace mailcore {
    class MessageHeader;
}

@interface MCOMessageHeader (Private)

- (id) initWithMCMessageHeader:(mailcore::MessageHeader *)header;
+ (MCOAddress *) addressWithMCMessageHeader:(mailcore::MessageHeader *)header;

@end
#endif

#endif
