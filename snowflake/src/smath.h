#pragma once

#include <cmath>

#define S_PI 3.14159265358979323846
#define S_PI32 3.14159265359f

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

// Row-Major order
union SAPI Mat2 {
    f32 f[2][2];

    struct {
        f32 m0, m1;
        f32 m2, m3;
    };

    inline f32* operator[](const i32& index)
    {
        return f[index];
    }
};

// Row-Major order
union SAPI Mat3 {
    f32 f[3][3];

    struct {
        f32 m0, m1, m2;
        f32 m3, m4, m5;
        f32 m6, m7, m8;
    };

    inline f32* operator[](const i32& index)
    {
        return f[index];
    }
};

// Row-Major order
union SAPI Mat4 {
    f32 f[4][4];

    struct {
        f32 m0, m1, m2, m3;
        f32 m4, m5, m6, m7;
        f32 m8, m9, m10, m11;
        f32 m12, m13, m14, m15;
    };

    inline f32* operator[](const i32& index)
    {
        return f[index];
    }
};

// NOTE: cmath wrapper functions to keep the API consistent
// NOTE: Should implement with SSE intrinsics

static inline f32 Sin(f32 angle)
{
    return sinf(angle);
}

static inline f32 Cos(f32 angle)
{
    return cosf(angle);
}

static inline f32 Tan(f32 angle)
{
    return tanf(angle);
}

static inline f32 ASin(f32 x)
{
    return asinf(x);
}

static inline f32 ACos(f32 x)
{
    return acosf(x);
}

static inline f32 ATan(f32 x)
{
    return atanf(x);
}

static inline f32 ATan2(f32 y, f32 x)
{
    return atan2f(y, x);
}

static inline f32 Ceil(f32 x)
{
    return ceilf(x);
}

static inline f32 Floor(f32 x)
{
    return floorf(x);
}

static inline f32 Trunc(f32 x)
{
    return truncf(x);
}

static inline f32 Round(f32 x)
{
    return truncf(x);
}

static inline f32 Sqrt(f32 x)
{
    return sqrtf(x);
}

static inline i32 Clamp(i32 x, i32 min, i32 max)
{
    const i32 t = x < min ? min : x;
    return t > max ? max : t;
}

static inline f32 Clamp(f32 x, f32 min, f32 max)
{
    const f32 t = x < min ? min : x;
    return t > max ? max : t;
}

static inline i32 Max(i32 a, i32 b)
{
    return ((a > b) ? a : b);
}

static inline f32 Max(f32 a, f32 b)
{
    return ((a > b) ? a : b);
}

static inline i32 Min(i32 a, i32 b)
{
    return ((a < b) ? a : b);
}

static inline f32 Min(f32 a, f32 b)
{
    return ((a < b) ? a : b);
}

static inline i32 Abs(i32 a)
{
    return ((a > 0) ? a : -a);
}

static inline f32 Abs(f32 a)
{
    return ((a > 0) ? a : -a);
}

static inline i32 Square(i32 x)
{
    return x * x;
}

static inline f32 Square(f32 x)
{
    return x * x;
}

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

static inline Mat2 Matrix2Identity()
{
    Mat2 result = {
        1.0f, 0.0f,
        0.0f, 1.0f
    };

    return result;
}

static inline f32 Matrix2Determinant(Mat2 mat)
{
    f32 result = 0.0f;

    result = mat.m0 * mat.m3 - mat.m1 * mat.m2;
    return result;
}

static inline Mat2 Matrix2Transpose(Mat2 mat)
{
    Mat2 result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m2;
    result.m2 = mat.m1;
    result.m3 = mat.m3;

    return result;
}

static inline Mat2 Matrix2Add(Mat2 left, Mat2 right)
{
    Mat2 result = { 0 };

    result.m0 = left.m0 + right.m0;
    result.m1 = left.m1 + right.m1;
    result.m2 = left.m2 + right.m2;
    result.m3 = left.m3 + right.m3;

    return result;
}

static inline Mat2 Matrix2Subtract(Mat2 left, Mat2 right)
{
    Mat2 result = { 0 };

    result.m0 = left.m0 - right.m0;
    result.m1 = left.m1 - right.m1;
    result.m2 = left.m2 - right.m2;
    result.m3 = left.m3 - right.m3;

    return result;
}

