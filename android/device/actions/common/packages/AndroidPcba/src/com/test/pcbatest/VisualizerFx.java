package com.test.pcbatest;

import java.io.IOException;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.audiofx.Equalizer;
import android.media.audiofx.Visualizer;
import android.net.Uri;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;

public class VisualizerFx extends LinearLayout {
    private static final String TAG = "AudioFxActivity";

    private static final float VISUALIZER_HEIGHT_DIP = 160f;

    private LinearLayout mLinearLayout;
    private MediaPlayer mMediaPlayer;
    private Visualizer mVisualizer;
    private VisualizerView mVisualizerView;
    private Context mContext;
    private int mMaxVolumeFadeIn = 0;
    private int mCurVolumeFadeIn = 0;

    final Handler mHandler = new Handler();
    Runnable mUpdateFadeIn = new Runnable() {
        public void run() {
            if (mCurVolumeFadeIn < 10) {
                mCurVolumeFadeIn++;
                AudioManager audMgr = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
                // audMgr.setStreamVolume(AudioManager.STREAM_MUSIC,
                // mCurVolumeFadeIn, mCurVolumeFadeIn);
				if (mMediaPlayer != null && mHandler != null) {
					mMediaPlayer.setVolume((float) mCurVolumeFadeIn / 10.0F, (float) mCurVolumeFadeIn / 10.0F);
					mHandler.postDelayed(mUpdateFadeIn, 200);
				}
            }
        }
    };

    public VisualizerFx(Context context) {
        super(context);
        mContext = context;
        // TODO Auto-generated constructor stub
        initialize();
    }

    public VisualizerFx(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
        initialize();
    }

