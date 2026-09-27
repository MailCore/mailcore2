//
//  MCOJMAPPart.h
//  mailcore2
//

#ifndef MAILCORE_MCJMAPPART_H

#define MAILCORE_MCJMAPPART_H

#import <MailCore/MCOAbstractPart.h>

@interface MCOJMAPPart : MCOAbstractPart

@property (nonatomic, copy) NSString * partID;
@property (nonatomic, copy) NSString * blobID;
@property (nonatomic, assign) unsigned int size;
@property (nonatomic, copy) NSString * language;
@property (nonatomic, copy) NSArray<NSString *> * languages;

@end

#endif
