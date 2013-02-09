/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait‡n, Mirage Technologies S.L.     |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
package org.openscenegraph.osgsimple;

import java.io.File;

import org.openscenegraph.osg.db.ReadFile;
import org.openscenegraph.osg.viewer.Viewer;

import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.InputMethodManager;

public class OSGActivity extends Activity {
	Viewer mView;

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		try {
			mView = new Viewer(this);
			mView.init(false, 16, 8);
			File externalStorage = Environment.getExternalStorageDirectory();
			String path = externalStorage + "/osgAndroid/dumptruck.osg";
			mView.setSceneData(ReadFile.readNodeFile(path));
			mView.setDefaultSettings();
			setContentView(mView);
		} catch (Exception e) {
			Log.w("OSGActivity", e.getMessage());
		}
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}
	

	@Override
	protected void onPause() {
		super.onPause();
		mView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mView.onResume();
	}
	

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		boolean value = false;
		switch (keyCode) {
		case KeyEvent.KEYCODE_HOME:
		case KeyEvent.KEYCODE_BACK:
		case KeyEvent.KEYCODE_MENU:
			break;
		default:
			value = mView.onKeyDown(keyCode, event);
			break;
		}
		return value;
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		boolean value = false;
		switch (keyCode) {
		case KeyEvent.KEYCODE_MENU:
			InputMethodManager mgr = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
			mgr.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
			value = true;
			break;
		case KeyEvent.KEYCODE_BACK:
		case KeyEvent.KEYCODE_HOME:
			value = true;
			finish();
			break;
		default:
			value = mView.onKeyUp(keyCode, event);
			break;
		}
		return value;
	}
}