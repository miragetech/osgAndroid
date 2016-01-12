package org.openscenegraph.osg.viewer;

import javax.microedition.khronos.egl.EGL;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import org.openscenegraph.osg.Native;
import org.openscenegraph.osg.core.Camera;
import org.openscenegraph.osg.core.Image;
import org.openscenegraph.osg.core.Matrix;
import org.openscenegraph.osg.core.Node;
import org.openscenegraph.osg.core.Vec2;
import org.openscenegraph.osg.core.Vec3;
import org.openscenegraph.osg.core.Vec2Array;
import org.openscenegraph.osg.core.Vec3Array;
import android.graphics.Bitmap;

import android.opengl.GLSurfaceView;
import android.util.Log;

import java.nio.IntBuffer;

public class OffScreenViewer implements Native {

	private native long nativeCreatePBufferViewer();
	private native void nativeSetUpViewerAsEmbedded(long cptr, int x, int y, int width, int height);
	private native void nativeSetViewport(long cptr, int x, int y, int width, int height);
	private native void nativeSetView(long cptr, int width, int height, float fov);
	private native void nativeSetupCallback(long cptr);
	private native long nativeGetFrame(long cptr, long matrix_ptr);
	private native long nativeGetFrameImage(long cptr);
	private native void nativeSetViewMatrix(long cptr, long matrix_ptr);
	private native void nativeSetRenderMatrix(long cptr, long matrix_ptr);
	private native void nativeDisposeViewer(long cptr);
	private native void nativeSetSceneData(long cptr, long cptrNode);
	private native void nativeFrame(long cptr);
	private native long nativeGetCamera(long cptr);
	private native long nativeRaycast(long viewer_cptr, long camera_ptr, long vec2array_cptr);


	private EGLConfig eglConf = null;
	private EGLSurface eglSurface = null;
	private EGLContext eglCtx = null;
	private EGLDisplay eglDisp = null;
	private EGL10 mEGL = null;
	private GL10 mGL = null;
	private String mThreadOwner;
	private GLSurfaceView.Renderer mRenderer = null;
	
	private int mWidth, mHeight;
	private Bitmap mBitmap;

	private long _cptr = 0;
	private static String TAG = "org.openscenegraph.osg.viewer.OffScreenViewer";
	private static final boolean DEBUG = false;
	public long getNativePtr() {
		return _cptr;
	}

	public OffScreenViewer(int width, int height, float fov) {
		Log.w(TAG, "Creating OffScreenViewer ...");
		_cptr = nativeCreatePBufferViewer();
		mWidth = width; mHeight = height;
		
		int[] s_configAttribs2 = { EGL10.EGL_RED_SIZE, 
									8, 
									EGL10.EGL_GREEN_SIZE, 
									8, 
									EGL10.EGL_BLUE_SIZE, 
									8, 
									EGL10.EGL_ALPHA_SIZE, 
									8, 
									EGL10.EGL_SURFACE_TYPE,
									EGL10.EGL_PBUFFER_BIT,
									EGL10.EGL_NONE };
		
		int surfaceAttr[] = {
				EGL10.EGL_WIDTH, width,
				EGL10.EGL_HEIGHT, height,
				EGL10.EGL_NONE
		};
		
		int[] version = new int[2];
		
        // No error checking performed, minimum required code to elucidate logic
        mEGL = (EGL10) EGLContext.getEGL();
        eglDisp = mEGL.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
        mEGL.eglInitialize(eglDisp, version);
        ConfigChooserGLES11 selection = new ConfigChooserGLES11(8, 8, 8, 8, 16, 8);
        selection.setAttributes(s_configAttribs2);
        eglConf = selection.chooseConfig(mEGL, eglDisp);

        //int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        //int[] ctxAttr = {
        //        EGL_CONTEXT_CLIENT_VERSION, 2,
        //        EGL10.EGL_NONE
        //};
        //int[] ctxAttr = {};
        //eglCtx = mEGL.eglCreateContext(eglDisp, eglConf, EGL10.EGL_NO_CONTEXT, ctxAttr);
        eglCtx = mEGL.eglCreateContext(eglDisp, eglConf, EGL10.EGL_NO_CONTEXT, null);

        eglSurface = mEGL.eglCreatePbufferSurface(eglDisp, eglConf, surfaceAttr);
        mEGL.eglMakeCurrent(eglDisp, eglSurface, eglSurface, eglCtx);

        mGL = (GL10) eglCtx.getGL();

        // Record thread owner of OpenGL context
        mThreadOwner = Thread.currentThread().getName();
        
        
        /*
         * set up renderer
         */
        mRenderer = new OSGRenderer(asViewerBase());
        // Does this thread own the OpenGL context?
        if (!Thread.currentThread().getName().equals(mThreadOwner)) {
            Log.e(TAG, "setRenderer: This thread does not own the OpenGL context.");
            return;
        }

        // Call the renderer initialization routines
        mRenderer.onSurfaceCreated(mGL, eglConf);
        mRenderer.onSurfaceChanged(mGL, width, height);
        //float fov = new Float(2.0*Math.atan(4.6032/(2.0*3.97)));
        setView(width, height, fov);
        nativeSetupCallback(_cptr);
	}

