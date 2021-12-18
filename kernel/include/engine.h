#ifndef ENGINE_H
#define ENGINE_H

#include "stdint.h"

#include "algorithm"
#include "cmath"
#include "vector"
#include "string"

namespace olc
{
    // Pixel Game Engine Advanced Configuration
    constexpr uint8_t  nMouseButtons = 5;
    constexpr uint8_t  nDefaultAlpha = 0xFF;
    constexpr uint32_t nDefaultPixel = (nDefaultAlpha << 24);
    enum rcode { FAIL = 0, OK = 1, NO_FILE = -1 };

    // O------------------------------------------------------------------------------O
    // | olc::Pixel - Represents a 32-Bit RGBA colour                                 |
    // O------------------------------------------------------------------------------O
    struct Pixel
    {
        union
        {
            uint32_t n = nDefaultPixel;
            struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
        };

        enum Mode { NORMAL, MASK, ALPHA, CUSTOM };

        Pixel();
        Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = nDefaultAlpha);
        Pixel(uint32_t p);
        Pixel& operator = (const Pixel& v) = default;
        bool   operator ==(const Pixel& p) const;
        bool   operator !=(const Pixel& p) const;
        Pixel  operator * (const float i) const;
        Pixel  operator / (const float i) const;
        Pixel& operator *=(const float i);
        Pixel& operator /=(const float i);
        Pixel  operator + (const Pixel& p) const;
        Pixel  operator - (const Pixel& p) const;
        Pixel& operator +=(const Pixel& p);
        Pixel& operator -=(const Pixel& p);
        Pixel  inv() const;
    };

    Pixel PixelF(float red, float green, float blue, float alpha = 1.0f);
    Pixel PixelLerp(const olc::Pixel& p1, const olc::Pixel& p2, float t);


    // O------------------------------------------------------------------------------O
    // | USEFUL CONSTANTS                                                             |
    // O------------------------------------------------------------------------------O
    static const Pixel
        GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
        RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
        YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
        GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
        CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
        BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
        MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
        WHITE(255, 255, 255), BLACK(0, 0, 0), BLANK(0, 0, 0, 0);

    // Thanks to scripticuk and others for updating the key maps
    // NOTE: The GLUT platform will need updating, open to contributions ;)
    enum Key
    {
        NONE,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        UP, DOWN, LEFT, RIGHT,
        SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
        BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
        NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
        NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD,
        EQUALS, COMMA, MINUS,
        OEM_1, OEM_2, OEM_3, OEM_4, OEM_5, OEM_6, OEM_7, OEM_8,
        CAPS_LOCK, ENUM_END
    };

    namespace Mouse
    {
        static constexpr int32_t LEFT = 0;
        static constexpr int32_t RIGHT = 1;
        static constexpr int32_t MIDDLE = 2;
    };

    // O------------------------------------------------------------------------------O
    // | olc::HWButton - Represents the state of a hardware button (mouse/key/joy)    |
    // O------------------------------------------------------------------------------O
    struct HWButton
    {
        bool bPressed = false;	// Set once during the frame the event occurs
        bool bReleased = false;	// Set once during the frame the event occurs
        bool bHeld = false;		// Set true for all frames between pressed and released events
    };


    // O------------------------------------------------------------------------------O
    // | olc::vX2d - A generic 2D vector type                                         |
    // O------------------------------------------------------------------------------O
