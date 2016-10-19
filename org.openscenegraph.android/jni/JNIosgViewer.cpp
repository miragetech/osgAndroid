/* @License
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait�n, Mirage Technologies S.L.     |
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

#include <jni.h>
#include <android/log.h>
#include "screenview.h"

#include "JNIUtils.h"
#include <stdlib.h>
#include <sstream>

#include <osg/ref_ptr>
#include <osg/observer_ptr>
#include <osg/Version>
#include <osg/DisplaySettings>

#include <osg/CoordinateSystemNode>
#include <osg/ref_ptr>
#include <osg/Array>

#include <osg/io_utils>
#include <osg/ImageUtils>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/GraphicsContext>
#include <osg/Texture2D>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/Image>
#include <osg/State>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/CameraManipulator>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgGA/GUIEventAdapter>
#include <osgGA/StateSetManipulator>

#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/LineSegmentIntersector>


USE_OSGPLUGIN(ive)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(rgb)
USE_OSGPLUGIN(bmp)
USE_OSGPLUGIN(tga)
USE_OSGPLUGIN(gif)
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(OpenFlight)

#ifdef USE_FREETYPE
    USE_OSGPLUGIN(freetype)
#endif

USE_DOTOSGWRAPPER_LIBRARY(osg)
//USE_DOTOSGWRAPPER_LIBRARY(osgAnimation)
USE_DOTOSGWRAPPER_LIBRARY(osgFX)
USE_DOTOSGWRAPPER_LIBRARY(osgParticle)
USE_DOTOSGWRAPPER_LIBRARY(osgShadow)
USE_DOTOSGWRAPPER_LIBRARY(osgSim)
USE_DOTOSGWRAPPER_LIBRARY(osgTerrain)
USE_DOTOSGWRAPPER_LIBRARY(osgText)
USE_DOTOSGWRAPPER_LIBRARY(osgViewer)
USE_DOTOSGWRAPPER_LIBRARY(osgVolume)
USE_DOTOSGWRAPPER_LIBRARY(osgWidget)

USE_SERIALIZER_WRAPPER_LIBRARY(osg)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgUtil)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgGA)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgViewer)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgUI)
USE_SERIALIZER_WRAPPER_LIBRARY(osgAnimation)
USE_SERIALIZER_WRAPPER_LIBRARY(osgFX)
USE_SERIALIZER_WRAPPER_LIBRARY(osgManipulator)
USE_SERIALIZER_WRAPPER_LIBRARY(osgParticle)
USE_SERIALIZER_WRAPPER_LIBRARY(osgShadow)
USE_SERIALIZER_WRAPPER_LIBRARY(osgSim)
USE_SERIALIZER_WRAPPER_LIBRARY(osgTerrain)
USE_SERIALIZER_WRAPPER_LIBRARY(osgText)
USE_SERIALIZER_WRAPPER_LIBRARY(osgVolume)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgPresentation)

#define  LOG_TAG    "org.openscenegraph.osg.viewer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class OsgMsgNotifyHandler : public osg::NotifyHandler
{
public:
    void notify(osg::NotifySeverity severity, const char *message)
    {
        switch ( severity ) {

            case osg::DEBUG_FP:
                __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,"%s",message);
              break;
            case osg::DEBUG_INFO:
                __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",message);
              break;
            case osg::NOTICE:
            case osg::INFO:
                __android_log_print(ANDROID_LOG_INFO,LOG_TAG,"%s",message);
              break;
            case osg::WARN:
                __android_log_print(ANDROID_LOG_WARN,LOG_TAG,"%s",message);
              break;
            case osg::FATAL:
            case osg::ALWAYS:
                __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s",message);
              break;
            default:
                __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",message);
              break;
        }
    }
};


extern "C"
{

/*
 * osgViewer::ViewerBase
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_ViewerBase_nativeFrame(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::ViewerBase *v = reinterpret_cast<osgViewer::ViewerBase*> (cptr);
	if (v == NULL)
		return;

	v->frame();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_ViewerBase_nativeSetUpViewerAsEmbedded(JNIEnv *, jclass, jlong cptr, jint x, jint y, jint width, jint height)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    v->setUpViewerAsEmbeddedInWindow(x, y, width, height);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_ViewerBase_nativeSetViewport(JNIEnv *, jclass, jlong cptr, jint x, jint y, jint width, jint height)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    v->getEventQueue()->windowResize(x, y, width, height);
}


/*
 * osgViewer::Viewer
 */
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeCreateViewer(JNIEnv *, jclass)
{
    //osg::setNotifyLevel(osg::INFO);
    //osg::setNotifyHandler( new OsgMsgNotifyHandler() );
    osgViewer::Viewer *viewer = new osgViewer::Viewer();
    viewer->setSceneData(new osg::Group());
    OSG_NOTICE << "OSG Version:" << osgGetVersion() << std::endl;
    viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
    viewer->getCamera()->setCullMask(0xffffffff);
    viewer->getCamera()->setCullMaskLeft(0x00000001);
    viewer->getCamera()->setCullMaskRight(0x00000002);
    viewer->addEventHandler(new osgViewer::StatsHandler);
    // add the state manipulator
    viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    viewer->ref();
    return reinterpret_cast<jlong> (viewer);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetViewMatrix(JNIEnv* env, jclass, jlong cptr, jlong matrix_ptr)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(matrix_ptr);
    if(viewer != 0 && m!=0)
    {
    	osg::Matrixd mat = osg::Matrixd(*m);
    	osg::Matrixd pretransform = osg::Matrixd::scale(1,1,1);
    	osg::Matrixd inverse = osg::Matrixd::inverse(mat);
    	osg::Matrixd transformation = inverse*pretransform;
    	osg::Vec3d eye, center, up;
    	transformation.getLookAt(eye, center, up);


        osgGA::MultiTouchTrackballManipulator* view_interaction = reinterpret_cast<osgGA::MultiTouchTrackballManipulator*>(viewer->getCameraManipulator());
        view_interaction->setHomePosition(eye,center,up);
        viewer->home();
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetViewMatrixDistance(JNIEnv* env, jclass, jlong cptr, jlong matrix_ptr, jdouble distance)
{

    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer *>(cptr);

    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(matrix_ptr);
    if(viewer != 0 && m!=0)
    {
    	osg::Matrixd mat = osg::Matrixd(*m);
    	osg::Vec3d eye, center, up;
    	viewer->getCamera()->getViewMatrixAsLookAt(eye,center,up);

    	viewer->getCamera()->setViewMatrix(mat);
    	viewer->getCamera()->getViewMatrixAsLookAt(eye,center,up);

        osgGA::MultiTouchTrackballManipulator* view_interaction = reinterpret_cast<osgGA::MultiTouchTrackballManipulator*>(viewer->getCameraManipulator());
        view_interaction->setDistance(distance);
        view_interaction->setHomePosition(eye,center,up);
        viewer->home();
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetPerspectiveMatrix(JNIEnv* env, jclass, jlong cptr, jint width, jint height, jfloat fov)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (viewer == NULL)
        return;

    viewer->getCamera()->setProjectionMatrix(osg::Matrixd::perspective(osg::RadiansToDegrees(fov), jfloat(width)/jfloat(height), 0.1, 1500.0));
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetDefaultSettings(JNIEnv *, jclass, jlong cptr)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (viewer == NULL)
        return;
    viewer->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator());
    viewer->addEventHandler(new osgViewer::StatsHandler);
    // add the state manipulator
    viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    viewer->getCamera()->getOrCreateStateSet()->setGlobalDefaults();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeDisposeViewer(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;
	v->unref();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetSceneData(JNIEnv *, jclass, jlong viewer, jlong node)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (viewer);
	if (v == NULL)
		return;
	osg::Node *n = reinterpret_cast<osg::Node*>(node);
	if (n == NULL)
		return;
	v->setSceneData(n);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeFrame(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;

	v->frame();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetUpViewerAsEmbedded(JNIEnv *, jclass, jlong cptr, jint x, jint y, jint width, jint height)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    v->setUpViewerAsEmbeddedInWindow(x, y, width, height);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetViewport(JNIEnv *, jclass, jlong cptr, jint x, jint y, jint width, jint height)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    v->getEventQueue()->windowResize(x, y, width, height);
}


JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeMouseMotion(JNIEnv *, jclass, jlong cptr, jint x, jint y)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;
	v->getEventQueue()->mouseMotion(x, y);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeMousePassiveMotion(JNIEnv *, jclass, jlong cptr, jint x, jint y)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;
	v->getEventQueue()->mouseMotion(x, y);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeMouse(JNIEnv *, jclass, jlong cptr, jint button, jint state, jint x, jint y)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;
	if (state == 0)
		v->getEventQueue()->mouseButtonRelease(x, y, button);
	else
		v->getEventQueue()->mouseButtonPress(x, y,	button);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeKeyboard(JNIEnv *, jclass, jlong cptr, jint key, jint x, jint y, jboolean keydown)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;

	LOGE("KeyPressed: %d",key);
	if (keydown)
		v->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol) key);
	else
		v->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol) key);
}

