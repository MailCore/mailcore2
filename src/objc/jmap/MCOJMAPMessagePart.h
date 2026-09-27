//
//  MCOJMAPMessagePart.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPMESSAGEPART_H

#define MAILCORE_MCOJMAPMESSAGEPART_H

#import <MailCore/MCOAbstractMessagePart.h>

@interface MCOJMAPMessagePart : MCOAbstractMessagePart

@property (nonatomic, copy) NSString * partID;
@property (nonatomic, copy) NSString * blobID;
@property (nonatomic, assign) unsigned int size;

@end

#endif
