//
//  MCBase64.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 7/30/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCBASE64_H_
#define __MAILCORE_MCBASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

extern char * MCDecodeBase64(const char * in, int len);
extern char * MCEncodeBase64(const char * in, int len);

#ifdef __cplusplus
}
#endif

#endif
