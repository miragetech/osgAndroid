package org.openscenegraph.osg.core;

public class DrawArrays extends PrimitiveSet {
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateDrawArrays(int mode, int first, int count);
	
	public DrawArrays(long cptr)
	{
		super(cptr);
	}
	
	public DrawArrays(int mode, int first, int count)
	{
		_cptr = nativeCreateDrawArrays(mode, first, count);
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
