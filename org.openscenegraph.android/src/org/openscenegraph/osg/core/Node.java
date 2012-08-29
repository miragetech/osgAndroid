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

public class Node implements Native {
	static {
		Library.initLibrary();
	}
	private static native void nativeDispose(long cptr);
	private static native void nativeSetUpdateCallback(long cptr, long ucb);
	private static native void nativeSetRenderBinDetails(long cptr, int order, String bin);
	private static native void nativeSetTexture2D(long cptr, long cptrimage);
	private static native void nativeSetMode(long cptr, int mode, int value);
	
    public static class Values
    {   
        /** means that associated GLMode and Override is disabled.*/
        public static final int OFF          = 0x0;
        /** means that associated GLMode is enabled and Override is disabled.*/
        public static final int ON           = 0x1;
        /** Overriding of GLMode's or StateAttributes is enabled, so that state below it is overridden.*/
        public static final int OVERRIDE     = 0x2;
        /** Protecting of GLMode's or StateAttributes is enabled, so that state from above cannot override this and below state.*/
        public static final int PROTECTED    = 0x4;   
        /** means that GLMode or StateAttribute should be inherited from above.*/
        public static final int INHERIT      = 0x8;
    };
	
	protected long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	public Node(long cptr) {
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
	
	public Node() {
		_cptr = 0;
	}

	public void setUpdateCallback(UpdateCallback am) {
		nativeSetUpdateCallback(_cptr, am.getNativePtr());
		
	}
	
	public void setRenderBinDetails(int order, String bin) {
		nativeSetRenderBinDetails(_cptr,order,bin);
	}
	
	public void setTexture2D(Image image) {
		nativeSetTexture2D(_cptr,image.getNativePtr());
	}
	/** 
	 * Sets the stateset mode to the value
	 * @param mode GLenum mode values (GL_LIGHTING, GL_TEXTURE, GL_DEPTH_TEST, ...etc)
	 * @param value: @see {@link Values}
	 */
	public void setMode(int mode, int value) {
		nativeSetMode(_cptr,mode,value);
	}

}
