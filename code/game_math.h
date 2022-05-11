#pragma once

#include <stdint.h>
#include <math.h>

#define PI 3.141592654f

#pragma warning(disable : 4201)
union vec2 {
    struct {
        float x, y;
    };
    struct {
        float width, height;
    };

    float values[2];
};

union vec3 {
    struct {
        union {
            struct {
                vec2 xy;
            };
            struct {
                float x, y;
            };
        };
        float z;
    };
    struct {
        float r, g, b;
    };
    struct {
        float width, height, depth;
    };

    float values[3];
};

union vec4 {
    struct {
        union {
            struct {
                vec3 xyz;
            };
            struct {
                float x, y, z;
            };
        };
        float w;
    };
    struct {
        union {
            struct {
                vec3 rgb;
            };
            struct {
                float r, g, b;
            };
        };
        float a;
    };
    struct {
        float width, height, depth, foo;
    };

    float values[4];
};
#pragma warning(default : 4201)

vec2 Vec(float x, float y) {
    vec2 v = vec2{x, y};
    return v;
}

vec3 Vec(float x, float y, float z) {
    vec3 v = vec3{x, y, z};
    return v;
}

vec4 Vec(float r, float g, float b, float a) {
    vec4 v = vec4{r, g, b, a};
    return v;
}

vec2 VecI(int32_t x, int32_t y) {
    vec2 v = vec2{(float)x, (float)y};
    return v;
}

vec2 VecU(uint32_t x, uint32_t y) {
    vec2 v = vec2{0, 0};
    v.x = static_cast<float>(x);
    v.y = static_cast<float>(y);
    return v;
}

vec3 VecI(int32_t x, int32_t y, int32_t z) {
    vec3 v = vec3{(float)x, (float)y, (float)z};
    return v;
}

vec4 VecI(int32_t r, int32_t g, int32_t b, int32_t a) {
    vec4 v = vec4{(float)r, (float)g, (float)b, (float)a};
    return v;
}

bool veceq(float val1, float val2) {
    float diff = val2 - val1;
    if (diff < 0.0f) {
        diff = -diff;
    }
    bool isEqual = diff <= 0.0001f;
    return isEqual;
}

/********** vec2 overloads **********/
vec2 operator -(vec2 v1) {
    vec2 result = {};

    result.x = -v1.x;
    result.y = -v1.y;

    return result;
}

