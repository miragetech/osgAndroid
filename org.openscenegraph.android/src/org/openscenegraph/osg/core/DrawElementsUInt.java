package org.openscenegraph.osg.core;

public class DrawElementsUInt extends PrimitiveSet {
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateDrawElementsUInt(int mode, int count);
	public static native int nativeSize(long cptr);
	public static native void nativePushBackUInt(long cptr, int value);
	
	public DrawElementsUInt(long cptr)
	{
		super(cptr);
	}
	
	public DrawElementsUInt(int mode, int count)
	{
		_cptr = nativeCreateDrawElementsUInt(mode, count);
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
	
	public void push_back(int index)
	{
		nativePushBackUInt(_cptr, index);
	}
}
