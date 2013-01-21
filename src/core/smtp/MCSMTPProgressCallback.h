#ifndef __MAILCORE_MCSMTPPROGRESSCALLBACK_H_

#define __MAILCORE_MCSMTPPROGRESSCALLBACK_H_

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
