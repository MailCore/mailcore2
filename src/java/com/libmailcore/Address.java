package com.libmailcore;

import java.util.List;

public class Address extends NativeObject {
    public native static Address addressWithDisplayName(String displayName, String mailbox);
    public native static Address addressWithMailbox(String mailbox);
    public native static Address addressWithRFC822String(String rfc822String);
    public native static Address addressWithNonEncodedRFC822String(String nonEncodedRFC822String);
    
    public native static List<Address> addressesWithRFC822String(String rfc822String);
    public native static List<Address> addressesWithNonEncodedRFC822String(String nonEncodedRFC822String);
    
    public native static String RFC822StringForAddresses(List<Address> addresses);
    public native static String nonEncodedRFC822StringForAddresses(List<Address> addresses);
    
    public Address()
    {
        setupNative();
    }
    
    public native String displayName();
    public native void setDisplayName(String displayName);
    
    public native String mailbox();
    public native void setMailbox(String address);
    
    public native String RFC822String();
    public native String nonEncodedRFC822String();
    
    private native void setupNative();
}
