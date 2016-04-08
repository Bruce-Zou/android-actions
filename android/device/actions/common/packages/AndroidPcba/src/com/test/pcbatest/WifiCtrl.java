package com.test.pcbatest;

import java.util.List;

import android.content.Context;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.renderscript.Sampler.Value;
import android.text.format.Formatter;
import android.util.Log;

public class WifiCtrl {
    private final static String TAG = "WifiCtrl";
    private StringBuffer mStringBuffer = new StringBuffer();
    private List<ScanResult> listResult;
    private ScanResult mScanResult;
    
    private WifiManager mWifiManager;
    
    private WifiInfo mWifiInfo = null;
    
    private List<WifiConfiguration> mWifiConfiguration;
    
    WifiLock mWifiLock;
    
    private Context mContext;

    /**
     * 
     */
    public WifiCtrl(Context context) {
        mContext = context;
        mWifiManager = (WifiManager) context
                .getSystemService(Context.WIFI_SERVICE);
        mWifiInfo = mWifiManager.getConnectionInfo();
        
    }
    /**
     * 
     */
    public boolean wifiIsEnable(){
        return mWifiManager.setWifiEnabled(true);
    }
    /**
     * 
     */
    public void openNetCard() {
        if (!mWifiManager.isWifiEnabled()) {
            mWifiManager.setWifiEnabled(true);
        }
    }

    /**
     * 
     */
    public void closeNetCard() {
        if (mWifiManager.isWifiEnabled()) {
            mWifiManager.setWifiEnabled(false);
        }
    }
    
    public int getNetCardWorkState() {
        int state = mWifiManager.getWifiState();
        if (state == WifiManager.WIFI_STATE_DISABLING) {
            Log.i(TAG, "WIFI_STATE_DISABLING");
        } else if (state == WifiManager.WIFI_STATE_DISABLED) {
            Log.i(TAG, "WIFI_STATE_DISABLED");
        } else if (state == WifiManager.WIFI_STATE_ENABLING) {
            Log.i(TAG, "WIFI_STATE_ENABLING");
        } else if (state == WifiManager.WIFI_STATE_ENABLED) {
            Log.i(TAG, "WIFI_STATE_ENABLED");
        } else {
            //Log.i(TAG, "---_---");
        }
        return state;
    }
    public void checkNetCardState() {
        if (mWifiManager.getWifiState() == 0) {
            //Log.i(TAG, "closing");
        } else if (mWifiManager.getWifiState() == 1) {
            //Log.i(TAG, "closed");
        } else if (mWifiManager.getWifiState() == 2) {
            //Log.i(TAG, "opening");
        } else if (mWifiManager.getWifiState() == 3) {
            //Log.i(TAG, "opened");
        } else {
            //Log.i(TAG, "---_---......didnt get the state---_---");
        }
    }
    
    public void scan() {
        mWifiManager.startScan();
        listResult = mWifiManager.getScanResults();
        if (listResult != null) {
            //Log.i(TAG, "scanResults != null");
        } else {
           // Log.i(TAG, "scanResults == null");
        }
    }
    
    
    public ScanResult getScanResult(int index) {
        if (listResult == null || listResult.size() <= 0 || index >= listResult.size()) {
            return null;
        }
        return listResult.get(index);
    }
    public String getScanResult() {
        // clear the scanresult last time
        if (mStringBuffer != null) {
            mStringBuffer = new StringBuffer();
        }
        // start to scan
        scan();
        listResult = mWifiManager.getScanResults();
        if (listResult != null) {
            mStringBuffer.append("INDEX -> SSID : BSSID : CAPABILITIES : FREQUENCY : LEVEL : DESC\n\n");
            for (int i = 0; i < listResult.size(); i++) {
                mScanResult = listResult.get(i);
                /*TODO:should be fixed======
                String tempstr = mContext.getString(R.string.test_wifi_signal);
                if(mScanResult.level >= -50) {
                    tempstr += mContext.getString(R.string.test_wifi_signal_very_good);
                } else if(mScanResult.level < -50 && mScanResult.level >= -60) {
                    tempstr += mContext.getString(R.string.test_wifi_signal_good);
                } else if(mScanResult.level < -60 && mScanResult.level >= -70) {
                    tempstr += mContext.getString(R.string.test_wifi_signal_ok);
                } else if(mScanResult.level < -70 && mScanResult.level >= -80) {
                    tempstr += mContext.getString(R.string.test_wifi_signal_poor);
                } else if(mScanResult.level < -80) {
                    tempstr += mContext.getString(R.string.test_wifi_signal_very_poor);
                } 
                
                mStringBuffer = mStringBuffer.append("NO.").append(i + 1)
                        .append("-> ").append(mScanResult.SSID).append(" : ")
                        .append(mScanResult.BSSID).append(" : ")
                        .append(mScanResult.capabilities).append(" : ")
                        .append(mScanResult.frequency).append(" : ")
                        .append(mScanResult.level).append(" : ")
                        .append(mScanResult.describeContents()).append("  ")
                        .append(tempstr).append("\n\n");
                
                */
            }
        }
        //Log.i(TAG, mStringBuffer.toString());
        return mStringBuffer.toString();
    }

