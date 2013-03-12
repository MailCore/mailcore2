//
//  MCOMessageHeader.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageHeader.h"
#import "MCOMessageHeader+Private.h"

#include "MCMessageHeader.h"
#include "NSString+MCO.h"
#include "NSArray+MCO.h"
#include "NSObject+MCO.h"
#include "NSData+MCO.h"
#include "MCOAddress.h"
#include "MCOAddress+Private.h"

@implementation MCOMessageHeader {
    mailcore::MessageHeader * _nativeHeader;
}

- (id) init
{
    self = [super init];
    
    _nativeHeader = new mailcore::MessageHeader();
    
    return self;
}

- (id) initWithMCMessageHeader:(mailcore::MessageHeader *)header
{
    self = [super init];
    
    _nativeHeader = header;
    _nativeHeader->retain();
    
    return self;
}

+ (MCOMessageHeader *) messageHeaderWithMCMessageHeader:(mailcore::MessageHeader *)header
{
    if (header == NULL)
        return nil;
    
    return [[[self alloc] initWithMCMessageHeader:header] autorelease];
}

- (id) copyWithZone:(NSZone *)zone
{
    mailcore::MessageHeader * header = (mailcore::MessageHeader *) _nativeHeader->copy();
    MCOMessageHeader * result = [[MCOMessageHeader alloc] initWithMCMessageHeader:header];
    MC_SAFE_RELEASE(header);
    return result;
}

- (NSString *) description
{
    return [NSString mco_stringWithMCObject:_nativeHeader];
}

- (NSString *) messageID
{
    return [NSString mco_stringWithMCString:_nativeHeader->messageID()];
}

- (void) setMessageID:(NSString *)messageID
{
    _nativeHeader->setMessageID([messageID mco_mcString]);
}

- (NSArray *) references
{
    mailcore::Array * msgIDs = _nativeHeader->references();
    if (msgIDs == NULL)
        return nil;
    
    NSMutableArray * result = [NSMutableArray array];
    for(unsigned int i = 0 ; i < msgIDs->count() ; i ++) {
        mailcore::String * nativeMsgID = (mailcore::String *) msgIDs->objectAtIndex(i);
        [result addObject:[NSString mco_stringWithMCString:nativeMsgID]];
    }
    return result;
}

- (void) setReferences:(NSArray *)references
{
    _nativeHeader->setReferences((mailcore::Array *) [references mco_mcArray]);
}

- (NSArray *) inReplyTo
{
    mailcore::Array * msgIDs = _nativeHeader->inReplyTo();
    if (msgIDs == NULL)
        return nil;
    
    NSMutableArray * result = [NSMutableArray array];
    for(unsigned int i = 0 ; i < msgIDs->count() ; i ++) {
        mailcore::String * nativeMsgID = (mailcore::String *) msgIDs->objectAtIndex(i);
        [result addObject:[NSString mco_stringWithMCString:nativeMsgID]];
    }
    return result;
}

- (void) setInReplyTo:(NSArray *)inReplyTo
{
    _nativeHeader->setInReplyTo([inReplyTo mco_mcArray]);
}

- (NSDate *) date
{
    return [NSDate dateWithTimeIntervalSince1970:_nativeHeader->date()];
}

- (void) setDate:(NSDate *)date
{
    _nativeHeader->setDate([date timeIntervalSince1970]);
}

- (NSDate *) receivedDate
{
    return [NSDate dateWithTimeIntervalSince1970:((NSTimeInterval) _nativeHeader->receivedDate())];
}

- (void) setReceivedDate:(NSDate *)receivedDate
{
    _nativeHeader->setReceivedDate(((NSTimeInterval) [receivedDate timeIntervalSince1970]));
}

- (MCOAddress *) sender
{
    return [[[MCOAddress alloc] initWithMCAddress:_nativeHeader->sender()] autorelease];
}

