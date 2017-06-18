#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <algorithm>
#include <vector>
#include <string>

#include "graph_global.h"


namespace D6KGraphics
{
	// DST -  D Six Thousand 
	class GRAPH_EXPORT CDSTPoint
	{
		// constructors & destructors
	public:
		CDSTPoint()
			: x(0.)
			, y(0.)
			, z(0.)
		{
		}

		CDSTPoint(double X, double Y, double Z = 0.)
			: x(X)
			, y(Y)
			, z(Z)
		{
		}

		CDSTPoint(const CDSTPoint& pt)
			: x(pt.x)
			, y(pt.y)
			, z(pt.z)
		{
		}

		// 	CDSTPoint(const CPoint& pt)
		// 		: x((double)pt.x)
		// 		, y((double)pt.y)
		// 		, z(0)
		// 	{
		// 	}

		// member functions
	public:
		void SetPoint(double X, double Y, double Z = 0.)
		{
			x = X;
			y = Y;
			z = Z;
		}

		void SetPoint(const CDSTPoint& pt)
		{
			SetPoint(pt.x, pt.y, pt.z);
		}

		void Offset(double X, double Y, double Z = 0.)
		{
			x += X;
			y += Y;
			z += Z;
		}

		void Offset(const CDSTPoint& pt)
		{
			Offset(pt.x, pt.y, pt.z);
		}

		void Swap()
		{
			double tmp = x;
			x = y;
			y = tmp;
		}

		CDSTPoint Ortho() const
		{
			return CDSTPoint(-y, x);
		}

		void Scale(double dblRatio, const CDSTPoint& ptOffset = CDSTPoint())
		{
			Scale(dblRatio, dblRatio, dblRatio, ptOffset);
		}

		void Scale(const CDSTPoint& ptRatio, const CDSTPoint& ptOffset = CDSTPoint())
		{
			Scale(ptRatio.x, ptRatio.y, ptRatio.z, ptOffset);
		}

		void Scale(double dblRatioX, double dblRatioY, double dblRatioZ, const CDSTPoint& ptOffset = CDSTPoint())
		{
			x = ptOffset.x + (x - ptOffset.x) * dblRatioX;
			y = ptOffset.y + (y - ptOffset.y) * dblRatioY;
			z = ptOffset.z + (z - ptOffset.z) * dblRatioZ;
		}

		// operators
	public:
		CDSTPoint& operator = (const CDSTPoint& pt)
		{
			x = pt.x;
			y = pt.y;
			z = pt.z;
			return *this;
		}

		const CDSTPoint& operator += (const CDSTPoint& pt)
		{
			Offset(pt.x, pt.y, pt.z);
			return *this;
		}

		const CDSTPoint& operator -= (const CDSTPoint& pt)
		{
			Offset(-pt.x, -pt.y, -pt.z);
			return *this;
		}

		const CDSTPoint& operator *= (double value)
		{
			x *= value;
			y *= value;
			z *= value;
			return *this;
		}

		const CDSTPoint& operator /= (double value)
		{
			_ASSERTE(value != 0.0);
			x /= value;
			y /= value;
			z /= value;
			return *this;
		}

		bool operator == (const CDSTPoint& pt) const
		{
			return x == pt.x && y == pt.y && z == pt.z;
		}

		bool operator != (const CDSTPoint& pt) const
		{
			return x != pt.x || y != pt.y || z != pt.z;
		}

		CDSTPoint operator + (const CDSTPoint& pt) const
		{
			return CDSTPoint(x + pt.x, y + pt.y, z + pt.z);
		}

		CDSTPoint operator - (const CDSTPoint& pt) const
		{
			return CDSTPoint(x - pt.x, y - pt.y, z - pt.z);
		}

		CDSTPoint operator - () const
		{
			return CDSTPoint(-x, -y, -z);
		}

		CDSTPoint operator * (double value) const
		{
			return CDSTPoint(x * value, y * value, z * value);
		}

		CDSTPoint operator / (double value) const
		{
			_ASSERTE(value != 0.0);
			return CDSTPoint(x / value, y / value, z / value);
		}

// 		operator CPoint() const
// 		{
// 			return CPoint((long)x, (long)y);
// 		}

	public:
		double x;
		double y;
		double z;
	};
	
	class GRAPH_EXPORT CDSTSize
	{
		// constructors & destructors
	public:
		CDSTSize()
			: cx(0.)
			, cy(0.)
		{
		}

