#ifndef _POSH_H_
#define _POSH_H

#include "Giraffe.h"

class Posh : public Giraffe {
public:
	Posh(int width, int height);
private:
	std::vector<Vec2> GetPoints(int i);
	void DrawOutline(HDC hdc, std::vector<POINT> points);
	HBRUSH brush;
};

#endif