- (void) setSender:(MCOAddress *)sender
{
    _nativeHeader->setSender((mailcore::Address *) [sender mco_mcObject]);
}

- (MCOAddress *) from
{
    return [MCOAddress addressWithMCAddress:_nativeHeader->from()];
}

- (void) setFrom:(MCOAddress *)from
{
    _nativeHeader->setFrom((mailcore::Address *) [from mco_mcObject]);
}

- (NSArray *) to
{
    mailcore::Array * addresses = _nativeHeader->to();
    if (addresses == NULL)
        return nil;
    
    NSMutableArray * result = [NSMutableArray array];
    for(unsigned int i = 0 ; i < addresses->count() ; i ++) {
        mailcore::Address * nativeAddress = (mailcore::Address *) addresses->objectAtIndex(i);
        [result addObject:[MCOAddress addressWithMCAddress:nativeAddress]];
    }
    return result;
}

- (void) setTo:(NSArray *)to
{
    _nativeHeader->setTo((mailcore::Array *) [to mco_mcObject]);
}

- (NSArray *) cc
{
    mailcore::Array * addresses = _nativeHeader->cc();
    if (addresses == NULL)
        return nil;
    
    NSMutableArray * result = [NSMutableArray array];
    for(unsigned int i = 0 ; i < addresses->count() ; i ++) {
        mailcore::Address * nativeAddress = (mailcore::Address *) addresses->objectAtIndex(i);
        [result addObject:[MCOAddress addressWithMCAddress:nativeAddress]];
    }
    return result;
}

- (void) setCc:(NSArray *)cc
{
    _nativeHeader->setCc((mailcore::Array *) [cc mco_mcObject]);
}

- (NSArray *) bcc
{
    mailcore::Array * addresses = _nativeHeader->bcc();
    if (addresses == NULL)
        return nil;
    
    NSMutableArray * result = [NSMutableArray array];
    for(unsigned int i = 0 ; i < addresses->count() ; i ++) {
        mailcore::Address * nativeAddress = (mailcore::Address *) addresses->objectAtIndex(i);
        [result addObject:[MCOAddress addressWithMCAddress:nativeAddress]];
    }
    return result;
}

- (void) setBcc:(NSArray *)bcc
{
    _nativeHeader->setBcc((mailcore::Array *) [bcc mco_mcObject]);
}

- (NSArray *) replyTo
{
    mailcore::Array * addresses = _nativeHeader->replyTo();
    if (addresses == NULL)
        return nil;
    
    NSMutableArray * result = [NSMutableArray array];
    for(unsigned int i = 0 ; i < addresses->count() ; i ++) {
        mailcore::Address * nativeAddress = (mailcore::Address *) addresses->objectAtIndex(i);
        [result addObject:[MCOAddress addressWithMCAddress:nativeAddress]];
    }
    return result;
}

- (void) setReplyTo:(NSArray *)replyTo
{
    _nativeHeader->setReplyTo((mailcore::Array *) [replyTo mco_mcObject]);
}

- (NSString *) subject
{
    return [NSString mco_stringWithMCString:_nativeHeader->subject()];
}

- (void) setSubject:(NSString *)subject
{
    _nativeHeader->setSubject([subject mco_mcString]);
}

- (NSString *) userAgent
{
    return [NSString mco_stringWithMCString:_nativeHeader->userAgent()];
}

- (void) setUserAgent:(NSString *)userAgent
{
    _nativeHeader->setUserAgent([userAgent mco_mcString]);
}

- (NSString *) extractedSuject
{
    return [NSString mco_stringWithMCString:_nativeHeader->extractedSubject()];
}

- (NSString *) partialExtractedSuject
{
    return [NSString mco_stringWithMCString:_nativeHeader->partialExtractedSubject()];
}

- (void) importHeadersData:(NSData *)data
{
    _nativeHeader->importHeadersData([data mco_mcData]);
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeHeader;
}

@end
