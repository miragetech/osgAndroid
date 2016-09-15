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

#include <osg/io_utils>
#include <osg/ImageUtils>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "JNIUtils.h"
#include <stdlib.h>
#include <sstream>

#define  LOG_TAG    "org.openscenegraph.osg.db"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


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



JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_db_ReadFile_nativeReadNodeFile(JNIEnv *env, jclass, jstring filename)
{
	osg::Node *node = osgDB::readNodeFile(jstring2string(env,filename));
	if(node == 0) 
	{
		LOGE("Error loading scene");
		return 0;
	}
	node->ref();
	return reinterpret_cast<jlong>(node);

}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_db_ReadFile_nativeReadImageFile(JNIEnv *env, jclass, jstring filename)
{
	osg::Image* node = osgDB::readImageFile(jstring2string(env,filename));
	if(node == 0)
	{
		LOGE("Error loading image");
		return 0;
	}
	node->ref();
	return reinterpret_cast<jlong>(node);

}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_db_WriteFile_nativeWriteNodeFile(JNIEnv* env, jclass, jstring filename, jlong node_cptr)
{
	osg::Node* node = reinterpret_cast<osg::Node*>(node_cptr);
	if(node!=0)
	{
		return osgDB::writeNodeFile(*node, jstring2string(env,filename));
	}
	return false;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_db_WriteFile_nativeWriteImageFile(JNIEnv* env, jclass, jstring filename, jlong image_cptr)
{
	osg::Image* image = reinterpret_cast<osg::Image*>(image_cptr);
	if(image!=NULL)
	{
		return osgDB::writeImageFile(*image, jstring2string(env,filename));
	}
	return false;
}

}
