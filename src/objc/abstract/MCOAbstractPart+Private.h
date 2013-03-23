//
//  MCOAbstractPart+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTPART_PRIVATE_H_

#define __MAILCORE_MCOABSTRACTPART_PRIVATE_H_

#include <mailcore/MCOAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
    class AbstractPart;
}

@interface MCOAbstractPart (Private)

- (id) initWithMCPart:(mailcore::AbstractPart *)part;

@end

#endif

#endif