	public void close()
	{
		try {
			//Log.w(TAG, "Closing OffScreen Renderer ...");
			finalize();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		} catch (Throwable e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	protected void finalize() throws Throwable {
		//Log.w(TAG, "Finalizing OffScreen Renderer ...");
		if(mEGL!=null)
		{
	        mEGL.eglMakeCurrent(eglDisp, eglSurface, eglSurface, eglCtx);
	        /*
			if(mRenderer!=null)
			{
				mRenderer.onDrawFrame(mGL);
				mRenderer.onDrawFrame(mGL);
			}
			*/
			dispose();
			mRenderer = null;

			mEGL.eglMakeCurrent(eglDisp, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
	        mEGL.eglDestroySurface(eglDisp, eglSurface);
	        eglSurface = null;
	        mEGL.eglDestroyContext(eglDisp, eglCtx);
	        eglCtx = null;
	        mEGL.eglTerminate(eglDisp);
	        mEGL = null;
		}
		//Log.w(TAG, "OffScreen Renderer destroyed ...");
		super.finalize();
	}

	public void dispose() {
		Log.w(TAG, "Disposing Off-Screen Viewer");
		if (_cptr != 0)
			nativeDisposeViewer(_cptr);
		_cptr = 0;
	}

	public ViewerBase asViewerBase()
	{
		return new ViewerBase(_cptr);
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
	 * Sets the scene to render
	 * 
	 */
	public void setSceneData(Node node) {
		nativeSetSceneData(_cptr, node.getNativePtr());
	}

	public void setViewMatrix(Matrix mat)
	{
		nativeSetViewMatrix(_cptr, mat.getNativePtr());
	}
	
	public void setRenderMatrix(Matrix mat)
	{
		nativeSetRenderMatrix(_cptr, mat.getNativePtr());
	}
	
	public void setView(int width, int height, float fov)
	{
		nativeSetView(_cptr, width, height, fov);
	}

	/**
	 * Gets the viewer main camera
	 * 
	 * @return Camera
	 */
	public Camera getCamera() {
		return new Camera(nativeGetCamera(_cptr));
	}
	
	public Image getNextFrameImage(Matrix mat)
	{
		nativeSetViewMatrix(_cptr, mat.getNativePtr());
		
		if(mRenderer != null)
		{
			mRenderer.onDrawFrame(mGL);
			mRenderer.onDrawFrame(mGL);
		}
		
		convertToBitmap();
		long image_ptr = nativeGetFrameImage(_cptr);
		Image result = new Image();
		if(image_ptr!=0)
			result = new Image(image_ptr);
		return result;
	}
	

    private void convertToBitmap() {
        int[] iat = new int[mWidth * mHeight];
        IntBuffer ib = IntBuffer.allocate(mWidth * mHeight);
        mGL.glReadPixels(0, 0, mWidth, mHeight, mGL.GL_RGBA, mGL.GL_UNSIGNED_BYTE, ib);
        int[] ia = ib.array();

        //Stupid !
        // Convert upside down mirror-reversed image to right-side up normal
        // image.
        for (int i = 0; i < mHeight; i++) {
            for (int j = 0; j < mWidth; j++) {
                iat[(mHeight - i - 1) * mWidth + j] = ia[i * mWidth + j];
            }
        }
        

        mBitmap = Bitmap.createBitmap(mWidth, mHeight, Bitmap.Config.ARGB_8888);
        mBitmap.copyPixelsFromBuffer(IntBuffer.wrap(iat));
    }
    
    public Bitmap GetRenderImage()
    {
    	return mBitmap;
    }
    
    public Vec3Array Raycast(Vec2Array image_points, Camera cam)
    {
    	return new Vec3Array(nativeRaycast(_cptr, cam.getNativePtr(), image_points.getNativePtr()));
    }
	
	/**
	 * Determine configuration
	 */
	private static class ConfigChooserGLES11 implements	GLSurfaceView.EGLConfigChooser {

		public ConfigChooserGLES11(int r, int g, int b, int a, int depth, int stencil) {
			mRedSize = r;
			mGreenSize = g;
			mBlueSize = b;
			mAlphaSize = a;
			mDepthSize = depth;
			mStencilSize = stencil;
		}
		
		/*
		 * This EGL config specification is used to specify 2.0 rendering. We
		 * use a minimum size of 4 bits for red/green/blue, but will perform
		 * actual matching in chooseConfig() below.
		 */
		public int[] s_configAttribs2;
		public void setAttributes(int[] attributes)
		{
			s_configAttribs2 = attributes;
		}
		
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
		
			/*
			 * Get the number of minimally matching EGL configurations
			 */
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);
		
			int numConfigs = num_config[0];
		
			if (numConfigs <= 0) {
				throw new IllegalArgumentException(
						"No configs match configSpec");
			}
		
			/*
			 * Allocate then read the array of minimally matching EGL configs
			 */
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs,
					num_config);
		
			if (DEBUG) {
				printConfigs(egl, display, configs);
			}
			/*
			 * Now return the "best" one
			 */
			return chooseConfig(egl, display, configs);
		}
		
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
				EGLConfig[] configs) {
			for (EGLConfig config : configs) {
				int sf = findConfigAttrib(egl, display, config, EGL10.EGL_SURFACE_TYPE, 0);
				int d = findConfigAttrib(egl, display, config,
						EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config,
						EGL10.EGL_STENCIL_SIZE, 0);
		
				// We need at least mDepthSize and mStencilSize bits
				if (d < mDepthSize || s < mStencilSize)
					continue;
		
				// We want an *exact* match for red/green/blue/alpha
				int r = findConfigAttrib(egl, display, config,
						EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config,
						EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config,
						EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config,
						EGL10.EGL_ALPHA_SIZE, 0);
		
				if (r == mRedSize && g == mGreenSize && b == mBlueSize
						&& a == mAlphaSize && ((sf & EGL10.EGL_PBUFFER_BIT) > 0) )
					return config;
			}
			return null;
		}
		
		private int findConfigAttrib(EGL10 egl, EGLDisplay display,
				EGLConfig config, int attribute, int defaultValue) {
		
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}
		
		private void printConfigs(EGL10 egl, EGLDisplay display,
				EGLConfig[] configs) {
			int numConfigs = configs.length;
			Log.w(TAG, String.format("%d configurations", numConfigs));
			for (int i = 0; i < numConfigs; i++) {
				Log.w(TAG, String.format("Configuration %d:\n", i));
				printConfig(egl, display, configs[i]);
			}
		}
		
		private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config) {
			int[] attributes = { EGL10.EGL_BUFFER_SIZE, EGL10.EGL_ALPHA_SIZE,
					EGL10.EGL_BLUE_SIZE,
					EGL10.EGL_GREEN_SIZE,
					EGL10.EGL_RED_SIZE,
					EGL10.EGL_DEPTH_SIZE,
					EGL10.EGL_STENCIL_SIZE,
					EGL10.EGL_CONFIG_CAVEAT,
					EGL10.EGL_CONFIG_ID,
					EGL10.EGL_LEVEL,
					EGL10.EGL_MAX_PBUFFER_HEIGHT,
					EGL10.EGL_MAX_PBUFFER_PIXELS,
					EGL10.EGL_MAX_PBUFFER_WIDTH,
					EGL10.EGL_NATIVE_RENDERABLE,
					EGL10.EGL_NATIVE_VISUAL_ID,
					EGL10.EGL_NATIVE_VISUAL_TYPE,
					0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
					EGL10.EGL_SAMPLES,
					EGL10.EGL_SAMPLE_BUFFERS,
					EGL10.EGL_SURFACE_TYPE,
					EGL10.EGL_TRANSPARENT_TYPE,
					EGL10.EGL_TRANSPARENT_RED_VALUE,
					EGL10.EGL_TRANSPARENT_GREEN_VALUE,
					EGL10.EGL_TRANSPARENT_BLUE_VALUE,
					0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
					0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
					0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
					0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
					EGL10.EGL_LUMINANCE_SIZE, EGL10.EGL_ALPHA_MASK_SIZE,
					EGL10.EGL_COLOR_BUFFER_TYPE, EGL10.EGL_RENDERABLE_TYPE,
					0x3042 // EGL10.EGL_CONFORMANT
			};
			String[] names = { "EGL_BUFFER_SIZE", "EGL_ALPHA_SIZE",
					"EGL_BLUE_SIZE", "EGL_GREEN_SIZE", "EGL_RED_SIZE",
					"EGL_DEPTH_SIZE", "EGL_STENCIL_SIZE", "EGL_CONFIG_CAVEAT",
					"EGL_CONFIG_ID", "EGL_LEVEL", "EGL_MAX_PBUFFER_HEIGHT",
					"EGL_MAX_PBUFFER_PIXELS", "EGL_MAX_PBUFFER_WIDTH",
					"EGL_NATIVE_RENDERABLE", "EGL_NATIVE_VISUAL_ID",
					"EGL_NATIVE_VISUAL_TYPE", "EGL_PRESERVED_RESOURCES",
					"EGL_SAMPLES", "EGL_SAMPLE_BUFFERS", "EGL_SURFACE_TYPE",
					"EGL_TRANSPARENT_TYPE", "EGL_TRANSPARENT_RED_VALUE",
					"EGL_TRANSPARENT_GREEN_VALUE",
					"EGL_TRANSPARENT_BLUE_VALUE", "EGL_BIND_TO_TEXTURE_RGB",
					"EGL_BIND_TO_TEXTURE_RGBA", "EGL_MIN_SWAP_INTERVAL",
					"EGL_MAX_SWAP_INTERVAL", "EGL_LUMINANCE_SIZE",
					"EGL_ALPHA_MASK_SIZE", "EGL_COLOR_BUFFER_TYPE",
					"EGL_RENDERABLE_TYPE", "EGL_CONFORMANT" };
			int[] value = new int[1];
			for (int i = 0; i < attributes.length; i++) {
				int attribute = attributes[i];
				String name = names[i];
				if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
					Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
				} else {
					// Log.w(TAG, String.format("  %s: failed\n", name));
					while (egl.eglGetError() != EGL10.EGL_SUCCESS)
						;
				}
			}
		}
		
		// Subclasses can adjust these values:
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}
}
