#ifndef _NORM_H_
#define _NORM_H_

#include "Giraffe.h"

class Norm : public Giraffe {
public:
	Norm(int width, int height);
private:
	std::vector<Vec2> GetPoints(int i);
};


#endif