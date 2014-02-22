#ifndef MAILCORE_MCPOPPROGRESSCALLBACK_H

#define MAILCORE_MCPOPPROGRESSCALLBACK_H

#ifdef __cplusplus

namespace mailcore {
    
    class POPSession;
    
    class POPProgressCallback {
    public:
        virtual void bodyProgress(POPSession * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
