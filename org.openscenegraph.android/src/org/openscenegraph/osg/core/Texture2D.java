package org.openscenegraph.osg.core;

public class Texture2D extends Texture {
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateTexture2D();
	private static native void nativeSetImage(long cptr, long image_cptr);
	private static native long nativeGetImage(long cptr);
	
	public Texture2D(long cptr)
	{
		super(cptr);
	}
	
	public Texture2D()
	{
		super( nativeCreateTexture2D() );
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
	
	@Override
	public void setImage(int face, Image image)
	{
		nativeSetImage(_cptr, image.getNativePtr());
	}

	public void setImage(Image image)
	{
		nativeSetImage(_cptr, image.getNativePtr());
	}
	
	@Override
	public Image getImage(int face)
	{
		long image_ptr = nativeGetImage(_cptr);
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

	public Image getImage()
	{
		long image_ptr = nativeGetImage(_cptr);
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
