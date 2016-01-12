package org.openscenegraph.osg.core;

public class Drawable {
	private static native void nativeDispose(long cptr);

	protected long _cptr;
	
	public Drawable() {
		_cptr = 0;
	}
	
	public Drawable(long cptr) {
		_cptr = cptr;	
	}

	public long getNativePtr() {
		return _cptr;
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
}