static inline Mat2 Matrix2Multiply(Mat2 left, Mat2 right)
{
    Mat2 result = { 0 };

    result.m0 = left.m0 * right.m0 + left.m1 * right.m2;
    result.m1 = left.m0 * right.m1 + left.m1 * right.m3;
    result.m2 = left.m2 * right.m0 + left.m3 * right.m2;
    result.m3 = left.m2 * right.m1 + left.m3 * right.m3;

    return result;
}

static inline Mat2 Matrix2MultiplyValue(Mat2 mat, f32 f)
{
    Mat2 result = { 0 };

    result.m0 = mat.m0 * f;
    result.m1 = mat.m1 * f;
    result.m2 = mat.m2 * f;
    result.m3 = mat.m3 * f;

    return result;
}

static inline Mat2 Matrix2Inverse(Mat2 mat)
{
    Mat2 result = { 0 };

    f32 invertDet = 1.0f / Matrix2Determinant(mat);

    result.m0 = mat.m3 * invertDet;
    result.m1 = -mat.m1 * invertDet;
    result.m2 = -mat.m2 * invertDet;
    result.m3 = mat.m0 * invertDet;

    return result;
}

inline Mat2 operator+(const Mat2& left, const Mat2& right)
{
    Mat2 result = Matrix2Add(left, right);
    return result;
}

inline Mat2& operator+=(Mat2& left, const Mat2& right)
{
    left = Matrix2Add(left, right);
    return left;
}

inline Mat2 operator-(const Mat2& left, const Mat2& right)
{
    Mat2 result = Matrix2Subtract(left, right);
    return result;
}

inline Mat2& operator-=(Mat2& left, const Mat2& right)
{
    left = Matrix2Subtract(left, right);
    return left;
}

inline Mat2 operator*(const Mat2& left, const Mat2& right)
{
    Mat2 result = Matrix2Multiply(left, right);
    return result;
}

inline Mat2& operator*=(Mat2& left, const Mat2& right)
{
    left = Matrix2Multiply(left, right);
    return left;
}

inline Mat2 operator*(const Mat2& left, const f32& right)
{
    Mat2 result = Matrix2MultiplyValue(left, right);
    return result;
}

inline Mat2 operator*(const f32& left, const Mat2& right)
{
    Mat2 result = Matrix2MultiplyValue(right, left);
    return result;
}

inline Mat2& operator*=(Mat2& left, const f32& right)
{
    left = Matrix2MultiplyValue(left, right);
    return left;
}

static inline Mat3 Matrix3Identity()
{
    Mat3 result = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    return result;
}

static inline f32 Matrix3Determinant(Mat3 mat)
{
    f32 result = mat.m0 * mat.m4 * mat.m8 - mat.m0 * mat.m5 * mat.m7 -
                 mat.m1 * mat.m3 * mat.m8 + mat.m1 * mat.m5 * mat.m6 +
                 mat.m2 * mat.m3 * mat.m7 - mat.m2 * mat.m4 * mat.m6;

    return result;
}

static inline Mat3 Matrix3Transpose(Mat3 mat)
{
    Mat3 result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m3;
    result.m2 = mat.m6;
    result.m3 = mat.m1;
    result.m4 = mat.m4;
    result.m5 = mat.m7;
    result.m6 = mat.m2;
    result.m7 = mat.m5;
    result.m8 = mat.m8;

    return result;
}

static inline Mat3 Matrix3Add(Mat3 left, Mat3 right)
{
    Mat3 result = { 0 };

    result.m0 = left.m0 + right.m0;
    result.m1 = left.m1 + right.m1;
    result.m2 = left.m2 + right.m2;
    result.m3 = left.m3 + right.m3;
    result.m4 = left.m4 + right.m4;
    result.m5 = left.m5 + right.m5;
    result.m6 = left.m6 + right.m6;
    result.m7 = left.m7 + right.m7;
    result.m8 = left.m8 + right.m8;

    return result;
}

