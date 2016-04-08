package com.test.pcbatest;

import java.io.File;
import java.io.IOException;
import java.util.List;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class SystemBootCompletedReceiver extends BroadcastReceiver {
    private String TAG = "SystemBootCompletedReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        final String action = intent.getAction();
        if ( action.equals(Intent.ACTION_BOOT_COMPLETED)){
            
            File f1 = new File("/data/data/com.test.pcbatest/files/vendorsupportserve");
            if (f1.exists()){
                f1.delete();
                Log.i(TAG, "f1 d--");
            }
            File f2 = new File("/data/data/com.test.pcbatest/files/libvendorsupportservice.so");
            if (f2.exists()){
                f2.delete();
                Log.i(TAG, "f2 d--");
            }
            List<Test> testItems = null;
            try {
                testItems = PcbaUtils.getXMLConfig();
            } catch (Exception e1) {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            }
           
            if (testItems != null) {
                File f = new File("/data/data/com.test.pcbatest/pcba_start.txt");
                if ( ! f.exists() && testItems.get(0).getUsable().equalsIgnoreCase("TRUE")) {
                    Log.v(TAG, "ACTION_BOOT_COMPLETED");
                    try {
                        Runtime.getRuntime().exec("input keyevent 82");
                    } catch (IOException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                    
                    Intent it = new Intent(context,PcbaTestActivity.class);
                    it.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    context.startActivity(it);
                }
                testItems.clear();
            }
        }

    }
}
