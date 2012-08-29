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

public class AnimationPathCallback implements UpdateCallback{
	static {
		Library.initLibrary();
	}
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateAnimationPathCallback();
	private static native void nativeSetAnimationPath(long cptr, long path); 
	private static native long nativeGetAnimationPath(long cptr); 
	private static native void	nativeSetPivotPoint(long cptr, long vec3); 
	private static native long nativeGetPivotPoint(long cptr); 
	private static native void nativeSetUseInverseMatrix(long cptr, boolean useInverseMatrix);
	private static native boolean nativeGetUseInverseMatrix(long cptr);
	private static native void nativeSetTimeOffset(long cptr, double offset); 
	private static native double nativeGetTimeOffset(long cptr); 
	private static native void nativeSetTimeMultiplier(long cptr, double multiplier); 
	private static native double nativeGetTimeMultiplier(long cptr); 
	private static native void nativeReset(long cptr);
	private static native void nativeSetPause(long cptr, boolean pause);
	private static native boolean nativeGetPause(long cptr); 
	private static native double nativeGetAnimationTime(long cptr);
	
	private long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	public AnimationPathCallback(long cptr) {
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
	
	public AnimationPathCallback() {
		_cptr = nativeCreateAnimationPathCallback();
	}
	
	public void setAnimationPath(AnimationPath path) { 
    	nativeSetAnimationPath(_cptr, path.getNativePtr()); 
    }
    
	public AnimationPath getAnimationPath() { 
    	return new AnimationPath(nativeGetAnimationPath(_cptr)); 
    }

	public void setPivotPoint(Vec3 pivot) { 
    	nativeSetPivotPoint(_cptr, pivot.getNativePtr()); 
    }
    
	public Vec3 getPivotPoint() { 
    	return new Vec3(nativeGetPivotPoint(_cptr)); 
    }

	public void setUseInverseMatrix(boolean useInverseMatrix) { 
    	nativeSetUseInverseMatrix(_cptr, useInverseMatrix);
    }
	
	public boolean getUseInverseMatrix() { 
    	return nativeGetUseInverseMatrix(_cptr);
    }

	public void setTimeOffset(double offset) { 
    	nativeSetTimeOffset(_cptr,offset); 
    }
    
	public double getTimeOffset() { 
    	return nativeGetTimeOffset(_cptr); 
    }

	public void setTimeMultiplier(double multiplier) { 
    	nativeSetTimeMultiplier(_cptr,multiplier); 
    }
    
	public double getTimeMultiplier() { 
    	return nativeGetTimeMultiplier(_cptr); 
    }

	public void reset() {
    	nativeReset(_cptr);
    }

	public void setPause(boolean pause) {
    	nativeSetPause(_cptr,pause);
    }
    
	public boolean getPause() { 
    	return nativeGetPause(_cptr); 
    }

    /** Get the animation time that is used to specify the position along
      * the AnimationPath. Animation time is computed from the formula:
      *   ((_latestTime-_firstTime)-_timeOffset)*_timeMultiplier.*/
	public double getAnimationTime() {
    	return nativeGetAnimationTime(_cptr);
    }
}
