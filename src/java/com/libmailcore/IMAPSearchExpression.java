package com.libmailcore;

import java.util.Date;

public class IMAPSearchExpression extends NativeObject {
    public IMAPSearchExpression()
    {
        setupNative();
    }
    
    public static native IMAPSearchExpression searchAll();
    public static native IMAPSearchExpression searchFrom(String value);
    public static native IMAPSearchExpression searchTo(String value);
    public static native IMAPSearchExpression searchCc(String value);
    public static native IMAPSearchExpression searchBcc(String value);
    public static native IMAPSearchExpression searchRecipient(String value);
    public static native IMAPSearchExpression searchSubject(String value);
    public static native IMAPSearchExpression searchContent(String value);
    public static native IMAPSearchExpression searchBody(String value);
    public static native IMAPSearchExpression searchHeader(String header, String value);
    public static native IMAPSearchExpression searchUIDs(IndexSet uids);
    public static native IMAPSearchExpression searchRead();
    public static native IMAPSearchExpression searchUnread();
    public static native IMAPSearchExpression searchFlagged();
    public static native IMAPSearchExpression searchUnflagged();
    public static native IMAPSearchExpression searchAnswered();
    public static native IMAPSearchExpression searchUnanswered();
    public static native IMAPSearchExpression searchDraft();
    public static native IMAPSearchExpression searchUndraft();
    public static native IMAPSearchExpression searchDeleted();
    public static native IMAPSearchExpression searchSpam();
    public static native IMAPSearchExpression searchBeforeDate(Date date);
    public static native IMAPSearchExpression searchOnDate(Date date);
    public static native IMAPSearchExpression searchSinceDate(Date date);
    public static native IMAPSearchExpression searchBeforeReceivedDate(Date date);
    public static native IMAPSearchExpression searchOnReceivedDate(Date date);
    public static native IMAPSearchExpression searchSinceReceivedDate(Date date);
    public static native IMAPSearchExpression searchSizeLarger(long size);
    public static native IMAPSearchExpression searchSizeSmaller(long size);
    public static native IMAPSearchExpression searchGmailThreadID(long number);
    public static native IMAPSearchExpression searchGmailMessageID(long number);
    public static native IMAPSearchExpression searchGmailRaw(String expr);
    public static native IMAPSearchExpression searchAnd(IMAPSearchExpression left, IMAPSearchExpression right);
    public static native IMAPSearchExpression searchOr(IMAPSearchExpression left, IMAPSearchExpression right);
    public static native IMAPSearchExpression searchNot(IMAPSearchExpression notExpr);
    
    private native void setupNative();
}
