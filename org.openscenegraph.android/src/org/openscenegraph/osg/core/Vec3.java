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

public class Vec3 implements Native {
	static {
		Library.initLibrary();
	}
	private static native long nativeCreateVec3();
	private static native void nativeDispose(long cptr);
	private static native float nativeX(long cptr);
	private static native float nativeY(long cptr);
	private static native float nativeZ(long cptr);
	private static native void nativeSet(long cptr, float x, float y, float z);
	private static native void nativeSetX(long cptr, float value);
	private static native void nativeSetY(long cptr, float value);
	private static native void nativeSetZ(long cptr, float value);
	private static native float nativeLength(long cptr);
	private static native float nativeLength2(long cptr);
	private static native long nativeDiv(long cptr, float value);
	private static native float nativeDotProduct(long cptr, long rhs);
	private static native long nativeCrossProduct(long cptr, long rhs);
	private static native long nativeEscalarProduct(long cptr, float value);
	private static native long nativeSum(long cptr, long rhs);
	private static native long nativeSub(long cptr, long rhs);
	private static native float nativeNormalize(long cptr);
	private static native long nativeNegation(long cptr);
	
	private long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	public Vec3(long cptr) {
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
	
	public Vec3() {
		_cptr = nativeCreateVec3();	
	}
	
	public Vec3(float x, float y, float z) {
		_cptr = nativeCreateVec3();
		set(x,y,z);
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
	
	public void set(float x, float y, float z) {
		nativeSet(_cptr, x, y, z);
	}
	
	public void set(Vec3 v) {
		set(v.x(),v.y(),v.z());
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
	public Vec3 div(float value) {
		return new Vec3(nativeDiv(_cptr,value));
	}

	/** Dot product. */
	public float dotProduct(Vec3 rhs) {
		return nativeDotProduct(_cptr,rhs.getNativePtr());
	}

	/** Cross product. */
	public Vec3 crossProduct(Vec3 rhs) {
		return new Vec3(nativeCrossProduct(_cptr,rhs.getNativePtr()));
	}

	/** Multiply by scalar. */
	public Vec3 escalarProduct(float rhs) {
		return new Vec3(nativeEscalarProduct(_cptr,rhs));
	}

	/** Binary vector add. */
	public Vec3 sum(Vec3 rhs) {
		return new Vec3(nativeSum(_cptr,rhs.getNativePtr()));
	}

	/** Binary vector subtract. */
	public Vec3 sub(Vec3 rhs) {
		return new Vec3(nativeSub(_cptr,rhs.getNativePtr()));

	}

	/** normalization of the vector. */
	public float normalize() {
		return nativeNormalize(_cptr);
	}

	/** Negation operator. Returns the negative of the Vec3d. */

	public Vec3 negation() {
		return new Vec3(nativeNegation(_cptr));
	}
	
	@Override
	public boolean equals(Object obj) {

		Vec3 in = (Vec3) obj;
		
		if(in.x()==x() && in.y()==y() && in.z()==z()) return true;
		return super.equals(obj);
	
	}

}
