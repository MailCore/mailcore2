//
//  main.m
//  testUI
//
//  Created by DINH Viêt Hoà on 1/19/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <MailCore/MailCore.h>
#include <unicode/putil.h>

int main(int argc, char *argv[])
{
    u_setDataDirectory("/usr/local/share/icu");
    mailcore::AutoreleasePool * pool = new mailcore::AutoreleasePool();
    return NSApplicationMain(argc, (const char **)argv);
    pool->release();
}
