package com.libmailcore;

public class IMAPMessagesRequestKind {
    public final static int IMAPMessagesRequestKindUid = 0;
    public final static int IMAPMessagesRequestKindFlags = 1 << 0;
    public final static int IMAPMessagesRequestKindHeaders = 1 << 1;
    public final static int IMAPMessagesRequestKindStructure = 1 << 2;
    public final static int IMAPMessagesRequestKindInternalDate = 1 << 3;
    public final static int IMAPMessagesRequestKindFullHeaders = 1 << 4;
    public final static int IMAPMessagesRequestKindHeaderSubject = 1 << 5;
    public final static int IMAPMessagesRequestKindGmailLabels = 1 << 6;
    public final static int IMAPMessagesRequestKindGmailMessageID = 1 << 7;
    public final static int IMAPMessagesRequestKindGmailThreadID = 1 << 8;
    public final static int IMAPMessagesRequestKindExtraHeaders = 1 << 9;
    public final static int IMAPMessagesRequestKindSize = 1 << 10;
}
