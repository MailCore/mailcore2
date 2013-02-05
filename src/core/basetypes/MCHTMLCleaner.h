//
//  HTMLCleaner.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 2/3/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__HTMLCleaner__
#define __mailcore2__HTMLCleaner__

#include <mailcore/MCString.h>

namespace mailcore {
    
    class HTMLCleaner {
    public:
        static String * cleanHTML(String * input);
    };
    
}

#endif /* defined(__mailcore2__HTMLCleaner__) */
