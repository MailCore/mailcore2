//
//  MCOIMAPFetchNamespaceOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHNAMESPACEOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHNAMESPACEOPERATION_H_

// This class implements an operation to fetch namespaces.

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPFetchNamespaceOperation : MCOIMAPBaseOperation

// Starts the asynchronous operation.
//
// On success, the completion block will be called with nil as error.
// namespaces is a dictionary with the following keys:
// MCOIMAPNamespacePersonal for personal namespaces,
// MCOIMAPNamespaceOther for other namespaces,
// MCOIMAPNamespaceShared for shared namespaces.
// Values are MCOIMAPNamespace.
//
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// namespaces will be nil.
- (void) start:(void (^)(NSError * error, NSDictionary * namespaces))completionBlock;

@end

#endif
