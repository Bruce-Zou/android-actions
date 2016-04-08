package com.actions.support;

import android.os.ServiceManager;
import android.util.Log;

public class SupportManager {
	private static final String TAG = "SupportManager";

	private ISupportService mService;

	public SupportManager() {
		mService = ISupportService.Stub.asInterface(ServiceManager
				.getService("supportservice"));
		if (mService == null) {
			Log.e(TAG, "fail to get SupportService");
		}
	}

	public void monitorPackageRemove(String name, String action) {
		try {
			mService.monitorPackageRemove(name, action);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
