#include "Core/Math/MatrixOperations.hpp"

namespace lum {

	Matrix4 ToMat4( const Quaternion& q ) {
		Matrix4 result( 1.0f );

		float32 xx = q.mX * q.mX, yy = q.mY * q.mY, zz = q.mZ * q.mZ;
		float32 xy = q.mX * q.mY, xz = q.mX * q.mZ, yz = q.mY * q.mZ;
		float32 wx = q.mW * q.mX, wy = q.mW * q.mY, wz = q.mW * q.mZ;

		result.mData[ 0 ] = 1 - 2 * (yy + zz);
		result.mData[ 1 ] = 2 * (xy + wz);
		result.mData[ 2 ] = 2 * (xz - wy);

		result.mData[ 4 ] = 2 * (xy - wz);
		result.mData[ 5 ] = 1 - 2 * (xx + zz);
		result.mData[ 6 ] = 2 * (yz + wx);

		result.mData[ 8 ] = 2 * (xz + wy);
		result.mData[ 9 ] = 2 * (yz - wx);
		result.mData[ 10 ] = 1 - 2 * (xx + yy);

		result.mData[ 15 ] = 1.0f;

		return result;
	}

	Matrix4 Transpose( const Matrix4& m ) {
		Matrix4 result;
		for (uint32 col = 0; col < 4; col++)
			for (uint32 row = 0; row < 4; row++)
				result.mData[ row * 4 + col ] = m.mData[ col * 4 + row ];
		return result;
	}

	Matrix4 RotateX( const Matrix4& m, float32 angle ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		Matrix4 rot( 1.0f );
		rot.mData[ 5 ] = c;
		rot.mData[ 6 ] = s;
		rot.mData[ 9 ] = -s;
		rot.mData[ 10 ] = c;
		return m * rot;
	}

	Matrix4 RotateY( const Matrix4& m, float32 angle ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		Matrix4 rot( 1.0f );
		rot.mData[ 0 ] = c;
		rot.mData[ 2 ] = -s;
		rot.mData[ 8 ] = s;
		rot.mData[ 10 ] = c;
		return m * rot;
	}

	Matrix4 RotateZ( const Matrix4& m, float32 angle ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		Matrix4 rot( 1.0f );
		rot.mData[ 0 ] = c;
		rot.mData[ 1 ] = s;
		rot.mData[ 4 ] = -s;
		rot.mData[ 5 ] = c;
		return m * rot;
	}

	Matrix4 Rotate( const Matrix4& m, float32 angle, float32 ax, float32 ay, float32 az ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		float32 t = 1.0f - c;

		float32 len = sqrt( ax * ax + ay * ay + az * az );
		ax /= len; ay /= len; az /= len;

		Matrix4 rot( 1.0f );

		rot.mData[ 0 ] = t * ax * ax + c;
		rot.mData[ 1 ] = t * ax * ay - s * az;
		rot.mData[ 2 ] = t * ax * az + s * ay;

		rot.mData[ 4 ] = t * ax * ay + s * az;
		rot.mData[ 5 ] = t * ay * ay + c;
		rot.mData[ 6 ] = t * ay * az - s * ax;

		rot.mData[ 8 ] = t * ax * az - s * ay;
		rot.mData[ 9 ] = t * ay * az + s * ax;
		rot.mData[ 10 ] = t * az * az + c;

		return m * rot;
	}

	Matrix4 Rotate( const Matrix4& m, const Quaternion& q ) {
		return m * ToMat4( q );
	}

