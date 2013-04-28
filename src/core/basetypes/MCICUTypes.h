//
//  MCICUTypes.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 4/18/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCICUTYPES_H_
#define __MAILCORE_MCICUTYPES_H_

#if defined(__CHAR16_TYPE__)
typedef __CHAR16_TYPE__ UChar;
#else
typedef uint16_t UChar;
#endif

#endif
