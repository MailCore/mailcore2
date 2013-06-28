//
//  MCNetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCNETSERVICE_H_
#define __MAILCORE_MCNETSERVICE_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
    class NetService : public Object {
    
    public:
        NetService();
        virtual ~NetService();
        
        virtual void setHostname(String * hostname);
        virtual String * hostname();
        
        virtual void setPort(unsigned int port);
        virtual unsigned int port();
        
        virtual void setConnectionType(ConnectionType connectionType);
        virtual ConnectionType connectionType();
        	
        virtual String * normalizedHostnameWithEmail(String * email);
        
    public: // serialization
        static NetService * serviceWithInfo(HashMap * info);
        virtual HashMap * info();
    
    public: //subclass behavior
        NetService(NetService * other);
        virtual String * description();
        virtual Object * copy();
    		
    private:
        String * mHostname;
        unsigned int mPort;
        ConnectionType mConnectionType;
        
        void init();
        void fillWithInfo(HashMap * info);
    };
}

#endif
	
#endif