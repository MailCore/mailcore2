package com.libmailcore;

import java.util.List;

public class IMAPNamespaceItem extends NativeObject {
    
    public native void setPrefix(String prefix);
    public native String prefix();

    public native void setDelimiter(char delimiter);
    public native char delimiter();

    public native String pathForComponents(List<String> components);
    public native List<String> componentsForPath(String path);

    public native boolean containsFolder(String folder);
}
