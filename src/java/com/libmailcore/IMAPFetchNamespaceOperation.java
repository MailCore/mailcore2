package com.libmailcore;

import java.util.Map;

public class IMAPFetchNamespaceOperation extends IMAPOperation {
    public native Map<String,IMAPNamespace> namespaces();
}
