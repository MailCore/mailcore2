#ifndef MAILCORE_MCNNTPPROGRESSCALLBACK_H

#define MAILCORE_MCNNTPPROGRESSCALLBACK_H

#ifdef __cplusplus

namespace mailcore {
    
    class NNTPSession;
    
    class NNTPProgressCallback {
    public:
        virtual void bodyProgress(NNTPSession * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