#if !defined(OLC_IGNORE_VEC2D)
    template <class T>
    struct v2d_generic
    {
        T x = 0;
        T y = 0;
        v2d_generic() : x(0), y(0) {}
        v2d_generic(T _x, T _y) : x(_x), y(_y) {}
        v2d_generic(const v2d_generic& v) : x(v.x), y(v.y) {}
        v2d_generic& operator=(const v2d_generic& v) = default;
        T mag() const { return T(std::sqrt(x * x + y * y)); }
        T mag2() const { return x * x + y * y; }
        v2d_generic  norm() const { T r = 1 / mag(); return v2d_generic(x * r, y * r); }
        v2d_generic  perp() const { return v2d_generic(-y, x); }
        v2d_generic  floor() const { return v2d_generic(std::floor(x), std::floor(y)); }
        v2d_generic  ceil() const { return v2d_generic(std::ceil(x), std::ceil(y)); }
        v2d_generic  max(const v2d_generic& v) const { return v2d_generic(std::max(x, v.x), std::max(y, v.y)); }
        v2d_generic  min(const v2d_generic& v) const { return v2d_generic(std::min(x, v.x), std::min(y, v.y)); }
        v2d_generic  cart() { return { std::cos(y) * x, std::sin(y) * x }; }
        v2d_generic  polar() { return { mag(), std::atan2(y, x) }; }
        T dot(const v2d_generic& rhs) const { return this->x * rhs.x + this->y * rhs.y; }
        T cross(const v2d_generic& rhs) const { return this->x * rhs.y - this->y * rhs.x; }
        v2d_generic  operator +  (const v2d_generic& rhs) const { return v2d_generic(this->x + rhs.x, this->y + rhs.y); }
        v2d_generic  operator -  (const v2d_generic& rhs) const { return v2d_generic(this->x - rhs.x, this->y - rhs.y); }
        v2d_generic  operator *  (const T& rhs)           const { return v2d_generic(this->x * rhs, this->y * rhs); }
        v2d_generic  operator *  (const v2d_generic& rhs) const { return v2d_generic(this->x * rhs.x, this->y * rhs.y); }
        v2d_generic  operator /  (const T& rhs)           const { return v2d_generic(this->x / rhs, this->y / rhs); }
        v2d_generic  operator /  (const v2d_generic& rhs) const { return v2d_generic(this->x / rhs.x, this->y / rhs.y); }
        v2d_generic& operator += (const v2d_generic& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
        v2d_generic& operator -= (const v2d_generic& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
        v2d_generic& operator *= (const T& rhs) { this->x *= rhs; this->y *= rhs; return *this; }
        v2d_generic& operator /= (const T& rhs) { this->x /= rhs; this->y /= rhs; return *this; }
        v2d_generic& operator *= (const v2d_generic& rhs) { this->x *= rhs.x; this->y *= rhs.y; return *this; }
        v2d_generic& operator /= (const v2d_generic& rhs) { this->x /= rhs.x; this->y /= rhs.y; return *this; }
        v2d_generic  operator +  () const { return { +x, +y }; }
        v2d_generic  operator -  () const { return { -x, -y }; }
        bool operator == (const v2d_generic& rhs) const { return (this->x == rhs.x && this->y == rhs.y); }
        bool operator != (const v2d_generic& rhs) const { return (this->x != rhs.x || this->y != rhs.y); }
        // const std::string str() const { return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")"; }
        // friend std::ostream& operator << (std::ostream& os, const v2d_generic& rhs) { os << rhs.str(); return os; }
        operator v2d_generic<int32_t>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
        operator v2d_generic<float>() const { return { static_cast<float>(this->x), static_cast<float>(this->y) }; }
        operator v2d_generic<double>() const { return { static_cast<double>(this->x), static_cast<double>(this->y) }; }
    };

    // Note: joshinils has some good suggestions here, but they are complicated to implement at this moment, 
    // however they will appear in a future version of PGE
    template<class T> inline v2d_generic<T> operator * (const float& lhs, const v2d_generic<T>& rhs)
    { return v2d_generic<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y)); }
    template<class T> inline v2d_generic<T> operator * (const double& lhs, const v2d_generic<T>& rhs)
    { return v2d_generic<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y)); }
    template<class T> inline v2d_generic<T> operator * (const int& lhs, const v2d_generic<T>& rhs)
    { return v2d_generic<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y)); }
    template<class T> inline v2d_generic<T> operator / (const float& lhs, const v2d_generic<T>& rhs)
    { return v2d_generic<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y)); }
    template<class T> inline v2d_generic<T> operator / (const double& lhs, const v2d_generic<T>& rhs)
    { return v2d_generic<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y)); }
    template<class T> inline v2d_generic<T> operator / (const int& lhs, const v2d_generic<T>& rhs)
    { return v2d_generic<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y)); }

    // To stop dandistine crying...
    template<class T, class U> inline bool operator < (const v2d_generic<T>& lhs, const v2d_generic<U>& rhs)
    { return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x); }
    template<class T, class U> inline bool operator > (const v2d_generic<T>& lhs, const v2d_generic<U>& rhs)
    { return lhs.y > rhs.y || (lhs.y == rhs.y && lhs.x > rhs.x); }

    typedef v2d_generic<int32_t> vi2d;
    typedef v2d_generic<uint32_t> vu2d;
    typedef v2d_generic<float> vf2d;
    typedef v2d_generic<double> vd2d;
#endif

    
    // class ResourcePack : public std::streambuf
    // {
    // public:
    //     ResourcePack();
    //     ~ResourcePack();
    //     bool AddFile(const std::string& sFile);
    //     bool LoadPack(const std::string& sFile, const std::string& sKey);
    //     bool SavePack(const std::string& sFile, const std::string& sKey);
    //     ResourceBuffer GetFileBuffer(const std::string& sFile);
    //     bool Loaded();
    // private:
    //     struct sResourceFile { uint32_t nSize; uint32_t nOffset; };
    //     std::map<std::string, sResourceFile> mapFiles;
    //     std::ifstream baseFile;
    //     std::vector<char> scramble(const std::vector<char>& data, const std::string& key);
    //     std::string makeposix(const std::string& path);
    // };

    class ResourcePack
    {
        int dumbdumb;
    };

    // class ImageLoader
    // {
    // public:
    //     ImageLoader() = default;
    //     virtual ~ImageLoader() = default;
    //     virtual olc::rcode LoadImageResource(olc::Sprite* spr, const std::string& sImageFile, olc::ResourcePack* pack) = 0;
    //     virtual olc::rcode SaveImageResource(olc::Sprite* spr, const std::string& sImageFile) = 0;
    // };


    // O------------------------------------------------------------------------------O
    // | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
    // O------------------------------------------------------------------------------O
    class Sprite
    {
    public:
        Sprite();
        Sprite(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);
        Sprite(int32_t w, int32_t h);
        Sprite(const olc::Sprite&) = delete;
        ~Sprite();

    public:
        olc::rcode LoadFromFile(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);

    public:
        int32_t width = 0;
        int32_t height = 0;
        enum Mode { NORMAL, PERIODIC };
        enum Flip { NONE = 0, HORIZ = 1, VERT = 2 };

    public:
        void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
        Pixel GetPixel(int32_t x, int32_t y) const;
        bool  SetPixel(int32_t x, int32_t y, Pixel p);
        Pixel GetPixel(const olc::vi2d& a) const;
        bool  SetPixel(const olc::vi2d& a, Pixel p);
        Pixel Sample(float x, float y) const;
        Pixel SampleBL(float u, float v) const;
        Pixel* GetData();
        olc::Sprite* Duplicate();
        olc::Sprite* Duplicate(const olc::vi2d& vPos, const olc::vi2d& vSize);
        std::vector<olc::Pixel> pColData;
        Mode modeSample = Mode::NORMAL;

        // static ImageLoader* loader;
    };



} // olc



#endif // ENGINE_H