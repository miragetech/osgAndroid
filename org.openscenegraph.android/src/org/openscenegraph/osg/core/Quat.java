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

public class Quat implements Native {
	static {
		Library.initLibrary();
	}
	private static native long nativeCreateQuat();
	private static native void nativeDispose(long cptr);
	private static native void nativeSet(long cptr, float q1, float q2, float q3, float q4);
	private static native float nativeX(long cptr);
	private static native float nativeY(long cptr);
	private static native float nativeZ(long cptr);
	private static native float nativeW(long cptr);
	private static native void nativeMakeRotateAngleVec3(long cptr, float angle, float x, float y, float z);
	private static native void nativeMakeRotateVec3Vec3(long cptr, long vec1, long vec2);
	
	private long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	public Quat(long cptr) {
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
	
	public Quat() {
		_cptr = nativeCreateQuat();	
	}
	
	public Quat(float x, float y, float z, float w) {
		_cptr = nativeCreateQuat();	
		set(x,y,z,w);
	}
	
	public void set(float x, float y, float z, float w) {
		nativeSet(_cptr,x,y,z,w);
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

    public void makeRotate( float  angle, float  x, float  y, float  z ) {
    	nativeMakeRotateAngleVec3(_cptr,angle, x,y,z);
    }
    
    public void makeRotate( Vec3 vec1, Vec3 vec2 ) {
    	nativeMakeRotateVec3Vec3(_cptr,vec1.getNativePtr(),vec2.getNativePtr());
    }

}