    private void initialize() {
        mMediaPlayer = MediaPlayer.create(mContext, R.raw.scarborough_fair);

        mLinearLayout = this;
        setupVisualizerFxAndUI();

        // Make sure the visualizer is enabled only when you actually want to
        // receive data, and
        // when it makes sense to receive data.
        mVisualizer.setEnabled(true);

        // When the stream ends, we don't need to collect any more data. We
        // don't do this in
        // setupVisualizerFxAndUI because we likely want to have more,
        // non-Visualizer related code
        // in this callback.
        mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            public void onCompletion(MediaPlayer mediaPlayer) {
                // mVisualizer.setEnabled(false);
                mMediaPlayer.start();
               //mStatusTextView.setText("play over");
            }
        });

        // getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // setVolumeControlStream(mContext,
        // AudioManager.STREAM_MUSIC);//==================
        // mMediaPlayer.stop();
        // mMediaPlayer.start();
    }

    private void setupVisualizerFxAndUI() {
        mVisualizerView = new VisualizerView(mContext);
        mVisualizerView.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                (int) (VISUALIZER_HEIGHT_DIP * getResources().getDisplayMetrics().density)));
        mLinearLayout.removeAllViews();
        mLinearLayout.addView(mVisualizerView);

        final int maxCR = Visualizer.getMaxCaptureRate();
        mVisualizer = new Visualizer(mMediaPlayer.getAudioSessionId());
        mVisualizer.setCaptureSize(256);
        mVisualizer.setDataCaptureListener(new Visualizer.OnDataCaptureListener() {
            public void onWaveFormDataCapture(Visualizer visualizer, byte[] bytes, int samplingRate) {
                mVisualizerView.updateVisualizer(bytes);
            }

            public void onFftDataCapture(Visualizer visualizer, byte[] fft, int samplingRate) {
                mVisualizerView.updateVisualizer(fft);
            }
        }, maxCR / 2, false, true);
    }

    /**
     * A simple class that draws waveform data received from a
     * {@link Visualizer.OnDataCaptureListener#onWaveFormDataCapture }
     */
    class VisualizerView extends View {
        private byte[] mBytes;
        private float[] mPoints;
        private Rect mRect = new Rect();

        private Paint mForePaint = new Paint();
        private int mSpectrumNum = 48;
        private boolean mFirst = true;

        public VisualizerView(Context context) {
            super(context);
            init();
        }

        private void init() {
            mBytes = null;

            mForePaint.setStrokeWidth(8f);
            mForePaint.setAntiAlias(true);
            mForePaint.setColor(Color.rgb(0, 128, 255));
        }

        public void updateVisualizer(byte[] fft) {
            if (mFirst) {
                // mInfoView.setText(mInfoView.getText().toString() +
                // "\nCaptureSize: " + fft.length);
                mFirst = false;
            }

            byte[] model = new byte[fft.length / 2 + 1];

            model[0] = (byte) Math.abs(fft[0]);
            for (int i = 2, j = 1; j < mSpectrumNum;) {
                model[j] = (byte) Math.hypot(fft[i], fft[i + 1]);
                i += 2;
                j++;
            }
            mBytes = model;
            invalidate();
        }

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);

            if (mBytes == null) {
                return;
            }

            if (mPoints == null || mPoints.length < mBytes.length * 4) {
                mPoints = new float[mBytes.length * 4];
            }

            mRect.set(0, 0, getWidth(), getHeight());

          //draw
            // for (int i = 0; i < mBytes.length - 1; i++) {
            // mPoints[i * 4] = mRect.width() * i / (mBytes.length - 1);
            // mPoints[i * 4 + 1] = mRect.height() / 2
            // + ((byte) (mBytes[i] + 128)) * (mRect.height() / 2) / 128;
            // mPoints[i * 4 + 2] = mRect.width() * (i + 1) / (mBytes.length -
            // 1);
            // mPoints[i * 4 + 3] = mRect.height() / 2
            // + ((byte) (mBytes[i + 1] + 128)) * (mRect.height() / 2) / 128;
            // }

          //draw
            final int baseX = mRect.width() / mSpectrumNum;
            final int height = mRect.height();

            for (int i = 0; i < mSpectrumNum; i++) {
                if (mBytes[i] < 0) {
                    mBytes[i] = 127;
                }

                final int xi = baseX * i + baseX / 2;

                mPoints[i * 4] = xi;
                mPoints[i * 4 + 1] = height;

                mPoints[i * 4 + 2] = xi;
                mPoints[i * 4 + 3] = height - mBytes[i];
            }

            canvas.drawLines(mPoints, mForePaint);
        }
    }

    public void stop() {
        if (mMediaPlayer != null) {
            mMediaPlayer.stop();
        }
    }

    public void start() {
        if (mMediaPlayer != null) {
            mMediaPlayer.start();
        }
    }

    public void pause() {
        if (mMediaPlayer != null) {
            mMediaPlayer.pause();
        }
    }

    public void fadeIn() {
        AudioManager audMgr = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
        mMaxVolumeFadeIn = audMgr.getStreamVolume(AudioManager.STREAM_MUSIC);
        mCurVolumeFadeIn = 0;
        // audMgr.setStreamVolume(AudioManager.STREAM_MUSIC, mCurVolumeFadeIn,
        // mCurVolumeFadeIn);
        mMediaPlayer.setVolume((float) mCurVolumeFadeIn, (float) mCurVolumeFadeIn);
        mHandler.postDelayed(mUpdateFadeIn, 100);
    }

    public void changePlayFile(Uri uri,boolean reocordfile) {
        mVisualizer.setEnabled(false);
        try {
            releaseMediaPlayer();
            if (reocordfile) {
              mMediaPlayer = MediaPlayer.create(mContext, uri);
            }else {
                mMediaPlayer = MediaPlayer.create(mContext, R.raw.scarborough_fair);
            }
//            mMediaPlayer = MediaPlayer.create(mContext, uri);
//            mMediaPlayer = MediaPlayer.create(mContext, R.raw.scarborough_fair);
            // mVisualizer.release();
            setupVisualizerFxAndUI();
            mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                public void onCompletion(MediaPlayer mediaPlayer) {
                    // mVisualizer.setEnabled(false);
                    mMediaPlayer.start();
                    // mStatusTextView.setText("music player");
                }
            });
            mMediaPlayer.start();

            /*
             * mMediaPlayer.stop(); mMediaPlayer.reset();
             * mMediaPlayer.setDataSource(file); mMediaPlayer.prepare();
             * mMediaPlayer.start();
             */
        } catch (Exception e) {
            e.printStackTrace();
        }
        mVisualizer.setEnabled(true);
    }

    private void releaseMediaPlayer() {
        if (mMediaPlayer != null) {
            if (mMediaPlayer.isPlaying()) {
                mMediaPlayer.stop();
            }
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
    }

    public void release() {
        mVisualizer.setEnabled(false);
        releaseMediaPlayer();
    }
}
