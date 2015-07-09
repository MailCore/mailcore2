//
//  MCZipMac.mm
//  mailcore2
//
//  Created by juliangsp on 7/7/15.
//  Copyright (c) 2015 MailCore. All rights reserved.
//

#import "MCZipMac.h"
#import "NSObject+MCO.h"
#import <Foundation/Foundation.h>

using namespace mailcore;

String * mailcore::TemporaryDirectoryForZip()
{
    NSError * error;
    NSString * newDirString;
    NSURL * directoryURL;
    
    error = nil;
    newDirString = [NSTemporaryDirectory() stringByAppendingPathComponent:[[NSProcessInfo processInfo] globallyUniqueString]];
    directoryURL = [NSURL fileURLWithPath:newDirString isDirectory:YES];
    
    [[NSFileManager defaultManager] createDirectoryAtURL:directoryURL withIntermediateDirectories:YES attributes:nil error:&error];
    
    if (error != nil)
        return nil;
    
    return MCO_FROM_OBJC(String, newDirString);
}
