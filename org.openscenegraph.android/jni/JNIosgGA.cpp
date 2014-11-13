/* @License
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2014 Rafael Gait�n, Mirage Technologies S.L.     |
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

#include <osgGA/GUIEventAdapter>

#include "GLES2ShaderGenVisitor.h"

#define  LOG_TAG    "org.openscenegraph.osg.ga"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


extern "C"
{

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_ga_GUIEventAdapter_nativeDispose(JNIEnv *env, jclass, jlong cptr)
{
	osgGA::GUIEventAdapter* ea = reinterpret_cast<osgGA::GUIEventAdapter*>(cptr);
	if(ea == NULL)
		return;
	ea->unref();
}


}
