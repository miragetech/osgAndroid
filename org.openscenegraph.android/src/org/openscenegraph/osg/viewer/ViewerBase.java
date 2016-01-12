package org.openscenegraph.osg.viewer;

import org.openscenegraph.osg.core.Object;

public class ViewerBase extends Object {

	private native void nativeSetUpViewerAsEmbedded(long cptr, int x, int y, int width, int height);
	private native void nativeSetViewport(long cptr, int x, int y, int width, int height);
	private native void nativeFrame(long cptr);
	
	//protected long _cptr;
	//public long getNativePtr() {
	//	return _cptr;
	//}
	
	public ViewerBase()
	{
		super();
	}
	
	public ViewerBase(long cptr)
	{
		super(cptr);
	}
	
	/**
	 * Set The viewport of the scene.
	 * 
	 * @param width
	 *            viewport width.
	 * @param height
	 *            viewport height.
	 */
	public synchronized void setViewport(int x, int y, int width, int height) {
		nativeSetViewport(_cptr, x, y, width, height);
	}
	
	/**
	 * Convenience method for setting up the viewer so it can be used embedded
	 * in an external managed window.
	 */
	public void setUpViewerAsEmbedded(int x, int y, int width, int height) {
		nativeSetUpViewerAsEmbedded(_cptr, x, y, width, height);
	}

	/**
	 * Execute a frame
	 * 
	 */
	public void frame() {
		if (_cptr == 0)
			return;
		nativeFrame(_cptr);
	}
}
