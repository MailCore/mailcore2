//
//  MCJSON.h
//  hermes
//
//  Created by DINH Viêt Hoà on 4/8/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCJSON_H_
#define __MAILCORE_MCJSON_H_

#include <MailCore/MCObject.h>
#include <MailCore/MCHashMap.h>
#include <MailCore/MCArray.h>
#include <MailCore/MCValue.h>
#include <MailCore/MCString.h>
#include <MailCore/MCData.h>

#ifdef __cplusplus

namespace mailcore {

    class Null;
    
    class JSON {
    public:
        static String * objectToJSONString(Object * object);
        static Data * objectToJSONData(Object * object);
        static Object * objectFromJSONString(String * json);
        static Object * objectFromJSONData(Data * json);
    };
    
}

#endif

#endif /* defined(__hermes__MCJSON__) */
