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

import android.util.Log;


public class Geometry extends Drawable {
	public static final int BIND_OFF = 0;
	public static final int BIND_OVERALL = 1;
	public static final int BIND_PER_PRIMITIVE_SET = 2;
	public static final int BIND_PER_VERTEX = 4;
	
	private static native long nativeCreateGeometry();
	private static native void nativeDispose(long cptr);
	private static native void nativeSetVertexArray(long cptr, float[][] inputBuffer);
	private static native void nativeSetVertexArrayNative(long cptr, long array_cptr);
	private static native long nativeGetVertexArray(long cptr);
	private static native void nativeSetColorArray(long cptr, float[][] inputBuffer);
	private static native void nativeSetColorArrayNative(long cptr, long array_cptr);
	private static native long nativeGetColorArray(long cptr);
	private static native void nativeSetNormalArray(long cptr, float[][] inputBuffer);
	private static native void nativeSetNormalArrayNative(long cptr, long array_cptr);
	private static native long nativeGetNormalArray(long cptr);
	private static native void nativeSetTexCoordArray(long cptr, float[][] inputBuffer, int tex_unit);
	private static native void nativeSetTexCoordArrayNative(long cptr, long array_cptr, int tex_unit);
	private static native long nativeGetTexCoordArray(long cptr, int tex_unit);
	private static native void nativeSetColorBinding(long cptr, int binding);
	private static native void nativeSetNormalBinding(long cptr, int binding);
	private static native boolean nativeAddPrimitiveSet(long cptr, long set_cptr);
	private static native void nativeSetPrimitiveSetList(long cptr, long list_cptr);
	private static native boolean nativeTextureFromPosePoint(long cptr, int i, long Cg_ptr, long trmat_ptr, long R_ptr, long img_ptr);
	private static native int nativeTextureFromPose(long cptr, long Cg_ptr, long trmat_ptr, long R_ptr, long img_ptr);
	
	public Geometry(long cptr) {
		super(cptr);	
	}
	
	@Override
	public void dispose() {
		if(_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
		//Log.w("org.openscenegraph.osg.core.Geometry", "Object destroyed");
	}
	
	public Geometry() {
		_cptr = nativeCreateGeometry();
	}
	
	public void setVertexArray(float array[][])
	{
		nativeSetVertexArray(_cptr, array);
	}

	public void setColorArray(float array[][])
	{
		nativeSetColorArray(_cptr, array);
	}
	
	public void setNormalArray(float array[][])
	{
		nativeSetNormalArray(_cptr, array);
	}

	public void setTexCoordArray(int unit, float array[][])
	{
		nativeSetTexCoordArray(_cptr, array, unit);
	}
	
	public void setVertexArray(Vec3Array vertices)
	{
		nativeSetVertexArrayNative(_cptr, vertices.getNativePtr());
	}
	
	public Vec3Array getVertexArray()
	{
		return new Vec3Array(nativeGetVertexArray(_cptr));
	}

	public void setColorArray(Vec4Array colors)
	{
		nativeSetColorArrayNative(_cptr, colors.getNativePtr());
	}
	
	public Vec4Array getColorArray()
	{
		return new Vec4Array(nativeGetColorArray(_cptr));
	}
	
	public void setNormalArray(Vec3Array normal)
	{
		nativeSetNormalArrayNative(_cptr, normal.getNativePtr());
	}
	
	public Vec3Array getNormalArray()
	{
		return new Vec3Array(nativeGetNormalArray(_cptr));
	}

	public void setTexCoordArray(int unit, Vec2Array tex_coords)
	{
		nativeSetTexCoordArrayNative(_cptr, tex_coords.getNativePtr(), unit);
	}
	
	public Vec2Array getTexCoordArray(int unit)
	{
		return new Vec2Array(nativeGetTexCoordArray(_cptr, unit));
	}
	
	public void setNormalBinding(int _binding)
	{
		nativeSetNormalBinding(_cptr, _binding);
	}
	
	public void setColorBinding(int binding)
	{
		nativeSetColorBinding(_cptr, binding);
	}
	
	public void setPrimitiveSetList(PrimitiveSetList list)
	{
		nativeSetPrimitiveSetList(_cptr, list.getNativePtr());
	}
	
	public boolean addPrimitiveSet(PrimitiveSet primitiveset) {
		return nativeAddPrimitiveSet(_cptr, primitiveset.getNativePtr());
	}
	
	public int TextureFromPose(Vec3 Cg, Matrix trmat, Vec3 RvecCam, Image img)
	{
		if(img==null)
		{
			Log.e("org.openscenegraph.osg.core.Geometry", "Texture from Pose: image input is null - ABORT.");
			return 0;
		}
		
		return nativeTextureFromPose(_cptr, Cg.getNativePtr(), trmat.getNativePtr(), RvecCam.getNativePtr(), img.getNativePtr());
	}
	
	public boolean TextureFromPosePoint(int index, Vec3 Cg, Matrix trmat, Vec3 RvecCam, Image img)
	{
		if(img==null)
		{
			Log.e("org.openscenegraph.osg.core.Geometry", "Texture from Pose: image input is null - ABORT.");
			return false;
		}
		
		return nativeTextureFromPosePoint(_cptr, index, Cg.getNativePtr(), trmat.getNativePtr(), RvecCam.getNativePtr(), img.getNativePtr());
	}	
}
