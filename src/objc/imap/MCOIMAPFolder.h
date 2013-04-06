//
//  MCOIMAPFolder.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFOLDER_H_

#define __MAILCORE_MCOIMAPFOLDER_H_

// This class implements an IMAP folder.

#import <Foundation/Foundation.h>
#import <MailCore/MCOConstants.h>

@interface MCOIMAPFolder : NSObject <NSCopying>

// It's the path of the folder.
@property (nonatomic, copy) NSString * path;

// It's the delimiter for each component of the path.
@property (nonatomic, assign) char delimiter;

// It's the flags of the folder. See MCOIMAPFolderFlag.
@property (nonatomic, assign) MCOIMAPFolderFlag flags;

@end

#endif
