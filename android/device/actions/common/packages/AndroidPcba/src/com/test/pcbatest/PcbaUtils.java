package com.test.pcbatest;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;

import android.util.Log;
import android.util.Xml;

public class PcbaUtils {
    private static final String TAG = "PcbaUtils";
    private static final String XML_CONFIG_NAME = "/system/etc/actions_pcba_android.xml";

    public static List<Test> getXMLConfig() throws Exception {
        InputStream xml = null;
        try {
            File f = new File(XML_CONFIG_NAME);
            if (!f.exists()) {
                return null;
            }
            xml = new FileInputStream(XML_CONFIG_NAME);

        } catch (FileNotFoundException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }
        List<Test> config = null;
        Test item = null;
        XmlPullParser pullParser = Xml.newPullParser();
        pullParser.setInput(xml, "UTF-8"); // 为Pull解释器设置要解析的XML数据
        int event = pullParser.getEventType();

        while (event != XmlPullParser.END_DOCUMENT) {

            switch (event) {

            case XmlPullParser.START_DOCUMENT:
                // Log.v(TAG, "XmlPullParser.START_DOCUMENT");
                config = new ArrayList<Test>();
                break;
            case XmlPullParser.START_TAG:
                // Log.v(TAG, "XmlPullParser.START_TAG");
                if ("TestCase".equals(pullParser.getName())) {
                    int id = Integer.valueOf(pullParser.getAttributeValue(0));
                    Log.v(TAG, "id:" + id);
                    item = new Test();
                    item.setId(id);
                }
                if ("name".equals(pullParser.getName())) {
                    String name = pullParser.nextText();
                    // Log.v(TAG, "name:"+name);
                    item.setName(name);
                }
                if ("usable".equals(pullParser.getName())) {
                    String usable = pullParser.nextText();
                    // Log.v(TAG, "usable:"+usable);
                    item.setUsable(usable);
                }
                break;
            case XmlPullParser.END_TAG:
                // Log.v(TAG, "XmlPullParser.END_TAG");
                if ("TestCase".equals(pullParser.getName())) {
                    config.add(item);
                    item = null;
                }
                break;

            }

            event = pullParser.next();
        }

        xml.close();
        return config;
    }

}
