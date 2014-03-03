//
//  MCOAbstractPart+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCOABSTRACTPART_PRIVATE_H

#define MAILCORE_MCOABSTRACTPART_PRIVATE_H

#include <MailCore/MCOAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
    class AbstractPart;
}

@interface MCOAbstractPart (Private)

- (id) initWithMCPart:(mailcore::AbstractPart *)part;

@end

#endif

#endif
