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

#include <jni.h>
#include <android/log.h>

#include <osg/Version>
#include <osg/DisplaySettings>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>


#define  LOG_TAG    "org.openscenegraph.osg.viewer.JNIViewer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class OsgMsgNotifyHandler : public osg::NotifyHandler
{
public:
    void notify(osg::NotifySeverity severity, const char *message)
    {
        switch ( severity ) {

            case osg::DEBUG_FP:
                __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,message);
              break;
            case osg::DEBUG_INFO:
                __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,message);
              break;
            case osg::NOTICE:
            case osg::INFO:
                __android_log_print(ANDROID_LOG_INFO,LOG_TAG,message);
              break;
            case osg::WARN:
                __android_log_print(ANDROID_LOG_WARN,LOG_TAG,message);
              break;
            case osg::FATAL:
            case osg::ALWAYS:
                __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,message);
              break;
            default:
                __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,message);
              break;
        }
    }
};


extern "C"
{

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeCreateViewer(JNIEnv *, jclass)
{
    osg::setNotifyLevel(osg::INFO);
    osg::setNotifyHandler( new OsgMsgNotifyHandler() );
    osgViewer::Viewer *viewer = new osgViewer::Viewer();
    viewer->setSceneData(new osg::Group());
    OSG_NOTICE << "OSG Version:" << osgGetVersion() << std::endl;
    viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
    viewer->getCamera()->setCullMask(0xffffffff);
    viewer->getCamera()->setCullMaskLeft(0x00000001);
    viewer->getCamera()->setCullMaskRight(0x00000002);
    viewer->addEventHandler(new osgViewer::StatsHandler);
    //viewer->getCamera()->setClearColor(osg::Vec4(0.0,0.0,0.0,0.0));
    // add the state manipulator
    viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    viewer->ref();
    return reinterpret_cast<jlong> (viewer);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_viewer_Viewer_nativeSetDefaultSettings(JNIEnv *, jclass, jlong cptr)
{
    osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer*> (cptr);
    if (viewer == NULL)
        return;
    viewer->setCameraManipulator(new osgGA::TrackballManipulator());
    viewer->addEventHandler(new osgViewer::StatsHandler);
    //viewer->getCamera()->setClearColor(osg::Vec4(0.0,0.0,0.0,0.0));
    // add the state manipulator
    viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
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
JNIEXPORT jint  JNICALL Java_org_openscenegraph_osg_viewer_DisplaySettings_native_1getNumMultiSamples(JNIEnv *, jclass,jlong cptr)
{
	osg::DisplaySettings* ds=  reinterpret_cast<osg::DisplaySettings*>(cptr);
	return ds->getNumMultiSamples();	
}

}
