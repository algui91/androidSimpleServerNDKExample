package com.elbauldelprogramador.simpleserver;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	private TextView tv = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		/*
		 * Create a TextView and set its content. the text is retrieved by
		 * calling a native function.
		 */
		setContentView(R.layout.activity_main);
		tv = (TextView) findViewById(R.id.textView2);
		// tv.setText(stringFromJNI());
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
					return stringFromJNI();
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

	/*
	 * A native method that is implemented by the 'hello-jni' native library,
	 * which is packaged with this application.
	 */
	public native String stringFromJNI();

	/*
	 * This is another native method declaration that is *not* implemented by
	 * 'hello-jni'. This is simply to show that you can declare as many native
	 * methods in your Java code as you want, their implementation is searched
	 * in the currently loaded native libraries only the first time you call
	 * them.
	 * 
	 * Trying to call this function will result in a
	 * java.lang.UnsatisfiedLinkError exception !
	 */
	public native String unimplementedStringFromJNI();

	/*
	 * this is used to load the 'hello-jni' library on application startup. The
	 * library has already been unpacked into
	 * /data/data/com.example.hellojni/lib/libhello-jni.so at installation time
	 * by the package manager.
	 */
	static {
		System.loadLibrary("sServer");
	}
}
