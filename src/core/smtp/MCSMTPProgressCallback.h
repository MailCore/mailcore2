#ifndef MAILCORE_MCSMTPPROGRESSCALLBACK_H

#define MAILCORE_MCSMTPPROGRESSCALLBACK_H

#ifdef __cplusplus

namespace mailcore {
    
    class SMTPSession;
    
    class SMTPProgressCallback {
    public:
        virtual void bodyProgress(SMTPSession * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
