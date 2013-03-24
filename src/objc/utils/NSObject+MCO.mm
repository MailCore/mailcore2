//
//  NSObject+MCO.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSObject+MCO.h"

#include <typeinfo>

#import "NSData+MCO.h"
#import "NSString+MCO.h"
#import "NSDictionary+MCO.h"
#import "NSArray+MCO.h"
#import "NSValue+MCO.h"

#include "MCBaseTypes.h"
#include "MCUtils.h"

static chash * classHash = NULL;

static void init(void)
{
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        classHash = chash_new(CHASH_DEFAULTSIZE, CHASH_COPYKEY);
    });
}

void MCORegisterClass(Class aClass, const std::type_info * info)
{
    init();
    
    size_t hash_value = info->hash_code();
    chashdatum key;
    chashdatum value;
    key.data = &hash_value;
    key.len = sizeof(hash_value);
    value.data = aClass;
    value.len = 0;
    chash_set(classHash, &key, &value, NULL);
}

static Class classWithTypeInfo(const std::type_info * info)
{
    init();
    
    size_t hash_value = info->hash_code();
    int r;
    chashdatum key;
    chashdatum value;
    key.data = &hash_value;
    key.len = sizeof(hash_value);
    r = chash_get(classHash, &key, &value);
    if (r < 0)
        return nil;
    
    return (Class) value.data;
}

@implementation NSObject (MCO)

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    if (object == NULL)
        return nil;
    
    //fprintf(stderr, "typeid: %i %i\n", typeid(* object).hash_code(), typeid(mailcore::Array).hash_code());
    size_t objectType = typeid(* object).hash_code();
    if (objectType == typeid(mailcore::Value).hash_code()) {
        return [NSValue mco_valueWithMCValue:(mailcore::Value *) object];
    }
    else if (objectType == typeid(mailcore::Data).hash_code()) {
        return [NSData mco_dataWithMCData:(mailcore::Data *) object];
    }
    else if (objectType == typeid(mailcore::String).hash_code()) {
        return [NSString mco_stringWithMCString:(mailcore::String *) object];
    }
    else if (objectType == typeid(mailcore::HashMap).hash_code()) {
        return [NSDictionary mco_dictionaryWithMCHashMap:(mailcore::HashMap *) object];
    }
    else if (objectType == typeid(mailcore::Array).hash_code()) {
        return [NSArray mco_arrayWithMCArray:(mailcore::Array *) object];
    }
    else {
        Class aClass = classWithTypeInfo(&typeid(* object));
        MCAssert(aClass != nil);
        return [aClass mco_objectWithMCObject:object];
    }
}

- (mailcore::Object *) mco_mcObject
{
    if ([self isKindOfClass:[NSValue class]]) {
        return [(NSValue *) self mco_mcValue];
    }
    else if ([self isKindOfClass:[NSData class]]) {
        return [(NSData *) self mco_mcData];
    }
    else if ([self isKindOfClass:[NSString class]]) {
        return [(NSString *) self mco_mcString];
    }
    else if ([self isKindOfClass:[NSArray class]]) {
        return [(NSArray *) self mco_mcArray];
    }
    else if ([self isKindOfClass:[NSDictionary class]]) {
        return [(NSDictionary *) self mco_mcHashMap];
    }
    else {
        if (![self respondsToSelector:@selector(mco_mcObject)]) {
            MCAssert(0);
        }
        return [self mco_mcObject];
    }
    
}

@end
