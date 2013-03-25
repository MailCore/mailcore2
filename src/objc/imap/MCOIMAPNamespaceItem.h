//
//  MCOIMAPNamespaceItem.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MCOIMAPNamespaceItem : NSObject <NSCopying>

@property (nonatomic, copy) NSString * prefix;
@property (nonatomic, assign) char delimiter;

- (NSString *) pathForComponents:(NSArray *)components;
- (NSArray *) componentsForPath:(NSString *)path;
- (BOOL) containsFolder:(NSString *)folder;

@end
