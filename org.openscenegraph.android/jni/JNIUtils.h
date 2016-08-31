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
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Image>

extern std::string jstring2string(JNIEnv *env, jstring jstr);


template<class T>
class ReferencedType: public T, public osg::Referenced
{
public:
	ReferencedType() { }
protected:
	~ReferencedType() { }
};
typedef ReferencedType<osg::Vec2> RefVec2;
typedef ReferencedType<osg::Vec3> RefVec3;
typedef ReferencedType<osg::Vec4> RefVec4;
typedef ReferencedType<osg::Quat> RefQuat;

inline osg::Vec4 Interpolate(osg::Vec2 _texcoord, osg::Image* _img)
{
	osg::Vec4 kernel[3][3];
	int l_k, l_l;
	float width = _img->s(), height = _img->t();
	float xc = _texcoord.x()*(width-1); float yc = (1.0-_texcoord.y())*(height-1);
	int xd = int(floor(xc)); int yd = int(floor(yc));

	//collect values
	for(int k = (xd-1); k < (xd+2); k++)
	{
		for(int l = (yd-1); l < (yd+2); l++)
		{
			l_k = k - (xd-1); l_l = l - (yd-1);
			if((k<0)||(k>int(width-1))||(l<0)||(l>int(height-1)))
			{
				kernel[l_k][l_l] = osg::Vec4(0,0,0,0);
			}
			else
			{
				kernel[l_k][l_l] = _img->getColor(k,l,0);
			}
		}
	}

	float final_colour_r=0, final_colour_g=0, final_colour_b=0, final_colour_a = 0;
	float x_offset, y_offset;
	x_offset = (xc - (float)xd) - 0.5;
	y_offset = (yc - (float)yd) - 0.5;
	double dist = 0.0;
	for(int k = 0; k < 3; k++)
	{
		for(int l = 0; l < 3; l++)
		{
			dist = (sqrt((abs((k-1)+x_offset) * abs((k-1)+x_offset)) + (abs((l-1)+y_offset) * abs((l-1)+y_offset))))/2.121320344;
			final_colour_r += (1-dist) * kernel[k][l].x();
			final_colour_g += (1-dist) * kernel[k][l].y();
			final_colour_b += (1-dist) * kernel[k][l].z();
			final_colour_a += (1-dist) * kernel[k][l].w();
		}
	}
	return osg::Vec4(final_colour_r/9.0, final_colour_g/9.0, final_colour_b/9.0, final_colour_a/9.0);
}
