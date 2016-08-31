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

#include <osg/Node>
#include <osg/Point>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Matrixf>
#include <osg/AnimationPath>
#include <osg/io_utils>
#include <osg/Texture2D>
#include <osg/PrimitiveSet>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Array>
#include <osg/LineWidth>

#define  LOG_TAG    "org.openscenegraph.osg.db.JNIOSGCore"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
{

/**
 * osg::Node
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::Node *node = reinterpret_cast<osg::Node *>(cptr);
	//if(node!=0)
	if((node != 0) && (node->referenceCount()<2))
		node->unref();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetUpdateCallback(JNIEnv *, jclass, jlong cptr,
		jlong lucb)
{
	osg::Node *node = reinterpret_cast<osg::Node *>(cptr);
	osg::NodeCallback *ucb = reinterpret_cast<osg::NodeCallback *>(lucb);
	if(node != 0)
		node->setUpdateCallback(ucb);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetRenderBinDetails(JNIEnv *env, jclass, jlong cptr,
		jint order, jstring bin)
{
	osg::Node *node = reinterpret_cast<osg::Node *>(cptr);
	if(node != 0)
	{
		node->getOrCreateStateSet()->setRenderBinDetails(order,jstring2string(env,bin));
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetTexture2D(JNIEnv *env, jclass, jlong cptr, jlong cptrimage)
{
	osg::Node *node = reinterpret_cast<osg::Node *>(cptr);
	osg::Image *image = reinterpret_cast<osg::Image *>(cptrimage);
	if(node != 0 && image != 0)
	{
		LOGE("Setting TEXTURE");
		osg::Texture2D *tex = new osg::Texture2D(image);
		node->getOrCreateStateSet()->setTextureAttribute(0,tex,
				osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetTexture(JNIEnv*, jclass, jlong cptr, jlong texture_cptr, jint tex_unit)
{
	osg::Node *node = reinterpret_cast<osg::Node *>(cptr);
	osg::Texture *texture = reinterpret_cast<osg::Texture*>(texture_cptr);
	if( node!=NULL && texture!=NULL)
	{
		node->getOrCreateStateSet()->setTextureAttribute((int)tex_unit, texture, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetMode(JNIEnv *env, jclass, jlong cptr,
		jint mode, jint value)
{
	osg::Node *node = reinterpret_cast<osg::Node *>(cptr);
	if(node != 0)
	{
		node->getOrCreateStateSet()->setMode( static_cast<GLenum>(mode),
				static_cast<unsigned int>(value));
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetPointSize(JNIEnv* env, jclass, jlong cptr, jfloat pt_size)
{
	osg::Node* node = reinterpret_cast<osg::Node*>(cptr);
	osg::Point* point = new osg::Point();
	point->setSize((float)pt_size);
	if(node != NULL)
		node->getOrCreateStateSet()->setAttributeAndModes(point, osg::StateAttribute::ON);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Node_nativeSetLineWidth(JNIEnv* env, jclass, jlong cptr, jfloat line_size)
{
	osg::Node* node = reinterpret_cast<osg::Node*>(cptr);
	osg::LineWidth* lw = new osg::LineWidth();
	lw->setWidth((float)line_size);
	if(node != NULL)
		node->getOrCreateStateSet()->setAttributeAndModes(lw, osg::StateAttribute::ON);
}


/**
 * osg::Group
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Group_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
    osg::Group *g = reinterpret_cast<osg::Group *>(cptr);
    //if(g!=0)
    if((g != 0) && (g->referenceCount()<2))
        g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Group_nativeCreateGroup(JNIEnv *, jclass, jlong cptr)
{
    osg::Group *g = new osg::Group();
    g->ref();
    return reinterpret_cast<jlong>(g);
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Group_nativeAddChild(JNIEnv *, jclass, jlong cptr,
                                                                             jlong cptrnode)
{
    osg::Group *g = reinterpret_cast<osg::Group *>(cptr);
    osg::Node *n = reinterpret_cast<osg::Node *>(cptrnode);
    if(g != 0 && n != 0)
        return g->addChild(n)?JNI_TRUE:JNI_FALSE;
    return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Group_nativeRemoveChild(JNIEnv *, jclass, jlong cptr,
                                                                             jlong cptrnode)
{
    osg::Group *g = reinterpret_cast<osg::Group *>(cptr);
    osg::Node *n = reinterpret_cast<osg::Node *>(cptrnode);
    if(g != 0 && n != 0)
        return g->removeChild(n)?JNI_TRUE:JNI_FALSE;
    return JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_Group_nativeGetNumChildren(JNIEnv *, jclass, jlong cptr)
{
    osg::Group *g = reinterpret_cast<osg::Group *>(cptr);
    if(g != 0)
        return g->getNumChildren();
    return 0;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Group_nativeGetChild(JNIEnv* env, jclass, jlong cptr, jint i)
{
    osg::Group *g = reinterpret_cast<osg::Group *>(cptr);
    if(g != 0)
    {
    	osg::Node* node = g->getChild((unsigned int)i);
        return reinterpret_cast<jlong>(node);
    }
    return 0l;
}

/*
 * osg::Geode
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geode_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::Geode *g = reinterpret_cast<osg::Geode *>(cptr);
	if(g!=NULL)
		g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geode_nativeCreateGeode(JNIEnv *, jclass)
{
    osg::Geode *g = new osg::Geode();
    g->ref();
    return reinterpret_cast<jlong>(g);
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Geode_nativeAddDrawable(JNIEnv *, jclass, jlong cptr, jlong cptr_drawable)
{
	bool result = false;
	osg::Geode *g = reinterpret_cast<osg::Geode*>(cptr);
	osg::Drawable* n = reinterpret_cast<osg::Drawable*>(cptr_drawable);
	if(g!=NULL)
	{
		result = g->addDrawable(n);
	}
	return result?JNI_TRUE:JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Geode_nativeRemoveDrawable(JNIEnv* env, jclass, jlong cptr, jlong cptr_drawable)
{
	bool result = false;
	osg::Geode* g = reinterpret_cast<osg::Geode*>(cptr);
	osg::Drawable* d = reinterpret_cast<osg::Drawable*>(cptr_drawable);
	if(g!=NULL)
	{
		result = g->removeDrawable(d);
	}
	return result?JNI_TRUE:JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Geode_nativeRemoveDrawables(JNIEnv* env, jclass, jlong cptr, jint i, jint m)
{
	bool result = false;
	osg::Geode* g = reinterpret_cast<osg::Geode*>(cptr);
	if(g!=NULL)
	{
		result = g->removeDrawables((unsigned int)i, (unsigned int)m);
	}
	return result?JNI_TRUE:JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Geode_nativeReplaceDrawable(JNIEnv* env, jclass, jlong cptr, jlong cptr_drawable_old, jlong cptr_drawable_new)
{
	bool result = false;
	osg::Geode* g = reinterpret_cast<osg::Geode*>(cptr);
	osg::Drawable* d_old = reinterpret_cast<osg::Drawable*>(cptr_drawable_old);
	osg::Drawable* d_new = reinterpret_cast<osg::Drawable*>(cptr_drawable_new);
	if(g!=NULL)
	{
		result = g->replaceDrawable(d_old,d_new);
	}
	return result?JNI_TRUE:JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Geode_nativeSetDrawable(JNIEnv* env, jclass, jlong cptr, jint i, jlong cptr_drawable)
{
	bool result = false;
	osg::Geode* g = reinterpret_cast<osg::Geode*>(cptr);
	osg::Drawable* d = reinterpret_cast<osg::Drawable*>(cptr_drawable);
	if(g!=NULL)
	{
		result = g->setDrawable((unsigned int)i, d);
	}
	return result?JNI_TRUE:JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_Geode_nativeGetNumDrawables(JNIEnv* env, jclass, jlong cptr)
{
	int result = 0;
	osg::Geode* g = reinterpret_cast<osg::Geode*>(cptr);
	if(g!=NULL)
	{
		result = g->getNumDrawables();
	}
	return reinterpret_cast<jint>(result);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geode_nativeGetDrawable(JNIEnv* env, jclass, jlong cptr, jint i)
{
	jlong result = 0;
	osg::Geode* g = reinterpret_cast<osg::Geode*>(cptr);
	if(g!=NULL)
	{
		osg::Drawable* d = g->getDrawable((int)i);
		result = reinterpret_cast<jlong>(d);
	}
	return result;
}

/*
 * osg::Geometry
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry *>(cptr);
	if(g!=NULL)
		g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeCreateGeometry(JNIEnv *, jclass)
{
    osg::Geometry *g = new osg::Geometry();
    g->ref();
    return reinterpret_cast<jlong>(g);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetVertexArray(JNIEnv* env, jclass, jlong cptr, jobjectArray inputBuffer)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);

	int array_n = env->GetArrayLength(inputBuffer);
	//printf("Array size: %i\n", array_n);
	osg::Vec3Array* _array = new osg::Vec3Array();

	jobject objArray;
	jsize subarray_n;
	float* data;
	for(int i = 0; i < array_n; i++)
	{
		objArray = env->GetObjectArrayElement(inputBuffer,i);
		jfloatArray* cdata = reinterpret_cast<jfloatArray*>(&objArray);
		subarray_n = env->GetArrayLength(*cdata);
		if( subarray_n>2 )
		{
			data = env->GetFloatArrayElements(*cdata, 0);
			_array->push_back(osg::Vec3(data[0] , data[1], data[2]));
			env->ReleaseFloatArrayElements(*cdata, data, 0);
		}
	}

	if(g!=NULL)
	{
		g->setVertexArray(_array);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetVertexArrayNative(JNIEnv* env, jclass, jlong cptr, jlong array_cptr)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(array_cptr);

	if( (g!=NULL) && (a!=NULL) )
	{
		g->setVertexArray(a);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeGetVertexArray(JNIEnv* env, jclass, jlong cptr)
{
	jlong result = 0;
	osg::Geometry* g = reinterpret_cast<osg::Geometry*>(cptr);
	if(g!=NULL)
	{
		osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(g->getVertexArray());
		result = reinterpret_cast<jlong>(a);
	}
	return result;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetNormalArray(JNIEnv* env, jclass, jlong cptr, jobjectArray inputBuffer)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);

	int array_n = env->GetArrayLength(inputBuffer);
	osg::Vec3Array* _array = new osg::Vec3Array();

	jobject objArray;
	jsize subarray_n;
	float* data;
	for(int i = 0; i < array_n; i++)
	{
		objArray = env->GetObjectArrayElement(inputBuffer,i);
		jfloatArray* cdata = reinterpret_cast<jfloatArray*>(&objArray);
		subarray_n = env->GetArrayLength(*cdata);
		if( subarray_n>2 )
		{
			data = env->GetFloatArrayElements(*cdata, 0);
			_array->push_back(osg::Vec3(data[0] , data[1], data[2]));
			env->ReleaseFloatArrayElements(*cdata, data, 0);
		}
	}

	if(g!=NULL)
	{
		g->setNormalArray(_array);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetNormalArrayNative(JNIEnv* env, jclass, jlong cptr, jlong array_cptr)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(array_cptr);

	if( (g!=NULL) && (a!=NULL) )
	{
		g->setNormalArray(a);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeGetNormalArray(JNIEnv* env, jclass, jlong cptr)
{
	jlong result = 0;
	osg::Geometry* g = reinterpret_cast<osg::Geometry*>(cptr);
	if(g!=NULL)
	{
		osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(g->getNormalArray());
		result = reinterpret_cast<jlong>(a);
	}
	return result;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetColorArray(JNIEnv* env, jclass, jlong cptr, jobjectArray inputBuffer)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);

	int array_n = env->GetArrayLength(inputBuffer);

	//LOGI("Colour Array size: %i\n", array_n);
	osg::Vec4Array* _array = new osg::Vec4Array();

	jobject objArray;
	jsize subarray_n;
	float* data;
	for(int i = 0; i < array_n; i++)
	{
		objArray = env->GetObjectArrayElement(inputBuffer,i);
		jfloatArray* cdata = reinterpret_cast<jfloatArray*>(&objArray);
		subarray_n = env->GetArrayLength(*cdata);
		//LOGI("subarray size at %i: %i\n", i, subarray_n);
		if( subarray_n>3 )
		{
			data = env->GetFloatArrayElements(*cdata, 0);
			_array->push_back(osg::Vec4(data[0], data[1], data[2], data[3]));
			env->ReleaseFloatArrayElements(*cdata, data, 0);
		}
	}

	if(g!=NULL)
	{
		g->setColorArray(_array);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetColorArrayNative(JNIEnv* env, jclass, jlong cptr, jlong array_cptr)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(array_cptr);

	if( (g!=NULL) && (a!=NULL) )
	{
		g->setColorArray(a);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeGetColorArray(JNIEnv* env, jclass, jlong cptr)
{
	jlong result = 0;
	osg::Geometry* g = reinterpret_cast<osg::Geometry*>(cptr);
	if(g!=NULL)
	{
		osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(g->getColorArray());
		result = reinterpret_cast<jlong>(a);
	}
	return result;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetTexCoordArray(JNIEnv* env, jclass, jlong cptr, jobjectArray inputBuffer, jint tex_unit)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);

	int array_n = env->GetArrayLength(inputBuffer);
	osg::Vec2Array* _array = new osg::Vec2Array();

	jobject objArray;
	jsize subarray_n;
	float* data;
	for(int i = 0; i < array_n; i++)
	{
		objArray = env->GetObjectArrayElement(inputBuffer,i);
		jfloatArray* cdata = reinterpret_cast<jfloatArray*>(&objArray);
		subarray_n = env->GetArrayLength(*cdata);
		if( subarray_n>1 )
		{
			data = env->GetFloatArrayElements(*cdata, 0);
			_array->push_back(osg::Vec2(data[0], data[1]));
			env->ReleaseFloatArrayElements(*cdata, data, 0);
		}
	}

	if(g!=NULL)
	{
		g->setTexCoordArray((int)tex_unit,_array);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetTexCoordArrayNative(JNIEnv* env, jclass, jlong cptr, jlong array_cptr, jint tex_unit)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(array_cptr);

	if( (g!=NULL) && (a!=NULL) )
	{
		g->setTexCoordArray((int)tex_unit,a);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeGetTexCoordArray(JNIEnv* env, jclass, jlong cptr, jint tex_unit)
{
	jlong result = 0;
	osg::Geometry* g = reinterpret_cast<osg::Geometry*>(cptr);
	if(g!=NULL)
	{
		osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(g->getTexCoordArray((int)tex_unit));
		result = reinterpret_cast<jlong>(a);
	}
	return result;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetNormalBinding(JNIEnv *, jclass, jlong cptr, jbyte binding)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	g->setNormalBinding(osg::Geometry::AttributeBinding(binding));
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetColorBinding(JNIEnv *, jclass, jlong cptr, jbyte binding)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	g->setColorBinding(osg::Geometry::AttributeBinding(binding));
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeAddPrimitiveSet(JNIEnv *, jclass, jlong cptr, jlong set_cptr)
{
	osg::Geometry *g = reinterpret_cast<osg::Geometry*>(cptr);
	osg::PrimitiveSet *n = reinterpret_cast<osg::PrimitiveSet*>(set_cptr);
	if(g!=NULL)
	{
        return g->addPrimitiveSet(n)?JNI_TRUE:JNI_FALSE;
	}
	return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Geometry_nativeSetPrimitiveSetList(JNIEnv* env, jclass, jlong cptr, jlong list_cptr)
{
	osg::Geometry* g = reinterpret_cast<osg::Geometry*>(cptr);
	osg::Geometry::PrimitiveSetList* n = reinterpret_cast<osg::Geometry::PrimitiveSetList*>(list_cptr);
	if(g!=NULL)
	{
		g->setPrimitiveSetList(*n);
	}
}

/*
 * osg::Drawable
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Drawable_nativeDispose(JNIEnv* env, jclass, jlong cptr)
{
	osg::Drawable* g = reinterpret_cast<osg::Drawable*>(cptr);
	if(g!=NULL)
		g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Drawable_nativeAsDrawable(JNIEnv *, jclass, jlong cptr)
{
	osg::Drawable *g = reinterpret_cast<osg::Drawable *>(cptr);
	return reinterpret_cast<jlong>(g);
}

/*
 * osg::PrimitiveSet
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_PrimitiveSet_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::PrimitiveSet *g = reinterpret_cast<osg::PrimitiveSet *>(cptr);
	if(g!=NULL)
		g->unref();
}

/*
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_PrimitiveSet_nativeCreatePrimitiveSet(JNIEnv *, jclass, jint type, jint mode)
{
	osg::PrimitiveSet *g = NULL;
	switch(type)
	{
	case 0:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::PrimitiveType, (int)mode);
		break;
	}
	case 1:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::DrawArraysPrimitiveType, (int)mode);
		break;
	}
	case 2:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::DrawArrayLengthsPrimitiveType, (int)mode);
		break;
	}
	case 3:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::DrawElementsUBytePrimitiveType, (int)mode);
		break;
	}
	case 4:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::DrawElementsUShortPrimitiveType, (int)mode);
		break;
	}
	case 5:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::DrawElementsUIntPrimitiveType, (int)mode);
		break;
	}
	default:
	{
		g = new osg::PrimitiveSet(osg::PrimitiveSet::PrimitiveType, (int)mode);
		break;
	}
	}

	if(g!=NULL)
		g->ref();
    return reinterpret_cast<jlong>(g);
}
*/

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_PrimitiveSet_nativeAsPrimitiveSet(JNIEnv *, jclass, jlong cptr)
{
	osg::PrimitiveSet *g = reinterpret_cast<osg::PrimitiveSet *>(cptr);
	return reinterpret_cast<jlong>(g);
}

/*
 * osg::PrimitiveSetList
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_PrimitiveSetList_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::Geometry::PrimitiveSetList* g = reinterpret_cast<osg::Geometry::PrimitiveSetList*>(cptr);
	//if(g!=NULL)
	//	g->unref();
}


JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_PrimitiveSetList_nativeCreatePrimitiveSetList(JNIEnv* env, jclass)
{
	osg::Geometry::PrimitiveSetList* l = new osg::Geometry::PrimitiveSetList();
	//l->ref();
	return reinterpret_cast<jlong>(l);
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_PrimitiveSetList_nativeSize(JNIEnv* env, jclass, jlong cptr)
{
	osg::Geometry::PrimitiveSetList* g = reinterpret_cast<osg::Geometry::PrimitiveSetList*>(cptr);
	if(g!=NULL)
		return (jint)(g->size());
	return 0;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_PrimitiveSetList_nativePushBackPrimitiveSet(JNIEnv* env, jclass, jlong cptr, jlong object_cptr)
{
	osg::Geometry::PrimitiveSetList* l = reinterpret_cast<osg::Geometry::PrimitiveSetList*>(cptr);
	osg::PrimitiveSet* obj = reinterpret_cast<osg::PrimitiveSet*>(object_cptr);
	if((l!=NULL)&&(obj!=NULL))
	{
		l->push_back(obj);
	}
}

/*
 * osg::DrawArrays
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_DrawArrays_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::DrawArrays *g = reinterpret_cast<osg::DrawArrays *>(cptr);
	if(g!=NULL)
		g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_DrawArrays_nativeCreateDrawArrays(JNIEnv *, jclass, jint mode, jint first, jint count)
{
	osg::DrawArrays *g = new osg::DrawArrays((int)mode, first, count);
	g->ref();
	return reinterpret_cast<jlong>(g);
}

/*
 * osg::DrawElementsUInt
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_DrawElementsUInt_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::DrawElementsUInt *g = reinterpret_cast<osg::DrawElementsUInt *>(cptr);
	if(g!=NULL)
		g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_DrawElementsUInt_nativeCreateDrawElementsUInt(JNIEnv* env, jclass, jint mode, jint count)
{
	osg::DrawElementsUInt *g = new osg::DrawElementsUInt((int)mode, count);
	g->ref();
	return reinterpret_cast<jlong>(g);
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_DrawElementsUInt_nativeSize(JNIEnv* env, jclass, jlong cptr)
{
	osg::DrawElementsUInt* g = reinterpret_cast<osg::DrawElementsUInt*>(cptr);
	if(g!=NULL)
		return (jint)(g->getNumIndices());
	return 0;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_DrawElementsUInt_nativePushBackUInt(JNIEnv* env, jclass, jlong cptr, jint value)
{
	osg::DrawElementsUInt* l = reinterpret_cast<osg::DrawElementsUInt*>(cptr);
	if((l!=NULL))
	{
		l->push_back(value);
	}
}

/*
 * osg::Texture
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Texture_nativeDispose(JNIEnv*, jclass, jlong cptr)
{
	osg::Texture* t = reinterpret_cast<osg::Texture*>(cptr);
	if(t!=NULL)
		t->unref();
}

//JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Texture_nativeCreateTexture(JNIEnv*, jclass)
//{
//	osg::Texture* t = new osg::Texture();
//	t->ref();
//	return reinterpret_cast<jlong>(t);
//}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Texture_nativeSetImage(JNIEnv*, jclass, jlong cptr, jint face, jlong image_cptr)
{
	osg::Texture2D* t = reinterpret_cast<osg::Texture2D*>(cptr);
	osg::Image* i = reinterpret_cast<osg::Image*>(image_cptr);
	if((t!=NULL)&&(i!=NULL))
		t->setImage((int)face, i);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Texture_nativeGetImage(JNIEnv*, jclass, jlong cptr, jint face)
{
	jlong result = 0;
	osg::Texture2D* t = reinterpret_cast<osg::Texture2D*>(cptr);
	if(t!=NULL)
	{
		osg::Image* i = t->getImage((int)face);
		result = reinterpret_cast<jlong>(i);
	}
	return result;
}

/*
 * osg::Texture2D
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Texture2D_nativeDispose(JNIEnv*, jclass, jlong cptr)
{
	osg::Texture2D* t = reinterpret_cast<osg::Texture2D*>(cptr);
	if(t!=NULL)
		t->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Texture2D_nativeCreateTexture2D(JNIEnv*, jclass)
{
	osg::Texture2D* t = new osg::Texture2D();
	t->ref();
	return reinterpret_cast<jlong>(t);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Texture2D_nativeSetImage(JNIEnv*, jclass, jlong cptr, jlong image_cptr)
{
	osg::Texture2D* t = reinterpret_cast<osg::Texture2D*>(cptr);
	osg::Image* i = reinterpret_cast<osg::Image*>(image_cptr);
	if((t!=NULL)&&(i!=NULL))
		t->setImage(i);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Texture2D_nativeGetImage(JNIEnv*, jclass, jlong cptr)
{
	jlong result = 0;
	osg::Texture2D* t = reinterpret_cast<osg::Texture2D*>(cptr);
	if(t!=NULL)
	{
		osg::Image* i = t->getImage();
		result = reinterpret_cast<jlong>(i);
	}
	return result;
}


/*
 * osg::Object
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Object_nativeDispose(JNIEnv*, jclass, jlong cptr)
{
	osg::Object *o = reinterpret_cast<osg::Object*>(cptr);
	if(o!=NULL)
		o->unref();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Object_nativeSetDataVariance(JNIEnv*, jclass, jlong cptr, jint dv)
{
	osg::Object *o = reinterpret_cast<osg::Object*>(cptr);
	if(o!=NULL)
		o->setDataVariance(osg::Object::DataVariance(dv));
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_Object_nativeGetDataVariance(JNIEnv*, jclass, jlong cptr)
{
	jint result = 0;
	osg::Object *o = reinterpret_cast<osg::Object*>(cptr);
	if(o!=NULL)
		result = (int)(o->getDataVariance());
	return result;
}


/*
 * osg::Array
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Array_nativeDispose(JNIEnv* env, jclass, jlong cptr)
{
	osg::Array* g = reinterpret_cast<osg::Array*>(cptr);
	if(g!=NULL)
		g->unref();
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_Array_nativeSize(JNIEnv* env, jclass, jlong cptr)
{
	osg::Array* g = reinterpret_cast<osg::Array*>(cptr);
	if(g!=NULL)
		return (jint)(g->getNumElements());
	return 0;
}

/*
 * osg::Vec2Array
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativeDispose(JNIEnv* env, jclass, jlong cptr)
{
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(cptr);
	if(a!=NULL)
		a->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativeCreateVec2Array(JNIEnv *, jclass)
{
	osg::Vec2Array *a = new osg::Vec2Array();
	a->ref();
	return reinterpret_cast<jlong>(a);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativePushBackVec2(JNIEnv* env, jclass, jlong cptr, jlong vec_cptr)
{
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(cptr);
	RefVec2 *v = reinterpret_cast<RefVec2*>(vec_cptr);
	//osg::Vec2* original = reinterpret_cast<osg::Vec2*>(v->ptr());
	if(a!=NULL)
	{
		//a->push_back(*original);
		a->push_back(*v);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativePushBackFloatArray(JNIEnv* env, jclass, jlong cptr, jfloatArray array_cptr)
{
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(cptr);
	float* data;
	jsize size_a = env->GetArrayLength(array_cptr);
	if( size_a == 2 )
	{
		data = env->GetFloatArrayElements(array_cptr, 0);
		if(a!=NULL)
		{
			a->push_back(osg::Vec2(data[0], data[1]));
		}
		env->ReleaseFloatArrayElements(array_cptr, data, 0);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativePopBack(JNIEnv* env, jclass, jlong cptr)
{
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(cptr);
	if((a!=NULL) && (a->getNumElements()>0))
	{
		//unsigned int pos = a->getNumElements()-1;
		osg::Vec2 v = a->back();
		a->pop_back();
		RefVec2 *refRes = new RefVec2();
		refRes->set(v.x(), v.y());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativeGet(JNIEnv* env, jclass, jlong cptr, jint i)
{
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(cptr);
	if((a!=NULL) && (a->getNumElements()>0))
	{
		osg::Vec2 v = a->at((unsigned int)i);
		RefVec2 *refRes = new RefVec2();
		refRes->set(v.x(), v.y());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Vec2Array_nativeSet(JNIEnv* env, jclass, jlong cptr, jint i, jlong vec_cptr)
{
	osg::Vec2Array* a = reinterpret_cast<osg::Vec2Array*>(cptr);
	RefVec2 *v = reinterpret_cast<RefVec2*>(vec_cptr);
	osg::Vec2* original = reinterpret_cast<osg::Vec2*>(v->ptr());
	if((a!=NULL) && (a->getNumElements()>i))
	{
		a->at((unsigned int)i) = *original;
		return JNI_TRUE;
	}
	return JNI_FALSE;
}

/*
 * osg::Vec3Array
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativeDispose(JNIEnv* env, jclass, jlong cptr)
{
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(cptr);
	if(a!=NULL)
		a->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativeCreateVec3Array(JNIEnv *, jclass)
{
	osg::Vec3Array *a = new osg::Vec3Array();
	a->ref();
	return reinterpret_cast<jlong>(a);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativePushBackVec3(JNIEnv* env, jclass, jlong cptr, jlong vec_cptr)
{
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(cptr);
	RefVec3 *v = reinterpret_cast<RefVec3*>(vec_cptr);
	//osg::Vec3* original = reinterpret_cast<osg::Vec3*>(v->ptr());
	if(a!=NULL)
	{
		//a->push_back(*original);
		a->push_back(*v);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativePushBackFloatArray(JNIEnv* env, jclass, jlong cptr, jfloatArray array_cptr)
{
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(cptr);
	float* data;
	jsize size_a = env->GetArrayLength(array_cptr);
	if( size_a == 3 )
	{
		data = env->GetFloatArrayElements(array_cptr, 0);
		if(a!=NULL)
		{
			a->push_back(osg::Vec3(data[0], data[1], data[2]));
		}
		env->ReleaseFloatArrayElements(array_cptr, data, 0);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativePopBack(JNIEnv* env, jclass, jlong cptr)
{
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(cptr);
	if((a!=NULL) && (a->getNumElements()>0))
	{
		//unsigned int pos = a->getNumElements()-1;
		osg::Vec3 v = a->back();
		a->pop_back();
		RefVec3 *refRes = new RefVec3();
		refRes->set(v.x(), v.y(), v.z());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativeGet(JNIEnv* env, jclass, jlong cptr, jint i)
{
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(cptr);
	if((a!=NULL) && (a->getNumElements()>0))
	{
		osg::Vec3 v = a->at((unsigned int)i);
		RefVec3 *refRes = new RefVec3();
		refRes->set(v.x(), v.y(), v.z());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Vec3Array_nativeSet(JNIEnv* env, jclass, jlong cptr, jint i, jlong vec_cptr)
{
	osg::Vec3Array* a = reinterpret_cast<osg::Vec3Array*>(cptr);
	RefVec3 *v = reinterpret_cast<RefVec3*>(vec_cptr);
	osg::Vec3* original = reinterpret_cast<osg::Vec3*>(v->ptr());
	if((a!=NULL) && (a->getNumElements()>i))
	{
		a->at((unsigned int)i) = *original;
		return JNI_TRUE;
	}
	return JNI_FALSE;
}

/*
 * osg::Vec4Array
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativeDispose(JNIEnv* env, jclass, jlong cptr)
{
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(cptr);
	if(a!=NULL)
		a->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativeCreateVec4Array(JNIEnv *, jclass)
{
	osg::Vec4Array *a = new osg::Vec4Array();
	a->ref();
	return reinterpret_cast<jlong>(a);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativePushBackVec4(JNIEnv* env, jclass, jlong cptr, jlong vec_cptr)
{
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(cptr);
	RefVec4 *v = reinterpret_cast<RefVec4*>(vec_cptr);
	//osg::Vec4* original = reinterpret_cast<osg::Vec4*>(v->ptr());
	if(a!=NULL)
	{
		//a->push_back(*original);
		a->push_back(*v);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativePushBackFloatArray(JNIEnv* env, jclass, jlong cptr, jfloatArray array_cptr)
{
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(cptr);
	float* data;
	jsize size_a = env->GetArrayLength(array_cptr);
	if( size_a == 4 )
	{
		data = env->GetFloatArrayElements(array_cptr, 0);
		if(a!=NULL)
		{
			a->push_back(osg::Vec4(data[0], data[1], data[2], data[3]));
		}
		env->ReleaseFloatArrayElements(array_cptr, data, 0);
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativePopBack(JNIEnv* env, jclass, jlong cptr)
{
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(cptr);
	if((a!=NULL) && (a->getNumElements()>0))
	{
		//unsigned int pos = a->getNumElements()-1;
		osg::Vec4 v = a->back();
		a->pop_back();
		RefVec4 *refRes = new RefVec4();
		refRes->set(v.x(), v.y(), v.z(), v.a());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativeGet(JNIEnv* env, jclass, jlong cptr, jint i)
{
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(cptr);
	if((a!=NULL) && (a->getNumElements()>0))
	{
		osg::Vec4 v = a->at((unsigned int)i);
		RefVec4 *refRes = new RefVec4();
		refRes->set(v.x(), v.y(), v.z(), v.a());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Vec4Array_nativeSet(JNIEnv* env, jclass, jlong cptr, jint i, jlong vec_cptr)
{
	osg::Vec4Array* a = reinterpret_cast<osg::Vec4Array*>(cptr);
	RefVec4 *v = reinterpret_cast<RefVec4*>(vec_cptr);
	osg::Vec4* original = reinterpret_cast<osg::Vec4*>(v->ptr());
	if((a!=NULL) && (a->getNumElements()>i))
	{
		a->at((unsigned int)i) = *original;
		return JNI_TRUE;
	}
	return JNI_FALSE;
}

/**
 * osg::MatrixTransform
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_MatrixTransform_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
    osg::MatrixTransform *g = reinterpret_cast<osg::MatrixTransform *>(cptr);
    g->getOrCreateStateSet()->setMode(GL_NORMALIZE,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    if(g != 0)
        g->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_MatrixTransform_nativeCreateMatrixTransform(JNIEnv *, jclass, jlong cptr)
{
    osg::MatrixTransform *g = new osg::MatrixTransform();
    g->ref();
    return reinterpret_cast<jlong>(g);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_MatrixTransform_nativeSetMatrix(JNIEnv *, jclass, jlong cptr,
                                                                             jlong cptrmatrix)
{
    osg::MatrixTransform *g = reinterpret_cast<osg::MatrixTransform *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptrmatrix);
    if(g != 0 && m != 0)
    {
        g->setMatrix(*m);
    }
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_MatrixTransform_nativeGetMatrix(JNIEnv *, jclass, jlong cptr)
{
    osg::MatrixTransform *g = reinterpret_cast<osg::MatrixTransform *>(cptr);
    if(g != 0)
    {
        osg::Matrix mat = g->getMatrix();
        osg::RefMatrixf *matrix = new osg::RefMatrixf(mat);
        matrix->ref();
        return reinterpret_cast<jlong>(matrix);
    }
    return 0l;
}


/**
 * osg::Camera
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
	if(camera != 0)
		camera->unref();
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetClearColor(JNIEnv *, jclass, jlong cptr,
																			jfloat r, jfloat g, jfloat b, jfloat a)
{
	osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
	if(camera != 0)
	{
		camera->setClearColor(osg::Vec4(r,g,b,a));
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetClearColorVec(JNIEnv* env, jclass, jlong cptr, jlong vec_cptr)
{
	osg::Camera* g = reinterpret_cast<osg::Camera*>(cptr);
	RefVec4* v = reinterpret_cast<RefVec4*>(vec_cptr);
	if((g!=NULL)&&(v!=NULL))
		g->setClearColor(*v);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Camera_nativeGetClearColorVec(JNIEnv* env, jclass, jlong cptr)
{
    osg::Camera *g = reinterpret_cast<osg::Camera *>(cptr);
    if(g != 0)
    {
        osg::Vec4 v = g->getClearColor();
		RefVec4 *refRes = new RefVec4();
		refRes->set(v.x(), v.y(), v.z(), v.a());
		refRes->ref();
        return reinterpret_cast<jlong>(refRes);
    }
    return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Camera_nativeGetViewMatrix(JNIEnv* env, jclass, jlong cptr)
{
	osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    if(camera != 0)
    {
        osg::Matrix mat = camera->getViewMatrix();
        osg::RefMatrixf *matrix = new osg::RefMatrixf(mat);
        matrix->ref();
        return reinterpret_cast<jlong>(matrix);
    }
    return 0l;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetViewMatrixAsLookAt(JNIEnv *, jclass, jlong cptr,
                                                                            jlong eye, jlong center, jlong up)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    RefVec3 *e = reinterpret_cast<RefVec3 *>(eye);
    RefVec3 *c = reinterpret_cast<RefVec3 *>(center);
    RefVec3 *u = reinterpret_cast<RefVec3 *>(up);
    if(camera != 0)
    {
        camera->setViewMatrixAsLookAt(*e,*c,*u);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetViewMatrix(JNIEnv *, jclass, jlong cptr,
                                                                            jlong cptrmatrix)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptrmatrix);
    if(camera != 0 && m!=0)
    {
        camera->setViewMatrix(*m);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetProjectionMatrix(JNIEnv *, jclass, jlong cptr,
                                                                            jlong cptrmatrix)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptrmatrix);
    if(camera != 0 && m!=0)
    {
        camera->setProjectionMatrix(*m);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetProjectionMatrixAsOrtho2D(JNIEnv *, jclass, jlong cptr,
                                                                            jdouble left, jdouble right, jdouble bottom, jdouble top)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    if(camera != 0)
    {
        camera->setProjectionMatrixAsOrtho2D(left,right,bottom,top);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetProjectionMatrixAsPerspective(JNIEnv *, jclass, jlong cptr,
                                                                            jdouble fovy, jdouble aspectRatio, jdouble zNear, jdouble zFar)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    if(camera != 0)
    {
        camera->setProjectionMatrixAsPerspective(fovy,aspectRatio,zNear,zFar);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetCullLeftMask(JNIEnv *env, jclass, jlong cptr,
		jboolean value)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    if(camera != 0)
    {
        if(value == JNI_TRUE)
        {
        	camera->setNodeMask(0x00000001);
        }
        else
        {
        	camera->setNodeMask(0xffffffff);
        }
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Camera_nativeSetCullRightMask(JNIEnv *env, jclass, jlong cptr,
		jboolean value)
{
    osg::Camera *camera = reinterpret_cast<osg::Camera *>(cptr);
    if(camera != 0)
    {
        if(value == JNI_TRUE)
        {
        	camera->setNodeMask(0x00000002);
        }
        else
        {
        	camera->setNodeMask(0xffffffff);
        }
    }
}

/**
 * osg::Vec2
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
		v->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeCreateVec2(JNIEnv *, jclass)
{
	RefVec2 *v = new RefVec2();
	v->ref();
	return reinterpret_cast<jlong>(v);
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeX(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		return v->x();
	}
	return 0.0f;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeY(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		return v->y();
	}
	return 0.0f;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeSet(JNIEnv *, jclass, jlong cptr,
		jfloat x, jfloat y)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		v->set(x,y);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeSetX(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		v->x() = value;
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeSetY(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		v->y() = value;
	}
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeLength(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		return v->length();
	}
	return 0.0f;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeLength2(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		return v->length2();
	}
	return 0.0f;
}

JNIEXPORT long JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeDiv(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		osg::Vec2 res = *v/value;
		RefVec2 *refRes = new RefVec2();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeDotProduct(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	RefVec2 *rhs = reinterpret_cast<RefVec2 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		return (*v)*(*rhs);
	}
	return 0;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeEscalarProduct(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		osg::Vec2 res = (*v)*value;
		RefVec2 *refRes = new RefVec2();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeSum(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	RefVec2 *rhs = reinterpret_cast<RefVec2 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec2 res = (*v)+(*rhs);
		RefVec2 *refRes = new RefVec2();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeSub(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	RefVec2 *rhs = reinterpret_cast<RefVec2 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec2 res = (*v)-(*rhs);
		RefVec2 *refRes = new RefVec2();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeNormalize(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		return v->normalize();
	}
	return 0;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec2_nativeNegation(JNIEnv *, jclass, jlong cptr)
{
	RefVec2 *v = reinterpret_cast<RefVec2 *>(cptr);
	if(v != 0)
	{
		osg::Vec2 res = -(*v);
		RefVec2 *refRes = new RefVec2();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

/**
 * osg::Vec3
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
		v->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeCreateVec3(JNIEnv *, jclass)
{
	RefVec3 *v = new RefVec3();
	v->ref();
	return reinterpret_cast<jlong>(v);
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeX(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		return v->x();
	}
	return 0.0f;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeY(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		return v->y();
	}
	return 0.0f;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeZ(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		return v->z();
	}
	return 0.0f;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeSet(JNIEnv *, jclass, jlong cptr,
		jfloat x, jfloat y, jfloat z)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		v->set(x,y,z);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeSetX(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		v->x() = value;
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeSetY(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		v->y() = value;
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeSetZ(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		v->z() = value;
	}
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeLength(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		return v->length();
	}
	return 0.0f;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeLength2(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		return v->length2();
	}
	return 0.0f;
}

JNIEXPORT long JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeDiv(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		osg::Vec3 res = *v/value;
		RefVec3 *refRes = new RefVec3();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeDotProduct(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	RefVec3 *rhs = reinterpret_cast<RefVec3 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		return (*v)*(*rhs);
	}
	return 0;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeCrossProduct(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	RefVec3 *rhs = reinterpret_cast<RefVec3 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec3 res = (*v)^(*rhs);
		RefVec3 *refRes = new RefVec3();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeEscalarProduct(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		osg::Vec3 res = (*v)*value;
		RefVec3 *refRes = new RefVec3();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeSum(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	RefVec3 *rhs = reinterpret_cast<RefVec3 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec3 res = (*v)+(*rhs);
		RefVec3 *refRes = new RefVec3();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeSub(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	RefVec3 *rhs = reinterpret_cast<RefVec3 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec3 res = (*v)-(*rhs);
		RefVec3 *refRes = new RefVec3();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeNormalize(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		return v->normalize();
	}
	return 0;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec3_nativeNegation(JNIEnv *, jclass, jlong cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(cptr);
	if(v != 0)
	{
		osg::Vec3 res = -(*v);
		RefVec3 *refRes = new RefVec3();
		refRes->set(res);
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

/**
 * osg::Vec4
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
		v->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeCreateVec4(JNIEnv *, jclass)
{
	RefVec4 *v = new RefVec4();
	v->ref();
	return reinterpret_cast<jlong>(v);
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeX(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->x();
	}
	return 0.0f;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeY(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->y();
	}
	return 0.0f;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeZ(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->z();
	}
	return 0.0f;
}

JNIEXPORT float JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeW(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->w();
	}
	return 0.0f;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSet(JNIEnv *, jclass, jlong cptr,
		jfloat x, jfloat y, jfloat z, jfloat w)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		v->set(x,y,z,w);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSetX(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		v->x() = value;
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSetY(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		v->y() = value;
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSetZ(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		v->z() = value;
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSetW(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		v->w() = value;
	}
}


JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeLength(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->length();
	}
	return 0.0f;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeLength2(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->length2();
	}
	return 0.0f;
}

JNIEXPORT long JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeDiv(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		osg::Vec4 res = *v/value;
		RefVec4 *refRes = new RefVec4();
		refRes->set(res.x(),res.y(),res.z(),res.w());
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeEscalarProduct(JNIEnv *, jclass, jlong cptr,
		jfloat value)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		osg::Vec4 res = (*v)*value;
		RefVec4 *refRes = new RefVec4();
		refRes->set(res.x(),res.y(),res.z(),res.w());
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSum(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	RefVec4 *rhs = reinterpret_cast<RefVec4 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec4 res = (*v)+(*rhs);
		RefVec4 *refRes = new RefVec4();
		refRes->set(res.x(),res.y(),res.z(),res.w());
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeSub(JNIEnv *, jclass, jlong cptr,
		jlong lrhs)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	RefVec4 *rhs = reinterpret_cast<RefVec4 *>(lrhs);
	if(v != 0 && rhs !=0)
	{
		osg::Vec4 res = (*v)-(*rhs);
		RefVec4 *refRes = new RefVec4();
		refRes->set(res.x(),res.y(),res.z(),res.w());
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeNormalize(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		return v->normalize();
	}
	return 0;
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Vec4_nativeNegation(JNIEnv *, jclass, jlong cptr)
{
	RefVec4 *v = reinterpret_cast<RefVec4 *>(cptr);
	if(v != 0)
	{
		osg::Vec4 res = -(*v);
		RefVec4 *refRes = new RefVec4();
		refRes->set(res.x(),res.y(),res.z(),res.w());
		refRes->ref();
		return reinterpret_cast<jlong>(refRes);
	}
	return 0l;
}

/**
 * osg::Quat
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Quat_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		q->unref();
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Quat_nativeCreateQuat(JNIEnv *, jclass)
{
	RefQuat *q = new RefQuat();
	q->set(0,0,0,1);
	q->ref();
	return reinterpret_cast<jlong>(q);
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Quat_nativeSet(JNIEnv *, jclass, jlong cptr,
		jfloat q1, jfloat q2, jfloat q3, jfloat q4)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		q->set(q1,q2,q3,q4);
}
JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Quat_nativeX(JNIEnv *, jclass, jlong cptr)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		return q->x();
	return 0.0f;
}
JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Quat_nativeY(JNIEnv *, jclass, jlong cptr)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		return q->y();
	return 0.0f;
}
JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Quat_nativeZ(JNIEnv *, jclass, jlong cptr)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		return q->z();
	return 0.0f;
}
JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Quat_nativeW(JNIEnv *, jclass, jlong cptr)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		return q->w();
	return 1.0f;
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Quat_nativeMakeRotateAngleVec3(JNIEnv *, jclass, jlong cptr,
		jfloat angle, jfloat x, jfloat y, jfloat z)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	if(q != 0)
		q->makeRotate(angle, x, y, z);
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Quat_nativeMakeRotateVec3Vec3(JNIEnv *, jclass, jlong cptr,
		jlong lvec1, jlong lvec2)
{
	RefQuat *q = reinterpret_cast<RefQuat *>(cptr);
	RefVec3 *vec1 = reinterpret_cast<RefVec3 *>(lvec1);
	RefVec3 *vec2 = reinterpret_cast<RefVec3 *>(lvec2);
	if(q != 0 && vec1 != 0 && vec2 != 0)
		q->makeRotate(*vec1,*vec2);
}

/**
 * osg::Matrix
 */
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeCreateMatrix(JNIEnv *, jclass)
{
    osg::RefMatrixf *m = new osg::RefMatrixf();
    m->ref();
    return reinterpret_cast<jlong>(m);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        m->unref();
    }
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeInverse(JNIEnv* env, jclass, jlong cptr)
{
	osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
	osg::RefMatrixf *result = NULL;
	if(m!=0)
	{
		osg::Matrixf inv = osg::Matrixf::inverse(*m);
		result = new osg::RefMatrixf(inv);
		result->ref();
	}
	return reinterpret_cast<jlong>(result);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeTranspose(JNIEnv* env, jclass, jlong cptr)
{
	osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
	osg::RefMatrixf *result = NULL;
	if(m!=0)
	{
		osg::Matrixf _mat = osg::Matrixf(*m);
		float* data = (float*)(_mat.ptr());
		//LOGI("[%f,%f,%f,%f]",data[0],data[1],data[2],data[3]);
		//LOGI("[%f,%f,%f,%f]",data[4],data[5],data[6],data[7]);
		//LOGI("[%f,%f,%f,%f]",data[8],data[9],data[10],data[11]);
		//LOGI("[%f,%f,%f,%f]",data[12],data[13],data[14],data[15]);
		osg::Matrixf* _t = new osg::Matrixf();
		_t->set(data[0], data[4], data[8], data[12], data[1], data[5], data[9], data[13], data[2], data[6], data[10], data[14], data[3], data[7], data[11], data[15]);
		result = new osg::RefMatrixf(*_t);
		result->ref();
	}
	return reinterpret_cast<jlong>(result);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeScale(JNIEnv* env, jclass, jlong vec_cptr)
{
	RefVec3 *v = reinterpret_cast<RefVec3 *>(vec_cptr);
	osg::RefMatrixf *result = NULL;
	if(v!=0)
	{
		osg::Matrixf mScale = osg::Matrixf::scale(v->x(), v->y(), v->z());
		result = new osg::RefMatrixf(mScale);
		result->ref();
	}
	return reinterpret_cast<jlong>(result);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMult(JNIEnv* env, jclass, jlong cptr, jlong m1_ptr, jlong m2_ptr)
{
	osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
	osg::RefMatrixf *m1 = reinterpret_cast<osg::RefMatrixf *>(m1_ptr);
	osg::RefMatrixf *m2 = reinterpret_cast<osg::RefMatrixf *>(m2_ptr);
	if((m!=NULL)&&(m1!=NULL)&&(m2!=NULL))
	{
		m->mult(*m1, *m2);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeSet(JNIEnv *, jclass, jlong cptr,
        jfloat a00, jfloat a01, jfloat a02, jfloat a03, jfloat a10, jfloat a11, jfloat a12, jfloat a13,
        jfloat a20, jfloat a21, jfloat a22, jfloat a23, jfloat a30, jfloat a31, jfloat a32, jfloat a33)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        m->set(a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22,
                a23, a30, a31, a32, a33);
        //osg::notify(osg::NOTICE) << "Matrix:" << *m << std::endl;
    }
}

JNIEXPORT jfloat JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeGet(JNIEnv* env, jclass, jlong cptr, jint row, jint column)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
    	float* data = (float*)(m->ptr());
    	return data[row*4+column];
    }
    return 0.f;
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeGetTranslation(JNIEnv* env, jclass, jlong cptr)
{
	osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
	osg::RefMatrixf *result = NULL;
	if(m!=0)
	{
		osg::Matrixf _mat = osg::Matrixf(*m);
		float* data = (float*)(_mat.ptr());
		osg::Matrixf* _t = new osg::Matrixf();
		_t->set(1, 0, 0, data[3], 0, 1, 0, data[7], 0, 0, 1, data[11], data[12], data[13], data[14], data[15]);
		result = new osg::RefMatrixf(*_t);
		result->ref();
	}
	return reinterpret_cast<jlong>(result);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeGetRotation(JNIEnv* env, jclass, jlong cptr)
{
	osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
	osg::RefMatrixf *result = NULL;
	if(m!=0)
	{
		osg::Matrixf _mat = osg::Matrixf(*m);
		float* data = (float*)(_mat.ptr());
		osg::Matrixf* _t = new osg::Matrixf();
		_t->set(data[0], data[1], data[2], 0, data[4], data[5], data[6], 0, data[8], data[9], data[10], 0, 0, 0, 0, 1);
		result = new osg::RefMatrixf(*_t);
		result->ref();
	}
	return reinterpret_cast<jlong>(result);
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeIsIdentity(JNIEnv *, jclass, jlong cptr)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        return m->isIdentity()?JNI_TRUE:JNI_FALSE;
    }
    return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMakeIdentity(JNIEnv *, jclass, jlong cptr)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        m->makeIdentity();
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMakeScale(JNIEnv *, jclass, jlong cptr, jfloat v1, jfloat v2,
        jfloat v3)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        m->makeScale(v1,v2,v3);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMakeTranslate(JNIEnv *, jclass, jlong cptr, jfloat v1,
        jfloat v2, jfloat v3)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        m->makeTranslate(v1,v2,v3);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMakeRotate(JNIEnv *, jclass, jlong cptr, jfloat angle,
        jfloat x, jfloat y, jfloat z)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    if(m != 0)
    {
        m->makeRotate(angle,x,y,z);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMakeRotateQuat(JNIEnv *, jclass, jlong cptr, jlong quat)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    RefQuat *q = reinterpret_cast<RefQuat*>(quat);
    if(m != 0 && q != 0)
    {
        m->makeRotate(*q);
    }
}

JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeInvert(JNIEnv *, jclass, jlong cptr,
                                                                                jlong matrix)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    osg::RefMatrixf *rhs = reinterpret_cast<osg::RefMatrixf *>(matrix);
    if(m != 0)
    {
        return m->invert(*rhs)?JNI_TRUE:JNI_FALSE;
    }
    return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativePreMult(JNIEnv *, jclass, jlong cptr, jlong matrix)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    osg::RefMatrixf *rhs = reinterpret_cast<osg::RefMatrixf *>(matrix);
    if(m != 0)
    {
        m->preMult(*rhs);
    }
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativePostMult(JNIEnv *, jclass, jlong cptr, jlong matrix)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    osg::RefMatrixf *rhs = reinterpret_cast<osg::RefMatrixf *>(matrix);
    if(m != 0)
    {
        m->postMult(*rhs);
    }
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativePreMultVec3(JNIEnv *, jclass, jlong cptr, jlong vec)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    RefVec3 *rhs = reinterpret_cast<RefVec3 *>(vec);
    RefVec3 *r = new RefVec3();

    if(m != 0)
    {
    	osg::Vec3 res = m->preMult(*rhs);
    	r->set(res.x(),res.y(),res.z());
    }

    r->ref();
    return reinterpret_cast<jlong>(r);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativePostMultVec3(JNIEnv *, jclass, jlong cptr, jlong vec)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    RefVec3 *rhs = reinterpret_cast<RefVec3 *>(vec);
    RefVec3 *r = new RefVec3();

    if(m != 0)
    {
        osg::Vec3 res = m->postMult(*rhs);
        r->set(res.x(),res.y(),res.z());
    }

    r->ref();
    return reinterpret_cast<jlong>(r);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativePreMultVec4(JNIEnv *, jclass, jlong cptr, jlong vec)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    RefVec4 *rhs = reinterpret_cast<RefVec4 *>(vec);
    RefVec4 *r = new RefVec4();

    if(m != 0)
    {
        osg::Vec4 res = m->preMult(*rhs);
        r->set(res.x(),res.y(),res.z(),res.w());
    }

    r->ref();
    return reinterpret_cast<jlong>(r);
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Matrix_nativePostMultVec4(JNIEnv *, jclass, jlong cptr, jlong vec)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    RefVec4 *rhs = reinterpret_cast<RefVec4 *>(vec);
    RefVec4 *r = new RefVec4();

    if(m != 0)
    {
    	osg::Vec4 res = m->postMult(*rhs);
        r->set(res.x(),res.y(),res.z(),res.w());
    }

    r->ref();
    return reinterpret_cast<jlong>(r);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Matrix_nativeMakeLookAt(JNIEnv *, jclass, jlong cptr,
		jlong cptr_eye, jlong cptr_center, jlong cptr_up)
{
    osg::RefMatrixf *m = reinterpret_cast<osg::RefMatrixf *>(cptr);
    RefVec3 *eye = reinterpret_cast<RefVec3 *>(cptr_eye);
    RefVec3 *center = reinterpret_cast<RefVec3 *>(cptr_center);
    RefVec3 *up = reinterpret_cast<RefVec3 *>(cptr_up);
    if(m != 0 && eye !=0 && center != 0 && up != 0)
    {
        m->makeLookAt(*eye,*center,*up);
    }
}



/**
 * osg::AnimationPathCallback
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		cb->unref();
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeCreateAnimationPathCallback(JNIEnv *, jclass)
{
	osg::AnimationPathCallback *cb = new osg::AnimationPathCallback();
	cb->ref();
	return reinterpret_cast<jlong>(cb);
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeSetAnimationPath(JNIEnv *, jclass, jlong cptr,
		jlong path)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	osg::AnimationPath *ap = reinterpret_cast<osg::AnimationPath *>(path);
	if(cb != 0 && ap != 0)
	{
		cb->setAnimationPath(ap);
	}
}
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetAnimationPath(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		return reinterpret_cast<jlong>(cb->getAnimationPath());
	}
	return 0L;
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeSetPivotPoint(JNIEnv *, jclass, jlong cptr,
		jlong vec3)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	RefVec3 *pivot = reinterpret_cast<RefVec3 *>(vec3);
	if(cb != 0 && pivot != 0)
	{
		cb->setPivotPoint(*pivot);
	}
}
JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetPivotPoint(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		osg::Vec3 ret = cb->getPivotPoint();
		RefVec3 *refRet = new RefVec3();
		refRet->set(ret.x(),ret.y(),ret.z());
		refRet->ref();
		return reinterpret_cast<jlong>(refRet);
	}
	return 0L;
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeSetUseInverseMatrix(JNIEnv *, jclass, jlong cptr,
		jboolean useInverseMatrix)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		cb->setUseInverseMatrix((useInverseMatrix==JNI_TRUE)?true:false);
	}
}
JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetUseInverseMatrix(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		return cb->getUseInverseMatrix()?JNI_TRUE:JNI_FALSE;
	}
	return JNI_FALSE;
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeSetTimeOffset(JNIEnv *, jclass, jlong cptr,
		jdouble offset)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		cb->setTimeOffset(offset);
	}
}
JNIEXPORT jdouble JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetTimeOffset(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		return cb->getTimeOffset();
	}
	return 0.0;
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeSetTimeMultiplier(JNIEnv *, jclass, jlong cptr,
		jdouble multiplier)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		cb->setTimeMultiplier(multiplier);
	}
}
JNIEXPORT jdouble JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetTimeMultiplier(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		return cb->getTimeMultiplier();
	}
	return 0.0;
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeReset(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		cb->reset();
	}
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeSetPause(JNIEnv *, jclass, jlong cptr,
		jboolean pause)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		cb->setPause((pause==JNI_TRUE)?true:false);
	}
}
JNIEXPORT jboolean JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetPause(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		return cb->getPause()?JNI_TRUE:JNI_FALSE;
	}
	return JNI_FALSE;
}
JNIEXPORT jdouble JNICALL Java_org_openscenegraph_osg_core_AnimationPathCallback_nativeGetAnimationTime(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPathCallback *cb = reinterpret_cast<osg::AnimationPathCallback *>(cptr);
	if(cb != 0)
	{
		return cb->getAnimationTime();
	}
	return 0.0;
}


/**
 * osg::AnimationPath
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	if(am != 0)
	{
		am->unref();
	}
}

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeCreateAnimationPath(JNIEnv *, jclass)
{
	osg::AnimationPath *am = new osg::AnimationPath();
	am->ref();
	return reinterpret_cast<jlong>(am);
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeInsert(JNIEnv *, jclass, jlong cptr,
		jdouble time, jlong position, jlong rotation, jlong scale)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	RefVec3 *p = reinterpret_cast<RefVec3 *>(position);
	RefQuat *q = reinterpret_cast<RefQuat *>(rotation);
	RefVec3 *s = reinterpret_cast<RefVec3 *>(scale);
	if(	am != 0 &&
		p != 0 &&
		q != 0 &&
		s != 0)
	{
		osg::AnimationPath::ControlPoint cp;
		cp.setPosition(*p);
		cp.setRotation(*q);
		cp.setScale(*s);
		am->insert(time,cp);
	}
}
JNIEXPORT jdouble JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeGetFirstTime(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	if(am != 0)
	{
		return am->getFirstTime();
	}
	return 0;
}
JNIEXPORT jdouble JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeGetLastTime(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	if(am != 0)
	{
		return am->getLastTime();
	}
	return 0;
}
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeSetLoopMode(JNIEnv *, jclass, jlong cptr,
		jint lm)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	if(am != 0)
	{
		am->setLoopMode(osg::AnimationPath::LoopMode(lm));
	}
}
JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeGetLoopMode(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	if(am != 0)
	{
		return (int)am->getLoopMode();
	}
	return 0;
}

/**
 *  osg::Image
**/

JNIEXPORT jlong JNICALL Java_org_openscenegraph_osg_core_Image_nativeCreateImage(JNIEnv *, jclass)
{
	osg::Image *image = new osg::Image();
	image->ref();
	return reinterpret_cast<jlong>(image);
}


JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Image_nativeDispose(JNIEnv *, jclass, long cptr)
{
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		im->unref();
	}
}
	
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Image_nativeSetImage(JNIEnv *env, jclass, long cptr, 
			    jint s,jint t,jint r,
			    jint internalTextureformat,
			    jint pixelFormat,jint type,
			    jbyteArray buffer)
{
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		jbyte *imageBuffer;
		imageBuffer = env->GetByteArrayElements(buffer, JNI_FALSE);
		int ms = s;
		int mt = t;
		int mr = r;
		int imageSize = mr*mt*mr*4;
		OSG_WARN << "Setting IMAGE of (my) computed size:" <<mr*mt*mr*4 << std::endl;
        //unsigned char* imageBuffer = (unsigned char *)env->GetDirectBufferAddress(buffer);
        //int imageSize = (int) env->GetDirectBufferCapacity(buffer);
        //LOGE("Setting IMAGE of size:%d",imageSize);
		if(!im->data())
		{
			im->allocateImage(ms, mt, mr, static_cast<GLenum>(pixelFormat), static_cast<GLenum>(type), 1);
			im->setInternalTextureFormat(4);
		}
        memcpy(im->data(), imageBuffer, imageSize);
		//release arrays:
		env->ReleaseByteArrayElements(buffer, imageBuffer, 0);
	}
}

JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Image_nativeDirty(JNIEnv *, jclass, long cptr)
{
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		im->dirty();
	}
}

JNIEXPORT jbyte JNICALL Java_org_openscenegraph_osg_core_Image_nativeGetRed(JNIEnv* env, jclass, jlong cptr, jint s, jint t, jint r)
{
	unsigned char result = 0;
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		result = (unsigned char)(im->getColor(s,t,r).x());
	}
	return jbyte(result);
}

