package com.libmailcore;

import java.util.List;

public class IMAPFetchFoldersOperation extends IMAPOperation {
    public native List<IMAPFolder> folders();
}
