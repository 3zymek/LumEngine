/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/

#include "Core/CoreDefines.hpp"

namespace lum {

	namespace rhi {

		struct VertexInputPass;

	} // namespace rhi

	class VertexLayoutConfiguration {
	public:

		LUM_NODISCARD static const VertexLayoutConfiguration& GetStatic( ) noexcept;
		LUM_NODISCARD rhi::VertexInputPass GetVertexInputPass( ) const;

	private:

		VertexLayoutConfiguration( const VertexLayoutConfiguration& ) = delete;
		VertexLayoutConfiguration( VertexLayoutConfiguration&& ) = delete;
		VertexLayoutConfiguration( ) = default;

	};

} // namespace lum