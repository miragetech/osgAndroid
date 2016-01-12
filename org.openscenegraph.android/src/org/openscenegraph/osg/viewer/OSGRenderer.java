/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait�n, Mirage Technologies S.L.     |
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
package org.openscenegraph.osg.viewer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class OSGRenderer implements GLSurfaceView.Renderer {
	protected ViewerBase _viewer;

	public OSGRenderer(ViewerBase viewer) {
		_viewer = viewer;
	}
	
	public void onDrawFrame(GL10 gl) {
		if(_viewer.getNativePtr()!=0)
			_viewer.frame();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		if(_viewer.getNativePtr()!=0)
		{
			_viewer.setUpViewerAsEmbedded(0, 0, width, height);
			_viewer.setViewport(0, 0, width, height);
		}
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	}
}
