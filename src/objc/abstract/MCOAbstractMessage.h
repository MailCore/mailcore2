//
//  MCOAbstractMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCOMessageHeader;
@class MCOAbstractPart;

@interface MCOAbstractMessage : NSObject

@property (nonatomic, retain) MCOMessageHeader * header;

- (MCOAbstractPart *) partForContentID:(NSString *)contentID;
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

@end
