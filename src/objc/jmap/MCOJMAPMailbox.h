//
//  MCOJMAPMailbox.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPMAILBOX_H

#define MAILCORE_MCOJMAPMAILBOX_H

#import <Foundation/Foundation.h>

@interface MCOJMAPMailbox : NSObject <NSCopying>

@property (nonatomic, copy) NSString * identifier;
@property (nonatomic, copy) NSString * name;
@property (nonatomic, copy) NSString * parentID;
@property (nonatomic, copy) NSString * role;
@property (nonatomic, assign) int sortOrder;
@property (nonatomic, assign, getter=isSubscribed) BOOL subscribed;
@property (nonatomic, assign) int totalEmails;
@property (nonatomic, assign) int unreadEmails;
@property (nonatomic, assign) int totalThreads;
@property (nonatomic, assign) int unreadThreads;
@property (nonatomic, copy) NSDictionary * rights;

@end

#endif
