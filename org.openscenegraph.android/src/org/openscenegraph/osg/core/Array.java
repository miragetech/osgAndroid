package org.openscenegraph.osg.core;

public class Array {
	private static native void nativeDispose(long cptr);
	private static native int nativeSize(long cptr);

	protected long _cptr;
	
	public Array() {
		_cptr = 0;
	}
	
	public Array(long cptr) {
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
	
	public int size()
	{
		return nativeSize(_cptr);
	}
}
