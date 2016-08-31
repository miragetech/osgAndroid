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

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_util_GeometryUtils_nativeTextureFromPose(JNIEnv *env, jclass, jstring in_filepath, jstring out_filepath, jlong Cg_ptr, jlong trmat_ptr, jlong R_ptr, jlong img_ptr)
{
	osg::Group* _node = reinterpret_cast<osg::Group*>(osgDB::readNodeFile(jstring2string(env,in_filepath)));
	if(_node == 0)
	{
		LOGE("Error loading scene");
		return 0;
	}
	if(_node->getNumChildren()<1)
	{
		LOGE("No geodes within scene.");
		return 0;
	}
	osg::Geode* _geode = reinterpret_cast<osg::Geode*>(_node->getChild(_node->getNumChildren()-1));
	if(_geode==NULL)
	{
		LOGE("Invalid Geode.");
		return 0;
	}
	if(_geode->getNumDrawables()<1)
	{
		LOGE("Geode has no drawables");
		return 0;
	}
	osg::Geometry* _geometry = reinterpret_cast<osg::Geometry*>(_geode->getDrawable(_geode->getNumDrawables()-1));
	if(_geometry==NULL)
	{
		LOGE("Invalid Geometry.");
		return 0;
	}

	int colored_point = 0;
	bool use_texcoord = false, new_color_array = false;

	osg::Vec3Array* varray = (osg::Vec3Array*)(_geometry->getVertexArray());
	if(varray==NULL)
	{
		LOGE("Invalid VertexArray");
		return 0;
	}

	osg::Vec4Array* carray = (osg::Vec4Array*)(_geometry->getColorArray());
	if(carray==NULL)
	{
		carray = new osg::Vec4Array();
		carray->ref();
		new_color_array = true;
	}
	if(carray->getNumElements() < varray->getNumElements())
	{
		carray->resizeArray(varray->getNumElements());
	}
	osg::Vec2Array* tarray = (osg::Vec2Array*)(_geometry->getTexCoordArray(0));
	if(tarray!=NULL)
	{
		use_texcoord = true;
		if(tarray->getNumElements() < varray->getNumElements())
		{
			tarray->resizeArray(varray->getNumElements());
		}
	}

	osg::Image* img = reinterpret_cast<osg::Image *>(img_ptr);
	if(img==NULL)
	{
		LOGE("Invalid Image");
		return 0;
	}

	RefVec3* Cg = reinterpret_cast<RefVec3 *>(Cg_ptr);
	RefVec3* R = reinterpret_cast<RefVec3 *>(R_ptr);
	osg::RefMatrixf* trmat = reinterpret_cast<osg::RefMatrixf* >(trmat_ptr);
	osg::Matrixf rotmat;
	rotmat.set((*trmat)(0,0), (*trmat)(0,1), (*trmat)(0,2), 0, (*trmat)(1,0), (*trmat)(1,1), (*trmat)(1,2), 0, (*trmat)(2,0), (*trmat)(2,1), (*trmat)(2,2), 0, 0, 0, 0, 1);

	//std::stringstream sstrm;
	//sstrm << "Mat: " << rotmat(0,0) << ", " << rotmat(0,1) << ", " << rotmat(0,2) << std::endl;
	//sstrm << rotmat(1,0) << ", " << rotmat(1,1) << ", " << rotmat(1,2) << std::endl;
	//sstrm << rotmat(2,0) << ", " << rotmat(2,1) << ", " << rotmat(2,2) << std::endl;
	//sstrm << "Cg: " << Cg->x() << ", " << Cg->y() << ", " << Cg->z() << std::endl;
	//__android_log_write(ANDROID_LOG_INFO,LOG_TAG,sstrm.str().c_str());

	osg::Vec3 M, D;
	for(unsigned int i = 0; i < varray->getNumElements(); i++)
	{
		M = varray->at(i);
		D = rotmat * (M-(*Cg));
		if(D.z()>0)
		{
			float u = ((R->z()*(D.x()/D.z()))+(R->x()/2.0f))/R->x();
			float v = ((R->z()*(D.y()/D.z()))+(R->y()/2.0f))/R->y();
			if( (u>0) && (u<1) && (v>0) && (v<1) )
			{
				osg::Vec2 tex_coord(u,v);
				if(use_texcoord)
				{
					tarray->at((unsigned int)i) = tex_coord;
				}
				osg::Vec4 _color = Interpolate(tex_coord, img);
				carray->at((unsigned int)i) = _color;
				colored_point++;
			}
		}
	}

	if(new_color_array)
	{
		_geometry->setColorArray(carray);
		_geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	}
	if(use_texcoord)
	{
		_geometry->setTexCoordArray(0,tarray);
	}

	osgDB::writeNodeFile(*_node, jstring2string(env,out_filepath));

	return reinterpret_cast<jint>(colored_point);
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_util_GeometryUtils_nativeTextureFromPoseImgfile(JNIEnv *env, jclass, jstring in_filepath, jstring out_filepath, jlong Cg_ptr, jlong trmat_ptr, jlong R_ptr, jstring img_filename)
{
	osg::Group* _node = reinterpret_cast<osg::Group*>(osgDB::readNodeFile(jstring2string(env,in_filepath)));
	if(_node == 0)
	{
		LOGE("Error loading scene");
		return 0;
	}
	if(_node->getNumChildren()<1)
	{
		LOGE("No geodes within scene.");
		return 0;
	}
	osg::Geode* _geode = reinterpret_cast<osg::Geode*>(_node->getChild(_node->getNumChildren()-1));
	if(_geode==NULL)
	{
		LOGE("Invalid Geode.");
		return 0;
	}
	if(_geode->getNumDrawables()<1)
	{
		LOGE("Geode has no drawables");
		return 0;
	}
	osg::Geometry* _geometry = reinterpret_cast<osg::Geometry*>(_geode->getDrawable(_geode->getNumDrawables()-1));
	if(_geometry==NULL)
	{
		LOGE("Invalid Geometry.");
		return 0;
	}

	int colored_point = 0;
	bool use_texcoord = false, new_color_array = false;

	osg::Vec3Array* varray = (osg::Vec3Array*)(_geometry->getVertexArray());
	if(varray==NULL)
	{
		LOGE("Invalid VertexArray");
		return 0;
	}

	osg::Vec4Array* carray = (osg::Vec4Array*)(_geometry->getColorArray());
	if(carray==NULL)
	{
		carray = new osg::Vec4Array();
		carray->ref();
		new_color_array = true;
	}
	if(carray->getNumElements() < varray->getNumElements())
	{
		carray->resizeArray(varray->getNumElements());
	}
	osg::Vec2Array* tarray = (osg::Vec2Array*)(_geometry->getTexCoordArray(0));
	if(tarray!=NULL)
	{
		use_texcoord = true;
		if(tarray->getNumElements() < varray->getNumElements())
		{
			tarray->resizeArray(varray->getNumElements());
		}
	}

	osg::Image* img = osgDB::readImageFile(jstring2string(env,img_filename));
	if(img==NULL)
	{
		LOGE("Invalid Image");
		return 0;
	}

	RefVec3* Cg = reinterpret_cast<RefVec3 *>(Cg_ptr);
	RefVec3* R = reinterpret_cast<RefVec3 *>(R_ptr);
	osg::RefMatrixf* trmat = reinterpret_cast<osg::RefMatrixf* >(trmat_ptr);
	osg::Matrixf rotmat;
	rotmat.set((*trmat)(0,0), (*trmat)(0,1), (*trmat)(0,2), 0, (*trmat)(1,0), (*trmat)(1,1), (*trmat)(1,2), 0, (*trmat)(2,0), (*trmat)(2,1), (*trmat)(2,2), 0, 0, 0, 0, 1);

	//std::stringstream sstrm;
	//sstrm << "Mat: " << rotmat(0,0) << ", " << rotmat(0,1) << ", " << rotmat(0,2) << std::endl;
	//sstrm << rotmat(1,0) << ", " << rotmat(1,1) << ", " << rotmat(1,2) << std::endl;
	//sstrm << rotmat(2,0) << ", " << rotmat(2,1) << ", " << rotmat(2,2) << std::endl;
	//sstrm << "Cg: " << Cg->x() << ", " << Cg->y() << ", " << Cg->z() << std::endl;
	//__android_log_write(ANDROID_LOG_INFO,LOG_TAG,sstrm.str().c_str());

	osg::Vec3 M, D;
	for(unsigned int i = 0; i < varray->getNumElements(); i++)
	{
		M = varray->at(i);
		D = rotmat * (M-(*Cg));
		if(D.z()>0)
		{
			float u = ((R->z()*(D.x()/D.z()))+(R->x()/2.0f))/R->x();
			float v = ((R->z()*(D.y()/D.z()))+(R->y()/2.0f))/R->y();
			if( (u>0) && (u<1) && (v>0) && (v<1) )
			{
				osg::Vec2 tex_coord(u,v);
				if(use_texcoord)
				{
					tarray->at((unsigned int)i) = tex_coord;
				}
				osg::Vec4 _color = Interpolate(tex_coord, img);
				carray->at((unsigned int)i) = _color;
				colored_point++;
			}
		}
	}

	if(new_color_array)
	{
		_geometry->setColorArray(carray);
		_geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	}
	if(use_texcoord)
	{
		_geometry->setTexCoordArray(0,tarray);
	}

	osgDB::writeNodeFile(*_node, jstring2string(env,out_filepath));
	return reinterpret_cast<jint>(colored_point);
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
