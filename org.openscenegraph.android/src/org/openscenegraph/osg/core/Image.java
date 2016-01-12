/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait�n, Mirage Technologies S.L.     |
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

import android.graphics.Bitmap;
import android.graphics.Color;


public class Image extends Node {

	private static native long nativeCreateImage();

	private static native void nativeDispose(long cptr);
	
	private static native void nativeSetImage(long cptr, 
			int s,int t,int r,
            int internalTextureformat,
            int pixelFormat,int type,
            byte [] data);

	private static native void nativeDirty(long cptr);
	private static native int nativeS(long cptr);
	private static native int nativeT(long cptr);
	private static native byte nativeGetRed(long cptr, int s, int t, int r);
	private static native byte nativeGetGreen(long cptr, int s, int t, int r);
	private static native byte nativeGetBlue(long cptr, int s, int t, int r);
	private static native byte nativeGetAlpha(long cptr, int s, int t, int r);

	public Image(long cptr) {
		super(cptr);
	}

	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public Image() {
		_cptr = nativeCreateImage();
	}

    /** Set the image dimensions, format and data. */
    public void setImage(int s,int t,int r,
                  int internalTextureformat,
                  int pixelFormat,int type,
                  byte [] data) {
		nativeSetImage(getNativePtr(), s, t, r,
                internalTextureformat,
                pixelFormat, type,
                data);
	}
	
	public void dirty() {
		nativeDirty(getNativePtr());
	}
	
	public int s()
	{
		return nativeS(_cptr);
	}
	
	public int t()
	{
		return nativeT(_cptr);
	}

	public byte getRed(int s, int t, int r)
	{
		return nativeGetRed(_cptr, s, t, r);
	}
	
	public byte getGreen(int s, int t, int r)
	{
		return nativeGetGreen(_cptr, s, t, r);
	}
	
	public byte getBlue(int s, int t, int r)
	{
		return nativeGetBlue(_cptr, s, t, r);
	}
	
	public Bitmap toBitmap()
	{
		Bitmap result = Bitmap.createBitmap(s(), t(), Bitmap.Config.ARGB_8888);
		for(int x = 0; x < s(); x++)
		{
			for(int y = 0; y < t(); y++)
			{
				result.setPixel(x, y, Color.argb(255, Integer.valueOf(getRed(x, y, 0)), Integer.valueOf(getGreen(x, y, 0)), Integer.valueOf(getBlue(x, y, 0))));
			}
		}
		return result;
	}
}