    /**
     * 
     */
    public void connect() {
        mWifiInfo = mWifiManager.getConnectionInfo();
        
    }
    public void updateConnectInfo() {
        mWifiInfo = mWifiManager.getConnectionInfo();
        
    }

    /**
     * 
     */
    public void disconnectWifi() {
        int netId = getNetworkId();
        mWifiManager.disableNetwork(netId);
        mWifiManager.disconnect();
        mWifiInfo = null;
    }

    /**
     * 
     * 
     * @return String
     */
    public boolean isNetWorkOK() {
        if (mWifiInfo != null) {
            //Log.i(TAG, "work well");
            return true;
        } else {
           // Log.i(TAG, "disconnected");
            return false;
        }
    }
    public void checkNetWorkState() {
        if (mWifiInfo != null) {
            //Log.i(TAG, "work well");
        } else {
            //Log.i(TAG, "disconnected");
        }
    }

    /**
     * 
     */
    public int getNetworkId() {
        return (mWifiInfo == null) ? 0 : mWifiInfo.getNetworkId();
    }

    /**
     * 
     */
    public int getIPAddress() {
        return (mWifiInfo == null) ? 0 : mWifiInfo.getIpAddress();
    }
    /**
     * 
     */
    public int getGatewayAddress() {
        if(mWifiManager != null) {
            
            return mWifiManager.getDhcpInfo().gateway;
        }
        return 0;
    }


    // 
    public void acquireWifiLock() {
        mWifiLock.acquire();
    }

    // 
    public void releaseWifiLock() {
        if (mWifiLock.isHeld()) {
            mWifiLock.acquire();
        }
    }

    public void creatWifiLock() {
        mWifiLock = mWifiManager.createWifiLock("Test");
    }

    
    public List<WifiConfiguration> getConfiguration() {
        return mWifiConfiguration;
    }

    public void connectConfiguration(int index) {
        // 
        if (index >= mWifiConfiguration.size()) {
            return;
        }
        // 
        mWifiManager.enableNetwork(mWifiConfiguration.get(index).networkId,
                true);
    }

    
    public String getMacAddress() {
        return (mWifiInfo == null) ? "NULL" : mWifiInfo.getMacAddress();
    }

    
    public String getBSSID() {
        return (mWifiInfo == null) ? "NULL" : mWifiInfo.getBSSID();
    }

    
    public String getWifiInfo() {
        return (mWifiInfo == null) ? "NULL" : mWifiInfo.toString();
    }

    
    public int addNetwork(WifiConfiguration wcg) {
        int wcgID = mWifiManager.addNetwork(mWifiConfiguration.get(3));
        mWifiManager.enableNetwork(wcgID, true);
        return wcgID;
    }
    
    public String getConnectedSSID() {
        String retval = null;
         if(mWifiInfo == null || mWifiInfo.getSSID()== null || mWifiInfo.getSSID().equals("null")) {
            return null;
         }
         retval = mWifiInfo.getSSID();
//         return retval + " " +String.valueOf( mWifiInfo.getRssi()+"dBm");
         return retval;
    }
    
    public String getRssi() {
        if(mWifiInfo == null || mWifiInfo.getSSID()== null || mWifiInfo.getSSID().equals("null")) {
            return null;
         }
        return String.valueOf( mWifiInfo.getRssi()+"dBm");
        
    }
    public String getConnectedInfo() {
        String retval = null;//mContext.getString(R.string.test_wifi_unconnected);
        if(mWifiInfo == null)
            return retval;
        
        if(mWifiInfo.getSSID()== null || mWifiInfo.getSSID().equals("null")) {
            return retval;
        } else {
            int rssi = mWifiInfo.getRssi();
            /*TODO:should be fixed====
            String signal = mContext.getString(R.string.test_wifi_signal);
            if(rssi >= -50) {
                signal += mContext.getString(R.string.test_wifi_signal_very_good);
            } else if(rssi < -50 && rssi >= -60) {
                signal += mContext.getString(R.string.test_wifi_signal_good);
            } else if(rssi < -60 && rssi >= -70) {
                signal += mContext.getString(R.string.test_wifi_signal_ok);
            } else if(rssi < -70 && rssi >= -80) {
                signal += mContext.getString(R.string.test_wifi_signal_poor);
            } else if(rssi < -80) {
                signal += mContext.getString(R.string.test_wifi_signal_very_poor);
            } 
            
            String speed = mContext.getString(R.string.test_wifi_speed) + mWifiInfo.getLinkSpeed() + "Mbps";
            retval = mWifiInfo.toString() + "\n"+ signal + "\n"+ speed;
            */
        }        
        return retval;
    }
}
