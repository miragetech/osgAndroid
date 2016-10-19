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
package org.openscenegraph.osg.util;

import org.openscenegraph.osg.core.Camera;
import org.openscenegraph.osg.core.Image;
import org.openscenegraph.osg.core.Matrix;
import org.openscenegraph.osg.core.Vec3;

import android.util.Log;

public class GeometryUtils {
	private static native long nativeCreateScreenQuad(int x, int y, int w, int h, boolean isBackGround);
	private static native int nativeTextureFromPose(String in_filepath, String out_filepath, long Cg_ptr, long trmat_ptr, long R_ptr, long img_ptr);
	private static native int nativeTextureFromPoseImgfile(String in_filepath, String out_filepath, long Cg_ptr, long trmat_ptr, long R_ptr, String img_filename);
	
	public static Camera createScreenQuad(int x, int y, int w, int h, boolean isBackGround) {
		long cptr = nativeCreateScreenQuad(x,y,w,h,isBackGround);
		if(cptr == 0)
			return null;
		return new Camera(cptr);
	}

	public static int TextureFromPose(String in_geometryfile, String out_geometryfile, Vec3 Cg, Matrix trmat, Vec3 RvecCam, Image img)
	{
		if(img==null)
		{
			Log.e("org.openscenegraph.osg.core.Geometry", "Texture from Pose: image input is null - ABORT.");
			return 0;
		}
		
		return nativeTextureFromPose(in_geometryfile, out_geometryfile, Cg.getNativePtr(), trmat.getNativePtr(), RvecCam.getNativePtr(), img.getNativePtr());
	}

	public static int TextureFromPose(String in_geometryfile, String out_geometryfile, Vec3 Cg, Matrix trmat, Vec3 RvecCam, String img_filepath)
	{
		return nativeTextureFromPoseImgfile(in_geometryfile, out_geometryfile, Cg.getNativePtr(), trmat.getNativePtr(), RvecCam.getNativePtr(), img_filepath);
	}

}
