//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Provides matrix transformation utilities including translation,
//          rotation, scaling, projection, inversion and view matrix creation.
//
//=============================================================================//
#pragma once

#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/Math/Quaternion.hpp"

namespace lum {

	/* @brief Converts a quaternion to a 4x4 rotation matrix.
	* @param q Quaternion to convert.
	* @return Equivalent rotation matrix.
	*/
	Matrix4 LUM_API ToMat4( const Quaternion& q );

	/* @brief Returns the transpose of a 4x4 matrix.
	* @param m Matrix to transpose.
	* @return Transposed matrix.
	*/
	Matrix4 LUM_API Transpose( const Matrix4& m );

	//-------------------------------------------------
	// Translate
	//-------------------------------------------------

	/* @brief Applies a translation using a 4D vector.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param pos Translation vector.
	* @return Transformed matrix.
	*/
	template<cArithmetic tType>
	Matrix4 Translate( const Matrix4& m, const TVector4<tType>& pos ) {
		Matrix4 result( 1.0f );
		result.mData[ 12 ] = pos.mX;
		result.mData[ 13 ] = pos.mY;
		result.mData[ 14 ] = pos.mZ;
		return m * result;
	}

	/* @brief Applies a translation using a 3D vector.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param pos Translation vector.
	* @return Transformed matrix.
	*/
	template<cArithmetic tType>
	Matrix4 Translate( const Matrix4& m, const TVector3<tType>& pos ) {
		Matrix4 result( 1.0f );
		result.mData[ 12 ] = pos.mX;
		result.mData[ 13 ] = pos.mY;
		result.mData[ 14 ] = pos.mZ;
		return m * result;
	}

	/* @brief Applies a translation using a 2D vector.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param pos Translation vector.
	* @return Transformed matrix.
	*/
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

	/* @brief Applies scaling using a 4D vector.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param scale Scale factors.
	* @return Transformed matrix.
	*/
	template<cArithmetic tType>
	Matrix4 Scale( const Matrix4& m, const TVector4<tType>& scale ) {
		Matrix4 result( 1.0f );
		result.mData[ 0 ] = scale.mX;
		result.mData[ 5 ] = scale.mY;
		result.mData[ 10 ] = scale.mZ;
		return m * result;
	}

	/* @brief Applies scaling using a 3D vector.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param scale Scale factors.
	* @return Transformed matrix.
	*/
	template<cArithmetic tType>
	Matrix4 Scale( const Matrix4& m, const TVector3<tType>& scale ) {
		Matrix4 result( 1.0f );
		result.mData[ 0 ] = scale.mX;
		result.mData[ 5 ] = scale.mY;
		result.mData[ 10 ] = scale.mZ;
		return m * result;
	}

	/* @brief Applies scaling using a 2D vector.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param scale Scale factors.
	* @return Transformed matrix.
	*/
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

	/* @brief Rotates a matrix around the X axis.
	* @param m Source matrix.
	* @param angle Rotation angle in radians.
	* @return Rotated matrix.
	*/
	Matrix4 LUM_API RotateX( const Matrix4& m, float32 angle );

	/* @brief Rotates a matrix around the Y axis.
	* @param m Source matrix.
	* @param angle Rotation angle in radians.
	* @return Rotated matrix.
	*/
	Matrix4 LUM_API RotateY( const Matrix4& m, float32 angle );

	/* @brief Rotates a matrix around the Z axis.
	* @param m Source matrix.
	* @param angle Rotation angle in radians.
	* @return Rotated matrix.
	*/
	Matrix4 LUM_API RotateZ( const Matrix4& m, float32 angle );

	/* @brief Rotates a matrix around an arbitrary axis.
	* @param m Source matrix.
	* @param angle Rotation angle in radians.
	* @param ax X component of the rotation axis.
	* @param ay Y component of the rotation axis.
	* @param az Z component of the rotation axis.
	* @return Rotated matrix.
	*/
	Matrix4 LUM_API Rotate( const Matrix4& m, float32 angle, float32 ax, float32 ay, float32 az );

	/* @brief Rotates a matrix using Euler angles.
	* @tparam tType Arithmetic vector component type.
	* @param m Source matrix.
	* @param v Euler angles in radians.
	* @return Rotated matrix.
	*/
	template<cArithmetic tType>
	Matrix4 Rotate( const Matrix4& m, const TVector3<tType>& v ) {
		Quaternion q = FromEuler( v.mX, v.mY, v.mZ );
		return m * ToMat4( q );
	}

	/* @brief Rotates a matrix using a quaternion.
	* @param m Source matrix.
	* @param q Rotation quaternion.
	* @return Rotated matrix.
	*/
	Matrix4 LUM_API Rotate( const Matrix4& m, const Quaternion& q );


	/* @brief Returns the inverse of a 4x4 matrix.
	* @param m Matrix to invert.
	* @return Inverted matrix.
	*/
	Matrix4 LUM_API Inverse( const Matrix4& m );

	/* @brief Returns the inverse of a translation-rotation matrix.
	* @param m Matrix to invert.
	* @return Inverted matrix.
	*/
	Matrix4 LUM_API InverseTR( const Matrix4& m );

	/* @brief Creates a perspective projection matrix.
	* @param fov Vertical field of view in radians.
	* @param aspect Aspect ratio.
	* @param near Near clipping plane.
	* @param far Far clipping plane.
	* @return Perspective projection matrix.
	*/
	Matrix4 LUM_API Perspective( float32 fov, float32 aspect, float32 near, float32 far );

	/* @brief Creates an orthographic projection matrix.
	* @param left Left clipping plane.
	* @param right Right clipping plane.
	* @param bottom Bottom clipping plane.
	* @param top Top clipping plane.
	* @param near Near clipping plane.
	* @param far Far clipping plane.
	* @return Orthographic projection matrix.
	*/
	Matrix4 LUM_API Orthographic( float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far );

	/* @brief Creates a right-handed view matrix using the eye, target and up vectors.
	* @tparam tType Arithmetic vector component type.
	* @param eye Camera position.
	* @param target Point the camera is looking at.
	* @param up Up direction.
	* @return View matrix.
	*/
	template<cArithmetic tType>
	Matrix4 LookAt( const TVector3<tType>& eye, const TVector3<tType>& target, const TVector3<tType>& up ) {
		Vector3 f = Normalize( target - eye );
		Vector3 r = Normalize( Cross( f, up ) );
		Vector3 u = Cross( r, f );

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