//
//  MCOJMAPMessage.mm
//  mailcore2
//

#import "MCOJMAPMessage.h"

#include "MCJMAP.h"
#import "MCOUtils.h"

@implementation MCOJMAPMessage

#define nativeType mailcore::JMAPMessage

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (instancetype) init
{
    nativeType * msg = new nativeType();
    self = [self initWithMCMessage:msg];
    msg->release();
    return self;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCMessage:(nativeType *) object] autorelease];
}

MCO_SYNTHESIZE_NSCODING

MCO_OBJC_SYNTHESIZE_STRING(setIdentifier, identifier)
MCO_OBJC_SYNTHESIZE_STRING(setBlobID, blobID)
MCO_OBJC_SYNTHESIZE_STRING(setThreadID, threadID)
MCO_OBJC_SYNTHESIZE_ARRAY(setMailboxIDs, mailboxIDs)
MCO_OBJC_SYNTHESIZE_ARRAY(setKeywords, keywords)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setSize, size)
MCO_OBJC_SYNTHESIZE_STRING(setPreview, preview)
MCO_OBJC_SYNTHESIZE(AbstractPart, setMainPart, mainPart)
MCO_OBJC_SYNTHESIZE_ARRAY(setTextBody, textBody)
MCO_OBJC_SYNTHESIZE_ARRAY(setHTMLBody, htmlBody)
MCO_OBJC_SYNTHESIZE_ARRAY(setAttachments, attachments)

- (MCOAbstractPart *) partForPartID:(NSString *)partID
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->partForPartID([partID mco_mcString]));
}

@end
