//
//  MCOJMAPSubmission.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPSUBMISSION_H

#define MAILCORE_MCOJMAPSUBMISSION_H

#import <Foundation/Foundation.h>

@interface MCOJMAPSubmission : NSObject <NSCopying>

@property (nonatomic, copy) NSString * identifier;
@property (nonatomic, copy) NSString * emailID;
@property (nonatomic, copy) NSString * threadID;
@property (nonatomic, copy) NSString * identityID;
@property (nonatomic, copy) NSString * sendAt;
@property (nonatomic, copy) NSString * undoStatus;
@property (nonatomic, copy) NSDictionary * deliveryStatus;

@end

#endif
