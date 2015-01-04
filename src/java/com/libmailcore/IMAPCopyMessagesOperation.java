package com.libmailcore;

import java.util.Map;

public class IMAPCopyMessagesOperation extends IMAPOperation {
    public native Map<Long,Long> uidMapping();
}
