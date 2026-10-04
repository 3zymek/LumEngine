#pragma once


namespace lum::render {

	class PostprocessEffect {
	public:
		virtual ~PostprocessEffect( ) = 0;
		virtual void Execute( ) = 0;
	};

}