//
//  MCCertificateUtils.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 7/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCCERTIFICATEUTILS_H

#define MAILCORE_MCCERTIFICATEUTILS_H

#include <libetpan/libetpan.h>
#include <MailCore/MCString.h>

namespace mailcore {
    
    bool checkCertificate(mailstream * stream, String * hostname);
    bool checkCertificate(mailstream * stream, String * hostname, Data * clientCertificate, String * clientCertificatePassword, Array* pinningHosts, Array* pinningCerts);

}

#endif