		CDSTSize(double w, double h)
			: cx(w)
			, cy(h)
		{
		}

		CDSTSize(const CDSTSize& sz)
			: cx(sz.cx)
			, cy(sz.cy)
		{
		}

// 		CDSTSize(const CSize& sz)
// 			: cx((double)sz.cx)
// 			, cy((double)sz.cy)
// 		{
// 		}

		// public member function
	public:
		bool IsNull() const
		{
			return cx == 0.0 && cy == 0.0;
		}

		bool IsEmpty() const
		{
			return cx == 0.0 || cy == 0.0;
		}

		bool IsValid() const
		{
			return cx >= 0.0 && cy >= 0.0;
		}

		double Width() const
		{
			return cx;
		}

		double Height() const
		{
			return cy;
		}

		void SetSize(double w, double h)
		{
			cx = w;
			cy = h;
		}

		void SetSize(const CDSTSize& sz)
		{
			SetSize(sz.cx, sz.cy);
		}

		void SetSizeEmpty()
		{
			SetSize(0.0, 0.0);
		}

		void Inflate(double w, double h)
		{
			cx += w;
			cy += h;
		}

		void Inflate(const CDSTSize& sz)
		{
			Inflate(sz.cx, sz.cy);
		}

		void Deflate(double w, double h)
		{
			cx -= w;
			cy -= h;
		}

		void Deflate(const CDSTSize& sz)
		{
			Deflate(sz.cx, sz.cy);
		}

		void Swap()
		{
			double tmp = cx;
			cx = cy;
			cy = tmp;
		}

		// public operators
	public:
		const CDSTSize& operator = (const CDSTSize& sz)
		{
			cx = sz.cx;
			cy = sz.cy;
			return *this;
		}

		bool operator == (const CDSTSize& sz) const
		{
			return cx == sz.cx && cy == sz.cy;
		}

		bool operator != (const CDSTSize& sz) const
		{
			return cx != sz.cx || cy != sz.cy;
		}

		const CDSTSize& operator += (const CDSTSize& sz)
		{
			Inflate(sz.cx, sz.cy);
			return *this;
		}

		const CDSTSize& operator -= (const CDSTSize& sz)
		{
			Deflate(sz.cx, sz.cy);
			return *this;
		}

		const CDSTSize& operator *= (double value)
		{
			cx *= value;
			cy *= value;
			return *this;
		}

		const CDSTSize& operator /= (double value)
		{
		//	_ASSERTE(value != 0.0);
			cx /= value;
			cy /= value;
			return *this;
		}

		CDSTSize operator + (const CDSTSize& sz) const
		{
			return CDSTSize(cx + sz.cx, cy + sz.cy);
		}

		CDSTSize operator - (const CDSTSize& sz) const
		{
			return CDSTSize(cx - sz.cx, cy - sz.cy);
		}

		CDSTSize operator * (double value) const
		{
			return CDSTSize(cx * value, cy * value);
		}

		CDSTSize operator / (double value) const
		{
		//	_ASSERTE(value != 0.0);
			return CDSTSize(cx / value, cy / value);
		}

// 		operator CSize() const
// 		{
// 			return CSize((long)cx, (long)cy);
// 		}

	public:
		double	cx;
		double	cy;
	};
	
	class GRAPH_EXPORT CDSTRect
	{
		// constructors & destructors
	public:
		CDSTRect()
			: left(0.)
			, top(0.)
			, right(0.)
			, bottom(0.)
		{
		}

		CDSTRect(double l, double t, double r, double b)
			: left(l)
			, top(t)
			, right(r)
			, bottom(b)
		{
		}

		CDSTRect(const CDSTPoint& ptLT, const CDSTPoint& ptRB)
			: left(ptLT.x)
			, top(ptLT.y)
			, right(ptRB.x)
			, bottom(ptRB.y)
		{
		}

		CDSTRect(const CDSTPoint& pt, const CDSTSize& sz)
			: left(pt.x)
			, top(pt.y)
			, right(pt.x + sz.cx)
			, bottom(pt.y + sz.cy)
		{
		}

		CDSTRect(const CDSTRect& rt)
			: left(rt.left)
			, top(rt.top)
			, right(rt.right)
			, bottom(rt.bottom)
		{
		}

// 		CDSTRect(const CRect& rt)
// 			: left((double)rt.left)
// 			, top((double)rt.top)
// 			, right((double)rt.right)
// 			, bottom((double)rt.bottom)
// 		{
// 		}

