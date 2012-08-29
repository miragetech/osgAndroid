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
package org.openscenegraph.osg.viewer;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import org.openscenegraph.osg.Library;
import org.openscenegraph.osg.Native;
import org.openscenegraph.osg.core.Camera;
import org.openscenegraph.osg.core.Node;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

public class Viewer extends GLSurfaceView implements Native,
		View.OnTouchListener {
	static {
		Library.initLibrary();
	}
	private long _cptr;
	private static String TAG = "org.openscenegraph.osg.viewer.Viewer";
	private static final boolean DEBUG = false;

	public long getNativePtr() {
		return _cptr;
	}

	/**
	 * @return native pointer to the viewer
	 */
	private native long nativeCreateViewer();

	private native void nativeDisposeViewer(long cptr);

	private native void nativeSetDefaultSettings(long cptr);

	private native void nativeSetSceneData(long cptr, long cptrNode);

	private native void nativeSetViewport(long cptr, int x, int y, int width,
			int height);

	private native void nativeMouseMotion(long cptr, int x, int y);

	private native void nativeMousePassiveMotion(long cptr, int x, int y);

	private native void nativeMouse(long cptr, int button, int state, int x,
			int y);

	private native void nativeKeyboard(long cptr, int key, int x, int y,
			boolean keydown);

	private native void nativeSetUpViewerAsEmbedded(long cptr, int x, int y,
			int width, int height);

	private native void nativeFrame(long cptr);

	private native long nativeGetCamera(long cptr);
	
    private native void native_setDisplaySettings(long cptr, long cptrdisplay);

    private native long native_getDisplaySettings(long cptr);
    
    protected native void native_setFusionDistance(long cptr, float fusionDistance);

    protected native float native_getFusionDistance(long cptr);

	public Viewer(Context context) {
		super(context);
		// init(false, 16, 8);
		/* create the osg viewer */
		_cptr = nativeCreateViewer();
	}

	// public Viewer(Context context, boolean translucent, int depth, int
	// stencil) {
	// super(context);
	// init(translucent, depth, stencil);
	// }

	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public void dispose() {
		Log.i(TAG, "Disposing viewer");
		if (_cptr != 0)
			nativeDisposeViewer(_cptr);
		_cptr = 0;
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

	/**
	 * Enables default settings (TrackballManipulator, Empty group,
	 * StatsHandler, ...)
	 */
	public void setDefaultSettings() {
		nativeSetDefaultSettings(_cptr);
	}

	/**
	 * Sets the scene to render
	 * 
	 */
	public void setSceneData(Node node) {
		nativeSetSceneData(_cptr, node.getNativePtr());
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
     * Sets DisplaySettings
     */
    public void setDisplaySettings(DisplaySettings displaysettings) {
            native_setDisplaySettings(getNativePtr(), displaysettings.getNativePtr());
    }

    /**
     * Return DisplaySettings attached to this viewer
     */
    public DisplaySettings getDisplaySettings() {
            return new DisplaySettings(native_getDisplaySettings(getNativePtr()));

    }
    
    public float getFusionDistanceValue() {
        return native_getFusionDistance(getNativePtr());
	}
	
	public void setFusionDistance(float fusionDistance) {
	        native_setFusionDistance(getNativePtr(), fusionDistance);
	}



	/**
	 * Sets the position of the mouse. typically called from Mouse Listeners.
	 * 
	 * @param x
	 * @param y
	 */
	public synchronized void mouseMotion(int x, int y) {
		nativeMouseMotion(_cptr, x, y);
	}

	/**
	 * Sets the position of the mouse typically called from Mouse Listeners.
	 * 
	 * @param x
	 * @param y
	 */
	public synchronized void mousePassiveMotion(int x, int y) {
		nativeMousePassiveMotion(_cptr, x, y);
	}

	/**
	 * Sets the mouse button state and position. typically called from Mouse
	 * Listeners.
	 * 
	 * @param button
	 * @param state
	 * @param x
	 * @param y
	 */
	public synchronized void mouse(int button, int state, int x, int y) {
		nativeMouse(_cptr, button, state, x, y);
	}

	/**
	 * Sets the pressed key typically called from Mouse Listeners.
	 * 
	 * @param key
	 * @param x
	 * @param y
	 * @param keydown
	 */
	public synchronized void keyboard(int key, int x, int y, boolean keydown) {
		nativeKeyboard(_cptr, key, x, y, keydown);
	}

	/**
	 * Gets the viewer main camera
	 * 
	 * @return Camera
	 */
	public Camera getCamera() {
		return new Camera(nativeGetCamera(_cptr));
	}

	public void init(boolean translucent, int depth, int stencil,
			OSGRenderer renderer) {

		/*
		 * By default, GLSurfaceView() creates a RGB_565 opaque surface. If we
		 * want a translucent one, we should change the surface's format here,
		 * using PixelFormat.TRANSLUCENT for GL Surfaces is interpreted as any
		 * 32-bit surface with alpha by SurfaceFlinger.
		 */
		if (translucent) {
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}

		/*
		 * Setup the context factory for 2.0 rendering. See ContextFactory class
		 * definition below
		 */
		// setEGLContextFactory(new ContextFactoryGLES20());
		setEGLContextFactory(new ContextFactoryGLES11());

		/*
		 * We need to choose an EGLConfig that matches the format of our surface
		 * exactly. This is going to be done in our custom config chooser. See
		 * ConfigChooser class definition below.
		 */
		setEGLConfigChooser(translucent ? new ConfigChooser(8, 8, 8, 8, depth,
				stencil) : new ConfigChooser(5, 6, 5, 0, depth, stencil));

		/* Set the renderer responsible for frame rendering */
		setRenderer(renderer);

		setOnTouchListener(this);
	}

	public void init(boolean translucent, int depth, int stencil) {
		init(translucent, depth, stencil, new OSGRenderer(this));
	}

	private static class ContextFactoryGLES11 implements
			GLSurfaceView.EGLContextFactory {
		private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

		public EGLContext createContext(EGL10 egl, EGLDisplay display,
				EGLConfig eglConfig) {
			Log.w(TAG, "creating OpenGL ES 1.1 context");
			checkEglError("Before eglCreateContext", egl);
			int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 1, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig,
					EGL10.EGL_NO_CONTEXT, attrib_list);
			checkEglError("After eglCreateContext", egl);
			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display,
				EGLContext context) {
			egl.eglDestroyContext(display, context);
		}
	}

	private static void checkEglError(String prompt, EGL10 egl) {
		int error;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
			Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
		}
	}

	private static class ConfigChooser implements
			GLSurfaceView.EGLConfigChooser {

		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
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
		private static int EGL_OPENGL_ES_BIT = 1;
		private static int[] s_configAttribs2 = { EGL10.EGL_RED_SIZE, 4,
				EGL10.EGL_GREEN_SIZE, 4, EGL10.EGL_BLUE_SIZE, 4,
				EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT, EGL10.EGL_NONE };

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
						&& a == mAlphaSize)
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

	public boolean onTouch(View arg0, MotionEvent event) {
		int action = event.getAction() & MotionEvent.ACTION_MASK;

		switch (action) {
		case MotionEvent.ACTION_DOWN:
			mouse(1, 1, (int) event.getX(), (int) event.getY());
			break;
		case MotionEvent.ACTION_UP:
			mouse(1, 0, (int) event.getX(), (int) event.getY());
			break;
		case MotionEvent.ACTION_MOVE:
			mouseMotion((int) event.getX(), (int) event.getY());
			break;
		}
		return true;
	}

	public boolean onKey(View v, int keyCode, KeyEvent event) {
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		int key = event.getUnicodeChar();
		keyboard(key, 0, 0, true);
		return true;
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		int key = event.getUnicodeChar();
		keyboard(key, 0, 0, false);
		return true;
	}
}
