//
//  MCOJMAPPart.mm
//  mailcore2
//

#import "MCOJMAPPart.h"

#include "MCJMAP.h"
#import "MCOUtils.h"

@implementation MCOJMAPPart

#define nativeType mailcore::JMAPPart

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
MCO_OBJC_SYNTHESIZE_STRING(setLanguage, language)
MCO_OBJC_SYNTHESIZE_ARRAY(setLanguages, languages)

@end
