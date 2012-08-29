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

public class AnimationPath implements Native {
	static {
		Library.initLibrary();
	}
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateAnimationPath();
	private static native void nativeInsert(long cptr,double time, 
			long position,long rotation, long scale);
	private static native double nativeGetFirstTime(long cptr);
	private static native double nativeGetLastTime(long cptr);
	private static native void nativeSetLoopMode(long cptr, int lm);
	private static native int nativeGetLoopMode(long cptr);
	
	private long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	public AnimationPath(long cptr) {
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
	
	public AnimationPath() {
		_cptr = nativeCreateAnimationPath();
	}
	
	public class ControlPoint {
		public ControlPoint(Vec3 position, Quat rotation, Vec3 scale) {
			this.position = position;
			this.rotation = rotation;
			this.scale = scale;
		}
		public Vec3 position;
		public Quat rotation;
		public Vec3 scale;
	}
	
    /** Insert a control point into the AnimationPath.*/
    public void insert(double time, ControlPoint controlPoint) {
    	nativeInsert(_cptr,time,
    			controlPoint.position.getNativePtr(),
    			controlPoint.rotation.getNativePtr(),
    			controlPoint.scale.getNativePtr());
    }
    
    public double getFirstTime() {
    	return nativeGetFirstTime(_cptr);
    }
    public double getLastTime() {
    	return nativeGetLastTime(_cptr);
    }
    public double getPeriod() { return getLastTime()-getFirstTime(); }
    
    public class LoopMode {
    	public static final int SWING = 0;
    	public static final int LOOP = 1;
    	public static final int NO_LOOPING = 2;
    };
    
    /**
     * Sets the mode of animation @see {@link LoopMode}
     * @param LoopMode lm
     */
    public void setLoopMode(int lm) { 
    	nativeSetLoopMode(_cptr,lm);
    }
    /**
     * Gets the mode of animation @see {@link LoopMode
     * @return {@link LoopMode}
     */
    public int getLoopMode() {
    	return nativeGetLoopMode(_cptr);
    }
}
