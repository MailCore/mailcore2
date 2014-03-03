#ifndef MAILCORE_MCIMAPPROGRESSCALLBACK_H

#define MAILCORE_MCIMAPPROGRESSCALLBACK_H

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPSession;
    
    class IMAPProgressCallback {
    public:
        virtual void bodyProgress(IMAPSession * session, unsigned int current, unsigned int maximum) {};
        virtual void itemsProgress(IMAPSession * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
