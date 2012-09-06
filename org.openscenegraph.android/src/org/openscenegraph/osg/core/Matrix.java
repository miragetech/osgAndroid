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

public class Matrix implements Native {
	static {
		Library.initLibrary();
	}

	private static native long nativeCreateMatrix();

	private static native void nativeDispose(long cptr);

	private static native void nativeSet(long cptr, float a00, float a01,
			float a02, float a03, float a10, float a11, float a12, float a13,
			float a20, float a21, float a22, float a23, float a30, float a31,
			float a32, float a33);

	private static native boolean nativeIsIdentity(long cptr);

	private static native void nativeMakeIdentity(long cptr);

	private static native void nativeMakeScale(long cptr, float v1, float v2,
			float v3);

	private static native void nativeMakeTranslate(long cptr, float v1,
			float v2, float v3);

	private static native void nativeMakeRotate(long cptr, float angle,
			float x, float y, float z);

	private static native void nativeMakeRotateQuat(long cptr, long quat);

	private static native boolean nativeInvert(long cptr);

	private static native void nativePreMult(long cptr, long matrix);

	private static native void nativePostMult(long cptr, long matrix);
	
	private static native void nativeMakeLookAt(long cptr, long eye, long center, long up);

	private long _cptr;

	public long getNativePtr() {
		return _cptr;
	}

	public Matrix(long cptr) {
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

	public Matrix() {
		_cptr = nativeCreateMatrix();
	}

	public Matrix(float a00, float a01, float a02, float a03, float a10,
			float a11, float a12, float a13, float a20, float a21, float a22,
			float a23, float a30, float a31, float a32, float a33) {
		_cptr = nativeCreateMatrix();
		set(	a00, a01, a02, a03, 
				a10, a11, a12, a13, 
				a20, a21, a22, a23, 
				a30, a31, a32, a33);
	}

	public void set(float a00, float a01, float a02, float a03, float a10,
			float a11, float a12, float a13, float a20, float a21, float a22,
			float a23, float a30, float a31, float a32, float a33) {
		nativeSet(_cptr, 
				a00, a01, a02, a03, 
				a10, a11, a12, a13, 
				a20, a21, a22, a23, 
				a30, a31, a32, a33);
	}

	public boolean isIdentity() {
		return nativeIsIdentity(_cptr);
	}

	public void makeIdentity() {
		nativeMakeIdentity(_cptr);
	}

	public void makeScale(float v1, float v2, float v3) {
		nativeMakeScale(_cptr, v1, v2, v3);
	}

	public void makeTranslate(float v1, float v2, float v3) {
		nativeMakeTranslate(_cptr, v1, v2, v3);
	}

	public void makeRotate(long cptr, float angle, float x, float y, float z) {
		nativeMakeRotate(_cptr, angle, x, y, z);
	}

	public void makeRotate(Quat quat) {
		nativeMakeRotateQuat(_cptr, quat.getNativePtr());
	}

	public boolean invert() {
		return nativeInvert(_cptr);
	}

	public void preMult(Matrix matrix) {
		nativePreMult(_cptr, matrix.getNativePtr());
	}

	public void postMult(Matrix matrix) {
		nativePostMult(_cptr, matrix.getNativePtr());
	}
	
	public void makeLookAt(Vec3 eye, Vec3 center, Vec3 up) {
		nativeMakeLookAt(_cptr, eye.getNativePtr(), center.getNativePtr(), up.getNativePtr());
	}
}
