package com.libmailcore;

/** Operation to fetch the capabilities of the IMAP server. */
public class IMAPCapabilityOperation extends IMAPOperation {
    public native IndexSet capabilities();
}
