package com.elbauldelprogramador.simpleserver;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	private static final String TAG = "SimpleServer";
	private TextView tv = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		tv = (TextView) findViewById(R.id.textView2);
	}

	public void onClick(View target) {
		switch (target.getId()) {
		case R.id.button1:
			
			tv.setText("Ejecuta 'telnet <ip> 7890' desde el pc");
			final Button myButton = (Button) findViewById(R.id.button1);
			myButton.setEnabled(false);
			
			new AsyncTask<Void, Void, String>() {
				@Override
				protected String doInBackground(Void... params) {
					return startTelnetSession();
				}

				@Override
				protected void onPostExecute(String result) {
					tv.setText(result);
					myButton.setEnabled(true);
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