	public:
		enum DST_RECT_POINT
		{
			DST_RECT_POINT_FIRST = 0,
			DST_RECT_POINT_TOP_LEFT = DST_RECT_POINT_FIRST,
			DST_RECT_POINT_TOP_CENTER = 1,
			DST_RECT_POINT_TOP_RIGHT = 2,
			DST_RECT_POINT_CENTER_RIGHT = 3,
			DST_RECT_POINT_BOTTOM_RIGHT = 4,
			DST_RECT_POINT_BOTTOM_CENTER = 5,
			DST_RECT_POINT_BOTTOM_LEFT = 6,
			DST_RECT_POINT_CENTER_LEFT = 7,
			DST_RECT_POINT_CENTER = 8,
			DST_RECT_POINT_NULL = 9,
			DST_RECT_POINT_LAST = DST_RECT_POINT_NULL
		};

		// public member function
	public:
		bool IsRectNull() const
		{
			return left == 0.0 && top == 0.0 && right == 0.0 && bottom == 0.0;
		}

		bool IsRectEmpty() const
		{
			return left == right || top == bottom;
		}

		bool IsValid() const
		{
			return left < right && top < bottom;
		}

		bool PtInRect(double x, double y) const
		{
			return left <= x && right >= x && top <= y && bottom >= y;
		}

		bool PtInRect(const CDSTPoint& pt) const
		{
			return PtInRect(pt.x, pt.y);
		}

		bool RtInRect(double l, double t, double r, double b) const
		{
			return left <= l && right >= r && top <= t && bottom >= b;
		}

		bool RtInRect(const CDSTRect& rt) const
		{
			return RtInRect(rt.left, rt.top, rt.right, rt.bottom);
		}

		void Normalize()
		{
			if (right < left)
			{
				SwapLeftRight();
			}

			if (bottom < top)
			{
				SwapTopBottom();
			}
		}

		CDSTRect NormalizedRect() const
		{
			CDSTRect rect(*this);
			rect.Normalize();
			return rect;
		}

		double Width() const
		{
			return right - left;
		}

		void SetWidth(double w)
		{
			right = left + w;
		}

		double Height() const
		{
			return bottom - top;
		}

		void SetHeight(double h)
		{
			bottom = top + h;
		}

		void OffsetRect(double x, double y)
		{
			left += x;
			right += x;
			top += y;
			bottom += y;
		}

		void OffsetRect(const CDSTPoint& pt)
		{
			OffsetRect(pt.x, pt.y);
		}

		void OffsetRect(const CDSTSize& sz)
		{
			OffsetRect(sz.cx, sz.cy);
		}

		void SetPoint(double x, double y)
		{
			OffsetRect(x - left, y - top);
		}

		void SetPoint(const CDSTPoint& pt)
		{
			SetPoint(pt.x, pt.y);
		}

		void SetSize(double w, double h)
		{
			right = left + w;
			bottom = top + h;
		}

		void SetSize(const CDSTSize& sz)
		{
			SetSize(sz.cx, sz.cy);
		}

		CDSTSize Size() const
		{
			return CDSTSize(Width(), Height());
		}

		void SetRect(double l, double t, double r, double b)
		{
			left = l;
			top = t;
			right = r;
			bottom = b;
		}

		void SetRect(const CDSTPoint& ptLT, const CDSTPoint& ptRB)
		{
			SetRect(ptLT.x, ptLT.y, ptRB.x, ptRB.y);
		}

		void SetRect(const CDSTPoint& pt, const CDSTSize& sz)
		{
			SetRect(pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy);
		}

		void SetRect(const CDSTRect& rt)
		{
			SetRect(rt.left, rt.top, rt.right, rt.bottom);
		}

		CDSTPoint CenterPoint() const
		{
			return CDSTPoint((left + right) / 2.0, (top + bottom) / 2.0);
		}

		double Length() const
		{
			CDSTSize sz(Size());
			return 2.0 * ((sz.cx < 0.0 ? -sz.cx : sz.cx) + (sz.cy < 0.0 ? -sz.cy : sz.cy));
		}

		double Area() const
		{
			double a = Width() * Height();
			if (a < 0.0)
			{
				a = -a;
			}

			return a;
		}

