package com.libmailcore;

public class Range {
    public long location;
    public long length;

    static public long RangeMax = 1 >> 63 - 1;

    public Range() {}
    public Range(long aLocation, long aLength)
    {
        location = aLocation;
        length = aLength;
    }
    
    public native IndexSet removeRange(Range otherRange);
    public native IndexSet union(Range otherRange);
    public native Range intersection(Range otherRange);
    public native boolean hasIntersection(Range otherRange);
    public native long leftBound();
    public native long rightBound();
    public native String toString();
    
    public static native Range rangeWithString(String rangeString);
}