#include <vector>

namespace math::geometry
{

struct Point2D
{
	float x;
	float y;
};

struct Point3D : public Point2D
{
	float z;
};

enum class Direction : long
{
	UP = 0,
	DOWN,

	LEFT = -10,
	RIGHT
};

class Shape
{
private:
	std::vector<Point3D> m_points;
};

}
