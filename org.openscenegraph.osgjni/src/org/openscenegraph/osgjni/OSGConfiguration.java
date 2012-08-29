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
package org.openscenegraph.osgjni;

import org.openscenegraph.osg.core.Node;
import org.openscenegraph.osg.viewer.Viewer;

public class OSGConfiguration {
	static {
		Library.initLibrary();
	}
	private static native void nativeConfigureViewer(long viewer);
	private static native long nativeConfigureScene(long node);
	
	public static void configureViewer(Viewer viewer) {
		nativeConfigureViewer(viewer.getNativePtr());
	}

	/**
	 * Configures the scene inside the jni and returns the modified scene.
	 * If the input node is null it assumes that you create the node inside
	 * Otherwise it returns the new scene. The returned scene can be the same
	 * as input.
	 * @param node
	 */
	public static Node configureScene(Node node) {
		if(node == null)
			return new Node(nativeConfigureScene(0));
		else {
			long cptr = nativeConfigureScene(node.getNativePtr());
			if(cptr == node.getNativePtr())
			{
				return node;
			}
			else
			{
				return new Node(cptr);
			}
		}
	}
}
