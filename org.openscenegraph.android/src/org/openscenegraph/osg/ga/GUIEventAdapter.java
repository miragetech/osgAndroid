package org.openscenegraph.osg.ga;

import org.openscenegraph.osg.Native;

public class GUIEventAdapter implements Native
{
	private native void nativeDispose(long cptr);
	private long _cptr;
	
	public GUIEventAdapter(long cptr) {
		_cptr = cptr;
	}
	
	public long getNativePtr() {
		// TODO Auto-generated method stub
		return _cptr;
	}

	public void dispose() {
		// TODO Auto-generated method stub
		nativeDispose(_cptr);
	}
	
}