JNIEXPORT jbyte JNICALL Java_org_openscenegraph_osg_core_Image_nativeGetGreen(JNIEnv* env, jclass, jlong cptr, jint s, jint t, jint r)
{
	unsigned char result = 0;
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		result = (unsigned char)(im->getColor(s,t,r).y());
	}
	return jbyte(result);
}

JNIEXPORT jbyte JNICALL Java_org_openscenegraph_osg_core_Image_nativeGetBlue(JNIEnv* env, jclass, jlong cptr, jint s, jint t, jint r)
{
	unsigned char result = 0;
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		result = (unsigned char)(im->getColor(s,t,r).z());
	}
	return jbyte(result);
}

JNIEXPORT jbyte JNICALL Java_org_openscenegraph_osg_core_Image_nativeGetAlpha(JNIEnv* env, jclass, jlong cptr, jint s, jint t, jint r)
{
	unsigned char result = 0;
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		result = (unsigned char)(im->getColor(s,t,r).w());
	}
	return jbyte(result);
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_Image_nativeS(JNIEnv* env, jclass, jlong cptr)
{
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		return im->s();
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_org_openscenegraph_osg_core_Image_nativeT(JNIEnv* env, jclass, jlong cptr)
{
	osg::Image *im = reinterpret_cast<osg::Image *>(cptr);
	if(im != 0)
	{
		return im->t();
	}
	return 0;
}

}
