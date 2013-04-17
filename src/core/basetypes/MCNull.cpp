//
//  MCNull.cpp
//  hermes
//
//  Created by DINH Viêt Hoà on 4/9/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCNull.h"

using namespace mailcore;

Null * Null::null()
{
    Null * result = new Null();
    result->autorelease();
    return result;
}

