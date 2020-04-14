package com.libmailcore.androidexample;

import android.app.Activity;
import android.os.Bundle;
import android.app.ListFragment;
import android.os.Handler;
import android.os.Looper;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.util.Log;

import java.util.ArrayList;

import com.libmailcore.IMAPFetchMessagesOperation;
import com.libmailcore.IMAPMessage;
import com.libmailcore.MailException;
import com.libmailcore.OperationCallback;
import com.libmailcore.IndexSet;
import com.libmailcore.IMAPMessagesRequestKind;
import com.libmailcore.Range;

public class MailcoreThread
{
    public void start()
    {
        if(looper != null)
            return;

        Thread thread = new Thread()
        {
            @Override
            public void run()
            {
                if(Looper.myLooper() == null)
                    Looper.myLooper().prepare();
                looper = Looper.myLooper();
                looper.loop();
            }
        };

        thread.start();

        while(looper == null)
        {
            try
            {
                Thread.sleep(1);
            }
            catch(Exception x)
            {
                break;
            }
        }
    }

    public void stop()
    {
        looper.quit();
    }

    public Looper getLooper() { return looper; }

    public static MailcoreThread instance = new MailcoreThread();

    private Looper looper;

    private IMAPFetchMessagesOperation fetchMessagesOp;
    private ArrayAdapter<MessageAdapter> adapter;
    private java.util.List<IMAPMessage> messages;
}

