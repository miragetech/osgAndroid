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

#include <string>

#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>

extern std::string jstring2string(JNIEnv *env, jstring jstr);


template<class T>
class ReferencedType: public T, public osg::Referenced
{
public:
	ReferencedType() { }
protected:
	~ReferencedType() { }
};
typedef ReferencedType<osg::Vec3> RefVec3;
typedef ReferencedType<osg::Vec4> RefVec4;
typedef ReferencedType<osg::Quat> RefQuat;
