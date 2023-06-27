#pragma once

#include <cmath>

union SAPI Vec2 {
    f32 f[2];

    struct {
        f32 x, y;
    };

    struct {
        f32 u, v;
    };

    inline f32& operator[](const i32& index)
    {
        return f[index];
    }
};

union SAPI Vec3 {
    f32 f[3];

    struct {
        f32 x, y, z;
    };

    struct {
        f32 r, g, b;
    };

    struct {
        f32 u, v, w;
    };

    inline f32& operator[](const i32& index)
    {
        return f[index];
    }
};

union SAPI Vec4 {
    f32 f[4];

    struct {
        f32 x, y, z, w;
    };

    struct {
        f32 r, g, b, a;
    };

    inline f32& operator[](const i32& index)
    {
        return f[index];
    }
};

static inline Vec2 Vector2Zero()
{
    Vec2 result = { 0 };
    return result;
}

static inline Vec2 Vector2One()
{
    Vec2 result = { 1.0f, 1.0f };
    return result;
}

static inline Vec2 Vector2Add(const Vec2& v1, const Vec2& v2)
{
    Vec2 result = {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };

    return result;
}

static inline Vec2 Vector2AddValue(const Vec2& v, const f32& f)
{
    Vec2 result = {
        .x = v.x + f,
        .y = v.y + f
    };

    return result;
}

static inline Vec2 Vector2Subtract(const Vec2& v1, const Vec2& v2)
{
    Vec2 result = {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y
    };

    return result;
}

static inline Vec2 Vector2SubtractValue(const Vec2& v, const f32& f)
{
    Vec2 result = {
        .x = v.x - f,
        .y = v.y - f
    };

    return result;
}

static inline f32 Vector2DotProduct(const Vec2& v1, const Vec2& v2)
{
    f32 result = v1.x * v2.x + v1.y * v2.y;
    return result;
}

static inline Vec2 Vector2MultiplyValue(const Vec2& v, const f32& f)
{
    Vec2 result = {
        .x = v.x * f,
        .y = v.y * f
    };

    return result;
}

static inline Vec2 Vector2DivideValue(const Vec2& v, const f32& f)
{
    Vec2 result = {
        .x = v.x / f,
        .y = v.y / f
    };

    return result;
}

static inline f32 Vector2Distance(const Vec2& v1, const Vec2& v2)
{
    f32 result = sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
    return result;
}

static inline f32 Vector2DistanceSqr(const Vec2& v1, const Vec2& v2)
{
    f32 result = (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y);
    return result;
}

static inline f32 Vector2Length(const Vec2& v)
{
    f32 result = sqrtf((v.x * v.x) + (v.y * v.y));
    return result;
}

static inline f32 Vector2LengthSqr(const Vec2& v)
{
    f32 result = (v.x * v.x) + (v.y * v.y);
    return result;
}

static inline f32 Vector2Angle(const Vec2& v)
{
    float result = atanf(v.y / v.x);
    return result;
}

static inline Vec2 Vector2Negate(const Vec2& v)
{
    Vec2 result = {
        .x = -v.x,
        .y = -v.y
    };

    return result;
}

static inline Vec2 Vector2Normalize(const Vec2& v)
{
    Vec2 result = { 0 };
    float length = sqrtf((v.x * v.x) + (v.y * v.y));

    if (length > 0) {
        float invLength = 1.0f / length;
        result.x = v.x * invLength;
        result.y = v.y * invLength;
    }

    return result;
}

static inline Vec2 Vector2Invert(const Vec2& v)
{
    Vec2 result = {
        .x = 1.0f / v.x,
        .y = 1.0f / v.y
    };

    return result;
}

inline Vec2 operator+(const Vec2& left, const Vec2& right)
{
    Vec2 result = Vector2Add(left, right);
    return result;
}

inline Vec2 operator+(const Vec2& left, const f32& right)
{
    Vec2 result = Vector2AddValue(left, right);
    return result;
}

inline Vec2& operator+=(Vec2& left, const Vec2& right)
{
    left = Vector2Add(left, right);
    return left;
}

inline Vec2& operator+=(Vec2& left, const f32& right)
{
    left = Vector2AddValue(left, right);
    return left;
}

inline Vec2 operator-(const Vec2& left, const Vec2& right)
{
    Vec2 result = Vector2Subtract(left, right);
    return result;
}

inline Vec2 operator-(const Vec2& left, const f32& right)
{
    Vec2 result = Vector2SubtractValue(left, right);
    return result;
}

inline Vec2& operator-=(Vec2& left, const Vec2& right)
{
    left = Vector2Subtract(left, right);
    return left;
}

inline Vec2& operator-=(Vec2& left, const f32& right)
{
    left = Vector2SubtractValue(left, right);
    return left;
}

inline Vec2 operator*(const Vec2& left, const f32& right)
{
    Vec2 result = Vector2MultiplyValue(left, right);
    return result;
}

inline Vec2& operator*=(Vec2& left, const f32& right)
{
    left = Vector2MultiplyValue(left, right);
    return left;
}