		bool IntersectRect(const CDSTRect& rt)
		{
			*this &= rt;
			return !IsRectEmpty();
		}

		bool IntersectRect(const CDSTRect& rt1, const CDSTRect& rt2)
		{
			*this = rt1 & rt2;
			return !IsRectEmpty();
		}

		void UnionRect(const CDSTRect& rt)
		{
			*this |= rt;
		}

		void UnionRect(const CDSTRect& rt1, const CDSTRect& rt2)
		{
			*this = rt1 | rt2;
		}

		CDSTPoint TopLeft() const
		{
			return CDSTPoint(left, top);
		}

		CDSTPoint TopCenter() const
		{
			return CDSTPoint((left + right) / 2.0, top);
		}

		CDSTPoint TopRight() const
		{
			return CDSTPoint(right, top);
		}

		CDSTPoint CenterRight() const
		{
			return CDSTPoint(right, (top + bottom) / 2.0);
		}

		CDSTPoint BottomRight() const
		{
			return CDSTPoint(right, bottom);
		}

		CDSTPoint BottomCenter() const
		{
			return CDSTPoint((left + right) / 2.0, bottom);
		}

		CDSTPoint BottomLeft() const
		{
			return CDSTPoint(left, bottom);
		}

		CDSTPoint CenterLeft() const
		{
			return CDSTPoint(left, (top + bottom) / 2.0);
		}

		CDSTPoint RectPoint(DST_RECT_POINT point) const
		{
			switch (point)
			{
			case DST_RECT_POINT_TOP_LEFT:
				return TopLeft();
			case DST_RECT_POINT_TOP_CENTER:
				return TopCenter();
			case DST_RECT_POINT_TOP_RIGHT:
				return TopRight();
			case DST_RECT_POINT_CENTER_RIGHT:
				return CenterRight();
			case DST_RECT_POINT_BOTTOM_RIGHT:
				return BottomRight();
			case DST_RECT_POINT_BOTTOM_CENTER:
				return BottomCenter();
			case DST_RECT_POINT_BOTTOM_LEFT:
				return BottomLeft();
			case DST_RECT_POINT_CENTER_LEFT:
				return CenterLeft();
			case DST_RECT_POINT_CENTER:
				return CenterPoint();
			}

			return CDSTPoint();
		}

		void InflateRect(double l, double t, double r, double b)
		{
			left -= l;
			top -= t;
			right += r;
			bottom += b;
		}

		void InflateRect(double x, double y)
		{
			InflateRect(x, y, x, y);
		}

		void InflateRect(const CDSTPoint& pt)
		{
			InflateRect(pt.x, pt.y, pt.x, pt.y);
		}

		void InflateRect(const CDSTSize& sz)
		{
			InflateRect(sz.cx, sz.cy, sz.cx, sz.cy);
		}

		void InflateRect(const CDSTRect& rt)
		{
			InflateRect(rt.left, rt.top, rt.right, rt.bottom);
		}

		void DeflateRect(double l, double t, double r, double b)
		{
			left += l;
			top += t;
			right -= r;
			bottom -= b;
		}

		void DeflateRect(double x, double y)
		{
			DeflateRect(x, y, x, y);
		}

		void DeflateRect(const CDSTPoint& pt)
		{
			DeflateRect(pt.x, pt.y, pt.x, pt.y);
		}

		void DeflateRect(const CDSTSize& sz)
		{
			DeflateRect(sz.cx, sz.cy, sz.cx, sz.cy);
		}

		void DeflateRect(const CDSTRect& rt)
		{
			DeflateRect(rt.left, rt.top, rt.right, rt.bottom);
		}

		void SwapLeftRight()
		{
			double tmp = left;
			left = right;
			right = tmp;
		}

		void SwapTopBottom()
		{
			double tmp = top;
			top = bottom;
			bottom = tmp;
		}

		void Swap()
		{
			SwapLeftRight();
			SwapTopBottom();
		}

		void SetRectEmpty()
		{
			left = right = top = bottom = 0.;
		}

		void Scale(const CDSTSize& szRatio, DST_RECT_POINT point = DST_RECT_POINT_NULL)
		{
			Scale(szRatio.cx, szRatio.cy, RectPoint(point));
		}

		void Scale(const CDSTSize& szRatio, const CDSTPoint& ptOffset)
		{
			Scale(szRatio.cx, szRatio.cy, ptOffset);
		}