static inline Mat3 Matrix3Subtract(Mat3 left, Mat3 right)
{
    Mat3 result = { 0 };

    result.m0 = left.m0 - right.m0;
    result.m1 = left.m1 - right.m1;
    result.m2 = left.m2 - right.m2;
    result.m3 = left.m3 - right.m3;
    result.m4 = left.m4 - right.m4;
    result.m5 = left.m5 - right.m5;
    result.m6 = left.m6 - right.m6;
    result.m7 = left.m7 - right.m7;
    result.m8 = left.m8 - right.m8;

    return result;
}

static inline Mat3 Matrix3Multiply(Mat3 left, Mat3 right)
{
    Mat3 result = { 0 };

    result.m0 = left.m0 * right.m0 + left.m1 * right.m3 + left.m2 * right.m6;
    result.m1 = left.m0 * right.m1 + left.m1 * right.m4 + left.m2 * right.m7;
    result.m2 = left.m0 * right.m2 + left.m1 * right.m5 + left.m2 * right.m8;
    result.m3 = left.m3 * right.m0 + left.m4 * right.m3 + left.m5 * right.m6;
    result.m4 = left.m3 * right.m1 + left.m4 * right.m4 + left.m5 * right.m7;
    result.m5 = left.m3 * right.m2 + left.m4 * right.m5 + left.m5 * right.m8;
    result.m6 = left.m6 * right.m0 + left.m7 * right.m3 + left.m8 * right.m6;
    result.m7 = left.m6 * right.m1 + left.m7 * right.m4 + left.m8 * right.m7;
    result.m8 = left.m6 * right.m2 * left.m7 * right.m5 + left.m8 * right.m8;

    return result;
}

static inline Mat3 Matrix3MultiplyValue(Mat3 mat, f32 f)
{
    Mat3 result = { 0 };

    result.m0 = mat.m0 * f;
    result.m1 = mat.m1 * f;
    result.m2 = mat.m2 * f;
    result.m3 = mat.m3 * f;
    result.m4 = mat.m4 * f;
    result.m5 = mat.m5 * f;
    result.m6 = mat.m6 * f;
    result.m7 = mat.m7 * f;
    result.m8 = mat.m8 * f;

    return result;
}

static inline Mat3 Matrix3Inverse(Mat3 mat)
{
    Mat3 result = { 0 };

    f32 det = mat.m0 * mat.m4 * mat.m8 - mat.m0 * mat.m5 * mat.m7 -
              mat.m1 * mat.m3 * mat.m8 + mat.m1 * mat.m5 * mat.m6 +
              mat.m2 * mat.m3 * mat.m7 - mat.m2 * mat.m4 * mat.m6;

    f32 invertDet = 1.0f / det;

    result.m0 = (mat.m4 * mat.m8 - mat.m5 * mat.m7) * invertDet;
    result.m3 = -(mat.m3 * mat.m8 - mat.m5 * mat.m6) * invertDet;
    result.m6 = (mat.m3 * mat.m7 - mat.m4 * mat.m6) * invertDet;
    result.m1 = -(mat.m1 * mat.m8 - mat.m2 * mat.m7) * invertDet;
    result.m4 = (mat.m0 * mat.m8 - mat.m2 * mat.m6) * invertDet;
    result.m7 = -(mat.m0 * mat.m7 - mat.m1 * mat.m6) * invertDet;
    result.m2 = (mat.m1 * mat.m5 - mat.m2 * mat.m4) * invertDet;
    result.m5 = -(mat.m0 * mat.m5 - mat.m2 * mat.m3) * invertDet;
    result.m8 = (mat.m0 * mat.m4 - mat.m1 * mat.m3) * invertDet;

    return result;
}

inline Mat3 operator+(const Mat3& left, const Mat3& right)
{
    Mat3 result = Matrix3Add(left, right);
    return result;
}

inline Mat3& operator+=(Mat3& left, const Mat3& right)
{
    left = Matrix3Add(left, right);
    return left;
}

inline Mat3 operator-(const Mat3& left, const Mat3& right)
{
    Mat3 result = Matrix3Subtract(left, right);
    return result;
}

inline Mat3& operator-=(Mat3& left, const Mat3& right)
{
    left = Matrix3Subtract(left, right);
    return left;
}

inline Mat3 operator*(const Mat3& left, const Mat3& right)
{
    Mat3 result = Matrix3Multiply(left, right);
    return result;
}