inline Vec2 operator/(const Vec2& left, const f32& right)
{
    Vec2 result = Vector2DivideValue(left, right);
    return result;
}

inline Vec2& operator/=(Vec2& left, const f32& right)
{
    left = Vector2DivideValue(left, right);
    return left;
}

inline Vec2 operator-(const Vec2& right)
{
    Vec2 result = Vector2Negate(right);
    return result;
}

static inline Vec3 Vector3Zero()
{
    Vec3 result = { 0 };
    return result;
}

static inline Vec3 Vector3One()
{
    Vec3 result = { 1.0f, 1.0f, 1.0f };
    return result;
}

static inline Vec3 Vector3Add(const Vec3& v1, const Vec3& v2)
{
    Vec3 result = {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z
    };

    return result;
}

static inline Vec3 Vector3AddValue(const Vec3& v, const f32& f)
{
    Vec3 result = {
        .x = v.x + f,
        .y = v.y + f,
        .z = v.z + f
    };

    return result;
}

static inline Vec3 Vector3Subtract(const Vec3& v1, const Vec3& v2)
{
    Vec3 result = {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z
    };

    return result;
}

static inline Vec3 Vector3SubtractValue(const Vec3& v, const f32& f)
{
    Vec3 result = {
        .x = v.x - f,
        .y = v.y - f,
        .z = v.z - f
    };

    return result;
}

static inline f32 Vector3DotProduct(const Vec3& v1, const Vec3& v2)
{
    f32 result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return result;
}

static inline Vec3 Vector3MultiplyValue(const Vec3& v, const f32& f)
{
    Vec3 result = {
        .x = v.x * f,
        .y = v.y * f,
        .z = v.z * f
    };

    return result;
}

static inline Vec3 Vector3DivideValue(const Vec3& v, const f32& f)
{
    Vec3 result = {
        .x = v.x / f,
        .y = v.y / f,
        .z = v.z / f
    };

    return result;
}

static inline f32 Vector3Distance(const Vec3& v1, const Vec3& v2)
{
    f32 result = sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z));
    return result;
}

static inline f32 Vector3DistanceSqr(const Vec3& v1, const Vec3& v2)
{
    f32 result = (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z);
    return result;
}

static inline f32 Vector3Length(const Vec3& v)
{
    f32 result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return result;
}

static inline f32 Vector3LengthSqr(const Vec3& v)
{
    f32 result = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
    return result;
}

static inline Vec3 Vector3Negate(const Vec3& v)
{
    Vec3 result = {
        .x = -v.x,
        .y = -v.y,
        .z = -v.z
    };

    return result;
}

static inline Vec3 Vector3Normalize(const Vec3& v)
{
    Vec3 result = { 0 };
    float length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

    if (length > 0) {
        float invLength = 1.0f / length;
        result.x = v.x * invLength;
        result.y = v.y * invLength;
        result.z = v.z * invLength;
    }

    return result;
}

static inline Vec3 Vector3Invert(const Vec3& v)
{
    Vec3 result = {
        .x = 1.0f / v.x,
        .y = 1.0f / v.y,
        .z = 1.0f / v.z
    };

    return result;
}

inline Vec3 operator+(const Vec3& left, const Vec3& right)
{
    Vec3 result = Vector3Add(left, right);
    return result;
}

inline Vec3 operator+(const Vec3& left, const f32& right)
{
    Vec3 result = Vector3AddValue(left, right);
    return result;
}

inline Vec3& operator+=(Vec3& left, const Vec3& right)
{
    left = Vector3Add(left, right);
    return left;
}

inline Vec3& operator+=(Vec3& left, const f32& right)
{
    left = Vector3AddValue(left, right);
    return left;
}

inline Vec3 operator-(const Vec3& left, const Vec3& right)
{
    Vec3 result = Vector3Subtract(left, right);
    return result;
}

inline Vec3 operator-(const Vec3& left, const f32& right)
{
    Vec3 result = Vector3SubtractValue(left, right);
    return result;
}

inline Vec3& operator-=(Vec3& left, const Vec3& right)
{
    left = Vector3Subtract(left, right);
    return left;
}

inline Vec3& operator-=(Vec3& left, const f32& right)
{
    left = Vector3SubtractValue(left, right);
    return left;
}

inline Vec3 operator*(const Vec3& left, const f32& right)
{
    Vec3 result = Vector3MultiplyValue(left, right);
    return result;
}

inline Vec3& operator*=(Vec3& left, const f32& right)
{
    left = Vector3MultiplyValue(left, right);
    return left;
}

inline Vec3 operator/(const Vec3& left, const f32& right)
{
    Vec3 result = Vector3DivideValue(left, right);
    return result;
}

inline Vec3& operator/=(Vec3& left, const f32& right)
{
    left = Vector3DivideValue(left, right);
    return left;
}

inline Vec3 operator-(const Vec3& right)
{
    Vec3 result = Vector3Negate(right);
    return result;
}

