package org.openscenegraph.osg.core;

public class Vec2Array extends Array {
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateVec2Array();
	private static native void nativePushBackVec2(long cptr, long vec_cptr);
	private static native void nativePushBackFloatArray(long cptr, float[] array_cptr);
	private static native long nativePopBack(long cptr);
	private static native long nativeGet(long cptr, int i);
	private static native boolean nativeSet(long cptr, int i, long vec_cptr);
	
	public Vec2Array(long cptr) {
		super(cptr);	
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}
	
	public Vec2Array() {
		_cptr = nativeCreateVec2Array();
	}
	
	protected void push_back(float[] array)
	{
		nativePushBackFloatArray(_cptr, array);
	}
	
	protected void push_back(Vec2 vector)
	{
		nativePushBackVec2(_cptr, vector.getNativePtr());
	}

	public void add(float[] value)
	{
		push_back(value);
	}
	
	public void add(Vec2 value)
	{
		push_back(value);
	}
	
	protected Vec2 pop_back()
	{
		return new Vec2(nativePopBack(_cptr));
	}
	
	public Vec2 get(int i)
	{
		return new Vec2(nativeGet(_cptr, i));
	}
	
	public boolean set(int i, Vec2 v)
	{
		return nativeSet(_cptr, i, v.getNativePtr());
	}
	
	/*
	 * the "toArray" function replaces the vector array with the
	 * float array, thus the original vector array is empty after
	 * this operation.
	 */
	public float[][] toArray()
	{
		int N = size();
		int n = N-1;
		float buffer[][] = new float[N][2];
		for(int i = 0; i < N; i++)
		{
			Vec2 vec = pop_back();
			buffer[n-i][0] = vec.x();
			buffer[n-i][1] = vec.y();
		}
		return buffer;
	}
}
