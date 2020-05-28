#ifndef _ROBOT_H_
#define _ROBOT_H

#include "Giraffe.h"

class Robot : public Giraffe {
public:
	Robot(int width, int height);
private:
	std::vector<Vec2> GetPoints(int i);
	void DrawOutline(HDC hdc, std::vector<POINT> points);
};

#endif // !_ROBOT_H_
