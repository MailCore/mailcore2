package com.libmailcore;

public class IMAPFolderFlags {
    public final static int IMAPFolderFlagNone = 0;
    public final static int IMAPFolderFlagMarked      = 1 << 0;
    public final static int IMAPFolderFlagUnmarked    = 1 << 1;
    public final static int IMAPFolderFlagNoSelect    = 1 << 2;
    public final static int IMAPFolderFlagNoInferiors = 1 << 3;
    public final static int IMAPFolderFlagInbox       = 1 << 4;
    public final static int IMAPFolderFlagSentMail    = 1 << 5;
    public final static int IMAPFolderFlagStarred     = 1 << 6;
    public final static int IMAPFolderFlagAllMail     = 1 << 7;
    public final static int IMAPFolderFlagTrash       = 1 << 8;
    public final static int IMAPFolderFlagDrafts      = 1 << 9;
    public final static int IMAPFolderFlagSpam        = 1 << 10;
    public final static int IMAPFolderFlagImportant   = 1 << 11;
    public final static int IMAPFolderFlagArchive     = 1 << 12;
    public final static int IMAPFolderFlagAll = IMAPFolderFlagAllMail;
    public final static int IMAPFolderFlagJunk = IMAPFolderFlagSpam;
    public final static int IMAPFolderFlagFlagged = IMAPFolderFlagStarred;
    public final static int IMAPFolderFlagFolderTypeMask = IMAPFolderFlagInbox | IMAPFolderFlagSentMail | IMAPFolderFlagStarred | IMAPFolderFlagAllMail |
        IMAPFolderFlagTrash| IMAPFolderFlagDrafts | IMAPFolderFlagSpam | IMAPFolderFlagImportant | IMAPFolderFlagArchive;
}
