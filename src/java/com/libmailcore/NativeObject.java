package com.libmailcore;

import java.util.Map;
import java.io.Serializable;
import java.io.IOException;

public class NativeObject implements Cloneable, Serializable {
    protected void finalize() throws Throwable
    {
        super.finalize();
        unsetupNative();
    }

    protected native void initWithNative(long nativeHandle);
    private native void unsetupNative();
    public native String toString();
    public native Object clone() throws CloneNotSupportedException;

    private long nativeHandle;

    private static final long serialVersionUID = 1L;

    protected void writeObject(java.io.ObjectOutputStream out) throws IOException {
        byte[] data = serializableData();
        out.writeInt(data.length);
        out.write(data, 0, data.length);
    }

    protected void readObject(java.io.ObjectInputStream in) throws IOException, ClassNotFoundException {
        int len = in.readInt();
        byte[] data = new byte[len];
        in.read(data, 0, len);
    }

    private native byte[] serializableData();
    private native void importSerializableData(byte[] data);

    static {
        MainThreadUtils.singleton();
    }
}