inline Mat3& operator*=(Mat3& left, const Mat3& right)
{
    left = Matrix3Multiply(left, right);
    return left;
}

inline Mat3 operator*(const Mat3& left, const f32& right)
{
    Mat3 result = Matrix3MultiplyValue(left, right);
    return result;
}

inline Mat3 operator*(const f32& left, const Mat3& right)
{
    Mat3 result = Matrix3MultiplyValue(right, left);
    return result;
}

inline Mat3& operator*=(Mat3& left, const f32& right)
{
    left = Matrix3MultiplyValue(left, right);
    return left;
}

static inline Mat4 Matrix4Identity()
{
    Mat4 result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return result;
}

static inline f32 Matrix4Determinant(Mat4 mat)
{
    // @formatter:off
    f32 result =
             mat.m0 * mat.m5 * mat.m10 * mat.m15 - mat.m0 * mat.m5 * mat.m11 * mat.m14 - mat.m0 * mat.m6 * mat.m9 * mat.m15 +
             mat.m0 * mat.m6 * mat.m11 * mat.m13 + mat.m0 * mat.m7 * mat.m9 * mat.m14  - mat.m0 * mat.m7 * mat.m10 * mat.m13 -
             mat.m1 * mat.m4 * mat.m10 * mat.m15 + mat.m1 * mat.m4 * mat.m11 * mat.m14 + mat.m1 * mat.m6 * mat.m8 * mat.m15 -
             mat.m1 * mat.m6 * mat.m11 * mat.m12 - mat.m1 * mat.m7 * mat.m8 * mat.m14 + mat.m1 * mat.m7 * mat.m10 * mat.m12 +
             mat.m2 * mat.m4 * mat.m9 * mat.m15 - mat.m2 * mat.m4 * mat.m11 * mat.m13 - mat.m2 * mat.m5 * mat.m8 * mat.m15 +
             mat.m2 * mat.m5 * mat.m11 * mat.m12 + mat.m2 * mat.m7 * mat.m8 * mat.m13 - mat.m2 * mat.m7 * mat.m9 * mat.m12 -
             mat.m3 * mat.m4 * mat.m9 * mat.m14 + mat.m3 * mat.m4 * mat.m10 * mat.m13 + mat.m3 * mat.m5 * mat.m8 * mat.m14 -
             mat.m3 * mat.m5 * mat.m10 * mat.m12 - mat.m3 * mat.m6 * mat.m8 * mat.m13 + mat.m3 * mat.m6 * mat.m9 * mat.m12;
    // @formatter:on

    return result;
}

static inline Mat4 Matrix4Transpose(Mat4 mat)
{
    Mat4 result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
}

static inline Mat4 Matrix4Add(Mat4 left, Mat4 right)
{
    Mat4 result = { 0 };

    result.m0 = left.m0 + right.m0;
    result.m1 = left.m1 + right.m1;
    result.m2 = left.m2 + right.m2;
    result.m3 = left.m3 + right.m3;
    result.m4 = left.m4 + right.m4;
    result.m5 = left.m5 + right.m5;
    result.m6 = left.m6 + right.m6;
    result.m7 = left.m7 + right.m7;
    result.m8 = left.m8 + right.m8;
    result.m9 = left.m9 + right.m9;
    result.m10 = left.m10 + right.m10;
    result.m11 = left.m11 + right.m11;
    result.m12 = left.m12 + right.m12;
    result.m13 = left.m13 + right.m13;
    result.m14 = left.m14 + right.m14;
    result.m15 = left.m15 + right.m15;

    return result;
}

static inline Mat4 Matrix4Subtract(Mat4 left, Mat4 right)
{
    Mat4 result = { 0 };

    result.m0 = left.m0 - right.m0;
    result.m1 = left.m1 - right.m1;
    result.m2 = left.m2 - right.m2;
    result.m3 = left.m3 - right.m3;
    result.m4 = left.m4 - right.m4;
    result.m5 = left.m5 - right.m5;
    result.m6 = left.m6 - right.m6;
    result.m7 = left.m7 - right.m7;
    result.m8 = left.m8 - right.m8;
    result.m9 = left.m9 - right.m9;
    result.m10 = left.m10 - right.m10;
    result.m11 = left.m11 - right.m11;
    result.m12 = left.m12 - right.m12;
    result.m13 = left.m13 - right.m13;
    result.m14 = left.m14 - right.m14;
    result.m15 = left.m15 - right.m15;

    return result;
}

