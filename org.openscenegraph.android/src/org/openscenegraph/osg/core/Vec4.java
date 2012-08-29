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

public class Vec4 implements Native {
	static {
		Library.initLibrary();
	}
	private static native long nativeCreateVec4();
	private static native void nativeDispose(long cptr);
	private static native float nativeX(long cptr);
	private static native float nativeY(long cptr);
	private static native float nativeZ(long cptr);
	private static native float nativeW(long cptr);
	private static native void nativeSet(long cptr, float x, float y, float z, float w);
	private static native void nativeSetX(long cptr, float value);
	private static native void nativeSetY(long cptr, float value);
	private static native void nativeSetZ(long cptr, float value);
	private static native void nativeSetW(long cptr, float value);
	private static native float nativeLength(long cptr);
	private static native float nativeLength2(long cptr);
	private static native long nativeDiv(long cptr, float value);
	private static native long nativeEscalarProduct(long cptr, float value);
	private static native long nativeSum(long cptr, long rhs);
	private static native long nativeSub(long cptr, long rhs);
	private static native float nativeNormalize(long cptr);
	private static native long nativeNegation(long cptr);
	
	private long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	public Vec4(long cptr) {
		_cptr = cptr;	
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public void dispose() {
		if(_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}
	
	public Vec4() {
		_cptr = nativeCreateVec4();	
	}
	
	public Vec4(float r, float g, float b, float a) {
		_cptr = nativeCreateVec4();
		set(r,g,b,a);
	}
	
	public float x() {
		return nativeX(_cptr);
	}
	
	public float y() {
		return nativeY(_cptr);
	}
	
	public float z() {
		return nativeZ(_cptr);
	}
	
	public float w() { 
		return nativeW(_cptr);
	}
	
	public void set(float x, float y, float z, float w) {
		nativeSet(_cptr, x, y, z, w);
	}
	
	public void set(Vec4 v) {
		set(v.x(),v.y(),v.z(),v.w());
	}
	
	public void setX(float value) {
		nativeSetX(_cptr,value);
	}

	public void setY(float value) {
		nativeSetY(_cptr,value);
	}

	public void setZ(float value) {
		nativeSetZ(_cptr,value);
	}
	
	public void setW(float value) {
		nativeSetW(_cptr,value);
	}

	public String toString() {
		return "(" + x() + "," + y() + "," + z() + ")";
	}

	public float length() {
		return nativeLength(_cptr);
	}

	public float length2() {
		return nativeLength2(_cptr);
	}

	/** Divide by scalar. */
	public Vec4 div(float value) {
		return new Vec4(nativeDiv(_cptr,value));
	}

	/** Multiply by scalar. */
	public Vec4 escalarProduct(float rhs) {
		return new Vec4(nativeEscalarProduct(_cptr,rhs));
	}

	/** Binary vector add. */
	public Vec4 sum(Vec4 rhs) {
		return new Vec4(nativeSum(_cptr,rhs.getNativePtr()));
	}

	/** Binary vector subtract. */
	public Vec4 sub(Vec4 rhs) {
		return new Vec4(nativeSub(_cptr,rhs.getNativePtr()));

	}

	/** normalization of the vector. */
	public float normalize() {
		return nativeNormalize(_cptr);
	}

	/** Negation operator. Returns the negative of the Vec3d. */

	public Vec4 negation() {
		return new Vec4(nativeNegation(_cptr));
	}
	
	@Override
	public boolean equals(Object obj) {

		Vec4 in = (Vec4) obj;
		
		if(in.x()==x() && in.y()==y() && in.z()==z()) return true;
		return super.equals(obj);
	
	}

}
