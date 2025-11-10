#pragma once
#include <cinttypes>
namespace rmGUI
{
    template<typename T, uint16_t N>
    struct vec_fields
    {
        float data[N];
    };

    template<typename T>
    struct vec_fields<T, 1>
    {
        union
        {
            T data[1];
            struct{ T x; };
            struct{ T s; };
            struct{ T r; };
        };
    };

    template<typename T>
    struct vec_fields<T, 2>
    {
        union
        {
            T data[2];
            struct{ T x, y; };
            struct{ T s, t; };
            struct{ T r, g; };
        };
    };

    template<typename T>
    struct vec_fields<T, 3>
    {
        union
        {
            T data[3];
            struct{ T x, y, z; };
            struct{ T s, t, p; };
            struct{ T r, g, b; };
        };
    };

    template<typename T>
    struct vec_fields<T, 4>
    {
        union
        {
            T data[4];
            struct{ T x, y, z, w; };
            struct{ T s, t, p, q; };
            struct{ T r, g, b, a; };
        };
    };


    template<typename T, uint16_t N>
    struct vec : public vec_fields<T, N>
    {   
        using vec_fields<T, N>::data;
        constexpr vec<T,N> operator*(const vec<T,N>& other)
        {
            vec<T,N> res;
            for(int i = 0; i < N; i++)
                res[i] = data[i] * other[i];
            return res;
        }
        constexpr vec<T,N> operator+(const vec<T,N>& other)
        {
            vec<T,N> res;
            for(int i = 0; i < N; i++)
                res[i] = data[i] + other[i];
            return res;
        }
        constexpr vec<T,N> operator-(const vec<T,N>& other)
        {
            vec<T,N> res;
            for(int i = 0; i < N; i++)
                res[i] = data[i] - other[i];
            return res;
        }

        constexpr vec<T,N>& operator*=(const vec<T,N>& other)
        {
            for(int i = 0; i < N; i++)
                data[i] *= other[i];
            return *this;
        }
        constexpr vec<T,N>& operator+=(const vec<T,N>& other)
        {
            for(int i = 0; i < N; i++)
                data[i] += other[i];
            return *this;
        }
        constexpr vec<T,N>& operator-=(const vec<T,N>& other)
        {
            for(int i = 0; i < N; i++)
                data[i] -= other[i];
            return *this;
        }
        constexpr inline T operator[](std::size_t i)
        {
            return data[i];
        }
    };

    using float2 = vec<float, 2>;
    using float3 = vec<float, 3>;
    using float4 = vec<float, 4>;

    using double2 = vec<double, 2>;
    using double3 = vec<double, 3>;
    using double4 = vec<double, 4>;

    using int2 = vec<int, 2>;
    using int3 = vec<int, 3>;
    using int4 = vec<int, 4>;

    using uint2 = vec<unsigned int, 2>;
    using uint3 = vec<unsigned int, 3>;
    using uint4 = vec<unsigned int, 4>;
}