		void Scale(double dblRatioX, double dblRatioY, const CDSTPoint& ptOffset = CDSTPoint())
		{
			CDSTSize size(Size());

			left = ptOffset.x + (left - ptOffset.x) * dblRatioX;
			top = ptOffset.y + (top - ptOffset.y) * dblRatioY;
			SetSize(size.cx * dblRatioX, size.cy * dblRatioY);
		}

		// public operators
	public:
		const CDSTRect& operator = (const CDSTRect& rt)
		{
			left = rt.left;
			top = rt.top;
			right = rt.right;
			bottom = rt.bottom;
			return *this;
		}

		bool operator == (const CDSTRect& rt) const
		{
			return left == rt.left && top == rt.top && right == rt.right && bottom == rt.bottom;
		}

		bool operator != (const CDSTRect& rt) const
		{
			return !(*this == rt);
		}

		const CDSTRect& operator &= (const CDSTRect& rt)
		{
			*this = *this & rt;
			return *this;
		}

		const CDSTRect& operator |= (const CDSTRect& rt)
		{
			*this = *this | rt;
			return *this;
		}

		const CDSTRect& operator += (const CDSTPoint& pt)
		{
			OffsetRect(pt.x, pt.y);
			return *this;
		}

		const CDSTRect& operator -= (const CDSTPoint& pt)
		{
			OffsetRect(-pt.x, -pt.y);
			return *this;
		}

		const CDSTRect& operator += (const CDSTSize& sz)
		{
			SetSize(Width() + sz.cx, Height() + sz.cy);
			return *this;
		}

		const CDSTRect& operator -= (const CDSTSize& sz)
		{
			SetSize(Width() - sz.cx, Height() - sz.cy);
			return *this;
		}

		CDSTRect operator & (const CDSTRect& rt) const
		{
			CDSTRect tmp;
			tmp.left = std::max(left, rt.left);
			tmp.right = std::min(right, rt.right);
			tmp.top = std::max(top, rt.top);
			tmp.bottom = std::min(bottom, rt.bottom);

			if (tmp.left >= tmp.right || tmp.top >= tmp.bottom)
			{
				tmp.SetRectEmpty();
			}

			return tmp;
		}

		CDSTRect operator | (const CDSTRect& rt) const
		{
			if (IsValid())
			{
				if (rt.IsValid())
				{
					CDSTRect tmp;
					tmp.left = std::min(left, rt.left);
					tmp.right = std::max(right, rt.right);
					tmp.top = std::min(top, rt.top);
					tmp.bottom = std::max(bottom, rt.bottom);
					return tmp;
				}
				else
				{
					return *this;
				}
			}
			else
			{
				CDSTRect tmp(rt);
				return tmp;
			}
		}

		CDSTRect operator + (const CDSTPoint& pt) const
		{
			CDSTRect tmp;
			tmp.left = left + pt.x;
			tmp.right = right + pt.x;
			tmp.top = top + pt.y;
			tmp.bottom = bottom + pt.y;
			return tmp;
		}

		CDSTRect operator - (const CDSTPoint& pt) const
		{
			CDSTRect tmp;
			tmp.left = left - pt.x;
			tmp.right = right - pt.x;
			tmp.top = top - pt.y;
			tmp.bottom = bottom - pt.y;
			return tmp;
		}

// 		operator CRect() const
// 		{
// 			return CRect((long)left, (long)top, (long)right, (long)bottom);
// 		}

	public:
		double	left;
		double	right;
		double	top;
		double	bottom;
	};

	class GRAPH_EXPORT CDSTRoundedRect
	{
		// constructors & destructors
	public:
		CDSTRoundedRect()
			: radiusX(0)
			, radiusY(0)
		{
		}

		CDSTRoundedRect(const CDSTRect& rectSrc, double radiusXSrc, double radiusYSrc)
			: rect(rectSrc)
			, radiusX(radiusXSrc)
			, radiusY(radiusYSrc)
		{
		}

		CDSTRoundedRect(double l, double t, double r, double b, double radiusXSrc, double radiusYSrc)
			: rect(l, t, r, b)
			, radiusX(radiusXSrc)
			, radiusY(radiusYSrc)
		{
		}

	public:
		bool IsEmpty() const
		{
			return rect.IsRectEmpty() || radiusX == 0. || radiusY == 0.;
		}

