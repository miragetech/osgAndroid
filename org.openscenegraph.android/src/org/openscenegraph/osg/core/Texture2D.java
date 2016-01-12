/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait�n, Mirage Technologies S.L.     |
 | Contribution by Christian Kehl, Uni Research AS CIPR                        |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
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
