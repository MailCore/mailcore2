//
//  MCMD5.h
//  hermes
//
//  Created by DINH Viêt Hoà on 4/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCMD5_H_
#define __MAILCORE_MCMD5_H_

#include <MailCore/MCData.h>
#include <MailCore/MCString.h>

#ifdef __cplusplus

namespace mailcore {
    Data * md5Data(Data * data);
    String * md5String(Data * data);
}

#endif

#endif
