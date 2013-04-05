//
//  MCOIMAPNamespace.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPNAMESPACE_H_

#define __MAILCORE_MCOIMAPNAMESPACE_H_

#import <Foundation/Foundation.h>

@class MCOIMAPNamespace;

@interface MCOIMAPNamespace : NSObject <NSCopying>

+ (MCOIMAPNamespace *) namespaceWithPrefix:(NSString *)prefix delimiter:(char)delimiter;

- (NSString *) mainPrefix;
- (char) mainDelimiter;
- (NSArray *) prefixes;
- (NSString *) pathForComponents:(NSArray *)components;
- (NSString *) pathForComponents:(NSArray *)components prefix:(NSString *)prefix;
- (NSArray *) componentsFromPath:(NSString *)path;
- (BOOL) containsFolderPath:(NSString *)path;

@end

#endif
