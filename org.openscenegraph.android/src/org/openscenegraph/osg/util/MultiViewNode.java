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
package org.openscenegraph.osg.util;

import org.openscenegraph.osg.Library;
import org.openscenegraph.osg.core.Node;

public class MultiViewNode extends Node {
	static {
		Library.initLibrary();
	}

	private static native long nativeCreateMultiViewNode(int numCameras,
			int cameraCols, int cameraRows, int screenWidth, int screenHeight);

	private static native void nativeDispose(long cptr);

	private static native void nativeSetSceneData(long cptr, long node);

	private static native void nativeSetFusionDistance(long cptr,
			double fusionDistance);

	private static native void nativeSetScreenDistance(long cptr,
			double screenDistance);

	private static native void nativeSetScreenSize(long cptr, int width,
			int height);

	public MultiViewNode(long cptr) {
		super(cptr);
	}

	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public MultiViewNode(int numCameras, int cameraCols, int cameraRows,
			int screenWidth, int screenHeight) {
		_cptr = nativeCreateMultiViewNode(numCameras, cameraCols, cameraRows,
				screenWidth, screenHeight);
	}

	public void setSceneData(Node node) {
		nativeSetSceneData(_cptr, node.getNativePtr());
	}

	public void setFusionDistance(double fusionDistance) {
		nativeSetFusionDistance(_cptr, fusionDistance);
	}

	public void setScreenDistance(double screenDistance) {
		nativeSetScreenDistance(_cptr, screenDistance);
	}

	public void setScreenSize(int width, int height) {
		nativeSetScreenSize(_cptr, width, height);
	}
}