		void Scale(const CDSTSize& szRatio, CDSTRect::DST_RECT_POINT point = CDSTRect::DST_RECT_POINT_NULL)
		{
			Scale(szRatio.cx, szRatio.cy, rect.RectPoint(point));
		}

		void Scale(const CDSTSize& szRatio, const CDSTPoint& ptOffset)
		{
			Scale(szRatio.cx, szRatio.cy, ptOffset);
		}

		void Scale(double dblRatioX, double dblRatioY, const CDSTPoint& ptOffset = CDSTPoint())
		{
			rect.Scale(dblRatioX, dblRatioY, ptOffset);
			radiusX *= dblRatioX;
			radiusY *= dblRatioY;
		}

	public:
		CDSTRect	rect;
		double		radiusX;
		double		radiusY;
	};

	class GRAPH_EXPORT  CDSTEllipse
	{
		// constructors & destructors
	public:
		CDSTEllipse()
			: radiusX(0)
			, radiusY(0)
		{
		}

		CDSTEllipse(const CDSTPoint& ptSrc, double radiusXSrc, double radiusYSrc)
			: point(ptSrc)
			, radiusX(radiusXSrc)
			, radiusY(radiusYSrc)
		{
		}

		CDSTEllipse(const CDSTPoint& ptSrc, const CDSTSize& szSrc)
			: point(ptSrc)
			, radiusX(szSrc.cx * .5)
			, radiusY(szSrc.cy * .5)
		{
		}

		CDSTEllipse(const CDSTRect& rect)
			: point(rect.CenterPoint())
			, radiusX(rect.Width() * .5)
			, radiusY(rect.Height() * .5)
		{
		}

	public:
		bool IsNull() const
		{
			return radiusX <= 0. || radiusY <= 0.;
		}

		void Scale(const CDSTSize& szRatio, CDSTRect::DST_RECT_POINT point = CDSTRect::DST_RECT_POINT_NULL)
		{
			Scale(szRatio.cx, szRatio.cy, ((CDSTRect)*this).RectPoint(point));
		}

		void Scale(const CDSTSize& szRatio, const CDSTPoint& ptOffset)
		{
			Scale(szRatio.cx, szRatio.cy, ptOffset);
		}

		void Scale(double dblRatioX, double dblRatioY, const CDSTPoint& ptOffset = CDSTPoint())
		{
			point.Scale(dblRatioX, dblRatioY, 1.0, ptOffset);
			radiusX *= dblRatioX;
			radiusY *= dblRatioY;
		}

		operator CDSTRect() const { return CDSTRect(point.x - radiusX, point.y - radiusY, point.x + radiusX, point.y + radiusY); }

		// semimajor axis
		double A() const;

		// semiminor axis
		double B() const;

		// eccentricity (0 <= e < 1) 1 - error
		double E() const;

		// focal distance
		double C() const;

		// focal parameter
		double P() const;
		double Length() const;
		double Area() const;
		double GetRadius(double dblAngle) const;

		// get point of the ellipse. angle in rad
		CDSTPoint GetPoint(double dblAngle, bool bInvertY = true) const;

	public:
		CDSTPoint	point;
		double		radiusX;
		double		radiusY;
	};
	
	using DST_COLORREF = unsigned long;

