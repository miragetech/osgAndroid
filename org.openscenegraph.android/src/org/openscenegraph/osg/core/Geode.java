package org.openscenegraph.osg.core;

public class Geode extends Node {
	
	private static native long nativeCreateGeode();
	private static native void nativeDispose(long cptr);
	private static native boolean nativeAddDrawable(long cptr, long drawable_ptr);
	private static native boolean nativeRemoveDrawable(long cptr, long cptr_drawable);
	private static native boolean nativeRemoveDrawables(long cptr, int i, int m);
	private static native boolean nativeReplaceDrawable(long cptr, long cptr_drawable_old, long cptr_drawable_new);
	private static native boolean nativeSetDrawable(long cptr, int i, long cptr_drawable);
	private static native int nativeGetNumDrawables(long cptr);
	private static native long nativeGetDrawable(long cptr, int i);

	public Geode() {
		_cptr = nativeCreateGeode();
	}

	public Geode(long cptr) {
		super(cptr);
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public boolean addDrawable(Drawable _drawable)
	{
		return nativeAddDrawable(_cptr, _drawable.getNativePtr());
	}
	
	public boolean removeDrawable(Drawable _drawable)
	{
		return nativeRemoveDrawable(_cptr, _drawable.getNativePtr());
	}
	
	public boolean removeDrawables(int index, int numElements)
	{
		return nativeRemoveDrawables(_cptr, index, numElements);
	}
	
	public boolean replaceDrawable(Drawable old_element, Drawable new_element)
	{
		return nativeReplaceDrawable(_cptr, old_element.getNativePtr(), new_element.getNativePtr());
	}
	
	public boolean setDrawable(int index, Drawable _drawable)
	{
		return nativeSetDrawable(_cptr, index, _drawable.getNativePtr());
	}
	
	public int getNumDrawables()
	{
		return nativeGetNumDrawables(_cptr);
	}
	
	public Drawable getDrawable(int index)
	{
		return new Drawable(nativeGetDrawable(_cptr, index));
	}
	
	public Drawable getLastDrawable()
	{
		int pos = getNumDrawables()-1;
		return new Drawable(nativeGetDrawable(_cptr, pos));
	}
}
