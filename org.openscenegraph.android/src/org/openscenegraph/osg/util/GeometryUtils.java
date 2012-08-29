/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait‡n, Mirage Technologies S.L.     |
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

import org.openscenegraph.osg.Library;
import org.openscenegraph.osg.core.Camera;

public class GeometryUtils {
	static {
		Library.initLibrary();
	}
	private static native long nativeCreateScreenQuad(int x, int y, int w, int h, boolean isBackGround);
	
	public static Camera createScreenQuad(int x, int y, int w, int h, boolean isBackGround) {
		long cptr = nativeCreateScreenQuad(x,y,w,h,isBackGround);
		if(cptr == 0)
			return null;
		return new Camera(cptr);
	}

}
