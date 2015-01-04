package com.libmailcore;

import java.util.List;

public class IMAPNamespace extends NativeObject {
    public native String mainPrefix();
    public native char mainDelimiter();
    
    public native List<String> prefixes();
    
    public native String pathForComponents(List<String> components);
    public native String pathForComponentsAndPrefix(List<String> components, String prefix);
    
    public native List<String> componentsFromPath(String path);
    
    public native boolean containsFolderPath(String path);
    
    public static native IMAPNamespace namespaceWithPrefix(String prefix, char delimiter);
}
