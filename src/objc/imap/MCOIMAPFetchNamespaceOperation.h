//
//  MCOIMAPFetchNamespaceOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOIMAPBaseOperation.h>

#ifndef __MAILCORE_MCOIMAPFETCHNAMESPACEOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHNAMESPACEOPERATION_H_

@interface MCOIMAPFetchNamespaceOperation : MCOIMAPBaseOperation

- (void)start:(void (^)(NSError *error, NSDictionary * namespaces))completionBlock;

@end

#endif