vec2 operator -(vec2 v1, vec2 v2) {
    vec2 result = {};

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

vec2& operator -=(vec2 &v1, vec2 v2) {
    v1 = v1 - v2;
    return v1;
}

vec2 operator +(vec2 v1, vec2 v2) {
    vec2 result = {};

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

vec2& operator +=(vec2 &v1, vec2 v2) {
    v1 = v1 + v2;
    return v1;
}

vec2 operator /(vec2 v, float divisor) {
    vec2 result = {};

    result.x = v.x / divisor;
    result.y = v.y / divisor;

    return result;
}

vec2 operator *(vec2 v, float multiplier) {
    vec2 result = {};

    result.x = v.x * multiplier;
    result.y = v.y * multiplier;

    return result;
}

vec2 operator *(float multiplier, vec2 v) {
    vec2 result = v * multiplier;
    return result;
}

vec2& operator *=(vec2 &v1, float multiplier) {
    v1 = v1 * multiplier;
    return v1;
}

vec2 operator /=(vec2 &v, float divisor) {
    v = v / divisor; 
    return v;
}

bool operator ==(vec2 v1, vec2 v2) {
    bool result = veceq(v1.x, v2.x) && veceq(v1.y, v2.y);
    return result;
}

bool operator !=(vec2 v1, vec2 v2) {
    bool result = !veceq(v1.x, v2.x) || !veceq(v1.y, v2.y);
    return result;
}

/********** vec3 overloads **********/
vec3 operator -(vec3 v1) {
    vec3 result = {};

    result.x = -v1.x;
    result.y = -v1.y;
    result.z = -v1.z;

    return result;
}

vec3 operator -(vec3 v1, vec3 v2) {
    vec3 result = {};

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

vec3& operator -=(vec3 &v1, vec3 v2) {
    v1 = v1 - v2;
    return v1;
}

vec3 operator +(vec3 v1, vec3 v2) {
    vec3 result = {};

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

vec3& operator +=(vec3 &v1, vec3 v2) {
    v1 = v1 + v2;
    return v1;
}

vec3 operator /(vec3 v, float divisor) {
    vec3 result = {};

    result.x = v.x / divisor;
    result.y = v.y / divisor;
    result.z = v.z / divisor;

    return result;
}

vec3 operator *(vec3 v, float multiplier) {
    vec3 result = {};

    result.x = v.x * multiplier;;
    result.y = v.y * multiplier;
    result.z = v.z * multiplier;

    return result;
}

vec3 operator *(float multiplier, vec3 v) {
    vec3 result = v * multiplier;
    return result;
}

vec3 operator /=(vec3 &v, float divisor) {
    v = v / divisor; 
    return v;
}

vec3& operator *=(vec3 &v1, float multiplier) {
    v1 = v1 * multiplier;
    return v1;
}

bool operator ==(vec3 v1, vec3 v2) {
    bool result = veceq(v1.x, v2.x)
        && veceq(v1.y, v2.y)
        && veceq(v1.z, v2.z);
    return result;
}

bool operator !=(vec3 v1, vec3 v2) {
    bool result = !veceq(v1.x, v2.x)
        || !veceq(v1.y, v2.y)
        || !veceq(v1.z, v2.z);
    return result;
}

/********** vec4 overloads **********/
vec4 operator -(vec4 v1) {
    vec4 result = {};

    result.x = -v1.x;
    result.y = -v1.y;
    result.z = -v1.z;
    result.w = -v1.w;

    return result;
}

vec4 operator -(vec4 v1, vec4 v2) {
    vec4 result = {};

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    result.w = v1.w - v2.w;

    return result;
}

vec4& operator -=(vec4 &v1, vec4 v2) {
    v1 = v1 - v2;
    return v1;
}

vec4 operator +(vec4 v1, vec4 v2) {
    vec4 result = {};

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = v1.w + v2.w;

    return result;
}

vec4& operator +=(vec4 &v1, vec4 v2) {
    v1 = v1 + v2;
    return v1;
}

vec4 operator /(vec4 v, float divisor) {
    vec4 result = {};

    result.x = v.x / divisor;
    result.y = v.y / divisor;
    result.z = v.z / divisor;
    result.w = v.w / divisor;

    return result;
}

vec4 operator *(vec4 v, float multiplier) {
    vec4 result = {};

    result.x = v.x * multiplier;
    result.y = v.y * multiplier;
    result.z = v.z * multiplier;
    result.w = v.w * multiplier;

    return result;
}

vec4 operator *(float multiplier, vec4 v) {
    vec4 result = v * multiplier;
    return result;
}

vec4 operator /=(vec4 &v, float divisor) {
    v = v / divisor; 
    return v;
}

vec4& operator *=(vec4 &v1, float multiplier) {
    v1 = v1 * multiplier;
    return v1;
}

bool operator ==(vec4 v1, vec4 v2) {
    bool result = veceq(v1.x, v2.x)
        && veceq(v1.y, v2.y)
        && veceq(v1.z, v2.z)
        && veceq(v1.w, v2.w);
    return result;
}

bool operator !=(vec4 v1, vec4 v2) {
    bool result = !veceq(v1.x, v2.x)
        || !veceq(v1.y, v2.y)
        || !veceq(v1.z, v2.z)
        || !veceq(v1.w, v2.w);
    return result;
}
struct rect {
    float minX;
    float maxX;
    float minY;
    float maxY;
};

static bool IsRect0(rect r) {
    bool is0 = r.minX == 0 
        && r.maxX == 0
        && r.minY == 0 
        && r.maxY == 0;
    
    return is0;
}

namespace gmath {

    float epsilon = 0.0001f;

    bool Approx0(float value) {
        return value < gmath::epsilon && value > -gmath::epsilon;
    }

    bool Approx1(float value) {
        return (value + gmath::epsilon) >= 1.0f 
            && (value - gmath::epsilon) <= 1.0f;
    }

    // Hamming Weight
    uint32_t NumBitsSet(uint32_t bitset) {
       bitset = bitset - ((bitset >> 1) & 0x55555555);
       bitset = (bitset & 0x33333333) + ((bitset >> 2) & 0x33333333);
       bitset = (bitset + (bitset >> 4)) & 0x0F0F0F0F;
       return (bitset * 0x01010101) >> 24;
    }

    // only supporting positive whole number exponents
    float Pow(float base, int32_t exp) {

        if (exp == 0) {
            return 1.0f;
        }

        float result = base;
        for (int32_t i = 0; i < (exp-1); i++) {
            result *= base;
        }

        return result;
    }

    // TODO: can we get rid of HMM?
    float Sqrt(float value) {
        float result = sqrtf(value);
        return result;
    }

    float DegsToRads(float degs) {
        float rads = degs * (PI/180.0f);
        return rads;
    }

    float RadsToDegs(float rads) {
        float degs = rads * (180.0f/PI);
        return degs;
    }

    vec3 DegsToRadsV(vec3 degs) {
        vec3 rads = {0}; 
        rads.x = degs.x * (PI/180.0f);
        rads.y = degs.y * (PI/180.0f);
        rads.z = degs.z * (PI/180.0f);
        return rads;
    }

    vec3 RadsToDegsV(vec3 rads) {
        vec3 degs = {0};
        degs.x = rads.x * (180.0f/PI);
        degs.y = rads.y * (180.0f/PI);
        degs.z = rads.z * (180.0f/PI);
        return degs;
    }

    float Cos(float radians) {
        float result = cosf(radians);
        return result;
    }

    float Sin(float radians) {
        float result = sinf(radians);
        return result;
    }

    float ACos(float radians) {
        float result = acosf(radians);
        return result;
    }

    /***** vec2 maths *****/
    float Dot(vec2 v1, vec2 v2) {
        float result = v1.x * v2.x + v1.y * v2.y;
        return result;
    }
    
    vec2 Hadamard(vec2 v1, vec2 v2) {
        vec2 result = v1;

        result.x *= v2.x;
        result.y *= v2.y;

        return result;       
    }

    float MagnitudeSquared(vec2 v1) {
        float magSquared = Dot(v1, v1);
        return magSquared;
    }

    float Magnitude(vec2 v1) {
        float magSquared = MagnitudeSquared(v1);
        float mag = Sqrt(magSquared);
        return mag;
    }

    float Lerp(float f1, float f2, float t) {
        float result = ((1.0f - t) * f1) + (t * f2);
        return result;
    }

    // NOTE: a normalize function for a vector is just the vector
    //       divided by its length (or magnitude)

    vec2 Direction(vec2 v1) {
        float mag = Magnitude(v1);
        
        if (mag == 0.0) {
            return {0};
        }

        return v1/mag;
    }

    vec2 Perp(vec2 v1) {
        vec2 v2 = {-v1.y, v1.x};
        return v2;
    }

    vec2 Lerp(vec2 v1, vec2 v2, float t) {
        vec2 result = ((1.0f - t) * v1) + (t * v2);
        return result;
    }

    vec2 Vec2FromDegrees(float degrees) {
        float radians = degrees * (PI/180.0f);
        vec2 result = {Cos(radians), Sin(radians)};
        return result;
    }
    
    vec2 ReflectSimple(vec2 velocity, float reflectPercent, vec2 collNormal, float collTime) {
        float reflectCoefficient = 1.0f + reflectPercent;
        vec2 collV = velocity * collTime + collNormal * gmath::epsilon;
        vec2 postCollV = velocity * (1.0f - collTime);
        vec2 remainderV = postCollV - reflectCoefficient * (gmath::Dot(postCollV, collNormal) * collNormal);
        velocity = collV + remainderV;
        return velocity;
    }

    /***** vec3 maths *****/
    float Dot(vec3 v1, vec3 v2) {
        float result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        return result;
    }
    
    vec3 Hadamard(vec3 v1, vec3 v2) {
        vec3 result = v1;

        result.x *= v2.x;
        result.y *= v2.y;
        result.z *= v2.z;

        return result;       
    }

    float MagnitudeSquared(vec3 v1) {
        float magSquared = Dot(v1, v1);
        return magSquared;
    }

    float Magnitude(vec3 v1) {
        float magSquared = MagnitudeSquared(v1);
        float mag = Sqrt(magSquared);
        return mag;
    }

    vec3 Direction(vec3 v1) {
        float mag = Magnitude(v1);
        
        // should this be < epsilon && > epsilon??
        if (mag == 0.0) {
            return {0};
        }

        return v1/mag;
    }

    vec3 Lerp(vec3 v1, vec3 v2, float t) {
        vec3 result = ((1.0f - t) * v1) + (t * v2);
        return result;
    }

    /***** vec4 maths *****/
    float Dot(vec4 v1, vec4 v2) {
        float result = v1.r * v2.r + v1.g * v2.g + v1.b * v2.b + v1.a * v2.a;
        return result;
    }
    
    vec4 Hadamard(vec4 v1, vec4 v2) {
        vec4 result = v1;

        result.x *= v2.x;
        result.y *= v2.y;
        result.z *= v2.z;
        result.w *= v2.z;

        return result;       
    }

    float MagnitudeSquared(vec4 v1) {
        float magSquared = Dot(v1, v1);
        return magSquared;
    }

    float Magnitude(vec4 v1) {
        float magSquared = MagnitudeSquared(v1);
        float mag = Sqrt(magSquared);
        return mag;
    }

    vec4 Direction(vec4 v1) {
        float mag = Magnitude(v1);
        
        // should this be < epsilon && > epsilon??
        if (mag == 0.0) {
            return {0};
        }

        return v1/mag;
    }

    vec4 Lerp(vec4 v1, vec4 v2, float t) {
        vec4 result = ((1.0f - t) * v1) + (t * v2);
        return result;
    }

    uint32_t MinU(uint32_t val1, uint32_t val2) {
        if (val1 <= val2) {
            return val1;
        }

        return val2;
    }

    uint32_t MaxU(uint32_t val1, uint32_t val2) {
        if (val1 >= val2) {
            return val1;
        }

        return val2;
    }

    float MinF(float val1, float val2) {
        if (val1 <= val2) {
            return val1;
        }

        return val2;
    }

    float MaxF(float val1, float val2) {
        if (val1 >= val2) {
            return val1;
        }

        return val2;
    }

    float AbsF(float val) {
        if (val < 0.0f) {
            return -val;
        }

        return val;
    }

    bool EqualsF(float val1, float val2) {
        float absDiff = AbsF(val2 - val1);
        bool isEqual = epsilon >= absDiff;
        return isEqual;
    }

    float Clamp(float value, float min, float max) {
        if (value > max) {
            return max;
        } else if (value < min) {
            return min;
        }

        return value;
    }

    int32_t ClampI(int32_t value, int32_t min, int32_t max) {
        int32_t result = static_cast<int32_t>(Clamp((float)value, (float)min, (float)max));
        return result;
    }

    float Clamp01(float value) {
        float result = Clamp(value, 0.0f, 1.0f);
        return result;
    }

    vec3 Clamp01(vec3 v) {
        vec3 clamped = {0};
        clamped.x = Clamp01(v.x);
        clamped.y = Clamp01(v.y);
        clamped.z = Clamp01(v.z);
        return clamped;
    }

    float EpsilonShift01(float value) {
        if (Approx0(value)) {
            return 0.0f;
        } else if (Approx1(value)) {
            return 1.0f;
        }

        return value;
    }

    #pragma warning(disable : 4505)
    static int32_t RoundToInt32(float value) {
        if (value >= 0.0f) {
            value += 0.5f;
        } else {
            value -= 0.5f;
        }

        return (int32_t)value;
    }

    static rect GetRect(vec2 center, vec2 diameter) {
        rect r = {};

        vec2 outerRadius = diameter * 0.5f;
        
        r.minX = center.x - outerRadius.x;
        r.maxX = center.x + outerRadius.x;
        r.minY = center.y - outerRadius.y;
        r.maxY = center.y + outerRadius.y;

        return r;
    }

    static rect GetRectPoint(vec2 center) {
        vec2 pt = vec2{epsilon, epsilon};
        rect r = GetRect(center, pt);
        return r;
    }

    static bool IsInRect(vec2 p, rect r) {
        bool result = p.x < r.maxX 
            && p.x >= r.minX 
            && p.y < r.maxY 
            && p.y >= r.minY;

        return result;
    }
    
    static bool RectsOverlap(rect r1, rect r2) {
        bool notOverlapped = IsRect0(r1) ||
                             IsRect0(r2) ||
                             r1.minX >= r2.maxX || 
                             r1.maxX < r2.minX || 
                             r1.minY >= r2.maxY || 
                             r1.maxY < r2.minY;

        return !notOverlapped;
    }

    struct m4x4 {
        float elems[16];
    };

    static vec4 ColX(m4x4 m) {
        vec4 col = {m.elems[0], m.elems[4], m.elems[8], m.elems[12]};
        return col;
    }

    static vec4 ColY(m4x4 m) {
        vec4 col = {m.elems[1], m.elems[5], m.elems[9], m.elems[13]};
        return col;
    }

    static vec4 ColZ(m4x4 m) {
        vec4 col = {m.elems[2], m.elems[6], m.elems[10], m.elems[14]};
        return col;
    }

    static vec4 ColW(m4x4 m) {
        vec4 col = {m.elems[3], m.elems[7], m.elems[11], m.elems[15]};
        return col;
    }

    static vec4 ColI(m4x4 m, uint32_t i) {
        vec4 col = {0};

        switch (i) {
            case 0: {
                col = ColX(m); 
            } break;
            case 1: {
                col = ColY(m);
            } break;
            case 2: {
                col = ColZ(m);
            } break;
            case 3: {
                col = ColW(m);
            } break;
            default: {
                int *theVoid = 0;
                *theVoid = 1;
            };
        }

        return col;
    }

    static vec4 RowX(m4x4 m) {
        vec4 row = {m.elems[0], m.elems[1], m.elems[2], m.elems[3]};
        return row;
    }

    static vec4 RowY(m4x4 m) {
        vec4 row = {m.elems[4], m.elems[5], m.elems[6], m.elems[7]};
        return row;
    }

    static vec4 RowZ(m4x4 m) {
        vec4 row = {m.elems[8], m.elems[9], m.elems[10], m.elems[11]};
        return row;
    }

    static vec4 RowW(m4x4 m) {
        vec4 row = {m.elems[12], m.elems[13], m.elems[14], m.elems[15]};
        return row;
    }

    static vec4 RowI(m4x4 m, uint32_t i) {
        vec4 row = {0};

        switch (i) {
            case 0: {
                row = RowX(m); 
            } break;
            case 1: {
                row = RowY(m);
            } break;
            case 2: {
                row = RowZ(m);
            } break;
            case 3: {
                row = RowW(m);
            } break;
            default: {
                int *theVoid = 0;
                *theVoid = 1;
            };
        }

        return row;
    }

    m4x4 Cols4x4(vec4 X, vec4 Y, vec4 Z, vec4 W) {
        m4x4 result = {X.x, Y.x, Z.x, W.x,
                       X.y, Y.y, Z.y, W.y,
                       X.z, Y.z, Z.z, W.z,
                       X.w, Y.w, Z.w, W.w};
        return result;
    }

    m4x4 Cols3x3(vec3 X, vec3 Y, vec3 Z) {
        m4x4 result = {X.x, Y.x, Z.x, 0,
                       X.y, Y.y, Z.y, 0,
                       X.z, Y.z, Z.z, 0,
                       0,   0,   0,   1};

        return result;
    }

    m4x4 Rows3x3(vec3 X, vec3 Y, vec3 Z) {
        m4x4 result = {X.x, X.y, X.z, 0,
                       Y.x, Y.y, Y.z, 0,
                       Z.x, Z.y, Z.z, 0,
                       0,   0,   0,   1};

        return result;
    }

    m4x4 operator -(m4x4 L) {
        m4x4 result = {0};
        vec4 colx = -ColX(L);
        vec4 coly = -ColY(L);
        vec4 colz = -ColZ(L);
        vec4 colw = -ColW(L);
        result = Cols4x4(colx, coly, colz, colw);
        return result;
    }

    // post multiplication
    vec3 operator *(m4x4 L, vec3 R) {
        vec3 result = {0};
        result.x = Dot(RowX(L).xyz, R);
        result.y = Dot(RowY(L).xyz, R);
        result.z = Dot(RowZ(L).xyz, R);
        return result;
    }

    // post multiplication
    vec4 operator *(m4x4 L, vec4 R) {
        vec4 result = {0};
        result.x = Dot(RowX(L), R);
        result.y = Dot(RowY(L), R);
        result.z = Dot(RowZ(L), R);
        result.w = Dot(RowW(L), R);
        return result;
    }

    // left row dot product with right col
    m4x4 operator *(m4x4 L, m4x4 R) {
        m4x4 result = {0};
        for (uint32_t rIndex = 0; rIndex < 4; rIndex++) { // rows of L
            for (uint32_t cIndex = 0; cIndex < 4; cIndex++) { // cols of R
                result.elems[(rIndex * 4) + cIndex] = Dot(RowI(L, rIndex), ColI(R, cIndex));
            }
        }
        return result;
    }

    #pragma warning(default : 4505)

    // NOTE: 
    // - right handed rotation
    // - col major 
    m4x4 RotMatX(float radians) {
        float c = Cos(radians);
        float s = Sin(radians);

#if 0
        // left handed
        m4x4 m = {
            1,  0, 0, 0,
            0,  c, s, 0,
            0,  -s, c, 0,
            0,  0, 0, 1
        };
#endif

        // right handed...
        m4x4 m = {
            1, 0,  0, 0,
            0, c, -s, 0,
            0, s,  c, 0,
            0, 0,  0, 1
        };

        return m;
    }

    // NOTE: 
    // - right handed rotation
    // - col major 
    m4x4 RotMatY(float radians) {
        float c = Cos(radians);
        float s = Sin(radians);

#if 0
        // left handed...
        m4x4 m = {
            c, 0, -s, 0,
            0, 1,  0, 0,
            s, 0,  c, 0,
            0, 0,  0, 1
        };
#endif

        // right handed...
        m4x4 m = {
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1
        };

        return m;
    }

    // NOTE: 
    // - right handed rotation
    // - col major 
    m4x4 RotMatZ(float radians) {
        float c = Cos(radians);
        float s = Sin(radians);

#if 0
        // left handed...
        m4x4 m =  {
             c, s, 0, 0,
            -s, c, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1
        };

#endif
        // right handed...
        m4x4 m = {
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };

        return m;
    }

    m4x4 Identity4x4() {
        m4x4 result = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };

        return result;
    }

    // RH, so neg into z
    m4x4 DirZ4x4() {
        m4x4 result = {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
        };

        return result;
    }

    m4x4 Transpose(m4x4 M) {
        m4x4 result = {0};
        for (uint32_t row = 0; row < 4; row++) {
            for (uint32_t col = 0; col < 4; col++) {
                // copy column values of M to row values of result
                result.elems[(row * 4) + col] = M.elems[(col * 4) + row];
            }
        }
        return result;
    }

    // puts pos + w into last col of m
    m4x4 Translate(m4x4 m, vec3 pos, float w = 1.0f) {
        m4x4 result = m;
        result.elems[(4 * 0) + 3] = pos.x;
        result.elems[(4 * 1) + 3] = pos.y;
        result.elems[(4 * 2) + 3] = pos.z;
        result.elems[(4 * 3) + 3] = w;
        return result;
    }

    m4x4 CameraTransform(m4x4 m, vec3 pos) {
        // straight transpose will not work to produce view matrix
        // 1) yes transpose the axes components (rows instead of cols)
        // 2) no transpose to the translation component (still a col)
        // 3) use camera's position negated and dot producted for the translation 
        // (which is to transform the camera's position by the matrix, negate it, and stuff resulting
        // vector into the translation column of the transposed matrix)
        vec3 colx = gmath::ColX(m).xyz;
        vec3 coly = gmath::ColY(m).xyz;
        vec3 colz = gmath::ColZ(m).xyz;
        m4x4 result = Rows3x3(colx, coly, colz);
        vec3 negTransPos = -(result * pos);
        result = Translate(result, negTransPos);
        return result;
    }

}

namespace colors {

    static vec4 Create(float r, float g, float b, float a) {
        vec4 color = vec4{r, g, b, a};
        float inv255 = 1.0f/255.0f;
        color *= inv255;
        return color;
    }

    #pragma warning(disable : 4505)
    static vec4 Create(float r, float g, float b) {
        vec4 color = Create(r, g, b, 255.0f);
        return color;
    }
    #pragma warning(default : 4505)

    static const vec4 WHITE = vec4{1.0f, 1.0f, 1.0f, 1.0f};
    static const vec4 BLACK = vec4{0.0f, 0.0f, 0.0f, 1.0f};
    static const vec4 BROWN = vec4{0.6f, 0.3f, 0.0f, 1.0f};
    static const vec4 DARK_BROWN = vec4{0.4f, 0.2f, 0.0f, 1.0f};
    static const vec4 LIGHT_BROWN = vec4{0.8f, 0.4f, 0.0f, 1.0f};
    static const vec4 RED = vec4{1.0f, 0.0f, 0.0f, 1.0f};
    static const vec4 DARK_RED = vec4{0.5, 0.0f, 0.0f, 1.0f};
    static const vec4 LIGHT_GREEN = vec4{0.5, 1.0f, 0.0f, 1.0f};
    static const vec4 GREEN = vec4{0.0f, 1.0f, 0.0f, 1.0f};
    static const vec4 MID_GREEN = vec4{0.0f, 0.8f, 0.0f, 1.0f};
    static const vec4 DARK_GREEN = vec4{0.0f, 0.6f, 0.0f, 1.0f};
    static const vec4 CYAN = vec4{0.0f, 1.0f, 1.0f, 1.0f};
    static const vec4 BLUE = vec4{0.0f, 0.0f, 1.0f, 1.0f};
    static const vec4 LIGHT_BLUE = vec4{0.275f, 0.275f, 0.9f, 1.0f};
    static const vec4 DARK_BLUE = vec4{0.0f, 0.0f, 0.6f, 1.0f};
    static const vec4 TEAL = vec4{0.45f, 1.0f, 1.0f, 1.0f};
    static const vec4 ORANGE = vec4{1.0f, 0.56, 0.0f, 1.0f};
    static const vec4 GRAY = vec4{ 0.1255, 0.1255, 0.1255, 1.0f};
    static const vec4 GRAYISH = vec4{0.3569, 0.4314, 0.8824, 1.0f};
    static const vec4 YELLOW = vec4{1.0f, 1.0f, 0.0f, 1.0f};
    static const vec4 PURPLE = vec4{0.5, 0.0f, 0.5, 1.0f};
    static const vec4 FUSCHIA = vec4{1.0f, 0.0f, 1.0f, 1.0f};
    static const vec4 GOLD = vec4{1.0f, 0.8434, 0.0f, 1.0f};
}
