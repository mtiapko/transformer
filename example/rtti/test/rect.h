#ifndef __RECT_H__
#define __RECT_H__

#include <iostream>

#include "rtti/ObjectType.h"

struct Point2D
{
	float x;
	float y;
};

struct Point3D : Point2D
{
	float z;
	Point2D point;
};

class Rect : public rtti::ObjectType<Rect>
{
private:
	RTTI_OBJECT_TYPE_BODY;

	std::string name;
	Point3D top_left;
	Point3D bot_right;

	std::vector<float> arr;
	float arr2[5];
	char str_arr[16];

public:
	void print() noexcept
	{
		std::clog << "Rect values:\n"
			<< "\t- name:        " << this->name << '\n'

			<< "\t- bot_right.x: " << this->bot_right.x << '\n'
			<< "\t- bot_right.y: " << this->bot_right.y << '\n'
			<< "\t- bot_right.z: " << this->bot_right.z << '\n'

			<< "\t- top_left.x:  " << this->top_left.x << '\n'
			<< "\t- top_left.y:  " << this->top_left.y << '\n'
			<< "\t- top_left.z:  " << this->top_left.z << '\n'
			<< "\t- arr[]:       { ";

		for (const auto& v: arr)
			std::clog << v << ", ";

		std::clog << "}\n";

		/* arr2 */
		std::clog
			<< "\t- arr2[5]:     { ";

		for (const auto& v: arr2)
			std::clog << v << ", ";

		std::clog << "}\n";

		/* str_arr */
		std::clog
			<< "\t- str_arr[32]: \"" << str_arr << "\"\n";
	}
};

#endif // !__RECT_H__
