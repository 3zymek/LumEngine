#pragma once

#include "core/math/mat3.hpp"
#include "core/math/mat4.hpp"
#include "core/math/vec2.hpp"
#include "core/math/vec3.hpp"
#include "core/math/vec4.hpp"
#include "core/math/quaternion.hpp"

namespace lum {

	Matrix4 ToMat4( const Quaternion& q );

	Matrix4 Transpose( const Matrix4& m );

	//-------------------------------------------------
	// Translate
	//-------------------------------------------------

	template<cArithmetic tType>
	Matrix4 Translate( const Matrix4& m, const TVector4<tType>& pos ) {
		Matrix4 result( 1.0f );
		result.mData[ 12 ] = pos.mX;
		result.mData[ 13 ] = pos.mY;
		result.mData[ 14 ] = pos.mZ;
		return m * result;
	}
	template<cArithmetic tType>
	Matrix4 Translate( const Matrix4& m, const TVector3<tType>& pos ) {
		Matrix4 result( 1.0f );
		result.mData[ 12 ] = pos.mX;
		result.mData[ 13 ] = pos.mY;
		result.mData[ 14 ] = pos.mZ;
		return m * result;
	}
	template<cArithmetic tType>
	Matrix4 Translate( const Matrix4& m, const TVector2<tType>& pos ) {
		Matrix4 result( 1.0f );
		result.mData[ 12 ] = pos.mX;
		result.mData[ 13 ] = pos.mY;
		return m * result;
	}


	//-------------------------------------------------
	// Scale
	//-------------------------------------------------

	template<cArithmetic tType>
	Matrix4 Scale( const Matrix4& m, const TVector4<tType>& scale ) {
		Matrix4 result( 1.0f );
		result.mData[ 0 ] = scale.mX;
		result.mData[ 5 ] = scale.mY;
		result.mData[ 10 ] = scale.mZ;
		return m * result;
	}

	template<cArithmetic tType>
	Matrix4 Scale( const Matrix4& m, const TVector3<tType>& scale ) {
		Matrix4 result( 1.0f );
		result.mData[ 0 ] = scale.mX;
		result.mData[ 5 ] = scale.mY;
		result.mData[ 10 ] = scale.mZ;
		return m * result;
	}

	template<cArithmetic tType>
	Matrix4 Scale( const Matrix4& m, const TVector2<tType>& scale ) {
		Matrix4 result( 1.0f );
		result.mData[ 0 ] = scale.mX;
		result.mData[ 5 ] = scale.mY;
		return m * result;
	}



	//-------------------------------------------------
	// Rotate
	//-------------------------------------------------

	Matrix4 RotateX( const Matrix4& m, float32 angle );

	Matrix4 RotateY( const Matrix4& m, float32 angle );

	Matrix4 RotateZ( const Matrix4& m, float32 angle );

	Matrix4 Rotate( const Matrix4& m, float32 angle, float32 ax, float32 ay, float32 az );

	template<cArithmetic tType>
	Matrix4 Rotate( const Matrix4& m, const TVector3<tType>& v ) {
		Quaternion q = FromEuler( v.mX, v.mY, v.mZ );
		return m * ToMat4( q );
	}

	Matrix4 Rotate( const Matrix4& m, const Quaternion& q );



	Matrix4 Inverse( const Matrix4& m );
	Matrix4 InverseTR( const Matrix4& m );
	Matrix4 Perspective( float32 fov, float32 aspect, float32 near, float32 far );
	Matrix4 Orthographic( float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far );

	template<cArithmetic tType>
	Matrix4 LookAt( const TVector3<tType>& eye, const TVector3<tType>& target, const TVector3<tType>& up ) {
		Vector3 f = Normalize( target - eye );  // forward
		Vector3 r = Normalize( Cross( f, up ) ); // right
		Vector3 u = Cross( r, f );               // up

		Matrix4 result;
		result.mData[ 0 ] = r.mX;
		result.mData[ 1 ] = u.mX;
		result.mData[ 2 ] = -f.mX;

		result.mData[ 4 ] = r.mY;
		result.mData[ 5 ] = u.mY;
		result.mData[ 6 ] = -f.mY;

		result.mData[ 8 ] = r.mZ;
		result.mData[ 9 ] = u.mZ;
		result.mData[ 10 ] = -f.mZ;

		result.mData[ 12 ] = -Dot( r, eye );
		result.mData[ 13 ] = -Dot( u, eye );
		result.mData[ 14 ] = Dot( f, eye );
		result.mData[ 15 ] = 1.0f;
		return result;
	}




} // namespace lum
