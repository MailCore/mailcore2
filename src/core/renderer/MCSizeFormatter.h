//
//  MCSizeFormatter.h
//  testUI
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCSIZEFORMATTER_H_
#define __MAILCORE_MCSIZEFORMATTER_H_

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    class String;
    
    class SizeFormatter : public Object {
    public:
        static String * stringWithSize(unsigned int size);
    };
    
}

#endif

#endif
