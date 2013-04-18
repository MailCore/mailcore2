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
        static String * objectToJSONString(Object * object, bool pretty = false);
        static Data * objectToJSONData(Object * object, bool pretty = false);
        static Object * objectFromJSONString(String * json);
        static Object * objectFromJSONData(Data * json);
        
    private:
        static void * hashMapToJSON(HashMap * hashmap);
        static void * arrayToJSON(Array * array);
        static void * stringToJSON(String * string);
        static void * doubleToJSON(Value * value);
        static void * boolToJSON(Value * value);
        static void * nullToJSON(Null * value);
        static void * objectToJSON(Object * object);
        
        static HashMap * hashMapFromJSON(void * json);
        static Array * arrayFromJSON(void * json);
        static String * stringFromJSON(void * json);
        static Value * doubleFromJSON(void * json);
        static Value * boolFromJSON(void * json);
        static Null * nullFromJSON(void * json);
        static Object * objectFromJSON(void * json);
    };
    
}

#endif

#endif /* defined(__hermes__MCJSON__) */
