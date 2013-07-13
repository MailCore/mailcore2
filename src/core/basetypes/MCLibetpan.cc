//
//  MCLibetpan.cpp
//  mailcore2
//
//  Created by Hoa Dinh on 6/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCLibetpan.h"

#include <libetpan/libetpan.h>

__attribute__((constructor))
static void initialize() {
    // It will enable CFStream on platforms that supports it.
    mailstream_cfstream_enabled = 1;
}
