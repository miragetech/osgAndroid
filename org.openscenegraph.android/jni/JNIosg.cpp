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

#include <osg/Node>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Matrixf>
#include <osg/AnimationPath>
#include <osg/io_utils>
#include <osg/Texture2D>

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
	if(node != 0)
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


/**
 * osg::Group
 */
JNIEXPORT void JNICALL Java_org_openscenegraph_osg_core_Group_nativeDispose(JNIEnv *, jclass, jlong cptr)
{
    osg::Group *g = reinterpret_cast<osg::Group *>(cptr);
    if(g != 0)
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
}
JNIEXPORT jdouble JNICALL Java_org_openscenegraph_osg_core_AnimationPath_nativeGetLastTime(JNIEnv *, jclass, jlong cptr)
{
	osg::AnimationPath *am = reinterpret_cast<osg::AnimationPath *>(cptr);
	if(am != 0)
	{
		return am->getLastTime();
	}
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

}
