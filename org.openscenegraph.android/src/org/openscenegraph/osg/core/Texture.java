package org.openscenegraph.osg.core;

public class Texture extends Object {
	private static native void nativeDispose(long cptr);
	private static native void nativeSetImage(long cptr, int face, long image_cptr);
	private static native long nativeGetImage(long cptr, int face);
	
	public Texture(long cptr)
	{
		super(cptr);
	}
	
	@Override
	public void dispose()
	{
		if(_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}
	
	public void setImage(int face, Image image)
	{
		nativeSetImage(_cptr, face, image.getNativePtr());
	}
	
	public Image getImage(int face)
	{
		long image_ptr = nativeGetImage(_cptr, face);
		Image result;
		if(image_ptr != 0)
		{
			result = new Image(image_ptr);
		}
		else
		{
			result = new Image();
		}
		return result;
	}
}
