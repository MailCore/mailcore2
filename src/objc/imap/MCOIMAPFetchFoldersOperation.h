//
//  MCOIMAPFetchFoldersOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHFOLDERSOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHFOLDERSOPERATION_H_

#import <mailcore/MCOIMAPBaseOperation.h>

@interface MCOIMAPFetchFoldersOperation : MCOIMAPBaseOperation
- (void)start:(void (^)(NSError *error, NSArray *folder))completionBlock;
@end

#endif