	Matrix4 Inverse( const Matrix4& m ) {
		const float32* d = m.mData;

		float32 s0 = d[ 0 ] * d[ 5 ] - d[ 4 ] * d[ 1 ];
		float32 s1 = d[ 0 ] * d[ 9 ] - d[ 8 ] * d[ 1 ];
		float32 s2 = d[ 0 ] * d[ 13 ] - d[ 12 ] * d[ 1 ];
		float32 s3 = d[ 4 ] * d[ 9 ] - d[ 8 ] * d[ 5 ];
		float32 s4 = d[ 4 ] * d[ 13 ] - d[ 12 ] * d[ 5 ];
		float32 s5 = d[ 8 ] * d[ 13 ] - d[ 12 ] * d[ 9 ];

		float32 c5 = d[ 10 ] * d[ 15 ] - d[ 14 ] * d[ 11 ];
		float32 c4 = d[ 6 ] * d[ 15 ] - d[ 14 ] * d[ 7 ];
		float32 c3 = d[ 6 ] * d[ 11 ] - d[ 10 ] * d[ 7 ];
		float32 c2 = d[ 2 ] * d[ 15 ] - d[ 14 ] * d[ 3 ];
		float32 c1 = d[ 2 ] * d[ 11 ] - d[ 10 ] * d[ 3 ];
		float32 c0 = d[ 2 ] * d[ 7 ] - d[ 6 ] * d[ 3 ];

		float32 det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
		float32 inv = 1.0f / det;

		Matrix4 result;

		result.mData[ 0 ] = (d[ 5 ] * c5 - d[ 9 ] * c4 + d[ 13 ] * c3) * inv;
		result.mData[ 1 ] = (-d[ 1 ] * c5 + d[ 9 ] * c2 - d[ 13 ] * c1) * inv;
		result.mData[ 2 ] = (d[ 1 ] * c4 - d[ 5 ] * c2 + d[ 13 ] * c0) * inv;
		result.mData[ 3 ] = (-d[ 1 ] * c3 + d[ 5 ] * c1 - d[ 9 ] * c0) * inv;

		result.mData[ 4 ] = (-d[ 4 ] * c5 + d[ 8 ] * c4 - d[ 12 ] * c3) * inv;
		result.mData[ 5 ] = (d[ 0 ] * c5 - d[ 8 ] * c2 + d[ 12 ] * c1) * inv;
		result.mData[ 6 ] = (-d[ 0 ] * c4 + d[ 4 ] * c2 - d[ 12 ] * c0) * inv;
		result.mData[ 7 ] = (d[ 0 ] * c3 - d[ 4 ] * c1 + d[ 8 ] * c0) * inv;

		result.mData[ 8 ] = (d[ 7 ] * s5 - d[ 11 ] * s4 + d[ 15 ] * s3) * inv;
		result.mData[ 9 ] = (-d[ 3 ] * s5 + d[ 11 ] * s2 - d[ 15 ] * s1) * inv;
		result.mData[ 10 ] = (d[ 3 ] * s4 - d[ 7 ] * s2 + d[ 15 ] * s0) * inv;
		result.mData[ 11 ] = (-d[ 3 ] * s3 + d[ 7 ] * s1 - d[ 11 ] * s0) * inv;

		result.mData[ 12 ] = (-d[ 6 ] * s5 + d[ 10 ] * s4 - d[ 14 ] * s3) * inv;
		result.mData[ 13 ] = (d[ 2 ] * s5 - d[ 10 ] * s2 + d[ 14 ] * s1) * inv;
		result.mData[ 14 ] = (-d[ 2 ] * s4 + d[ 6 ] * s2 - d[ 14 ] * s0) * inv;
		result.mData[ 15 ] = (d[ 2 ] * s3 - d[ 6 ] * s1 + d[ 10 ] * s0) * inv;

		return result;
	}

	Matrix4 Perspective( float32 fov, float32 aspect, float32 near, float32 far ) {
		float32 tanHalf = tan( fov * 0.5f );

		Matrix4 result( 0.0f );

		result.mData[ 0 ] = 1.0f / (aspect * tanHalf);
		result.mData[ 5 ] = 1.0f / tanHalf;

		result.mData[ 10 ] = -(far + near) / (far - near);
		result.mData[ 11 ] = -1.0f;

		result.mData[ 14 ] = -(2.0f * far * near) / (far - near);

		return result;
	}

	Matrix4 Orthographic( float32 left, float32 right,
						  float32 bottom, float32 top,
						  float32 near, float32 far ) {
		Matrix4 result( 1.0f );

		result.mData[ 0 ] = 2.0f / (right - left);
		result.mData[ 5 ] = 2.0f / (top - bottom);
		result.mData[ 10 ] = -2.0f / (far - near);

		result.mData[ 12 ] = -(right + left) / (right - left);
		result.mData[ 13 ] = -(top + bottom) / (top - bottom);
		result.mData[ 14 ] = -(far + near) / (far - near);

		return result;
	}

	Matrix4 InverseTR( const Matrix4& m ) {
		Matrix4 result;

		result.mData[ 0 ] = m.mData[ 0 ]; result.mData[ 4 ] = m.mData[ 1 ]; result.mData[ 8 ] = m.mData[ 2 ];
		result.mData[ 1 ] = m.mData[ 4 ]; result.mData[ 5 ] = m.mData[ 5 ]; result.mData[ 9 ] = m.mData[ 6 ];
		result.mData[ 2 ] = m.mData[ 8 ]; result.mData[ 6 ] = m.mData[ 9 ]; result.mData[ 10 ] = m.mData[ 10 ];

		result.mData[ 12 ] = -(result.mData[ 0 ] * m.mData[ 12 ] +
								result.mData[ 4 ] * m.mData[ 13 ] +
								result.mData[ 8 ] * m.mData[ 14 ]);

		result.mData[ 13 ] = -(result.mData[ 1 ] * m.mData[ 12 ] +
								result.mData[ 5 ] * m.mData[ 13 ] +
								result.mData[ 9 ] * m.mData[ 14 ]);

		result.mData[ 14 ] = -(result.mData[ 2 ] * m.mData[ 12 ] +
								result.mData[ 6 ] * m.mData[ 13 ] +
								result.mData[ 10 ] * m.mData[ 14 ]);

		result.mData[ 3 ] = 0.0f;
		result.mData[ 7 ] = 0.0f;
		result.mData[ 11 ] = 0.0f;
		result.mData[ 15 ] = 1.0f;

		return result;
	}

} // namespace lum