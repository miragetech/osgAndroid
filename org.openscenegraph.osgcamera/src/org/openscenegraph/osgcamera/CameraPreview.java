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
package org.openscenegraph.osgcamera;

import java.io.IOException;
import java.util.List;

import org.openscenegraph.osg.util.OSGPreviewCallback;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Implementation of a Camera Surface View to render camera in the Android
 * Canvas.
 * 
 * @author Rafa Gait‡n <rafa.gaitan@mirage-tech.com>
 * @author Julio Campos <julio.campos@mirage-tech.com>
 * 
 */
public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
	/** Preview Callback to detect new frames **/
	private OSGPreviewCallback _previewCallback = null;
	private SurfaceHolder _holder;
	private Camera _camera;
	/** 0: Back Camera; 1: Front Camera; 2: Stereo Camera in LG Optimus 3D **/
	private int _cameraId = 0;

	private void init(Context context) {
		// Install a SurfaceHolder.Callback so we get notified when the
		// underlying surface is created and destroyed.
		_holder = getHolder();
		_holder.addCallback(this);
		_holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		_cameraId = 0;
	}

	public CameraPreview(Context context) {
		super(context);
		init(context);
	}

	public CameraPreview(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
		init(context);
	}

	public void surfaceCreated(SurfaceHolder holder) {
		// The Surface has been created, acquire the camera and tell it where
		// to draw.
		try {
			// Android API 2.3.x, previous versions does not allow to select the camera ID
			_camera = Camera.open(_cameraId);
			if (_camera != null) {
				_camera.setPreviewDisplay(holder);
				if (_previewCallback != null) {
					_camera.setPreviewCallbackWithBuffer(_previewCallback);
				}
			}
		} catch (IOException exception) {
			_camera.release();
			_camera = null;
		}
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		// Surface will be destroyed when we return, so stop the preview.
		// Because the CameraDevice object is not a shared resource, it's very
		// important to release it when the activity is paused.
		if (_camera != null) {
			_camera.setPreviewCallbackWithBuffer(null);
			_camera.stopPreview();
			_camera.release();
			_camera = null;
		}
	}

	/**
	 * Obtains the best resolution for the @see {@link Camera}
	 * 
	 * @param sizes
	 * @param w
	 * @param h
	 * @return
	 */
	private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
		final double ASPECT_TOLERANCE = 0.1;
		double targetRatio = (double) w / h;
		if (sizes == null)
			return null;

		Size optimalSize = null;
		double minDiff = Double.MAX_VALUE;

		int targetHeight = h;

		// Try to find an size match aspect ratio and size
		for (Size size : sizes) {
			double ratio = (double) size.width / size.height;
			if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE)
				continue;
			if (Math.abs(size.height - targetHeight) < minDiff) {
				optimalSize = size;
				minDiff = Math.abs(size.height - targetHeight);
			}
		}

		// Cannot find the one match the aspect ratio, ignore the requirement
		if (optimalSize == null) {
			minDiff = Double.MAX_VALUE;
			for (Size size : sizes) {
				if (Math.abs(size.height - targetHeight) < minDiff) {
					optimalSize = size;
					minDiff = Math.abs(size.height - targetHeight);
				}
			}
		}
		return optimalSize;
	}

	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		if (_camera != null) {
			Camera.Parameters parameters = _camera.getParameters();
			List<Size> sizes = parameters.getSupportedPreviewSizes();
			Size optimalSize = getOptimalPreviewSize(sizes, w, h);
			// 0 : first camera, 1 : second camera, 2 : dual(3D) camera
			parameters.setPreviewSize(optimalSize.width, optimalSize.height);
			parameters.setPictureSize(optimalSize.width, optimalSize.height);
			// parameters.setWhiteBalance(Parameters.WHITE_BALANCE_AUTO);
			parameters.setFocusMode(Parameters.FOCUS_MODE_AUTO);
			_camera.setParameters(parameters);
			if (_previewCallback != null) {
				_previewCallback.init(optimalSize.width, optimalSize.height,
						_camera);
			}
			_camera.startPreview();
		}
	}

	/**
	 * Sets the preview callback to be called when a new frame is retrieved from @see
	 * {@link Camera}
	 * 
	 * @param previewCallback
	 */
	public void setPreviewCallback(OSGPreviewCallback previewCallback) {
		_previewCallback = previewCallback;
	}
}