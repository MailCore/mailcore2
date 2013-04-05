//
//  MCOAbstractMessage+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTMESSAGE_PRIVATE_H_

#define __MAILCORE_MCOABSTRACTMESSAGE_PRIVATE_H_

#ifdef __cplusplus

namespace mailcore {
    class AbstractMessage;
}

@interface MCOAbstractMessage (Private)

- (id) initWithMCMessage:(mailcore::AbstractMessage *)message;

@end

#endif

#endif
