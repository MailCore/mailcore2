#ifndef __MAILCORE_MCPOPPROGRESSCALLBACK_H_

#define __MAILCORE_MCPOPPROGRESSCALLBACK_H_

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
