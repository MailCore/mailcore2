package com.libmailcore;

public class IMAPQuotaOperation extends IMAPOperation {
    public native int usage();
    public native int limit();
}
