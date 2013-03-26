//
//  MCOIMAPFolderInfo.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MCOIMAPFolderInfo : NSObject

+ (MCOIMAPFolderInfo *) info;

@property (nonatomic, assign) uint32_t uidNext;
@property (nonatomic, assign) uint32_t uidValidity;
@property (nonatomic, assign) uint64_t modSequenceValue;
@property (nonatomic, assign) int messageCount;

@end