static inline Mat4 Matrix4Multiply(Mat4 left, Mat4 right)
{
    Mat4 result = { 0 };

    result.m0 = left.m0 * right.m0 + left.m1 * right.m4 + left.m2 * right.m8 + left.m3 * right.m12;
    result.m1 = left.m0 * right.m1 + left.m1 * right.m5 + left.m2 * right.m9 + left.m3 * right.m13;
    result.m2 = left.m0 * right.m2 + left.m1 * right.m6 + left.m2 * right.m10 + left.m3 * right.m14;
    result.m3 = left.m0 * right.m3 + left.m1 * right.m7 + left.m2 * right.m11 + left.m3 * right.m15;
    result.m4 = left.m4 * right.m0 + left.m5 * right.m4 + left.m6 * right.m8 + left.m7 * right.m12;
    result.m5 = left.m4 * right.m1 + left.m5 * right.m5 + left.m6 * right.m9 + left.m7 * right.m13;
    result.m6 = left.m4 * right.m2 + left.m5 * right.m6 + left.m6 * right.m10 + left.m7 * right.m14;
    result.m7 = left.m4 * right.m3 + left.m5 * right.m7 + left.m6 * right.m11 + left.m7 * right.m15;
    result.m8 = left.m8 * right.m0 + left.m9 * right.m4 + left.m10 * right.m8 + left.m11 * right.m12;
    result.m9 = left.m8 * right.m1 + left.m9 * right.m5 + left.m10 * right.m9 + left.m11 * right.m13;
    result.m10 = left.m8 * right.m2 + left.m9 * right.m6 + left.m10 * right.m10 + left.m11 * right.m14;
    result.m11 = left.m8 * right.m3 + left.m9 * right.m7 + left.m10 * right.m11 + left.m11 * right.m15;
    result.m12 = left.m12 * right.m0 + left.m13 * right.m4 + left.m14 * right.m8 + left.m15 * right.m12;
    result.m13 = left.m12 * right.m1 + left.m13 * right.m5 + left.m14 * right.m9 + left.m15 * right.m13;
    result.m14 = left.m12 * right.m2 + left.m13 * right.m6 + left.m14 * right.m10 + left.m15 * right.m14;
    result.m15 = left.m12 * right.m3 + left.m13 * right.m7 + left.m14 * right.m11 + left.m15 * right.m15;

    return result;
}

static inline Mat4 Matrix4MultiplyValue(Mat4 mat, f32 f)
{
    Mat4 result = { 0 };

    result.m0 = mat.m0 * f;
    result.m1 = mat.m1 * f;
    result.m2 = mat.m2 * f;
    result.m3 = mat.m3 * f;
    result.m4 = mat.m4 * f;
    result.m5 = mat.m5 * f;
    result.m6 = mat.m6 * f;
    result.m7 = mat.m7 * f;
    result.m8 = mat.m8 * f;
    result.m9 = mat.m9 * f;
    result.m10 = mat.m10 * f;
    result.m11 = mat.m11 * f;
    result.m12 = mat.m12 * f;
    result.m13 = mat.m13 * f;
    result.m14 = mat.m14 * f;
    result.m15 = mat.m15 * f;

    return result;
}

static inline Vec4 Matrix4MultiplyVector4(Mat4 mat, Vec4 vec)
{
    Vec4 result = { 0 };

    result.x = mat.m0 * vec.x + mat.m1 * vec.y + mat.m2 * vec.z + mat.m3 * vec.w;
    result.y = mat.m4 * vec.x + mat.m5 * vec.y + mat.m6 * vec.z + mat.m7 * vec.w;
    result.z = mat.m8 * vec.x + mat.m9 * vec.y + mat.m10 * vec.z + mat.m11 * vec.w;
    result.w = mat.m12 * vec.x + mat.m13 * vec.y + mat.m14 * vec.z + mat.m15 * vec.w;

    return result;
}

