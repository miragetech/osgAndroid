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

#include <string>

#include <osgDB/Registry>

#include <osgViewer/Viewer>

#include <osg/Vec4>
#include <osg/AnimationPath>

#define  LOG_TAG    "org.openscenegraph.osgjni.OSGConfiguration"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static std::string jstring2string(JNIEnv *env, jstring jstr) {
    const char *str;
    str = env->GetStringUTFChars(jstr, NULL);
    if (str == NULL) {
        return ""; /* OutOfMemoryError already thrown */
    }
    std::string ret(str);
    env->ReleaseStringUTFChars(jstr, str);
    return ret;
}

extern "C"
{


JNIEXPORT void JNICALL Java_org_openscenegraph_osgjni_OSGConfiguration_nativeConfigureViewer(JNIEnv *env, jclass, jlong lviewer)
{
	osgViewer::Viewer *viewer = reinterpret_cast<osgViewer::Viewer*>(lviewer);
	viewer->getCamera()->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osgjni_OSGConfiguration_nativeConfigureScene(JNIEnv *env, jclass, jlong lnode)
{
	osg::Node *node = reinterpret_cast<osg::Node*>(lnode);
	osg::ref_ptr<osg::AnimationPath> am = new osg::AnimationPath();
	am->insert(0, osg::AnimationPath::ControlPoint(osg::Vec3d(0,0,0),osg::Quat(0,0,0,1),osg::Vec3d(0.5,0.5,0.5)));
	am->insert(5, osg::AnimationPath::ControlPoint(osg::Vec3d(0,0,0),osg::Quat(0,0,1.4,1),osg::Vec3d(3.5,3.5,3.5)));

	osg::ref_ptr<osg::AnimationPathCallback> amcb = new osg::AnimationPathCallback();
	amcb->setAnimationPath(am.get());
	am->setLoopMode(osg::AnimationPath::SWING);
	amcb->setPause(false);

	node->setUpdateCallback(amcb.get());

	return lnode;
}

}
