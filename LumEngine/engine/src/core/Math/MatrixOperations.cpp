#include "Core/Math/MatrixOperations.hpp"

namespace lum {

	Matrix4 ToMat4( const Quaternion& q ) {
		Matrix4 result( 1.0f );

		float32 xx = q.m_X * q.m_X, yy = q.m_Y * q.m_Y, zz = q.m_Z * q.m_Z;
		float32 xy = q.m_X * q.m_Y, xz = q.m_X * q.m_Z, yz = q.m_Y * q.m_Z;
		float32 wx = q.m_W * q.m_X, wy = q.m_W * q.m_Y, wz = q.m_W * q.m_Z;

		result.m_Data[ 0 ] = 1 - 2 * (yy + zz);
		result.m_Data[ 1 ] = 2 * (xy + wz);
		result.m_Data[ 2 ] = 2 * (xz - wy);

		result.m_Data[ 4 ] = 2 * (xy - wz);
		result.m_Data[ 5 ] = 1 - 2 * (xx + zz);
		result.m_Data[ 6 ] = 2 * (yz + wx);

		result.m_Data[ 8 ] = 2 * (xz + wy);
		result.m_Data[ 9 ] = 2 * (yz - wx);
		result.m_Data[ 10 ] = 1 - 2 * (xx + yy);

		result.m_Data[ 15 ] = 1.0f;

		return result;
	}

	Matrix4 Transpose( const Matrix4& m ) {
		Matrix4 result;
		for (uint32 col = 0; col < 4; col++)
			for (uint32 row = 0; row < 4; row++)
				result.m_Data[ row * 4 + col ] = m.m_Data[ col * 4 + row ];
		return result;
	}

	Matrix4 RotateX( const Matrix4& m, float32 angle ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		Matrix4 rot( 1.0f );
		rot.m_Data[ 5 ] = c;
		rot.m_Data[ 6 ] = s;
		rot.m_Data[ 9 ] = -s;
		rot.m_Data[ 10 ] = c;
		return m * rot;
	}

	Matrix4 RotateY( const Matrix4& m, float32 angle ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		Matrix4 rot( 1.0f );
		rot.m_Data[ 0 ] = c;
		rot.m_Data[ 2 ] = -s;
		rot.m_Data[ 8 ] = s;
		rot.m_Data[ 10 ] = c;
		return m * rot;
	}

	Matrix4 RotateZ( const Matrix4& m, float32 angle ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		Matrix4 rot( 1.0f );
		rot.m_Data[ 0 ] = c;
		rot.m_Data[ 1 ] = s;
		rot.m_Data[ 4 ] = -s;
		rot.m_Data[ 5 ] = c;
		return m * rot;
	}

	Matrix4 Rotate( const Matrix4& m, float32 angle, float32 ax, float32 ay, float32 az ) {
		float32 c = cos( angle );
		float32 s = sin( angle );
		float32 t = 1.0f - c;

		float32 len = sqrt( ax * ax + ay * ay + az * az );
		ax /= len; ay /= len; az /= len;

		Matrix4 rot( 1.0f );

		rot.m_Data[ 0 ] = t * ax * ax + c;
		rot.m_Data[ 1 ] = t * ax * ay - s * az;
		rot.m_Data[ 2 ] = t * ax * az + s * ay;

		rot.m_Data[ 4 ] = t * ax * ay + s * az;
		rot.m_Data[ 5 ] = t * ay * ay + c;
		rot.m_Data[ 6 ] = t * ay * az - s * ax;

		rot.m_Data[ 8 ] = t * ax * az - s * ay;
		rot.m_Data[ 9 ] = t * ay * az + s * ax;
		rot.m_Data[ 10 ] = t * az * az + c;

		return m * rot;
	}

	Matrix4 Rotate( const Matrix4& m, const Quaternion& q ) {
		return m * ToMat4( q );
	}

