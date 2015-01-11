package com.libmailcore;

import java.util.List;

public class IMAPMessage extends AbstractMessage {
    public native long sequenceNumber();
    public native void setSequenceNumber(long sequenceNumber);
    
    public native long uid();
    public native void setUid(long uid);
    
    public native long size();
    public native void setSize(long size);
    
    public native void setFlags(int flags);
    public native int flags();
    
    public native void setOriginalFlags(int flags);
    public native int originalFlags();
    
    public native void setCustomFlags(List<String> customFlags);
    public native List<String> customFlags();
    
    public native long modSeqValue();
    public native void setModSeqValue(long uid);
    
    public native void setMainPart(AbstractPart mainPart);
    public native AbstractPart mainPart();
    
    public native void setGmailLabels(List<String> labels);
    public native List<String> gmailLabels();
    
    public native void setGmailMessageID(long msgID);
    public native long gmailMessageID();
            
    public native void setGmailThreadID(long threadID);
    public native long gmailThreadID();
    
    public native AbstractPart partForPartID(String partID);
    
    public native AbstractPart partForContentID(String contentID);
    public native AbstractPart partForUniqueID(String uniqueID);
    
    public native String htmlRendering(String folder,
                                   HTMLRendererIMAPCallback dataCallback,
                                   HTMLRendererTemplateCallback htmlCallback);

    public String htmlRendering(String folder,
                                HTMLRendererIMAPCallback dataCallback)
    {
        return htmlRendering(folder, dataCallback, null);
    }

    private static final long serialVersionUID = 1L;
}
