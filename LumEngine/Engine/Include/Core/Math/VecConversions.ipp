#pragma once

namespace lum {

    // ============================================================
    // TVector2
    // ============================================================

    template<cArithmetic tType>
    template<cArithmetic tOther>
    constexpr TVector2<tType>::TVector2( const TVector3<tOther>& v )
        : m_X( static_cast<tType>(v.m_X) )
        , m_Y( static_cast<tType>(v.m_Y) ) {
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    constexpr TVector2<tType>::TVector2( const TVector4<tOther>& v )
        : m_X( static_cast<tType>(v.m_X) )
        , m_Y( static_cast<tType>(v.m_Y) ) {
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    TVector2<tType>::operator TVector3<tOther>( ) const {
        return {
            static_cast<tOther>(m_X),
            static_cast<tOther>(m_Y),
            tOther( 0 )
        };
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    TVector2<tType>::operator TVector4<tOther>( ) const {
        return {
            static_cast<tOther>(m_X),
            static_cast<tOther>(m_Y),
            tOther( 0 ),
            tOther( 0 )
        };
    }

    // ============================================================
    // TVector3
    // ============================================================

    template<cArithmetic tType>
    template<cArithmetic tOther>
    constexpr TVector3<tType>::TVector3( const TVector2<tOther>& v, tOther z )
        : m_X( static_cast<tType>(v.m_X) )
        , m_Y( static_cast<tType>(v.m_Y) )
        , m_Z( static_cast<tType>(z) ) {
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    constexpr TVector3<tType>::TVector3( const TVector4<tOther>& v )
        : m_X( static_cast<tType>(v.m_X) )
        , m_Y( static_cast<tType>(v.m_Y) )
        , m_Z( static_cast<tType>(v.m_Z) ) {
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    TVector3<tType>::operator TVector2<tOther>( ) const {
        return {
            static_cast<tOther>(m_X),
            static_cast<tOther>(m_Y)
        };
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    TVector3<tType>::operator TVector4<tOther>( ) const {
        return {
            static_cast<tOther>(m_X),
            static_cast<tOther>(m_Y),
            static_cast<tOther>(m_Z),
            tOther( 0 )
        };
    }

    // ============================================================
    // TVector4
    // ============================================================

    template<cArithmetic tType>
    template<cArithmetic tOther>
    constexpr TVector4<tType>::TVector4( const TVector3<tOther>& v, tOther w )
        : m_X( static_cast<tType>(v.m_X) )
        , m_Y( static_cast<tType>(v.m_Y) )
        , m_Z( static_cast<tType>(v.m_Z) )
        , m_W( static_cast<tType>(w) ) {
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    TVector4<tType>::operator TVector3<tOther>( ) const {
        return {
            static_cast<tOther>(m_X),
            static_cast<tOther>(m_Y),
            static_cast<tOther>(m_Z)
        };
    }

    template<cArithmetic tType>
    template<cArithmetic tOther>
    TVector4<tType>::operator TVector2<tOther>( ) const {
        return {
            static_cast<tOther>(m_X),
            static_cast<tOther>(m_Y)
        };
    }

} // namespace lum