	Matrix4 Inverse( const Matrix4& m ) {
		const float32* d = m.m_Data;

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

		result.m_Data[ 0 ] = (d[ 5 ] * c5 - d[ 9 ] * c4 + d[ 13 ] * c3) * inv;
		result.m_Data[ 1 ] = (-d[ 1 ] * c5 + d[ 9 ] * c2 - d[ 13 ] * c1) * inv;
		result.m_Data[ 2 ] = (d[ 1 ] * c4 - d[ 5 ] * c2 + d[ 13 ] * c0) * inv;
		result.m_Data[ 3 ] = (-d[ 1 ] * c3 + d[ 5 ] * c1 - d[ 9 ] * c0) * inv;

		result.m_Data[ 4 ] = (-d[ 4 ] * c5 + d[ 8 ] * c4 - d[ 12 ] * c3) * inv;
		result.m_Data[ 5 ] = (d[ 0 ] * c5 - d[ 8 ] * c2 + d[ 12 ] * c1) * inv;
		result.m_Data[ 6 ] = (-d[ 0 ] * c4 + d[ 4 ] * c2 - d[ 12 ] * c0) * inv;
		result.m_Data[ 7 ] = (d[ 0 ] * c3 - d[ 4 ] * c1 + d[ 8 ] * c0) * inv;

		result.m_Data[ 8 ] = (d[ 7 ] * s5 - d[ 11 ] * s4 + d[ 15 ] * s3) * inv;
		result.m_Data[ 9 ] = (-d[ 3 ] * s5 + d[ 11 ] * s2 - d[ 15 ] * s1) * inv;
		result.m_Data[ 10 ] = (d[ 3 ] * s4 - d[ 7 ] * s2 + d[ 15 ] * s0) * inv;
		result.m_Data[ 11 ] = (-d[ 3 ] * s3 + d[ 7 ] * s1 - d[ 11 ] * s0) * inv;

		result.m_Data[ 12 ] = (-d[ 6 ] * s5 + d[ 10 ] * s4 - d[ 14 ] * s3) * inv;
		result.m_Data[ 13 ] = (d[ 2 ] * s5 - d[ 10 ] * s2 + d[ 14 ] * s1) * inv;
		result.m_Data[ 14 ] = (-d[ 2 ] * s4 + d[ 6 ] * s2 - d[ 14 ] * s0) * inv;
		result.m_Data[ 15 ] = (d[ 2 ] * s3 - d[ 6 ] * s1 + d[ 10 ] * s0) * inv;

		return result;
	}

	Matrix4 Perspective( float32 fov, float32 aspect, float32 near, float32 far ) {
		float32 tanHalf = tan( fov * 0.5f );

		Matrix4 result( 0.0f );

		result.m_Data[ 0 ] = 1.0f / (aspect * tanHalf);
		result.m_Data[ 5 ] = 1.0f / tanHalf;

		result.m_Data[ 10 ] = -(far + near) / (far - near);
		result.m_Data[ 11 ] = -1.0f;

		result.m_Data[ 14 ] = -(2.0f * far * near) / (far - near);

		return result;
	}

	Matrix4 Orthographic( float32 left, float32 right,
						  float32 bottom, float32 top,
						  float32 near, float32 far ) {
		Matrix4 result( 1.0f );

		result.m_Data[ 0 ] = 2.0f / (right - left);
		result.m_Data[ 5 ] = 2.0f / (top - bottom);
		result.m_Data[ 10 ] = -2.0f / (far - near);

		result.m_Data[ 12 ] = -(right + left) / (right - left);
		result.m_Data[ 13 ] = -(top + bottom) / (top - bottom);
		result.m_Data[ 14 ] = -(far + near) / (far - near);

		return result;
	}

	Matrix4 InverseTR( const Matrix4& m ) {
		Matrix4 result;

		result.m_Data[ 0 ] = m.m_Data[ 0 ]; result.m_Data[ 4 ] = m.m_Data[ 1 ]; result.m_Data[ 8 ] = m.m_Data[ 2 ];
		result.m_Data[ 1 ] = m.m_Data[ 4 ]; result.m_Data[ 5 ] = m.m_Data[ 5 ]; result.m_Data[ 9 ] = m.m_Data[ 6 ];
		result.m_Data[ 2 ] = m.m_Data[ 8 ]; result.m_Data[ 6 ] = m.m_Data[ 9 ]; result.m_Data[ 10 ] = m.m_Data[ 10 ];

		result.m_Data[ 12 ] = -(result.m_Data[ 0 ] * m.m_Data[ 12 ] +
								result.m_Data[ 4 ] * m.m_Data[ 13 ] +
								result.m_Data[ 8 ] * m.m_Data[ 14 ]);

		result.m_Data[ 13 ] = -(result.m_Data[ 1 ] * m.m_Data[ 12 ] +
								result.m_Data[ 5 ] * m.m_Data[ 13 ] +
								result.m_Data[ 9 ] * m.m_Data[ 14 ]);

		result.m_Data[ 14 ] = -(result.m_Data[ 2 ] * m.m_Data[ 12 ] +
								result.m_Data[ 6 ] * m.m_Data[ 13 ] +
								result.m_Data[ 10 ] * m.m_Data[ 14 ]);

		result.m_Data[ 3 ] = 0.0f;
		result.m_Data[ 7 ] = 0.0f;
		result.m_Data[ 11 ] = 0.0f;
		result.m_Data[ 15 ] = 1.0f;

		return result;
	}

} // namespace lum