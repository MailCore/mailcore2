//
//  MCOHTMLRendererIMAPDelegate.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOHTMLRENDERERIMAPDELEGATE_H_

#define __MAILCORE_MCOHTMLRENDERERIMAPDELEGATE_H_

#import <MailCore/MCOHTMLRendererDelegate.h>

@class MCOIMAPPart;

@protocol MCOHTMLRendererIMAPDelegate <MCOHTMLRendererDelegate>

- (NSData *) MCOAbstractMessage:(MCOAbstractMessage *)msg dataForIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder;
- (void) MCOAbstractMessage:(MCOAbstractMessage *)msg prefetchAttachmentIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder;
- (void) MCOAbstractMessage:(MCOAbstractMessage *)msg prefetchImageIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder;

@end

#endif