static inline Vec4 Vector4MultiplyMatrix4(Vec4 vec, Mat4 mat)
{
    Vec4 result = { 0 };

    result.x = vec.x * mat.m0 + vec.y * mat.m4 + vec.z * mat.m8 + vec.w * mat.m12;
    result.y = vec.x * mat.m1 + vec.y * mat.m5 + vec.z * mat.m9 + vec.w * mat.m13;
    result.z = vec.x * mat.m2 + vec.y * mat.m6 + vec.z * mat.m10 + vec.w * mat.m14;
    result.w = vec.x * mat.m3 + vec.y * mat.m7 + vec.z * mat.m11 + vec.w * mat.m15;

    return result;
}

static inline Mat4 Matrix4Inverse(Mat4 mat)
{
    Mat4 result = { 0 };

    f32 a0 = mat.m0 * mat.m5 - mat.m4 * mat.m1;
    f32 a1 = mat.m0 * mat.m6 - mat.m4 * mat.m2;
    f32 a2 = mat.m0 * mat.m7 - mat.m4 * mat.m3;
    f32 a3 = mat.m1 * mat.m6 - mat.m5 * mat.m2;
    f32 a4 = mat.m1 * mat.m7 - mat.m5 * mat.m3;
    f32 a5 = mat.m2 * mat.m7 - mat.m6 * mat.m3;
    f32 b5 = mat.m10 * mat.m15 - mat.m14 * mat.m11;
    f32 b4 = mat.m9 * mat.m15 - mat.m13 * mat.m11;
    f32 b3 = mat.m9 * mat.m14 - mat.m13 * mat.m10;
    f32 b2 = mat.m8 * mat.m15 - mat.m12 * mat.m11;
    f32 b1 = mat.m8 * mat.m14 - mat.m12 * mat.m10;
    f32 b0 = mat.m8 * mat.m13 - mat.m12 * mat.m9;

    f32 invertDet = 1.0f / (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);

    result.m0 = (mat.m5 * b5 - mat.m6 * b4 + mat.m7 * b3) * invertDet;
    result.m1 = (-mat.m1 * b5 + mat.m2 * b4 - mat.m3 * b3) * invertDet;
    result.m2 = (mat.m13 * a5 - mat.m14 * a4 + mat.m15 * a3) * invertDet;
    result.m3 = (-mat.m9 * a5 + mat.m10 * a4 - mat.m11 * a3) * invertDet;

    result.m4 = (-mat.m4 * b5 + mat.m6 * b2 - mat.m7 * b1) * invertDet;
    result.m5 = (mat.m0 * b5 - mat.m2 * b2 + mat.m3 * b1) * invertDet;
    result.m6 = (-mat.m12 * a5 + mat.m14 * a2 - mat.m15 * a1) * invertDet;
    result.m7 = (mat.m8 * a5 - mat.m10 * a2 + mat.m11 * a1) * invertDet;

    result.m8 = (mat.m4 * b4 - mat.m5 * b2 + mat.m7 * b0) * invertDet;
    result.m9 = (-mat.m0 * b4 + mat.m1 * b2 - mat.m3 * b0) * invertDet;
    result.m10 = (mat.m12 * a4 - mat.m13 * a2 + mat.m15 * a0) * invertDet;
    result.m11 = (-mat.m8 * a4 + mat.m9 * a2 - mat.m11 * a0) * invertDet;

    result.m12 = (-mat.m4 * b3 + mat.m5 * b1 - mat.m6 * b0) * invertDet;
    result.m13 = (mat.m0 * b3 - mat.m1 * b1 + mat.m2 * b0) * invertDet;
    result.m14 = (-mat.m12 * a3 + mat.m13 * a1 - mat.m14 * a0) * invertDet;
    result.m15 = (mat.m8 * a3 - mat.m9 * a1 + mat.m10 * a0) * invertDet;

    return result;
}

