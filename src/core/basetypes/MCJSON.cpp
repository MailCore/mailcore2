//
//  MCJSON.cpp
//  hermes
//
//  Created by DINH Viêt Hoà on 4/8/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCJSON.h"

#include <string.h>

#include "libjson.h"
#include "MCNull.h"
#include <MailCore/MCUtils.h>
#include <MailCore/MCAssert.h>

using namespace mailcore;

String * JSON::objectToJSONString(Object * object, bool pretty)
{
    void * node = objectToJSON(object);
    char * cstr;
    if (pretty) {
        cstr = json_write_formatted(node);
    }
    else {
        cstr = json_write(node);
    }
    
    String * result = String::stringWithUTF8Characters(cstr);
    
    json_free(cstr);
    json_delete(node);
    
    return result;
}

Data * JSON::objectToJSONData(Object * object, bool pretty)
{
    void * node = objectToJSON(object);
    char * cstr;
    if (pretty) {
        cstr = json_write_formatted(node);
    }
    else {
        cstr = json_write(node);
    }
    
    Data * result = Data::dataWithBytes(cstr, (unsigned int) strlen(cstr));
    
    json_free(cstr);
    json_delete(node);
    
    return result;
}

Object * JSON::objectFromJSONString(String * json)
{
    void * node = json_parse(json->UTF8Characters());
    Object * object = objectFromJSON(node);
    json_delete(node);
    return object;
}

Object * JSON::objectFromJSONData(Data * json)
{
    //printf("rcv: [%.*s]\n", (int) json->length(), json->bytes());
    
    void * node = json_parse(json->bytes());
    Object * object = objectFromJSON(node);
    json_delete(node);
    return object;
}

void * JSON::hashMapToJSON(HashMap * hashmap)
{
    JSONNODE * result = json_new(JSON_NODE);
    Array * keys = hashmap->allKeys();
    for(unsigned int i = 0 ; i < keys->count() ; i ++) {
        String * key = (String *) keys->objectAtIndex(i);
        JSONNODE * item = objectToJSON(hashmap->objectForKey(key));
        json_set_name(item, key->UTF8Characters());
        json_push_back(result, item);
    }
    return result;
}

void * JSON::arrayToJSON(Array * array)
{
    JSONNODE * result = json_new(JSON_ARRAY);
    for(unsigned int i = 0 ; i < array->count() ; i ++) {
        Object * value = array->objectAtIndex(i);
        JSONNODE * item = objectToJSON(value);
        json_push_back(result, item);
    }
    return result;
}

void * JSON::stringToJSON(String * string)
{
    return json_new_a("", string->UTF8Characters());
}

void * JSON::doubleToJSON(Value * value)
{
    switch (value->type()) {
        case ValueTypeChar:
            return json_new_f("", value->charValue());
        case ValueTypeUnsignedChar:
            return json_new_f("", value->unsignedCharValue());
        case ValueTypeShort:
            return json_new_f("", value->shortValue());
        case ValueTypeUnsignedShort:
            return json_new_f("", value->unsignedShortValue());
        case ValueTypeInt:
            return json_new_f("", value->intValue());
        case ValueTypeUnsignedInt:
            return json_new_f("", value->unsignedIntValue());
        case ValueTypeLong:
            return json_new_f("", value->longValue());
        case ValueTypeUnsignedLong:
            return json_new_f("", value->unsignedLongValue());
        case ValueTypeLongLong:
            return json_new_f("", value->longLongValue());
        case ValueTypeUnsignedLongLong:
            return json_new_f("", value->unsignedLongLongValue());
        case ValueTypeFloat:
            return json_new_f("", value->floatValue());
        case ValueTypeDouble:
            return json_new_f("", value->doubleValue());
        default:
            return json_new_f("", 0.);
    }
}

void * JSON::boolToJSON(Value * value)
{
    return json_new_b("", value->boolValue());
}

void * JSON::nullToJSON(Null * value)
{
    return json_new(JSON_NULL);
}

void * JSON::objectToJSON(Object * object)
{
    String * name = object->className();
    if (name->isEqual(MCSTR("mailcore::Null"))) {
        return nullToJSON((Null *) object);
    }
    else if (name->isEqual(MCSTR("mailcore::Value"))) {
        Value * value = (Value *) object;
        if (value->type() == ValueTypeBool) {
            return boolToJSON(value);
        }
        else {
            return doubleToJSON(value);
        }
    }
    else if (name->isEqual(MCSTR("mailcore::String"))) {
        return stringToJSON((String *) object);
    }
    else if (name->isEqual(MCSTR("mailcore::Array"))) {
        return arrayToJSON((Array *) object);
    }
    else if (name->isEqual(MCSTR("mailcore::HashMap"))) {
        return hashMapToJSON((HashMap *) object);
    }
    else {
        MCAssert(0);
        return NULL;
    }
}

HashMap * JSON::hashMapFromJSON(void * json)
{
    HashMap * result = HashMap::hashMap();
    JSONNODE_ITERATOR i = json_begin(json);
    while (i != json_end(json)){
        json_char * ckey = json_name(*i);
        String * key = String::stringWithUTF8Characters(ckey);
        Object * value = objectFromJSON(* i);
        
        result->setObjectForKey(key, value);
        
        json_free(ckey);
        ++i;
    }
    return result;
}

Array * JSON::arrayFromJSON(void * json)
{
    Array * result = Array::array();
    JSONNODE_ITERATOR i = json_begin(json);
    while (i != json_end(json)){
        Object * obj = objectFromJSON(* i);
        result->addObject(obj);
        ++i;
    }
    return result;
}

String * JSON::stringFromJSON(void * json)
{
    json_char * cstr = json_as_string(json);
    String * result = String::stringWithUTF8Characters(cstr);
    json_free(cstr);
    return result;
}

Value * JSON::doubleFromJSON(void * json)
{
    json_number value = json_as_float(json);
    return Value::valueWithDoubleValue(value);
}

Value * JSON::boolFromJSON(void * json)
{
    json_bool_t value = json_as_bool(json);
    return Value::valueWithBoolValue(value);
}

Null * JSON::nullFromJSON(void * json)
{
    return Null::null();
}

Object * JSON::objectFromJSON(void * json)
{
    switch (json_type(json)) {
        case JSON_NULL:
            return nullFromJSON(json);
        case JSON_STRING:
            return stringFromJSON(json);
        case JSON_NUMBER:
            return doubleFromJSON(json);
        case JSON_BOOL:
            return boolFromJSON(json);
        case JSON_ARRAY:
            return arrayFromJSON(json);
        case JSON_NODE:
            return hashMapFromJSON(json);
        default:
            MCAssert(0);
            return NULL;
    }
}
