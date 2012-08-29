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
package org.openscenegraph.osg.core;

import org.openscenegraph.osg.Library;
import org.openscenegraph.osg.Native;

public class Camera extends Group implements Native  {
	static {
		Library.initLibrary();
	}

	private static native void nativeDispose(long cptr);

	private static native void nativeSetClearColor(long cptr, float r, float g,
			float b, float a);

	private static native void nativeSetViewMatrixAsLookAt(long cptr,
			long eyecptr, long centercptr, long upcptr);

	private static native void nativeSetProjectionMatrixAsOrtho2D(long cptr,
			double left, double right, double bottom, double top);

	private static native void nativeSetProjectionMatrixAsPerspective(
			long cptr, double fovy, double aspectRatio, double zNear,
			double zFar);

	private static native void nativeSetViewMatrix(long cptr, long cptrmatrix);

	private static native void nativeSetProjectionMatrix(long cptr,
			long cptrmatrix);
	
	private static native void nativeSetCullLeftMask(long cptr, boolean value);
	private static native void nativeSetCullRightMask(long cptr, boolean value);

	private long _cptr;

	public long getNativePtr() {
		return _cptr;
	}

	public Camera(long cptr) {
		_cptr = cptr;
	}

	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public void dispose() {
		if (_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}

	public void setClearColor(float r, float g, float b, float a) {
		nativeSetClearColor(_cptr, r, g, b, a);
	}

	public void setViewMatrixAsLookAt(Vec3 eye, Vec3 center, Vec3 up) {
		nativeSetViewMatrixAsLookAt(_cptr, eye.getNativePtr(),
				center.getNativePtr(), up.getNativePtr());
	}

	public void setViewMatrix(Matrix matrix) {
		nativeSetViewMatrix(_cptr, matrix.getNativePtr());
	}

	/**
	 * Set to a 2D orthographic projection. See OpenGL glOrtho2D documentation
	 * for further details.
	 */
	public void setProjectionMatrixAsOrtho2D(double left, double right,
			double bottom, double top) {
		nativeSetProjectionMatrixAsOrtho2D(_cptr, left, right, bottom, top);
	}

	/**
	 * Create a symmetrical perspective projection, See OpenGL gluPerspective
	 * documentation for further details. Aspect ratio is defined as
	 * width/height.
	 */
	public void setProjectionMatrixAsPerspective(double fovy,
			double aspectRatio, double zNear, double zFar) {
		nativeSetProjectionMatrixAsPerspective(_cptr, fovy, aspectRatio, zNear,
				zFar);
	}

	public void setProjectionMatrix(Matrix matrix) {
		nativeSetProjectionMatrix(_cptr, matrix.getNativePtr());
	}
	
	/**
	 * Enable/Disable the cull mask to be rendered only by left eye
	 * @param value
	 */
	public void setCullLeftMask(boolean value) {
		nativeSetCullLeftMask(_cptr, value);
	}
	/**
	 * Enable/Disable the cull mask to be rendered only by right eye
	 * @param value
	 */
	public void setCullRightMask(boolean value) {
		nativeSetCullRightMask(_cptr, value);
	}
}
