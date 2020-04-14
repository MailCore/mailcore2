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
#include <CFNetwork/CFNetwork.h>
#else
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#endif

#if defined(ANDROID) || defined(__ANDROID__)
#include <dirent.h>
#include "../../android_log.h"
#endif

#include "MCLog.h"
#include <MailCore/MCArray.h>
#include <MailCore/MCString.h>
#include <MailCore/MCData.h>

bool mailcore::checkCertificate(mailstream * stream, String * hostname)
{
    return checkCertificate(stream, hostname, NULL, NULL, NULL, NULL);
}

#if __APPLE__
namespace mailcore {
    
    bool checkPinning(String * hostname, Array * pinningHosts, Array* pinningCerts, SecKeyRef publicKey)
    {
        if(pinningHosts->count() == 0)
            return true;
        
        for(unsigned int i = 0; i < pinningHosts->count(); i++)
        {
            String* pinhost = (String*)pinningHosts->objectAtIndex(i);
            if(hostname->isEqual(pinhost))
            {
                Data* data = (Data*)pinningCerts->objectAtIndex(0); //TODO: use i instead of 0 ????
                int deb = data->length();
                CFDataRef dataref = CFDataCreate(NULL, (const UInt8 *)data->bytes() , (CFIndex) data->length());
                SecCertificateRef cert = SecCertificateCreateWithData(NULL, dataref);
                SecKeyRef pinningPublicKey = SecCertificateCopyPublicKey(cert);
                
                CFDataRef k0 = SecKeyCopyExternalRepresentation(pinningPublicKey, NULL);
                CFDataRef k1 = SecKeyCopyExternalRepresentation(publicKey, NULL);

                int len0 = CFDataGetLength(k0);
                int len1 = CFDataGetLength(k1);

                const UInt8* c0 = CFDataGetBytePtr(k0);
                const UInt8* c1 = CFDataGetBytePtr(k1);
                
                bool result = false;
                if(len0 == len1)
                    result = (memcmp(c0, c1, len0) == 0);
                
                CFRelease(k1);
                CFRelease(k0);
                CFRelease(pinningPublicKey);
                CFRelease(cert);
                CFRelease(dataref);

                return result;
            }
        }
        return false;
    }
}
#endif

#if defined(ANDROID) || defined(__ANDROID__)
namespace mailcore {
    
    bool checkPinning(String * hostname, Array * pinningHosts, Array* pinningCerts, unsigned char* publicPKeyData, int publicPKeyLen)
    {
                  bool match = false;

                __android_log_print(ANDROID_LOG_INFO, "NATIVE", "chk %s", hostname->UTF8Characters());

        if(!pinningHosts || pinningHosts->count() == 0)
            return true;
        
        for(unsigned int i = 0; i < pinningHosts->count(); i++)
        {
            String* pinhost = (String*)pinningHosts->objectAtIndex(i);

                __android_log_print(ANDROID_LOG_INFO, "NATIVE", "host %s vs %s", hostname->UTF8Characters(), pinhost->UTF8Characters());

            if(hostname->isEqual(pinhost))
            {
                Data* data = (Data*)pinningCerts->objectAtIndex(i);

                unsigned char* storedData = (unsigned char*)data->bytes();
                int storedLen = data->length();

                BIO* certBio = BIO_new(BIO_s_mem());
                BIO_write(certBio, storedData, storedLen);

                X509* storedCert = PEM_read_bio_X509(certBio, NULL, 0, NULL);       
                int storedPKeyLen = (int)i2d_X509_PUBKEY(X509_get_X509_PUBKEY(storedCert), NULL);
                
                unsigned char* storedPKeyData = (unsigned char*)malloc(storedPKeyLen);
                unsigned char* temp = storedPKeyData;
                int status = i2d_X509_PUBKEY(X509_get_X509_PUBKEY(storedCert), &temp);

                if(storedPKeyLen == publicPKeyLen)
                {
                   match = true;
                    for(int x = 0; x < publicPKeyLen; x++)
                    {
                        if(publicPKeyData[x] != storedPKeyData[x])
                       {
                            match = false;
                            break;
                        }
                    }
                }

                free(storedPKeyData);
                X509_free(storedCert);
                if(match)
                {
                    __android_log_print(ANDROID_LOG_INFO, "NATIVE", "Pinning PASSED.");
                    return true;
                }
            }
        }
                    __android_log_print(ANDROID_LOG_INFO, "NATIVE", "Pinning FAILED.");
        return false;
    }
}
#endif

