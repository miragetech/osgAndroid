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

#include "JNIUtils.h"
#include <stdlib.h>

#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Matrix>
#include <osg/Array>

#define  LOG_TAG    "org.openscenegraph.osg.util"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


extern "C"
{
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

}
