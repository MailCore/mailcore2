//
//  MCOJMAPMessagePart.mm
//  mailcore2
//

#import "MCOJMAPMessagePart.h"

#include "MCJMAP.h"
#import "MCOUtils.h"

@implementation MCOJMAPMessagePart

#define nativeType mailcore::JMAPMessagePart

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCPart:(nativeType *) object] autorelease];
}

MCO_SYNTHESIZE_NSCODING

MCO_OBJC_SYNTHESIZE_STRING(setPartID, partID)
MCO_OBJC_SYNTHESIZE_STRING(setBlobID, blobID)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setSize, size)

@end