	class GRAPH_EXPORT CDSTColor
	{
	public:
		enum DST_COLOR
		{
			AliceBlue = 0xF0F8FF,
			AntiqueWhite = 0xFAEBD7,
			Aqua = 0x00FEFE,
			Aquamarine = 0x7FFFD4,
			Azure = 0xF0FFFF,
			Beige = 0xF5F5DC,
			Bisque = 0xFFE4C4,
			Black = 0x000000,
			BlanchedAlmond = 0xFFEBCD,
			Blue = 0x0000FF,
			BlueViolet = 0x8A2BE2,
			Brown = 0xA52A2A,
			BurlyWood = 0xDEB887,
			CadetBlue = 0x5F9EA0,
			Chartreuse = 0x7FFF00,
			Chocolate = 0xD2691E,
			Coral = 0xFF7F50,
			CornflowerBlue = 0x6495ED,
			Cornsilk = 0xFFF8DC,
			Crimson = 0xDC143C,
			Cyan = 0x00FFFF,
			DarkBlue = 0x00008B,
			DarkCyan = 0x008B8B,
			DarkGoldenrod = 0xB8860B,
			DarkGray = 0xA9A9A9,
			DarkGreen = 0x006400,
			DarkKhaki = 0xBDB76B,
			DarkMagenta = 0x8B008B,
			DarkOliveGreen = 0x556B2F,
			DarkOrange = 0xFF8C00,
			DarkOrchid = 0x9932CC,
			DarkRed = 0x8B0000,
			DarkSalmon = 0xE9967A,
			DarkSeaGreen = 0x8FBC8F,
			DarkSlateBlue = 0x483D8B,
			DarkSlateGray = 0x2F4F4F,
			DarkTurquoise = 0x00CED1,
			DarkViolet = 0x9400D3,
			DeepPink = 0xFF1493,
			DeepSkyBlue = 0x00BFFF,
			DimGray = 0x696969,
			DodgerBlue = 0x1E90FF,
			Firebrick = 0xB22222,
			FloralWhite = 0xFFFAF0,
			ForestGreen = 0x228B22,
			Fuchsia = 0xFE00FE,
			Gainsboro = 0xDCDCDC,
			GhostWhite = 0xF8F8FF,
			Gold = 0xFFD700,
			Goldenrod = 0xDAA520,
			Gray = 0x808080,
			Green = 0x008000,
			GreenYellow = 0xADFF2F,
			Honeydew = 0xF0FFF0,
			HotPink = 0xFF69B4,
			IndianRed = 0xCD5C5C,
			Indigo = 0x4B0082,
			Ivory = 0xFFFFF0,
			Khaki = 0xF0E68C,
			Lavender = 0xE6E6FA,
			LavenderBlush = 0xFFF0F5,
			LawnGreen = 0x7CFC00,
			LemonChiffon = 0xFFFACD,
			LightBlue = 0xADD8E6,
			LightCoral = 0xF08080,
			LightCyan = 0xE0FFFF,
			LightGoldenrodYellow = 0xFAFAD2,
			LightGreen = 0x90EE90,
			LightGray = 0xD3D3D3,
			LightPink = 0xFFB6C1,
			LightSalmon = 0xFFA07A,
			LightSeaGreen = 0x20B2AA,
			LightSkyBlue = 0x87CEFA,
			LightSlateGray = 0x778899,
			LightSteelBlue = 0xB0C4DE,
			LightYellow = 0xFFFFE0,
			Lime = 0x00FF00,
			LimeGreen = 0x32CD32,
			Linen = 0xFAF0E6,
			Magenta = 0xFF00FF,
			Maroon = 0x800000,
			MediumAquamarine = 0x66CDAA,
			MediumBlue = 0x0000CD,
			MediumOrchid = 0xBA55D3,
			MediumPurple = 0x9370DB,
			MediumSeaGreen = 0x3CB371,
			MediumSlateBlue = 0x7B68EE,
			MediumSpringGreen = 0x00FA9A,
			MediumTurquoise = 0x48D1CC,
			MediumVioletRed = 0xC71585,
			MidnightBlue = 0x191970,
			MintCream = 0xF5FFFA,
			MistyRose = 0xFFE4E1,
			Moccasin = 0xFFE4B5,
			NavajoWhite = 0xFFDEAD,
			Navy = 0x000080,
			OldLace = 0xFDF5E6,
			Olive = 0x808000,
			OliveDrab = 0x6B8E23,
			Orange = 0xFFA500,
			OrangeRed = 0xFF4500,
			Orchid = 0xDA70D6,
			PaleGoldenrod = 0xEEE8AA,
			PaleGreen = 0x98FB98,
			PaleTurquoise = 0xAFEEEE,
			PaleVioletRed = 0xDB7093,
			PapayaWhip = 0xFFEFD5,
			PeachPuff = 0xFFDAB9,
			Peru = 0xCD853F,
			Pink = 0xFFC0CB,
			Plum = 0xDDA0DD,
			PowderBlue = 0xB0E0E6,
			Purple = 0x800080,
			Red = 0xFF0000,
			RosyBrown = 0xBC8F8F,
			RoyalBlue = 0x4169E1,
			SaddleBrown = 0x8B4513,
			Salmon = 0xFA8072,
			SandyBrown = 0xF4A460,
			SeaGreen = 0x2E8B57,
			SeaShell = 0xFFF5EE,
			Sienna = 0xA0522D,
			Silver = 0xC0C0C0,
			SkyBlue = 0x87CEEB,
			SlateBlue = 0x6A5ACD,
			SlateGray = 0x708090,
			Snow = 0xFFFAFA,
			SpringGreen = 0x00FF7F,
			SteelBlue = 0x4682B4,
			Tan = 0xD2B48C,
			Teal = 0x008080,
			Thistle = 0xD8BFD8,
			Tomato = 0xFF6347,
			Turquoise = 0x40E0D0,
			Violet = 0xEE82EE,
			Wheat = 0xF5DEB3,
			White = 0xFFFFFF,
			WhiteSmoke = 0xF5F5F5,
			Yellow = 0xFFFF00,
			YellowGreen = 0x9ACD32,
		};

