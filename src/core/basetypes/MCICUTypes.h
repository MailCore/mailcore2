//
//  MCICUTypes.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 4/18/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCICUTYPES_H

#define MAILCORE_MCICUTYPES_H

#ifndef __UMACHINE_H__
#include <stdint.h>

#ifdef _MSC_VER
typedef wchar_t UChar;
#elif defined(__cplusplus) && __cplusplus >= 201103L
typedef char16_t UChar;
#elif defined(__CHAR16_TYPE__)
typedef __CHAR16_TYPE__ UChar;
#else
typedef uint16_t UChar;
#endif
#endif

#endif
