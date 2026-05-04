#include "render/temporal_antialiasing.hpp"

namespace lum::render {

	void TemporalAntiAliasing::Initialize( rhi::RenderDevice* dev ) {

		mRenderDev = dev;

	}

	void TemporalAntiAliasing::EnsureFrameTex( const rhi::FTextureDescriptor& desc ) {

		mRenderDev->Delete( mPreviousFrameTex );
		mWidth = desc.mWidth;
		mHeight = desc.mHeight;

		mPreviousFrameTex = mRenderDev->CreateTexture( desc );

	}

	Matrix4 TemporalAntiAliasing::ApplyJitter( const Matrix4& projection ) {

		static const Vector2 skOffsets[ ] = {
			{ 0.500000f,  0.333333f },
			{ 0.250000f,  0.666667f },
			{ 0.750000f,  0.111111f },
			{ 0.125000f,  0.444444f },
			{ 0.625000f,  0.777778f },
			{ 0.375000f,  0.222222f },
			{ 0.875000f,  0.555556f },
			{ 0.125000f,  0.888889f },
		};

		uint32 index = mFrameIndex % 8;
		Vector2 offset = skOffsets[ index ];

		Vector2 jitter = (offset * 2.0f - 1.0f) / Vector2( mWidth, mHeight ) * 0.5f;

		Matrix4 jittered = projection;
		jittered[ 2 ][ 0 ] += jitter.x;
		jittered[ 2 ][ 1 ] += jitter.y;

		mFrameIndex++;

		return jittered;

	}


}