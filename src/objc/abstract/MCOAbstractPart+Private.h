//
//  MCOAbstractPart+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include <mailcore/MCOAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
    class AbstractPart;
}

@interface MCOAbstractPart (Private)

- (id) initWithPart:(mailcore::AbstractPart *)part;

@end

#endif
