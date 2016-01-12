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
