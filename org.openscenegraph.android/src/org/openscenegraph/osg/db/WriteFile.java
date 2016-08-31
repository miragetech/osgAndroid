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
package org.openscenegraph.osg.db;

import org.openscenegraph.osg.core.Image;
import org.openscenegraph.osg.core.Node;

public class WriteFile {
	private static native boolean nativeWriteNodeFile(String filename, long node_cptr);
	private static native boolean nativeWriteImageFile(String filename, long image_cptr);
	
	public static boolean writeNodeFile(String filename, Node node)
	{
		boolean result = false;
		if(node.getNativePtr()!=0)
			result = nativeWriteNodeFile(filename, node.getNativePtr());
		else
			result = false;
		return result;
	}

	public static boolean writeImageFile(String filename, Image image)
	{
		boolean result = false;
		if(image.getNativePtr()!=0)
			result = nativeWriteNodeFile(filename, image.getNativePtr());
		else
			result = false;
		return result;
	}
}
