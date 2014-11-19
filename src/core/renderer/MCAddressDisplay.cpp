//
//  MCAddressDisplay.cpp
//  testUI
//
//  Created by DINH Viêt Hoà on 1/27/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCAddressDisplay.h"

using namespace mailcore;

String * AddressDisplay::displayStringForAddress(Address * address)
{
    return address->nonEncodedRFC822String();
}

String * AddressDisplay::shortDisplayStringForAddress(Address * address)
{
    if ((address->displayName() != NULL) && (address->displayName()->length() > 0)) {
        return address->displayName();
    }
    else if (address->mailbox()) {
        return address->mailbox();
    }
    else {
        return MCSTR("invalid");
    }
}

String * AddressDisplay::veryShortDisplayStringForAddress(Address * address)
{
    if ((address->displayName() != NULL) && (address->displayName()->length() > 0)) {
        Array * components;
        String * senderName;
        
        senderName = address->displayName();
        senderName = (String *) senderName->copy()->autorelease();
        
        senderName->replaceOccurrencesOfString(MCSTR(","), MCSTR(" "));
        senderName->replaceOccurrencesOfString(MCSTR("'"), MCSTR(" "));
        senderName->replaceOccurrencesOfString(MCSTR("\""), MCSTR(" "));
        components = senderName->componentsSeparatedByString(MCSTR(" "));
        if (components->count() == 0) {
            return MCLOCALIZEDSTRING(MCSTR("invalid"));
        }
        return (String *) components->objectAtIndex(0);
    }
    else if (address->mailbox()) {
        Array * components = address->mailbox()->componentsSeparatedByString(MCSTR("@"));
        if (components->count() == 0) {
            return MCSTR("");
        }
        return (String *) components->objectAtIndex(0);
    }
    else {
        return MCLOCALIZEDSTRING(MCSTR("invalid"));
    }
}

String * AddressDisplay::displayStringForAddresses(Array * addresses)
{
    return Address::nonEncodedRFC822StringForAddresses(addresses);
}

String * AddressDisplay::shortDisplayStringForAddresses(Array * addresses)
{
    String * result = String::string();
    for(unsigned int i = 0 ; i < addresses->count() ; i ++) {
        Address * address = (Address *) addresses->objectAtIndex(i);
        if (i != 0) {
            result->appendString(MCSTR(", "));
        }
        result->appendString(shortDisplayStringForAddress(address));
    }
    return result;
}

String * AddressDisplay::veryShortDisplayStringForAddresses(Array * addresses)
{
    String * result = String::string();
    for(unsigned int i = 0 ; i < addresses->count() ; i ++) {
        Address * address = (Address *) addresses->objectAtIndex(i);
        if (i != 0) {
            result->appendString(MCSTR(", "));
        }
        result->appendString(veryShortDisplayStringForAddress(address));
    }
    return result;
}