#if defined(ANDROID) || defined(__ANDROID__)
#include "../../android_log.h"
#endif
//__android_log_print(ANDROID_LOG_INFO, "NATIVE", "mailcore::checkCertificate CC %d bytes", clientCertificate->length());

bool mailcore::checkCertificate(mailstream * stream, String * hostname, Data * clientCertificate, String * clientCertificatePassword, Array * pinningHosts, Array* pinningCerts)
{
#if defined(ANDROID) || defined(__ANDROID__)
    if(clientCertificate)
    {
        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "mailcore::checkCertificate CC %d bytes", clientCertificate->length());
    }

    if(pinningHosts && pinningHosts->count())
    {
        for(int i = 0; i < pinningHosts->count(); i++)
        {
            String* s = (String*)pinningHosts->objectAtIndex(i);
            Data* d = (Data*)pinningCerts->objectAtIndex(i);
            __android_log_print(ANDROID_LOG_INFO, "NATIVE", "mailcore::checkCertificate PH %s %d", s->UTF8Characters(), d->length());
        }
    }
    #endif

    bool pinningResult = false;

#if __APPLE__
    bool result = false;
    CFStringRef hostnameCFString;
    SecPolicyRef policy;
    CFMutableArrayRef certificates;
    SecTrustRef trust = NULL;
    SecTrustResultType trustResult;
    OSStatus status;
    unsigned int certCount;
    
    const UInt8* p;
    int l, h;

    carray * cCerts = mailstream_get_certificate_chain(stream);
    if (cCerts == NULL)
    {
        fprintf(stderr, "warning: No certificate chain retrieved");
        goto err;
    }
    
    hostnameCFString = CFStringCreateWithCharacters(NULL, (const UniChar *) hostname->unicodeCharacters(),
                                                                hostname->length());
    policy = SecPolicyCreateSSL(true, hostnameCFString);
    certificates = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
    
    certCount = carray_count(cCerts);
    for(unsigned int i = 0 ; i < certCount ; i ++)
    {
        MMAPString * str;
        str = (MMAPString *) carray_get(cCerts, i);
        
        p = (const UInt8 *) str->str;
        l = (int)str->len;
        for(h = 0; h < l; h++)
        {
            
            if (h > 0) printf(",");
            printf("0x%02x", p[h]);
        }
        printf("\n\n\n");
        
        CFDataRef data = CFDataCreate(NULL, (const UInt8 *) str->str, (CFIndex) str->len);
        SecCertificateRef cert = SecCertificateCreateWithData(NULL, data);

        SecKeyRef publicKey = SecCertificateCopyPublicKey(cert);
        if(!pinningResult) pinningResult = checkPinning(hostname, pinningHosts, pinningCerts, publicKey);
        
        CFArrayAppendValue(certificates, cert);
        CFRelease(data);
        CFRelease(cert);
        CFRelease(publicKey);
    }

    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    
    // The below API calls are not thread safe. We're making sure not to call the concurrently.
    pthread_mutex_lock(&lock);
    
    status = SecTrustCreateWithCertificates(certificates, policy, &trust);
    if (status != noErr) {
        pthread_mutex_unlock(&lock);
        goto free_certs;
    }
    
    status = SecTrustEvaluate(trust, &trustResult);
    if (status != noErr) {
        pthread_mutex_unlock(&lock);
        goto free_certs;
    }
    
    pthread_mutex_unlock(&lock);
    
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
    
    if(result == true)
        result = pinningResult;
    
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
    X509_STORE * store = NULL;
    X509_STORE_CTX * storectx = NULL;
    STACK_OF(X509) * certificates = NULL;
