#ifndef _COOL_H_
#define _COOL_H

#include "Giraffe.h"

class Cool : public Giraffe {
public:
	Cool(int width, int height);
private:
	std::vector<Vec2> GetPoints(int i);
	void DrawOutline(HDC hdc, std::vector<POINT> points);
	Vec2 Bezier(Vec2* P, float t);
	Vec2 BezierNorm(Vec2* P, float t);
};

#endif