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

#include <osgDB/ReadFile>

#include "JNIUtils.h"

#define  LOG_TAG    "org.openscenegraph.osg.db.JNIViewer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(ive)

USE_SERIALIZER_WRAPPER_LIBRARY(osg)
USE_SERIALIZER_WRAPPER_LIBRARY(osgAnimation)
USE_SERIALIZER_WRAPPER_LIBRARY(osgFX)
USE_SERIALIZER_WRAPPER_LIBRARY(osgManipulator)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgParticle) 	//B(black)SOD
USE_SERIALIZER_WRAPPER_LIBRARY(osgShadow)
//USE_SERIALIZER_WRAPPER_LIBRARY(osgSim)		//B(black)SOD
USE_SERIALIZER_WRAPPER_LIBRARY(osgTerrain)
USE_SERIALIZER_WRAPPER_LIBRARY(osgText)
USE_SERIALIZER_WRAPPER_LIBRARY(osgVolume)

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

}
