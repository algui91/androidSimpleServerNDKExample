
package com.elbauldelprogramador.simpleserver;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

    private static final String TAG = "SimpleServer";
    private TextView mTextView;
    private Button mButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextView = (TextView) findViewById(R.id.textView2);
    }

    public void onClick(View target) {
        switch (target.getId()) {
            case R.id.button1:

                mTextView.setText("Ejecuta 'telnet <ip> 7890' desde el pc");
                mButton = (Button) findViewById(R.id.button1);
                mButton.setEnabled(false);

                new AsyncTask<Void, Void, String>() {
                    @Override
                    protected String doInBackground(Void... params) {
                        return startTelnetSession();
                    }

                    @Override
                    protected void onPostExecute(String result) {
                        mTextView.setText(result);
                        mButton.setEnabled(true);
                    }
                }.execute();

                break;
            default:
                break;
        }
    }

    public native String startTelnetSession();

    static {
        Log.d(TAG, "libsServer loaded.");
        System.loadLibrary("sServer");
    }
}