		CDSTColor()
		{
			r = g = b = 0.;
			a = 0.;
		}

		CDSTColor(double rSrc, double gSrc, double bSrc, double aSrc = 1.)
		{
// 			ASSERT(rSrc >= 0. && rSrc <= 1.);
// 			ASSERT(gSrc >= 0. && gSrc <= 1.);
// 			ASSERT(bSrc >= 0. && bSrc <= 1.);
// 			ASSERT(aSrc >= 0. && aSrc <= 1.);

			r = rSrc;
			g = gSrc;
			b = bSrc;
			a = aSrc;
		}





		std::string ToString() const;

		double	r;
		double	g;
		double	b;
		double	a;
		protected:
			static std::vector<DST_COLORREF> m_arrColors;


	};

	class GRAPH_EXPORT CDSTGraphicsResource
	{
	public:
	};

	class GRAPH_EXPORT CDSTBrush : public CDSTGraphicsResource
	{


	};

	class GRAPH_EXPORT CDSTImage : public CDSTGraphicsResource
	{


	};

	class GRAPH_EXPORT CDSTStrokeStyle : public CDSTGraphicsResource
	{
	public:
		enum DST_CAP_STYLE
		{
			DST_CAP_STYLE_FIRST = 0,
			DST_CAP_STYLE_FLAT = DST_CAP_STYLE_FIRST,
			DST_CAP_STYLE_SQUARE = 1,
			DST_CAP_STYLE_ROUND = 2,
			DST_CAP_STYLE_TRIANGLE = 3,
			DST_CAP_STYLE_LAST = DST_CAP_STYLE_TRIANGLE
		};

		enum DST_LINE_JOIN
		{
			DST_LINE_JOIN_FIRST = 0,
			DST_LINE_JOIN_MITER = DST_LINE_JOIN_FIRST,
			DST_LINE_JOIN_BEVEL = 1,
			DST_LINE_JOIN_ROUND = 2,
			DST_LINE_JOIN_MITER_OR_BEVEL = 3,
			DST_LINE_JOIN_LAST = DST_LINE_JOIN_MITER_OR_BEVEL
		};

		enum DST_DASH_STYLE
		{
			DST_DASH_STYLE_FIRST = 0,
			DST_DASH_STYLE_SOLID = DST_DASH_STYLE_FIRST,
			DST_DASH_STYLE_DASH = 1,
			DST_DASH_STYLE_DOT = 2,
			DST_DASH_STYLE_DASH_DOT = 3,
			DST_DASH_STYLE_DASH_DOT_DOT = 4,
			DST_DASH_STYLE_CUSTOM = 5,
			DST_DASH_STYLE_LAST = DST_DASH_STYLE_CUSTOM
		};
	};

	class GRAPH_EXPORT  CDSTGraphicsManager
	{
	public:
		enum DST_GRAPHICS_MANAGER
		{
			DST_GRAPHICS_MANAGER_DEFAULT = 0,
			DST_GRAPHICS_MANAGER_GDI,           //! 基于GDI渲染
			DST_GRAPHICS_MANAGER_D2D            //! 基于D2D渲染
		};
	public:
		CDSTGraphicsManager();
		virtual ~CDSTGraphicsManager();

	public:
		virtual void DrawLine(
			const CDSTPoint& ptFrom, const CDSTPoint& ptTo, const CDSTBrush& brush,
			double lineWidth = 1., const CDSTStrokeStyle* pStrokeStyle = NULL) = 0;

	};

}

// 
// class GRAPH_EXPORT graph
// {
// public:
// 	graph();
// 	~graph();
// 
// private:
// 
// };

#endif // GRAPHICS_MANAGER_H
