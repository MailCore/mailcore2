//
//  MCOJMAPBlobUpload.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPBLOBUPLOAD_H

#define MAILCORE_MCOJMAPBLOBUPLOAD_H

#import <Foundation/Foundation.h>

@interface MCOJMAPBlobUpload : NSObject <NSCopying>

@property (nonatomic, copy) NSString * accountID;
@property (nonatomic, copy) NSString * blobID;
@property (nonatomic, copy) NSString * type;
@property (nonatomic, copy) NSString * name;
@property (nonatomic, assign) NSUInteger size;

@end

#endif
