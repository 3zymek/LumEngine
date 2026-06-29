#include "render/postprocess/antialiasing/taa.hpp"

namespace lum::render {

	void TemporalAntiAliasing::Initialize( rhi::RenderDevice* dev ) {

		mRenderDev = dev;

	}

	void TemporalAntiAliasing::EnsureFrameTex( const rhi::TextureCreateInfo& desc ) {

		if (mWidth == desc.mWidth && mHeight == desc.mHeight && mPreviousFrameTex != 0) {
			return;
		}

		if(mPreviousFrameTex != 0)
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
		Vector2 rawJitter = skOffsets[ index ] - Vector2( 0.5f, 0.5f );
		mCurrentJitter = rawJitter / Vector2( ToFloat32( mWidth ), ToFloat32( mHeight ) );

		Matrix4 jittered = projection;
		jittered[ 3 ][ 0 ] += mCurrentJitter.mX * 2.0f;
		jittered[ 3 ][ 1 ] += mCurrentJitter.mY * 2.0f;

		mFrameIndex++;

		return jittered;

	}


}