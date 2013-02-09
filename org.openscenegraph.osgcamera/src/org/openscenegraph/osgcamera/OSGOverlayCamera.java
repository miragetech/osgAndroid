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

import java.io.File;

import org.openscenegraph.osg.core.Matrix;
import org.openscenegraph.osg.core.MatrixTransform;
import org.openscenegraph.osg.core.Node;
import org.openscenegraph.osg.db.ReadFile;
import org.openscenegraph.osg.viewer.Viewer;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Environment;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;

public class OSGOverlayCamera extends Activity {

	CameraPreview mPreview;
	Viewer mOverlaySurfaceView;
	private MatrixTransform mRoot;

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
		File externalStorage = Environment.getExternalStorageDirectory();
		String path = externalStorage + "/osgAndroid/axes.ive";
		Node scene = ReadFile.readNodeFile(path);
		
		// Testing y-up conversion matrix
		Matrix y_up_mat = 
			new Matrix(1f,  0,  0, 0,
					   0,   0, 1f, 0,
				       0, -1f,  0, 0,
				       0,   0,  0, 1);

		mRoot = new MatrixTransform();
		mRoot.setMatrix(y_up_mat);

		MatrixTransform mt1 = new MatrixTransform();
		Matrix m1 = new Matrix();
		m1.makeTranslate(-3.0f, 0, 0);
		mt1.setMatrix(m1);
		mt1.addChild(scene);

		MatrixTransform mt2 = new MatrixTransform();
		Matrix m2 = new Matrix();
		m2.makeScale(2.0f, 2.0f, 2.0f);
		mt2.setMatrix(m2);
		mt2.addChild(scene);

		MatrixTransform mt3 = new MatrixTransform();
		Matrix m3 = new Matrix(
				1,   0, 0,0,
				0,   1, 0,0,
				0,   0, 1,0,
				3.0f,0, 0,1);
		Matrix s3 = new Matrix(
				1.5f,0,   0,   0,
				0,   1.5f,0,   0,
				0,   0,   1.5f,0,
				0,   0,   0,   1);
		m3.preMult(s3);
		mt3.setMatrix(m3);
		mt3.addChild(scene);

		mRoot.addChild(mt1); // x: -3.0f
		mRoot.addChild(mt2); // scale * 2.0f
		mRoot.addChild(mt3); // x: +3.0f

		mOverlaySurfaceView.setSceneData(mRoot);

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
