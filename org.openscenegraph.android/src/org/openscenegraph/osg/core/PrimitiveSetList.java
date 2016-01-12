package org.openscenegraph.osg.core;

public class PrimitiveSetList {
	public static native void nativeDispose(long cptr);
	public static native long nativeCreatePrimitiveSetList();
	public static native int nativeSize(long cptr);
	public static native void nativePushBackPrimitiveSet(long cptr, long object_cptr);
	
	protected long _cptr;
	
	public PrimitiveSetList() {
		_cptr = nativeCreatePrimitiveSetList();
	}
	
	public PrimitiveSetList(long cptr) {
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
	
	public void push_back(PrimitiveSet set)
	{
		nativePushBackPrimitiveSet(_cptr, set.getNativePtr());
	}
}
