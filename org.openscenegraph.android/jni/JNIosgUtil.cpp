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

#include "JNIUtils.h"
#include <stdlib.h>
#include <sstream>

#include <osg/Camera>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Matrix>
#include <osg/Array>
#include <osg/io_utils>
#include <osg/PrimitiveSet>

#include <osgViewer/Viewer>

#include <osgUtil/IntersectionVisitor>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/LineSegmentIntersector>

#include "GLES2ShaderGenVisitor.h"

#define  LOG_TAG    "org.openscenegraph.osg.util"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

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

extern "C"
{

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_util_GLES2ShaderConverter_nativeApply(JNIEnv *env, jclass, jlong viewerCptr, jlong nodeCptr)
{
	osgViewer::Viewer* viewer = reinterpret_cast<osgViewer::Viewer*>(viewerCptr);
	if(viewer == NULL)
		return;
	osg::Node* node = reinterpret_cast<osg::Node*>(nodeCptr);
	if(node == NULL)
	{
		return;
	}
	osgUtil::GLES2ShaderGenVisitor gles2Visitor;
	gles2Visitor.setRootStateSet(viewer->getCamera()->getOrCreateStateSet());
	node->accept(gles2Visitor);
}
/**
 * GeometryUtils
 */
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_util_GeometryUtils_nativeCreateScreenQuad(JNIEnv *env, jclass,
								jint x, jint y, jint w, jint h, jboolean isBackGround)
{
	osg::Camera *hud = new osg::Camera();	
	float z=-1.0f;

	osg::Vec4 color((float)(rand()%10)/10.0,1,(float)(rand()%10)/10.0,1);


    hud->setProjectionMatrix(osg::Matrix::ortho2D(0,w,0,h));
    hud->setViewport(x,y,w,h);
    hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    hud->setViewMatrix(osg::Matrix::identity());
    hud->setInheritanceMask(hud->getInheritanceMask() & ~osg::CullSettings::COMPUTE_NEAR_FAR_MODE);
    hud->setAllowEventFocus(false);
    hud->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    if(isBackGround == JNI_TRUE)
    {
    	hud->setRenderOrder(osg::Camera::NESTED_RENDER);
    	//hud->getOrCreateStateSet()->setRenderBinDetails(1,"DepthSortedBin");
    	hud->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    	hud->getOrCreateStateSet()->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::ON);
    }

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> plane =  new osg::Geometry;

	osg::Vec3 corner(0,0,z);
	osg::Vec3 widthVec(w, 0.0, z);
	osg::Vec3 heightVec(0.0, h, z);
	float l = 0.0f;
	float b = 0.0f;
	float r = 1.0;
	float t = 1.0;

	osg::Vec3Array* coords = new osg::Vec3Array(4);
	(*coords)[0] = corner+heightVec;
	(*coords)[1] = corner;
	(*coords)[2] = corner+widthVec;
	(*coords)[3] = corner+widthVec+heightVec;
	plane->setVertexArray(coords);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[0].set(l,t);
	(*tcoords)[1].set(l,b);
	(*tcoords)[2].set(r,b);
	(*tcoords)[3].set(r,t);
	plane->setTexCoordArray(0,tcoords);

	osg::Vec4Array* colours = new osg::Vec4Array(1);
	(*colours)[0] = color;
	plane->setColorArray(colours);
	plane->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec3Array* normals = new osg::Vec3Array(1);
	(*normals)[0] = widthVec^heightVec;
	(*normals)[0].normalize();
	plane->setNormalArray(normals);
	plane->setNormalBinding(osg::Geometry::BIND_OVERALL);

	plane->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
        
    geode->addDrawable(plane.get());
    hud->addChild(geode.get());
		
	hud->ref();
	return reinterpret_cast<jlong>(hud);

}


/*
 * IntersectorGroup
 */
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_util_IntersectorGroup_nativeCreateIntersectorGroup(JNIEnv* env, jclass)
{
	osgUtil::IntersectorGroup* g = new osgUtil::IntersectorGroup();
	g->ref();
	return reinterpret_cast<jlong>(g);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_util_IntersectorGroup_nativeDispose(JNIEnv* env, jclass, jlong cptr)
{
	osgUtil::IntersectorGroup *g = reinterpret_cast<osgUtil::IntersectorGroup *>(cptr);
    if(g!=0)
        g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_util_IntersectorGroup_nativeAddIntersector(JNIEnv* env, jclass, jlong cptr, jfloat x, jfloat y)
{
	osgUtil::IntersectorGroup *g = reinterpret_cast<osgUtil::IntersectorGroup *>(cptr);
	if(g!=0)
	{
		g->addIntersector( new osgUtil::LineSegmentIntersector( osgUtil::Intersector::WINDOW, x,y ) );
	}
}

/*
 * IntersectionVisitor
 */
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_util_IntersectionVisitor_nativeCreateIntersectionVisitor(JNIEnv* env, jclass, jlong base_ptr)
{
	osgUtil::IntersectionVisitor* iv;
	if(base_ptr==0)
		iv = new osgUtil::IntersectionVisitor();
	else
		iv = reinterpret_cast<osgUtil::IntersectionVisitor*>(base_ptr);
	iv->ref();
	return reinterpret_cast<jlong>(iv);
}


}
