package org.openscenegraph.osg.core;
import android.opengl.GLES11;

public class PrimitiveSet {
	public static final int PrimitiveType = 0;
	public static final int DrawArraysPrimitiveType = 1;
	public static final int DrawArrayLengthsPrimitiveType = 2;
	public static final int DrawElementsUBytePrimitiveType = 3;
	public static final int DrawElementsUShortPrimitiveType = 4;
	public static final int DrawElementsUIntPrimitiveType = 5;
	
	public static final int POINTS = GLES11.GL_POINTS;
	public static final int LINES = GLES11.GL_LINES;
	public static final int LINE_STRIP = GLES11.GL_LINE_STRIP;
	public static final int LINE_LOOP = GLES11.GL_LINE_LOOP;
	public static final int TRIANGLES = GLES11.GL_TRIANGLES;
	public static final int TRIANGLE_STRIP = GLES11.GL_TRIANGLE_STRIP;
	public static final int TRIANGLE_FAN = GLES11.GL_TRIANGLE_FAN;
	
	private static native long nativeCreatePrimitiveSet(int type, int mode);
	private static native void nativeDispose(long cptr);
	
	protected long _cptr;
	
	public long getNativePtr() {
		return _cptr;
	}
	
	public PrimitiveSet()
	{
		_cptr = 0;
	}
	
	public PrimitiveSet(long cptr) {
		_cptr = cptr;
	}
	
	public void dispose() {
		if(_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}
	
}
