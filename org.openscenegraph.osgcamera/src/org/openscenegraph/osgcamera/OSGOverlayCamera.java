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
package org.openscenegraph.osgcamera;

import org.openscenegraph.osg.db.ReadFile;
import org.openscenegraph.osg.viewer.Viewer;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;

public class OSGOverlayCamera extends Activity {
	
	CameraPreview mPreview;
	Viewer mOverlaySurfaceView;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// Hide the window title.
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		mPreview = new CameraPreview(this);
		mOverlaySurfaceView = new Viewer(this);
		mOverlaySurfaceView.init(true, 16, 8);
		mOverlaySurfaceView.getCamera().setClearColor(0,0,0,0);
		mOverlaySurfaceView.setSceneData(ReadFile.readNodeFile("/sdcard/axes.ive"));
		mOverlaySurfaceView.setDefaultSettings();

		setContentView(mOverlaySurfaceView);
		addContentView(mPreview, new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT));
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}
	
}
