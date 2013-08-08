//
//  MCCertificateUtils.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 7/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCCertificateUtils.h"

#if __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#endif

#include "MCLog.h"

bool mailcore::checkCertificate(mailstream * stream, String * hostname)
{
#if __APPLE__
    bool result = false;
    CFStringRef hostnameCFString;
    SecPolicyRef policy;
    CFMutableArrayRef certificates;
    SecTrustRef trust = NULL;
    SecTrustResultType trustResult;
    OSStatus status;
    
    carray * cCerts = mailstream_get_certificate_chain(stream);
    if (cCerts == NULL) {
        fprintf(stderr, "warning: No certificate chain retrieved");
        goto err;
    }
    
    hostnameCFString = CFStringCreateWithCharacters(NULL, (const UniChar *) hostname->unicodeCharacters(),
                                                                hostname->length());
    policy = SecPolicyCreateSSL(true, hostnameCFString);
    certificates = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
    
    for(unsigned int i = 0 ; i < carray_count(cCerts) ; i ++) {
        MMAPString * str;
        str = (MMAPString *) carray_get(cCerts, i);
        CFDataRef data = CFDataCreate(NULL, (const UInt8 *) str->str, (CFIndex) str->len);
        SecCertificateRef cert = SecCertificateCreateWithData(NULL, data);
        CFArrayAppendValue(certificates, cert);
        CFRelease(data);
        CFRelease(cert);
    }
    
    status = SecTrustCreateWithCertificates(certificates, policy, &trust);
    if (status != noErr) {
        goto free_certs;
    }
    
    status = SecTrustEvaluate(trust, &trustResult);
    switch (trustResult) {
        case kSecTrustResultUnspecified:
        case kSecTrustResultProceed:
            // certificate chain is ok
            result = true;
            break;
            
        default:
            // certificate chain is invalid
            break;
    }
    
    CFRelease(trust);
free_certs:
    CFRelease(certificates);
    mailstream_certificate_chain_free(cCerts);
    CFRelease(policy);
    CFRelease(hostnameCFString);
err:
    return result;
#else
    //TODO check certificate
    // for other platforms too.
    return true;
#endif
}
