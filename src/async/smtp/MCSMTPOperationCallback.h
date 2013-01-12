//
//  MCSMTPOperationCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCSMTPOperationCallback__
#define __mailcore2__MCSMTPOperationCallback__

namespace mailcore {
    
	class SMTPOperation;
    
	class SMTPOperationCallback {
	public:
		virtual void bodyProgress(SMTPOperation * session, unsigned int current, unsigned int maximum) {};
	};
}

#endif /* defined(__mailcore2__MCSMTPOperationCallback__) */
