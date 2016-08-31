/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait�n, Mirage Technologies S.L.     |
 | Contribution by Christian Kehl, Uni Research AS CIPR                        |
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

public class Vec3Array extends Array {
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateVec3Array();
	private static native void nativePushBackVec3(long cptr, long vec_cptr);
	private static native void nativePushBackFloatArray(long cptr, float[] array_cptr);
	private static native long nativePopBack(long cptr);
	private static native long nativeGet(long cptr, int i);
	private static native boolean nativeSet(long cptr, int i, long vec_cptr);
	
	public Vec3Array(long cptr) {
		super(cptr);	
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}
	
	public Vec3Array() {
		_cptr = nativeCreateVec3Array();
	}
	
	protected void push_back(float[] array)
	{
		nativePushBackFloatArray(_cptr, array);
	}
	
	protected void push_back(Vec3 vector)
	{
		nativePushBackVec3(_cptr, vector.getNativePtr());
	}

	public void add(float[] value)
	{
		push_back(value);
	}
	
	public void add(Vec3 value)
	{
		push_back(value);
	}
	
	protected Vec3 pop_back()
	{
		return new Vec3(nativePopBack(_cptr));
	}
	
	public Vec3 get(int i)
	{
		return new Vec3(nativeGet(_cptr, i));
	}
	
	public boolean set(int i, Vec3 v)
	{
		return nativeSet(_cptr, i, v.getNativePtr());
	}
	
	/*
	 * the "toArray" function replaces the vector array with the
	 * float array, thus the original vector array is empty after
	 * this operation.
	 */
	public float[][] toArray()
	{
		int N = size();
		int n = N-1;
		float buffer[][] = new float[N][3];
		for(int i = 0; i < N; i++)
		{
			Vec3 vec = pop_back();
			buffer[n-i][0] = vec.x();
			buffer[n-i][1] = vec.y();
			buffer[n-i][2] = vec.z();
		}
		return buffer;
	}
}