static inline Mat4 MatrixTranslate(Mat4 mat, Vec3 vec)
{
    Mat4 translateMatrix = {
        1.0f, 0.0f, 0.0f, vec.x,
        0.0f, 1.0f, 0.0f, vec.y,
        0.0f, 0.0f, 1.0f, vec.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    Mat4 result = Matrix4Multiply(mat, translateMatrix);
    return result;
}

static inline Mat4 MatrixScale(Mat4 mat, Vec3 scale)
{
    Mat4 scaleMatrix = {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    Mat4 result = Matrix4Multiply(mat, scaleMatrix);
    return result;
}

static inline Mat4 MatrixRotate(Mat4 mat, f32 angle, Vec3 axis)
{
    // https://learnopengl.com/Getting-started/Transformations

    Mat4 rotationMatrix = { 0 };

    // Cache for optimization
    f32 cosAngle = cosf(angle);
    f32 sinAngle = sinf(angle);
    f32 oneMinusCosAngle = (1.0f - cosAngle);
    f32 xy = axis.x * axis.y;
    f32 xz = axis.x * axis.z;
    f32 yz = axis.y * axis.z;

    rotationMatrix.m0 = cosAngle + (axis.x * axis.x) * oneMinusCosAngle;
    rotationMatrix.m1 = xy * oneMinusCosAngle - axis.z * sinAngle;
    rotationMatrix.m2 = xz * oneMinusCosAngle + axis.y * sinAngle;
    rotationMatrix.m4 = xy * oneMinusCosAngle + axis.z * sinAngle;
    rotationMatrix.m5 = cosAngle + (axis.y * axis.y) * oneMinusCosAngle;
    rotationMatrix.m6 = yz * oneMinusCosAngle - axis.x * sinAngle;
    rotationMatrix.m8 = xz * oneMinusCosAngle - axis.y * sinAngle;
    rotationMatrix.m9 = yz * oneMinusCosAngle + axis.x * sinAngle;
    rotationMatrix.m10 = cosAngle + (axis.z * axis.z) * oneMinusCosAngle;
    rotationMatrix.m15 = 1.0f;

    Mat4 result = Matrix4Multiply(mat, rotationMatrix);
    return result;
}

static inline Mat4 MatrixOrthogonal(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    // https://en.wikipedia.org/wiki/Orthographic_projection

    Mat4 result = { 0 };

    result.m0 = 2.0f / (right - left);
    result.m3 = -((right + left) / (right - left));
    result.m5 = 2.0f / (top - bottom);
    result.m7 = -((top + bottom) / (top - bottom));
    result.m10 = -2.0f / (far - near);
    result.m11 = -((far + near) / (far - near));
    result.m15 = 1.0f;

    return result;
}

inline Mat4 operator+(const Mat4& left, const Mat4& right)
{
    Mat4 result = Matrix4Add(left, right);
    return result;
}

inline Mat4& operator+=(Mat4& left, const Mat4& right)
{
    left = Matrix4Add(left, right);
    return left;
}

inline Mat4 operator-(const Mat4& left, const Mat4& right)
{
    Mat4 result = Matrix4Subtract(left, right);
    return result;
}

inline Mat4& operator-=(Mat4& left, const Mat4& right)
{
    left = Matrix4Subtract(left, right);
    return left;
}

inline Mat4 operator*(const Mat4& left, const Mat4& right)
{
    Mat4 result = Matrix4Multiply(left, right);
    return result;
}

inline Mat4& operator*=(Mat4& left, const Mat4& right)
{
    left = Matrix4Multiply(left, right);
    return left;
}

inline Mat4 operator*(const Mat4& left, const f32& right)
{
    Mat4 result = Matrix4MultiplyValue(left, right);
    return result;
}

inline Mat4 operator*(const f32& right, const Mat4& left)
{
    Mat4 result = Matrix4MultiplyValue(left, right);
    return result;
}

inline Mat4& operator*=(Mat4& left, const f32& right)
{
    left = Matrix4MultiplyValue(left, right);
    return left;
}

inline Vec4 operator*(const Mat4& left, const Vec4& right)
{
    Vec4 result = Matrix4MultiplyVector4(left, right);
    return result;
}

inline Vec4 operator*(const Vec4& left, const Mat4& right)
{
    Vec4 result = Vector4MultiplyMatrix4(left, right);
    return result;
}

inline Vec4& operator*=(Vec4& left, const Mat4& right)
{
    left = Vector4MultiplyMatrix4(left, right);
    return left;
}