static inline Vec4 Vector4Zero()
{
    Vec4 result = { 0 };
    return result;
}

static inline Vec4 Vector4One()
{
    Vec4 result = { 1.0f, 1.0f, 1.0f, 1.0f };
    return result;
}

static inline Vec4 Vector4Add(const Vec4& v1, const Vec4& v2)
{
    Vec4 result = {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z,
        .w = v1.w + v2.w
    };

    return result;
}

static inline Vec4 Vector4AddValue(const Vec4& v, const f32& f)
{
    Vec4 result = {
        .x = v.x + f,
        .y = v.y + f,
        .z = v.z + f,
        .w = v.w + f
    };

    return result;
}

static inline Vec4 Vector4Subtract(const Vec4& v1, const Vec4& v2)
{
    Vec4 result = {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
        .w = v1.w - v2.w
    };

    return result;
}

static inline Vec4 Vector4SubtractValue(const Vec4& v, const f32& f)
{
    Vec4 result = {
        .x = v.x - f,
        .y = v.y - f,
        .z = v.z - f,
        .w = v.w - f
    };

    return result;
}

static inline f32 Vector4DotProduct(const Vec4& v1, const Vec4& v2)
{
    f32 result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
    return result;
}

static inline Vec4 Vector4MultiplyValue(const Vec4& v, const f32& f)
{
    Vec4 result = {
        .x = v.x * f,
        .y = v.y * f,
        .z = v.z * f,
        .w = v.w * f
    };

    return result;
}

static inline Vec4 Vector4DivideValue(const Vec4& v, const f32& f)
{
    Vec4 result = {
        .x = v.x / f,
        .y = v.y / f,
        .z = v.z / f,
        .w = v.w / f
    };

    return result;
}

static inline f32 Vector4Distance(const Vec4& v1, const Vec4& v2)
{
    f32 result = sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z) +
        (v2.w - v1.w) * (v2.w - v1.w));
    return result;
}

static inline f32 Vector4DistanceSqr(const Vec4& v1, const Vec4& v2)
{
    f32 result = (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z) +
        (v2.w - v1.w) * (v2.w - v1.w);
    return result;
}

static inline f32 Vector4Length(const Vec4& v)
{
    f32 result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
    return result;
}

static inline f32 Vector4LengthSqr(const Vec4& v)
{
    f32 result = (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
    return result;
}

static inline Vec4 Vector4Negate(const Vec4& v)
{
    Vec4 result = {
        .x = -v.x,
        .y = -v.y,
        .z = -v.z
    };

    return result;
}

static inline Vec4 Vector4Normalize(const Vec4& v)
{
    Vec4 result = { 0 };
    float length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));

    if (length > 0) {
        float invLength = 1.0f / length;
        result.x = v.x * invLength;
        result.y = v.y * invLength;
        result.z = v.z * invLength;
        result.w = v.w * invLength;
    }

    return result;
}

static inline Vec4 Vector4Invert(const Vec4& v)
{
    Vec4 result = {
        .x = 1.0f / v.x,
        .y = 1.0f / v.y,
        .z = 1.0f / v.z,
        .w = 1.0f / v.w
    };

    return result;
}

inline Vec4 operator+(const Vec4& left, const Vec4& right)
{
    Vec4 result = Vector4Add(left, right);
    return result;
}

inline Vec4 operator+(const Vec4& left, const f32& right)
{
    Vec4 result = Vector4AddValue(left, right);
    return result;
}

inline Vec4& operator+=(Vec4& left, const Vec4& right)
{
    left = Vector4Add(left, right);
    return left;
}

inline Vec4& operator+=(Vec4& left, const f32& right)
{
    left = Vector4AddValue(left, right);
    return left;
}

inline Vec4 operator-(const Vec4& left, const Vec4& right)
{
    Vec4 result = Vector4Subtract(left, right);
    return result;
}

inline Vec4 operator-(const Vec4& left, const f32& right)
{
    Vec4 result = Vector4SubtractValue(left, right);
    return result;
}

inline Vec4& operator-=(Vec4& left, const Vec4& right)
{
    left = Vector4Subtract(left, right);
    return left;
}

inline Vec4& operator-=(Vec4& left, const f32& right)
{
    left = Vector4SubtractValue(left, right);
    return left;
}

inline Vec4 operator*(const Vec4& left, const f32& right)
{
    Vec4 result = Vector4MultiplyValue(left, right);
    return result;
}

inline Vec4& operator*=(Vec4& left, const f32& right)
{
    left = Vector4MultiplyValue(left, right);
    return left;
}

inline Vec4 operator/(const Vec4& left, const f32& right)
{
    Vec4 result = Vector4DivideValue(left, right);
    return result;
}

inline Vec4& operator/=(Vec4& left, const f32& right)
{
    left = Vector4DivideValue(left, right);
    return left;
}

inline Vec4 operator-(const Vec4& right)
{
    Vec4 result = Vector4Negate(right);
    return result;
}