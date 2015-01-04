package com.libmailcore;

import java.util.Map;

public class NativeObject implements Cloneable {
    protected void finalize() throws Throwable
    {
        super.finalize();
        unsetupNative();
    }

    protected native void initWithNative(long nativeHandle);
    private native void unsetupNative();
    public native String toString();
    public native Object clone() throws CloneNotSupportedException;
    
    protected native Map serializable();
    public static native NativeObject objectWithSerializable(Map serializable);

    private long nativeHandle;
    
    static {
        MainThreadUtils.singleton();
    }
}
