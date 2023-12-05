#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

/** Mat44f: 4x4 matrix with floats
 *
 * See vec2f.hpp for discussion. Similar to the implementation, the Mat44f is
 * intentionally kept simple and somewhat bare bones.
 *
 * The matrix is stored in row-major order (careful when passing it to OpenGL).
 *
 * The overloaded operator () allows access to individual elements. Example:
 *    Mat44f m = ...;
 *    float m12 = m(1,2);
 *    m(0,3) = 3.f;
 *
 * The matrix is arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
struct Mat44f
{
	float v[16];

	constexpr
	float& operator() (std::size_t aI, std::size_t aJ) noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
	constexpr
	float const& operator() (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
};

// Identity matrix
constexpr Mat44f kIdentity44f = { {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
} };

// Common operators for Mat44f.
// Note that you will need to implement these yourself.

constexpr
Mat44f operator*( Mat44f const& aLeft, Mat44f const& aRight ) noexcept
{
  float _00 = aLeft(0,0)*aRight(0,0) + aLeft(0,1)*aRight(1,0) + aLeft(0,2)*aRight(2,0) + aLeft(0,3)*aRight(3,0);
  float _01 = aLeft(0,0)*aRight(0,1) + aLeft(0,1)*aRight(1,1) + aLeft(0,2)*aRight(2,1) + aLeft(0,3)*aRight(3,1);
  float _02 = aLeft(0,0)*aRight(0,2) + aLeft(0,1)*aRight(1,2) + aLeft(0,2)*aRight(2,2) + aLeft(0,3)*aRight(3,2);
  float _03 = aLeft(0,0)*aRight(0,3) + aLeft(0,1)*aRight(1,3) + aLeft(0,2)*aRight(2,3) + aLeft(0,3)*aRight(3,3);
  float _10 = aLeft(1,0)*aRight(0,0) + aLeft(1,1)*aRight(1,0) + aLeft(1,2)*aRight(2,0) + aLeft(1,3)*aRight(3,0);
  float _11 = aLeft(1,0)*aRight(0,1) + aLeft(1,1)*aRight(1,1) + aLeft(1,2)*aRight(2,1) + aLeft(1,3)*aRight(3,1);
  float _12 = aLeft(1,0)*aRight(0,2) + aLeft(1,1)*aRight(1,2) + aLeft(1,2)*aRight(2,2) + aLeft(1,3)*aRight(3,2);
  float _13 = aLeft(1,0)*aRight(0,3) + aLeft(1,1)*aRight(1,3) + aLeft(1,2)*aRight(2,3) + aLeft(1,3)*aRight(3,3);
  float _20 = aLeft(2,0)*aRight(0,0) + aLeft(2,1)*aRight(1,0) + aLeft(2,2)*aRight(2,0) + aLeft(2,3)*aRight(3,0);
  float _21 = aLeft(2,0)*aRight(0,1) + aLeft(2,1)*aRight(1,1) + aLeft(2,2)*aRight(2,1) + aLeft(2,3)*aRight(3,1);
  float _22 = aLeft(2,0)*aRight(0,2) + aLeft(2,1)*aRight(1,2) + aLeft(2,2)*aRight(2,2) + aLeft(2,3)*aRight(3,2);
  float _23 = aLeft(2,0)*aRight(0,3) + aLeft(2,1)*aRight(1,3) + aLeft(2,2)*aRight(2,3) + aLeft(2,3)*aRight(3,3);
  float _30 = aLeft(3,0)*aRight(0,0) + aLeft(3,1)*aRight(1,0) + aLeft(3,2)*aRight(2,0) + aLeft(3,3)*aRight(3,0);
  float _31 = aLeft(3,0)*aRight(0,1) + aLeft(3,1)*aRight(1,1) + aLeft(3,2)*aRight(2,1) + aLeft(3,3)*aRight(3,1);
  float _32 = aLeft(3,0)*aRight(0,2) + aLeft(3,1)*aRight(1,2) + aLeft(3,2)*aRight(2,2) + aLeft(3,3)*aRight(3,2);
  float _33 = aLeft(3,0)*aRight(0,3) + aLeft(3,1)*aRight(1,3) + aLeft(3,2)*aRight(2,3) + aLeft(3,3)*aRight(3,3);

  return Mat44f { 
    _00, _01, _02, _03,
    _10, _11, _12, _13,
    _20, _21, _22, _23,
    _30, _31, _32, _33,
    };
}

constexpr
Vec4f operator*( Mat44f const& aLeft, Vec4f const& aRight ) noexcept
{
  float _0 = aLeft(0,0)*aRight[0] + aLeft(0,1)*aRight[1] + aLeft(0,2)*aRight[2] + aLeft(0,3)*aRight[3];
  float _1 = aLeft(1,0)*aRight[0] + aLeft(1,1)*aRight[1] + aLeft(1,2)*aRight[2] + aLeft(1,3)*aRight[3];
  float _2 = aLeft(2,0)*aRight[0] + aLeft(2,1)*aRight[1] + aLeft(2,2)*aRight[2] + aLeft(2,3)*aRight[3];
  float _3 = aLeft(3,0)*aRight[0] + aLeft(3,1)*aRight[1] + aLeft(3,2)*aRight[2] + aLeft(3,3)*aRight[3];

  return Vec4f {_0, _1, _2, _3};
}

// Functions:

Mat44f invert( Mat44f const& aM ) noexcept;

inline
Mat44f transpose( Mat44f const& aM ) noexcept
{
	Mat44f ret;
	for( std::size_t i = 0; i < 4; ++i )
	{
		for( std::size_t j = 0; j < 4; ++j )
			ret(j,i) = aM(i,j);
	}
	return ret;
}

inline
Mat44f make_rotation_x( float aAngle ) noexcept
{
  float ca = (float)cos(aAngle);
  float sa = (float)sin(aAngle);

  return Mat44f{
    1.f, 0.f,  0.f, 0.f,
    0.f,  ca,  -sa, 0.f,
    0.f,  sa,   ca, 0.f,
    0.f, 0.f,  0.f, 1.f
  };
}


inline
Mat44f make_rotation_y( float aAngle ) noexcept
{
  float ca = (float)cos(aAngle);
  float sa = (float)sin(aAngle);

  return Mat44f{
     ca, 0.f,  sa, 0.f,
    0.f, 1.f, 0.f, 0.f,
    -sa, 0.f,  ca, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
}

inline
Mat44f make_rotation_z( float aAngle ) noexcept
{
  float ca = (float)cos(aAngle);
  float sa = (float)sin(aAngle);

  return Mat44f{
     ca, -sa, 0.f, 0.f,
     sa,  ca, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
}

inline
Mat44f make_translation( Vec3f aTranslation ) noexcept
{
  return Mat44f{
    1.f, 0.f, 0.f, aTranslation.x,
    0.f, 1.f, 0.f, aTranslation.y,
    0.f, 0.f, 1.f, aTranslation.z,
    0.f, 0.f, 0.f,            1.f
  };
}

inline
Mat44f make_scaling( float sx, float sy, float sz) noexcept
{
  return Mat44f{
     sx, 0.f, 0.f, 0.f,
    0.f,  sy, 0.f, 0.f,
    0.f, 0.f,  sz, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
}

inline
Mat44f make_perspective_projection( float aFovInRadians, float aAspect, float aNear, float aFar ) noexcept
{
  float s = 1 / tan(aFovInRadians/2);
  float sx = s / aAspect;
  
  float a = -(aFar + aNear) / (aFar - aNear);
  float b = -2 * ((aFar*aNear) / (aFar-aNear));

  return Mat44f{
    sx,  0.f,  0.f, 0.f,
    0.f,   s,  0.f, 0.f,
    0.f, 0.f,    a,   b,
    0.f, 0.f, -1.f, 0.f
  };
}



#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
