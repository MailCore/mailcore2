//
//  MCNull.h
//  hermes
//
//  Created by DINH Viêt Hoà on 4/9/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCNULL_H_
#define __MAILCORE_MCNULL_H_

#include <MailCore/MCObject.h>

#ifdef __cplusplus

namespace mailcore {
    
    class Null : public Object {
    public:
        static Null * null();
    };
    
}

#endif

#endif
