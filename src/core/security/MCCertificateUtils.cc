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
    CFStringRef hostnameCFString = CFStringCreateWithCharacters(NULL, (const UniChar *) hostname->unicodeCharacters(),
                                                                hostname->length());
    SecPolicyRef policy = SecPolicyCreateSSL(true, hostnameCFString);
    
    CFMutableArrayRef certificates;
    SecTrustRef trust;
    certificates = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
    
    carray * cCerts = mailstream_get_certificate_chain(stream);
    if (cCerts == NULL) {
        fprintf(stderr, "warning: No certificate chain retrieved");
        return true;
    }
    for(unsigned int i = 0 ; i < carray_count(cCerts) ; i ++) {
        MMAPString * str;
        str = (MMAPString *) carray_get(cCerts, i);
        CFDataRef data = CFDataCreate(NULL, (const UInt8 *) str->str, (CFIndex) str->len);
        SecCertificateRef cert = SecCertificateCreateWithData(NULL, data);
        CFArrayAppendValue(certificates, cert);
        CFRelease(data);
        
        OSStatus status = SecTrustCreateWithCertificates(certificates, policy, &trust);
        SecTrustResultType trustResult;
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
    }
    
    CFRelease(trust);
    CFRelease(certificates);
    CFRelease(policy);
    CFRelease(hostnameCFString);
    
    return result;
#else
    //TODO check certificate
    // for other platforms too.
    return true;
#endif
}
