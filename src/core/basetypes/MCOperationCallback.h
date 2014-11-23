#ifndef MAILCORE_MCOPERATIONCALLBACK_H

#define MAILCORE_MCOPERATIONCALLBACK_H

#ifdef __cplusplus

namespace mailcore {
    
    class Operation;
    
    MAILCORE_EXPORT
    class OperationCallback {
    public:
        virtual void operationFinished(Operation * op) {}
    };
    
}

#endif

#endif