//////////////////////////
// osg::OffScreenViewer //
//////////////////////////

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeCreatePBufferViewer(JNIEnv* env, jclass, jint width, jint height, jfloat fov)
{

    osg::setNotifyLevel(osg::WARN);
    //osg::setNotifyLevel(osg::INFO);
    osg::setNotifyHandler( new OsgMsgNotifyHandler() );
    osgViewer::Viewer *viewer = new osgViewer::Viewer();
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer->setSceneData(new osg::Group());
    //viewer->getCamera()->getOrCreateStateSet()->setGlobalDefaults();

    viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
    viewer->getCamera()->setCullMask(0xffffffff);
    viewer->getCamera()->setCullMaskLeft(0x00000001);
    viewer->getCamera()->setCullMaskRight(0x00000002);

    viewer->getCamera()->setClearColor(osg::Vec4f(0.25f, 0.25f, 0.25f, 1.0f));
    viewer->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    LOGI("non-windowed context created.");

    viewer->setCameraManipulator(0x0);
    viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    viewer->ref();
    return reinterpret_cast<jlong> (viewer);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetUpViewerAsEmbedded(JNIEnv *, jclass, jlong cptr, jint x, jint y, jint width, jint height)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    v->setUpViewerAsEmbeddedInWindow(x, y, width, height);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetViewport(JNIEnv *, jclass, jlong cptr, jint x, jint y, jint width, jint height)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    //v->getEventQueue()->windowResize(x, y, width, height);
    v->getCamera()->setViewport(x,y,width,height);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetView(JNIEnv* env, jclass, jlong cptr, jint width, jint height, jfloat fov)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (viewer == NULL)
        return;

    viewer->getCamera()->setProjectionMatrix(osg::Matrixd::perspective(osg::RadiansToDegrees(fov), jfloat(width)/jfloat(height), 0.1, 1500.0));
    osg::Matrixd lookat_matrix = osg::Matrixd::lookAt(osg::Vec3d(0.0, 0.0, -1.0), osg::Vec3d(0.0, 0.0, 1.0), osg::Vec3d(0.0, 1.0, 0.0));
    viewer->getCamera()->setViewMatrix(lookat_matrix);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetupCallback(JNIEnv* env, jclass, jlong cptr)
{
    osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (v == NULL)
        return;
    WindowCaptureCallback* _callback = new WindowCaptureCallback(WindowCaptureCallback::DOUBLE_PBO, WindowCaptureCallback::START_FRAME, GL_BACK);
    if(_callback == NULL)
    {
        LOGE("Error setting callback function.");
        return;
    }
    v->getCamera()->setFinalDrawCallback(_callback);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeGetFrame(JNIEnv* env, jclass, jlong cptr, jlong matrix_ptr)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(matrix_ptr);

    if(viewer==0)
    {
        LOGE("Viewer unavailable.");
        return 0;
    }
    if(m==0)
    {
        LOGE("Matrix unavailable.");
        return 0;
    }
    osg::Matrixd _mat = osg::Matrixd(*m);

    //render
    viewer->getCamera()->setViewMatrix(_mat);
    viewer->frame();
    viewer->frame();

    WindowCaptureCallback* _callback = reinterpret_cast<WindowCaptureCallback*>(viewer->getCamera()->getFinalDrawCallback());
    jlong result;
    osg::Image* data = NULL;
    if(_callback != NULL)
    {
        data = _callback->getImage(viewer->getCamera()->getGraphicsContext());
        while(data==NULL)
        {
            viewer->frame();
            viewer->frame();
            data = _callback->getImage(viewer->getCamera()->getGraphicsContext());
        }
        osg::Image* result_image = reinterpret_cast<osg::Image*>( data->clone ( osg::CopyOp::DEEP_COPY_ALL ) );
        result_image->ref();
        result = reinterpret_cast<jlong>( result_image );
    }
    else
    {
        LOGE("Callback unavailable.");
        result = 0;
    }
    return result;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeGetFrameImage(JNIEnv* env, jclass, jlong cptr)
{
	osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer *>(cptr);

	if(viewer==0)
	{
		LOGE("Viewer unavailable.");
		return 0;
	}
	WindowCaptureCallback* _callback = reinterpret_cast<WindowCaptureCallback*>(viewer->getCamera()->getFinalDrawCallback());
	jlong result;
	osg::Image* data = NULL;
	if(_callback != NULL)
	{
		data = _callback->getImage(viewer->getCamera()->getGraphicsContext());
		while(data==NULL)
		{
			viewer->frame();
			viewer->frame();
			data = _callback->getImage(viewer->getCamera()->getGraphicsContext());
		}
		osg::Image* result_image = reinterpret_cast<osg::Image*>( data->clone ( osg::CopyOp::DEEP_COPY_ALL ) );
		result_image->ref();
		result = reinterpret_cast<jlong>( result_image );
	}
	else
	{
		LOGE("Callback unavailable.");
		result = 0;
	}
	return result;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetViewMatrix(JNIEnv* env, jclass, jlong cptr, jlong matrix_ptr)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(matrix_ptr);
    if(viewer != 0 && m!=0)
    {
    	osg::Matrixd _mat = osg::Matrixd(*m);

    	viewer->getCamera()->setViewMatrix(_mat);
    	osg::Vec3 eye, center, up;
    	viewer->getCamera()->getViewMatrixAsLookAt(eye,center,up);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetRenderMatrix(JNIEnv* env, jclass, jlong cptr, jlong matrix_ptr)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(matrix_ptr);
    if(viewer != 0 && m!=0)
    {
    	osg::Matrixd _mat = osg::Matrixd(*m);
    	viewer->getCamera()->setViewMatrix(_mat);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeDisposeViewer(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;
	v->unref();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeSetSceneData(JNIEnv *, jclass, jlong viewer, jlong node)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (viewer);
	if (v == NULL)
		return;
	osg::Node *n = reinterpret_cast<osg::Node*>(node);
	if (n == NULL)
		return;
	v->setSceneData(n);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeFrame(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;

	v->frame();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeGetCamera(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return 0;
	osg::Camera *camera;
	camera = v->getCamera();
	camera->ref();
	return reinterpret_cast<jlong>(camera);
}

JNICALL jlong JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeRaycast(JNIEnv* env, jclass, jlong viewer_cptr, jlong camera_ptr, jlong vec2array_cptr)
{
	osg::Vec3Array* v3a = new osg::Vec3Array();
	v3a->ref();

	osgViewer::View* viewer = reinterpret_cast<osgViewer::View*>(viewer_cptr);
	osg::Camera* cam = reinterpret_cast<osg::Camera*>(camera_ptr);
	osg::Vec2Array* va = reinterpret_cast<osg::Vec2Array*>(vec2array_cptr);
	osgUtil::LineSegmentIntersector::Intersections intersections;
	for(uint i = 0; i < va->size(); i++)
	{
		if(i==0)
		{
			osg::Matrixf cmat = cam->getViewMatrix();
			float* data = (float*)(cmat.ptr());
			LOGI("[%f,%f,%f,%f]",data[0],data[1],data[2],data[3]);
			LOGI("[%f,%f,%f,%f]",data[4],data[5],data[6],data[7]);
			LOGI("[%f,%f,%f,%f]",data[8],data[9],data[10],data[11]);
			LOGI("[%f,%f,%f,%f]",data[12],data[13],data[14],data[15]);
	    	osg::Vec3 eye, center, up;
	    	cam->getViewMatrixAsLookAt(eye,center,up);
	    	LOGI("Eye: (%f,%f,%f) - Center: (%f,%f,%f) - Up: (%f,%f,%f)",eye.x(), eye.y(), eye.z(), center.x(), center.y(), center.z(), up.x(), up.y(), up.z());
		}
		viewer->computeIntersections(const_cast<const osg::Camera*>(cam), osgUtil::Intersector::WINDOW, va->at(i).x(), va->at(i).y(), intersections);
		RefVec3 *refRes = new RefVec3();
		if(intersections.empty() == false)
		{
			refRes->set(intersections.begin()->getWorldIntersectPoint().x(), intersections.begin()->getWorldIntersectPoint().y(), intersections.begin()->getWorldIntersectPoint().z());
		}
		else
		{
			refRes->set(DBL_MAX,DBL_MAX,DBL_MAX);
		}
		refRes->ref();
		v3a->push_back(*refRes);
	}

	return reinterpret_cast<jlong>(v3a);
}

JNICALL jlong JNICALL Java_org_openscenegraph_osg_viewer_OffScreenViewer_nativeIntersections(JNIEnv* env, jclass, jlong scene_cptr, jlong vec2array_cptr, jlong tvec_cptr, jlong rquat_cptr)
{
	osg::Vec3Array* v3a = new osg::Vec3Array();
	v3a->ref();

	osg::Node* scene = reinterpret_cast<osg::Node*>(scene_cptr);
	osg::Vec2Array* va = reinterpret_cast<osg::Vec2Array*>(vec2array_cptr);
	osg::BoundingSphere bs = scene->getBound();
	osgUtil::LineSegmentIntersector::Intersections intersections;
	for(uint i = 0; i < va->size(); i++)
	{

	}

	return reinterpret_cast<jlong>(v3a);
}




///////////////////////////////////////////////////////////////////////////////
//Touch Events
///////////////////////////////////////////////////////////////////////////////
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeTouchBegan(JNIEnv * /*env*/, jobject /*obj*/, jlong cptr, jint index, jint state, jfloat x, jfloat y)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return 0;
	osgGA::GUIEventAdapter* me = v->getEventQueue()->touchBegan(index,(osgGA::GUIEventAdapter::TouchPhase)state,x,y);
	me->ref();
	return reinterpret_cast<jlong>(me);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeTouchMoved(JNIEnv * /*env*/, jobject /*obj*/, jlong cptr, jint index, jint state, jfloat x, jfloat y)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return 0;
	osgGA::GUIEventAdapter* me = v->getEventQueue()->touchMoved(index,(osgGA::GUIEventAdapter::TouchPhase)state,x,y);
	me->ref();
	return reinterpret_cast<jlong>(me);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeTouchEnded(JNIEnv * /*env*/, jobject /*obj*/, jlong cptr, jint index, jint state, jfloat x, jfloat y, jint nTaps)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return 0;
	osgGA::GUIEventAdapter* me = v->getEventQueue()->touchEnded(index,(osgGA::GUIEventAdapter::TouchPhase)state,x,y,nTaps);
	me->ref();
	return reinterpret_cast<jlong>(me);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeAddTouchPoint(JNIEnv * /*env*/, jobject /*obj*/, jlong cptr, jlong eaCptr, jint index, jint state, jfloat x, jfloat y)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return;
	osgGA::GUIEventAdapter* me = reinterpret_cast<osgGA::GUIEventAdapter*> (eaCptr);
	if (me == NULL)
		return;
	me->addTouchPoint(index,(osgGA::GUIEventAdapter::TouchPhase)state,x,y);
}


JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeGetCamera(JNIEnv *, jclass, jlong cptr)
{
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	if (v == NULL)
		return 0;
	osg::Camera *camera;
	camera = v->getCamera();
	camera->ref();
	return reinterpret_cast<jlong>(camera);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_native_1setDisplaySettings(
		JNIEnv *env, jclass, jlong cptr, jlong cptrdisplay) {
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	osg::DisplaySettings *ds =
			reinterpret_cast<osg::DisplaySettings*> (cptrdisplay);
	v->setDisplaySettings(ds);
}
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_native_1getDisplaySettings(
		JNIEnv *env, jclass, jlong cptr) {
	osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
	osg::DisplaySettings *ds = v->getDisplaySettings();
	ds->ref();
	return reinterpret_cast<jlong> (ds);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_native_1setFusionDistance(
                JNIEnv *env, jclass, jlong cptr, jfloat fusionDistance) {
       osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
        if (v == NULL)
                return;

        v->setFusionDistance(osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE,
                        fusionDistance);

}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_viewer_Viewer_native_1getFusionDistance(
                JNIEnv *env, jclass, jlong cptr) {
        osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
        if (v == NULL)
                return 1.0f;

        return v->getFusionDistanceValue();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_native_1home(
                JNIEnv *env, jclass, jlong cptr) {
        osgViewer::Viewer *v = reinterpret_cast<osgViewer::Viewer*> (cptr);
        if (v == NULL)
                return;

        v->home();
}





////// DisplaySettings ////////////


JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1createDisplaySettings(JNIEnv *, jclass)
{
	osg::DisplaySettings* ds= new osg::DisplaySettings();
	ds->ref();
	return reinterpret_cast<jlong>(ds);		
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->unref();	
}
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setDisplayType(JNIEnv *, jclass,jlong cptr, jint type)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setDisplayType(osg::DisplaySettings::DisplayType(type));	
}	
JNIEXPORT jint  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getDisplayType(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getDisplayType();
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setStereoMode(JNIEnv *, jclass,jlong cptr, jint mode)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setStereoMode(osg::DisplaySettings::StereoMode(mode));	
}
JNIEXPORT jint  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getStereoMode(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getStereoMode();	
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setStereo(JNIEnv *, jclass,jlong cptr, jboolean value)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setStereo(value==JNI_TRUE);	
}
JNIEXPORT jboolean  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getStereo(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	if (ds->getStereo())return JNI_TRUE;	
	return JNI_FALSE;
}
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setEyeSeparation(JNIEnv *, jclass,jlong cptr, jfloat eyeseparation)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setEyeSeparation(eyeseparation);	
}
JNIEXPORT jfloat  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getEyeSeparation(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getEyeSeparation();	
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setScreenWidth(JNIEnv *, jclass,jlong cptr, jfloat width)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setScreenWidth(width);	
}
JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getScreenWidth(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getScreenWidth();	
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setScreenHeight(JNIEnv *, jclass,jlong cptr, jfloat height)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setScreenHeight(height);	
}
JNIEXPORT jfloat  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getScreenHeight(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getScreenHeight();	
}
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setScreenDistance(JNIEnv *, jclass,jlong cptr, jfloat distance)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setScreenDistance(distance);	
}
JNIEXPORT jfloat  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getScreenDistance(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getScreenDistance();	
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setDoubleBuffer(JNIEnv *, jclass,jlong cptr, jboolean value)
{
	osg::DisplaySettings* ds= reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setDoubleBuffer(value==JNI_TRUE);	
}	
JNIEXPORT jboolean  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getDoubleBuffer(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds= reinterpret_cast<osg::DisplaySettings*>(cptr);
	if(ds->getDoubleBuffer()) return JNI_TRUE;	
	return JNI_FALSE;
}	
JNIEXPORT void  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1setNumMultiSamples(JNIEnv *, jclass,jlong cptr, jint samples)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	ds->setNumMultiSamples(samples);	
}
JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getNumMultiSamples(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getNumMultiSamples();	
}


}
