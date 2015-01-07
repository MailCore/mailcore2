package com.libmailcore;

import java.util.List;

public class POPFetchMessagesOperation extends POPOperation {
    public native List<POPMessageInfo> messages();
}
