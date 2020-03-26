package com.seeta.sdk;

import android.util.Log;

public class EyeBlinkDetector
{
    static {
        Log.e("EyeBlinkDetector", "Start Load");
        System.loadLibrary("EyeBlinkDetectorJni");
        Log.e("EyeBlinkDetector", "Finish Load");
    }

	public long impl = 0;	// native object pointer

	private native void construct(String model);
	private native void construct(String model, String device);
	public native void dispose();

    public EyeBlinkDetector() {
        this("");
    }

	public EyeBlinkDetector(String model) {
		this.construct(model);
	}

	public EyeBlinkDetector(String model, String device) {
		this.construct(model, device);
	}

	protected void finalize() throws java.lang.Throwable {
        super.finalize();
		this.dispose();
    }
	
	public static int LEFT_EYE  =  1;
	public static int RIGHT_EYE  =  2;
	
	public native int Detect(ImageData image, Point[] landmarks);//���ض���������ʾ�Ƿ�գ�ۣ����۱��ϻ��LEFT_EYEλ��Ϊ1�����۱��ϻ��RIGHT_EYEλ��Ϊ1��
}
