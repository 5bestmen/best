
#include "graphics_manager.h"
#include "graph_math.h"

namespace D6KGraphics
{
	CDSTPoint CDSTEllipse::GetPoint(double dblAngle, bool bInvertY/* = TRUE*/) const
	{
		CDSTPoint pt(point);

		double cosA = cos(dblAngle);
		double sinA = sin(dblAngle);

		double r = radiusX;
		if (radiusX != radiusY)
		{
			r = 0.0;
			double dist = dst_distance(radiusY * cosA, radiusX * sinA);
			if (dist > DBL_EPSILON)
			{
				r = (radiusX * radiusY) / dist;
			}
		}

		pt.x += r * cosA;

		if (bInvertY)
		{
			pt.y -= r * sinA;
		}
		else
		{
			pt.y += r * sinA;
		}

		return pt;
	}

	std::vector<DST_COLORREF>CDSTColor::m_arrColors;

	CDSTGraphicsManager::CDSTGraphicsManager()
	{
		 
	}

	CDSTGraphicsManager::~CDSTGraphicsManager()
	{
	}

}


