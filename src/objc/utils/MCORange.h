//
//  MCORange.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef mailcore2_MCORange_h
#define mailcore2_MCORange_h

#import <Foundation/Foundation.h>

#ifdef __cplusplus
#include <mailcore/MCBaseTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

@class MCOIndexSet;

typedef struct {
    uint64_t location;
    uint64_t length;
} MCORange;

extern MCORange RangeEmpty;

MCORange MCORangeMake(uint64_t location, uint64_t length);
MCOIndexSet * MCORangeRemoveRange(MCORange range1, MCORange range2);
MCOIndexSet * MCORangeUnion(MCORange range1, MCORange range2);
#ifdef __cplusplus
mailcore::Range MCORangeToMCRange(MCORange range);
MCORange MCORangeWithMCRange(mailcore::Range range);
#endif
MCORange MCORangeIntersection(MCORange range1, MCORange range2);
BOOL MCORangeHasIntersection(MCORange range1, MCORange range2);
uint64_t MCORangeLeftBound(MCORange range);
uint64_t MCORangeRightBound(MCORange range);

#ifdef __cplusplus
}
#endif

#endif
