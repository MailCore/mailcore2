package com.libmailcore;

import java.util.List;

public class NNTPFetchOverviewOperation extends NNTPOperation {
    public native List<MessageHeader> articles();
}
