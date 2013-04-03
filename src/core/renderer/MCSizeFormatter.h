//
//  MCSizeFormatter.h
//  testUI
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __testUI__MCSizeFormatter__
#define __testUI__MCSizeFormatter__

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

#endif /* defined(__testUI__MCSizeFormatter__) */
