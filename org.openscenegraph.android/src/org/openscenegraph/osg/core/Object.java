package org.openscenegraph.osg.core;

public class Object {
	private static native void nativeDispose(long cptr);
	private static native void nativeSetDataVariance(long cptr, int dv);
	private static native int nativeGetDataVariance(long cptr);

    public static class DataVariance
    {   
        public static final int DYNAMIC      = 0x0;
        public static final int STATIC       = 0x1;
        public static final int UNSPECIFIED  = 0x2;
    };

	protected long _cptr;
	
	public Object() {
		_cptr = 0;
	}
	
	public Object(long cptr) {
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
	
	public void setDataVariance(int dv)
	{
		nativeSetDataVariance(_cptr, dv);
	}
	
	public int getDataVariance()
	{
		return nativeGetDataVariance(_cptr);
	}
}
