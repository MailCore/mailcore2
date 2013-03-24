//
//  MCOIMAPFolder.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <mailcore/MCOConstants.h>

@interface MCOIMAPFolder : NSObject

@property (nonatomic, copy) NSString * path;
@property (nonatomic, assign) char delimiter;
@property (nonatomic, assign) MCOIMAPFolderFlag flags;

@end
