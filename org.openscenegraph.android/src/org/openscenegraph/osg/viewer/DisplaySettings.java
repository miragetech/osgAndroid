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

import org.openscenegraph.osg.Library;
import org.openscenegraph.osg.Native;

/**
 * class for encapsulating what visuals are required and have been
 * set up, and the status of stereo viewing. 
 * @author jtorres,rgaitan
 *
 */
public class DisplaySettings implements Native {
	static {
		Library.initLibrary();
	}

	public static class DisplayType {
		public static int MONITOR = 0; // / default
		public static int POWERWALL = 1;
		public static int REALITY_CENTER = 2;
		public static int HEAD_MOUNTED_DISPLAY = 3;
	}

	public static class StereoMode {
		public static int QUAD_BUFFER = 0; // / default
		public static int ANAGLYPHIC = 1;
		public static int HORIZONTAL_SPLIT = 2;
		public static int VERTICAL_SPLIT = 3;
		public static int LEFT_EYE = 4;
		public static int RIGHT_EYE = 5;
		public static int HORIZONTAL_INTERLACE = 6;
		public static int VERTICAL_INTERLACE = 7;
		public static int CHECKERBOARD = 8; 
	}

	private native static long native_createDisplaySettings();
	
	private static native void nativeDispose(long cptr);

	private native void native_setDisplayType(long cptr, int type);

	private native void native_setStereoMode(long cptr, int mode);

	private native void native_setEyeSeparation(long cptr, float separation);

	private native void native_setScreenWidth(long cptr, float width);

	private native void native_setScreenHeight(long cptr, float height);

	private native void native_setScreenDistance(long cptr, float distance);

	private native void native_setStereo(long cptr, boolean value);

	private native void native_setDoubleBuffer(long cptr, boolean value);

	private native int native_getDisplayType(long cptr);

	private native int native_getStereoMode(long cptr);

	private native float native_getEyeSeparation(long cptr);

	private native float native_getScreenWidth(long cptr);

	private native float native_getScreenHeight(long cptr);

	private native float native_getScreenDistance(long cptr);

	private native boolean native_getStereo(long cptr);

	private native boolean native_getDoubleBuffer(long cptr);

	private native void native_setNumMultiSamples(long cptr, int samples);
	
	private native int native_getNumMultiSamples(long cptr);
	
	private long _cptr;
	public long getNativePtr() {
		return _cptr;
	}
	
	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public void dispose() {
		if(_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}
	
	/**
	 * Default constructor
	 */
	public DisplaySettings() {
		_cptr = native_createDisplaySettings();
	}
	/**
	 * Copy Constructor
	 * @param cptr
	 */
	public DisplaySettings(long cptr) {
		_cptr = cptr;
	}
	/**
	 * Set the DisplayType in this DisplaySettings
	 * @param type
	 */
	public void setDisplayType(int type) {
		native_setDisplayType(getNativePtr(), type);
	}
	/**
	 * Return Display Type
	 * @return
	 */
	public int getDisplayType() {
		return native_getDisplayType(getNativePtr());
	}
	
	/**
	 * Sets stereo Mode
	 * @param mode
	 */
	public void setStereoMode(int mode) {
		native_setStereoMode(getNativePtr(), mode);
	}
	/**
	 * Return stereo mode
	 * @return
	 */
	public int getStereoMode() {
		return native_getStereoMode(getNativePtr());
	}
	
	/**
	 * Sets the eye separation in stereo mode for this DisplaySettings
	 * @param separation
	 */
	public void setEyeSeparation(float separation) {
		native_setEyeSeparation(getNativePtr(), separation);
	}
	/**
	 * Get the eye separation
	 * @return
	 */
	public float getEyeSeparation() {
		return native_getEyeSeparation(getNativePtr());
	}

	/**
	 * Set the screen width
	 * @param width
	 */
	public void setScreenWidth(float width) {
		native_setScreenWidth(getNativePtr(), width);
	}
	/**
	 * Return screen width
	 * @return
	 */
	public float getScreenWidth() {
		return native_getScreenWidth(getNativePtr());
	}
	/**
	 * Set screen height
	 * @param height
	 */
	public void setScreenHeight(float height) {
		native_setScreenHeight(getNativePtr(), height);
	}
	/**
	 * Get screen height
	 * @return
	 */
	public float getScreenHeight() {
		return native_getScreenHeight(getNativePtr());
	}
	/**
	 * Set screen distance
	 * @param distance
	 */
	public void setScreenDistance(float distance) {
		native_setScreenDistance(getNativePtr(), distance);
	}
	/**
	 * Return screen distance
	 * @return
	 */
	public float getScreenDistance() {
		return native_getScreenDistance(getNativePtr());
	}
	
	/**
	 * Enable/disable stereo
	 * @param value
	 */
	public void setStereo(boolean value) {
		native_setStereo(getNativePtr(), value);
	}

	/**
	 * Return if stereo is enabled/disabled
	 * @return
	 */
	public boolean getStereo() {
		return native_getStereo(getNativePtr());
	}

	/**
	 * Sets double buffer enabled/disabled
	 * @param value
	 */
	public void setDoubleBuffer(boolean value) {
		native_setDoubleBuffer(getNativePtr(), value);
	}

	/**
	 * Return if double buffer is enabled/disabled
	 * @return
	 */
	public boolean getDoubleBuffer() {
		return native_getDoubleBuffer(getNativePtr());
	}
	/**
	 * Set antialias by multisamplig
	 * @param samples
	 */
	public void setNumMultiSamples(int samples){
		native_setNumMultiSamples(getNativePtr(), samples);
	}
	/**
	 * Return number of multisamples
	 * @return
	 */
	public int getNumMultiSamples(){
		return native_getNumMultiSamples(getNativePtr());
	}
}

