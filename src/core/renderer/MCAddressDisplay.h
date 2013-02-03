//
//  MCAddressUI.h
//  testUI
//
//  Created by DINH Viêt Hoà on 1/27/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MCAddressDisplay__
#define __MCAddressDisplay__

#include <mailcore/MCAbstract.h>

#ifdef __cplusplus

namespace mailcore {
    
    class AddressDisplay {
        
    public:
        static String * displayStringForAddress(Address * address);
        static String * shortDisplayStringForAddress(Address * address);
        static String * veryShortDisplayStringForAddress(Address * address);
        
        static String * displayStringForAddresses(Array * addresses);
        static String * shortDisplayStringForAddresses(Array * addresses);
        static String * veryShortDisplayStringForAddresses(Array * addresses);
    };
    
};

#endif

#endif /* defined(__MCAddressDisplay__) */
