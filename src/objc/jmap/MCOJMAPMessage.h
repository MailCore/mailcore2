//
//  MCOJMAPMessage.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPMESSAGE_H

#define MAILCORE_MCOJMAPMESSAGE_H

#import <MailCore/MCOAbstractMessage.h>

@class MCOAbstractPart;

@interface MCOJMAPMessage : MCOAbstractMessage

@property (nonatomic, copy) NSString * identifier;
@property (nonatomic, copy) NSString * blobID;
@property (nonatomic, copy) NSString * threadID;
@property (nonatomic, copy) NSArray<NSString *> * mailboxIDs;
@property (nonatomic, copy) NSArray<NSString *> * keywords;
@property (nonatomic, assign) unsigned int size;
@property (nonatomic, copy) NSString * preview;
@property (nonatomic, strong) MCOAbstractPart * mainPart;
@property (nonatomic, copy) NSArray<MCOAbstractPart *> * textBody;
@property (nonatomic, copy) NSArray<MCOAbstractPart *> * htmlBody;
@property (nonatomic, copy) NSArray<MCOAbstractPart *> * attachments;

- (MCOAbstractPart *) partForPartID:(NSString *)partID;

@end

#endif
