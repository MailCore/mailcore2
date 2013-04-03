//
//  MCOIndexSet.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <MailCore/MCORange.h>

#ifndef __MAILCORE_MCOINDEXSET_H_

#define __MAILCORE_MCOINDEXSET_H_

// similar to NSMutableIndexSet but supports int64_t

@interface MCOIndexSet : NSObject <NSCopying>

+ (MCOIndexSet *) indexSet;
+ (MCOIndexSet *) indexSetWithRange:(MCORange)range;
+ (MCOIndexSet *) indexSetWithIndex:(uint64_t)idx;

- (unsigned int) count;
- (void) addIndex:(uint64_t)idx;
- (void) removeIndex:(uint64_t)idx;
- (BOOL) containsIndex:(uint64_t)idx;

- (void) addRange:(MCORange)range;
- (void) removeRange:(MCORange)range;
- (void) intersectsRange:(MCORange)range;

- (MCORange *) allRanges;
- (unsigned int) rangesCount;

@end

#endif
