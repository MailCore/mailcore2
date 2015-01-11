package com.libmailcore;

import java.util.List;

public class IndexSet extends NativeObject {
    public IndexSet() {
        setupNative();
    }
    
    public static native IndexSet indexSet();
    public static native IndexSet indexSetWithRange(Range range);
    public static native IndexSet indexSetWithIndex(long idx);
    
    public native int count();
    public native void addIndex(long idx);
    public native void removeIndex(long idx);
    public native boolean containsIndex(long idx);
    
    public native void addRange(Range range);
    public native void removeRange(Range range);
    public native void intersectsRange(Range range);
    
    public native void addIndexSet(IndexSet indexSet);
    public native void removeIndexSet(IndexSet indexSet);
    public native void intersectsIndexSet(IndexSet indexSet);
    
    public native List<Range> allRanges();
    public native int rangesCount();
    public native void removeAllIndexes();
    
    private native void setupNative();

    private static final long serialVersionUID = 1L;
}
