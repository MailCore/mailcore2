//
//  MCOIMAPFolder.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFOLDER_H_

#define __MAILCORE_MCOIMAPFOLDER_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOConstants.h>

@interface MCOIMAPFolder : NSObject <NSCopying>

@property (nonatomic, copy) NSString * path;
@property (nonatomic, assign) char delimiter;
@property (nonatomic, assign) MCOIMAPFolderFlag flags;

@end

#endif
