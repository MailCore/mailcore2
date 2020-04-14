package com.libmailcore;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import java.util.HashMap;

class MainThreadUtils {
    private static volatile MainThreadUtils instance = new MainThreadUtils();
    private Handler handler;
    private HashMap<Long, Runnable> runnablesForIdentifiers = new HashMap<Long, Runnable>();
    
    static MainThreadUtils singleton() {
        return instance;
    }

    // private constructor
    private MainThreadUtils() {
        System.loadLibrary("MailCore");
	System.loadLibrary("c++_shared");
    //    System.loadLibrary("gnustl_shared");

    if(Looper.myLooper() != Looper.getMainLooper()) { Log.i("NATIVE", "Mailcore on bg thread"); } else { Log.i("NATIVE", "Mailcore on MAIN thread"); }

        //handler = new Handler(Looper.myLooper());
        handler = new Handler(Looper.getMainLooper());
        setupNative();
}

    private native void setupNative();

    private native void runIdentifier(long identifier);
    private native void runIdentifierAndNotify(long identifier);

    private void runOnMainThread(final long identifier) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                runIdentifier(identifier);
            }
        });
    }

    private void runOnMainThreadAndWait(final long identifier) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                runIdentifierAndNotify(identifier);
            }
        });
    }

    private void runAfterDelay(final long identifier, int milliseconds) {
        Runnable runnable = new Runnable() {
                    @Override
                    public void run() {
                        runnablesForIdentifiers.remove(new Long(identifier));
                        runIdentifier(identifier);
                    }
                };
        runnablesForIdentifiers.put(new Long(identifier), runnable);
        handler.postDelayed(runnable, milliseconds);
    }
    
    private void cancelDelayedRun(final long identifier) {
        Runnable runnable = runnablesForIdentifiers.get(new Long(identifier));
        runnablesForIdentifiers.remove(new Long(identifier));
        handler.removeCallbacks(runnable);
    }
}
