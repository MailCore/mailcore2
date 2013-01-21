#ifndef __MAILCORE_MCOPERATIONCALLBACK_H_

#define __MAILCORE_MCOPERATIONCALLBACK_H_

#ifdef __cplusplus

namespace mailcore {
	
	class Operation;
	
	class OperationCallback {
	public:
		virtual void operationFinished(Operation * op) {}
	};
	
}

#endif

#endif
