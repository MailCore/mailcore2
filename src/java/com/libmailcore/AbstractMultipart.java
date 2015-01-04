package com.libmailcore;

import java.util.List;

public class AbstractMultipart extends AbstractPart {
    public native List<AbstractPart> parts();
    public native void setParts(List<AbstractPart> parts);
}
