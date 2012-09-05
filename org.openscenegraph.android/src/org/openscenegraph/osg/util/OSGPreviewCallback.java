package org.openscenegraph.osg.util;

import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;

/**
 * Interface that extends from @see {@link PreviewCallback} to add the init
 * method and configure initial @see {@link Camera} camera parameters
 * 
 * @author Rafa Gait‡n <rafa.gaitan@mirage-tech.com>
 * @author Julio Campos <julio.campos@mirage-tech.com>
 * 
 */
public interface OSGPreviewCallback extends PreviewCallback {

	public void init(int width, int height, Camera camera);

}
