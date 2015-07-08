//
//  MCZipMac.mm
//  mailcore2
//
//  Created by juliangsp on 7/7/15.
//  Copyright (c) 2015 MailCore. All rights reserved.
//

#import "MCZipMac.h"
#import <Foundation/Foundation.h>

string * mailcore::TemporaryDirectoryForZip()
{
    string * result = new std::string([NSTemporaryDirectory() UTF8String]);
    (*result) += "mailcore2-XXXXXX";
    return result;
}