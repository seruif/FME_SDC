#include <cmath>

struct XSVectorF
{
public:
	float X;
	float Y;
    
	XSVectorF offset_x(float off)
	{
		//X+=off;
		XSVectorF v;
		v.X = X + off;
		v.Y = Y;
		return v;
	}
	XSVectorF offset_y(float off)
	{
		//Y+=off;
		XSVectorF v;
		v.X = X;
		v.Y = Y + off;
		return v;
	}

	XSVectorF set_x(float off)
	{
		//X+=off;
		XSVectorF v;
		v.X = off;
		v.Y = Y;
		return v;
	}
	XSVectorF set_y(float off)
	{
		//Y+=off;
		XSVectorF v;
		v.X = X;
		v.Y = off;
		return v;
	}
	void set(float x, float y)
	{
		X = x;
		Y = y;
	}

	XSVectorF operator+(const XSVectorF& f)
	{
		XSVectorF v;
		v.X = X + f.X;
		v.Y = Y + f.Y;
		return v;
	}
	XSVectorF operator-(const XSVectorF& f)
	{
		XSVectorF v;
		v.X = X - f.X;
		v.Y = Y - f.Y;
		return v;
	}

	void operator+=(const XSVectorF& f)
	{
		X += f.X;
		Y += f.Y;
	}
	void operator-=(const XSVectorF& f)
	{
		X -= f.X;
		Y -= f.Y;
	}

	XSVectorF operator*(const XSVectorF& f)
	{
		XSVectorF v;
		v.X = X * f.X;
		v.Y = Y * f.Y;
		return v;
	}
	XSVectorF operator/(const float& f)
	{
		XSVectorF v;
		v.X = X / f;
		v.Y = Y / f;
		return v;
	}

	XSVectorF operator*(const float& f)
	{
		XSVectorF v;
		v.X = X * f;
		v.Y = Y * f;
		return v;
	}

	void operator*=(const float& f)
	{
		X *= f;
		Y *= f;
	}

	void operator/=(const float& f)
	{
		X /= f;
		Y /= f;
	}

	void operator*=(const XSVectorF& f)
	{
		X *= f.X;
		Y *= f.Y;
	}

//	bool operator < (const XSVectorF& f)
//	{
//        float d = sqrt(X*X+Y*Y);
//        float df = sqrt(f.X*f.X+f.Y*f.Y);
//		if (d < df)
//            return true;
//		return false;
//	}
//
//	bool operator > (const XSVectorF& f)
//	{
//        float d = sqrt(X*X+Y*Y);
//        float df = sqrt(f.X*f.X+f.Y*f.Y);
//        if (d > df)
//            return true;
//        return false;
//	}

	XSVectorF convert_to_real_coord(XSVectorF v1, XSVectorF v2)
	{
		XSVectorF v = v2 - v1;
		v.X *= X;
		v.Y *= Y;
		return v1 + v;
	}
};

//struct XSVectorI
//{
//public:
//	int X;
//	int Y;
//};

//XSVectorI xs_vectorf_to_i(XSVectorF f)
//{
//    XSVectorI i;
//    i.X = f.X;
//    i.Y = f.Y;
//    return i;
//}
//
//XSVectorF xs_vectori_to_f(XSVectorI i)
//{
//    XSVectorF f;
//    f.X = i.X;
//    f.Y = i.Y;
//    return f;
//}