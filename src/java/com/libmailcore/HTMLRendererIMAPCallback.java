package com.libmailcore;

public interface HTMLRendererIMAPCallback {
    byte[] dataForIMAPPart(String folder, IMAPPart part);
    void prefetchAttachmentIMAPPart(String folder, IMAPPart part);
    void prefetchImageIMAPPart(String folder, IMAPPart part);
}
