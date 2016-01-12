package org.openscenegraph.osg.core;

public class Vec4Array extends Array {
	private static native void nativeDispose(long cptr);
	private static native long nativeCreateVec4Array();
	private static native void nativePushBackVec4(long cptr, long vec_cptr);
	private static native void nativePushBackFloatArray(long cptr, float[] array_cptr);
	private static native long nativePopBack(long cptr);
	private static native long nativeGet(long cptr, int i);
	private static native boolean nativeSet(long cptr, int i, long vec_cptr);
	
	public Vec4Array(long cptr) {
		super(cptr);	
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}
	
	public Vec4Array() {
		_cptr = nativeCreateVec4Array();
	}
	
	protected void push_back(float[] array)
	{
		nativePushBackFloatArray(_cptr, array);
	}
	
	protected void push_back(Vec4 vector)
	{
		nativePushBackVec4(_cptr, vector.getNativePtr());
	}

	public void add(float[] value)
	{
		push_back(value);
	}
	
	public void add(Vec4 value)
	{
		push_back(value);
	}
	
	protected Vec4 pop_back()
	{
		return new Vec4(nativePopBack(_cptr));
	}
	
	public Vec4 get(int i)
	{
		return new Vec4(nativeGet(_cptr, i));
	}
	
	public boolean set(int i, Vec4 v)
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
		float buffer[][] = new float[N][4];
		for(int i = 0; i < N; i++)
		{
			Vec4 vec = pop_back();
			buffer[n-i][0] = vec.x();
			buffer[n-i][1] = vec.y();
			buffer[n-i][2] = vec.z();
			buffer[n-i][3] = vec.w();
		}
		return buffer;
	}
}
