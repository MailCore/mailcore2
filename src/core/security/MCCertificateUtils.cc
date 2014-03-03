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
#else
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
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
    if (status != noErr) {
        goto free_certs;
    }
    
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
    bool result = false;
    X509 *fCert = NULL;
    MMAPString *fstr = NULL;
    BIO *fBio = NULL;
    X509_STORE *store = NULL;
    X509_STORE_CTX *storectx = NULL;
    STACK *certificates = NULL;
    
    carray * cCerts = mailstream_get_certificate_chain(stream);
    if (cCerts == NULL) {
        fprintf(stderr, "warning: No certificate chain retrieved");
        goto free_certs;
    }
    
    store = X509_STORE_new();
    if (store == NULL) {
        goto free_certs;
    }
    
    X509_STORE_set_default_paths(store);
    
    storectx = X509_STORE_CTX_new();
    if (storectx == NULL) {
        goto free_certs;
    }
    
    fstr = (MMAPString *) carray_get(cCerts, 0);
    fBio = BIO_new_mem_buf((void *) fstr->str, strlen(fstr->str));
    fCert = PEM_read_bio_X509(fBio, NULL, 0, NULL);
    if (fCert == NULL) {
        goto free_certs;
    }
    
    certificates = (STACK *)sk_new(NULL);
    for(unsigned int i = 1 ; i < carray_count(cCerts) ; i ++) {
        MMAPString * str;
        str = (MMAPString *) carray_get(cCerts, i);
        BIO *bio = BIO_new_mem_buf((void *) fstr->str, fstr->len);
        X509 *certificate = PEM_read_bio_X509(bio, NULL, 0, NULL);
        sk_X509_push((STACK_OF(X509) *) certificates, certificate);
        BIO_free(bio);
    }
    
    if (X509_STORE_CTX_init(storectx, store, fCert, certificates) != 1) {
        goto free_certs;
    }
    
    result = X509_verify_cert(storectx);
    
    free_certs:
        X509_STORE_free(store);
        X509_STORE_CTX_free(storectx);
        mailstream_certificate_chain_free(cCerts);
        sk_X509_free((STACK_OF(X509) *) certificates);
        X509_free(fCert);
        BIO_free(fBio);
    err:
        return result;
#endif
    return true;
}
