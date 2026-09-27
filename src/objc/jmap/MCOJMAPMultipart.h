//
//  MCOJMAPMultipart.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPMULTIPART_H

#define MAILCORE_MCOJMAPMULTIPART_H

#import <MailCore/MCOAbstractMultipart.h>

@interface MCOJMAPMultipart : MCOAbstractMultipart

@property (nonatomic, copy) NSString * partID;
@property (nonatomic, copy) NSString * blobID;
@property (nonatomic, assign) unsigned int size;
@property (nonatomic, copy) NSString * language;
@property (nonatomic, copy) NSArray<NSString *> * languages;

@end

#endif