#if defined(ANDROID) || defined(__ANDROID__)
    DIR * dir = NULL;
    struct dirent * ent = NULL;
    FILE * f = NULL;
#endif
    int status;
    
    carray * cCerts = mailstream_get_certificate_chain(stream);
    if (cCerts == NULL) {
        fprintf(stderr, "warning: No certificate chain retrieved");
        goto err;
    }
    
    store = X509_STORE_new();
    if (store == NULL) {
        goto free_certs;
    }
    
#ifdef _MSC_VER
	HCERTSTORE systemStore = CertOpenSystemStore(NULL, L"ROOT");

	PCCERT_CONTEXT previousCert = NULL;
	while (1) {
		PCCERT_CONTEXT nextCert = CertEnumCertificatesInStore(systemStore, previousCert);
		if (nextCert == NULL) {
			break;
		}
		X509 * openSSLCert = d2i_X509(NULL, (const unsigned char **)&nextCert->pbCertEncoded, nextCert->cbCertEncoded);
		if (openSSLCert != NULL) {
			X509_STORE_add_cert(store, openSSLCert);
			X509_free(openSSLCert);
		}
		previousCert = nextCert;
	}
	CertCloseStore(systemStore, 0);
#elif defined(ANDROID) || defined(__ANDROID__)
    dir = opendir("/system/etc/security/cacerts");
    while (ent = readdir(dir)) {
        if (ent->d_name[0] == '.') {
            continue;
        }
        char filename[1024];
        snprintf(filename, sizeof(filename), "/system/etc/security/cacerts/%s", ent->d_name);
        f = fopen(filename, "rb");
        if (f != NULL) {
            X509 * cert = PEM_read_X509(f, NULL, NULL, NULL);
            if (cert != NULL) {
                X509_STORE_add_cert(store, cert);
                X509_free(cert);
            }
            fclose(f);
        }
    }
    closedir(dir);
#endif

	status = X509_STORE_set_default_paths(store);
    if (status != 1) {
        printf("Error loading the system-wide CA certificates");
    }
    
    certificates = sk_X509_new_null();
    for(unsigned int i = 0 ; i < carray_count(cCerts) ; i ++) {
        MMAPString * str;
        str = (MMAPString *) carray_get(cCerts, i);
        if (str == NULL) {
            goto free_certs;
        }
        BIO *bio = BIO_new_mem_buf((void *) str->str, str->len);
        X509 *certificate = d2i_X509_bio(bio, NULL);

       // android pinning 
        if(pinningResult == false)
        {
            int remotePKeyLen = (int)i2d_X509_PUBKEY(X509_get_X509_PUBKEY(certificate), NULL);
            unsigned char* remotePKeyData = (unsigned char*)malloc(remotePKeyLen);
            unsigned char* temp = remotePKeyData;
            i2d_X509_PUBKEY(X509_get_X509_PUBKEY(certificate), &temp);
            pinningResult = checkPinning(hostname, pinningHosts, pinningCerts, remotePKeyData, (int)remotePKeyLen);            
            free(remotePKeyData);
        }

        BIO_free(bio);
        if (!sk_X509_push(certificates, certificate)) {
            goto free_certs;
        }
    }

    storectx = X509_STORE_CTX_new();
    if (storectx == NULL) {
        goto free_certs;
    }
    
    status = X509_STORE_CTX_init(storectx, store, sk_X509_value(certificates, 0), certificates);
    if (status != 1) {
        goto free_certs;
    }
    
    status = X509_verify_cert(storectx);
    if (status == 1 || status == 0) {
        result = true;
    }
    
free_certs:
    mailstream_certificate_chain_free(cCerts);
    if (certificates != NULL) {
        sk_X509_pop_free((STACK_OF(X509) *) certificates, X509_free);
    }
    if (storectx != NULL) {
        X509_STORE_CTX_free(storectx);
    }
    if (store != NULL) {
        X509_STORE_free(store);
    }
err:

    if(result == true)
      result = pinningResult;

    return result;

#endif
}
