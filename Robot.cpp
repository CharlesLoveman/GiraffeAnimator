#include "Robot.h"


Robot::Robot(int width, int height) : Giraffe(width, height)
{
	Skeleton = { { { {0.0f, -1.0f}, {0.0f, 0.4f}, {-0.9f, -0.2f}, {-0.6f, 1.6f}, {-0.3f, 1.4f}, {0.3f, 1.6f}, {0.6f, 1.4f}, {0.9f, -1.2f}, {1.2f, -1.6f} } } };
	ControlPoints = { { { {-0.5, 0.2}, {-0.7f, 0.0f}, {-0.7f, 0.8f}, {-0.5f, 0.8f}, {-0.4f, 0.8f}, {-0.2f, 0.8f}, {0.2f, 0.8f}, {0.4f, 0.8f}, {0.5f, 0.8f}, {0.7f, 0.8f},{0.7f, 0.0f} } } };
	Children = { { {2,3,4,5,6,7,8}, {0,2,3,4,5,6,7,8}, {}, {}, {}, {}, {}, {8}, {} } };
	Parents = { { 1, 0, -1, -2, -4, -6, -8, -10, 7} };
	Distance = { { 1.0f, 0.0f, 0.36f, 0.8f, 0.8f, 0.8f, 0.8f, 1.5f, 0.5f} };

	Hurtboxes = { {{HurtCollider({1.2f, -1.35f}, 0.25f, 2.0f), HurtCollider({0.9f, -1.0f}, 0.3f, 1.5f), HurtCollider({0.7f, -0.3f}, 0.3f, 1.2f),HurtCollider({0.0f, 0.4f}, 0.7f, 1.0f),HurtCollider({-0.5f, 1.1f}, 0.3f, 0.5f),HurtCollider({0.35f, 1.1f}, 0.3f, 0.5f)}} };
	Hitboxes = { {} };
}

std::vector<Vec2> Robot::GetPoints(int i)
{
	std::vector<Vec2> points;

	Vec2 tailPerp = (Skeleton[i][2] - ControlPoints[i][1]).GetPerpendicular().Normalise();

	Vec2 leg1 = Skeleton[i][3] - (ControlPoints[i][2] + ControlPoints[i][3]) * 0.5f;
	Vec2 leg2 = Skeleton[i][4] - (ControlPoints[i][4] + ControlPoints[i][5]) * 0.5f;
	Vec2 leg3 = Skeleton[i][5] - (ControlPoints[i][6] + ControlPoints[i][7]) * 0.5f;
	Vec2 leg4 = Skeleton[i][6] - (ControlPoints[i][8] + ControlPoints[i][9]) * 0.5f;

	Vec2 neckPerp = (Skeleton[i][7] - ControlPoints[i][10]).GetPerpendicular().Normalise();

	Vec2 headPerp = (Skeleton[i][8] - Skeleton[i][7]).GetPerpendicular().Normalise();

	points.push_back(ControlPoints[i][1]);
	points.push_back(ControlPoints[i][1] + tailPerp * 0.2f);
	points.push_back(Skeleton[i][2] + tailPerp * 0.2f);
	points.push_back(Skeleton[i][2] + tailPerp * -0.2f);
	points.push_back(ControlPoints[i][1] + tailPerp * -0.2f);
	points.push_back(ControlPoints[i][1]);
	points.push_back(ControlPoints[i][2]);
	points.push_back(ControlPoints[i][2] + leg1);
	points.push_back(ControlPoints[i][3] + leg1);
	points.push_back(ControlPoints[i][3]);
	points.push_back(ControlPoints[i][4]);
	points.push_back(ControlPoints[i][4] + leg2);
	points.push_back(ControlPoints[i][5] + leg2);
	points.push_back(ControlPoints[i][5]);
	points.push_back(ControlPoints[i][6]);
	points.push_back(ControlPoints[i][6] + leg3);
	points.push_back(ControlPoints[i][7] + leg3);
	points.push_back(ControlPoints[i][7]);
	points.push_back(ControlPoints[i][8]);
	points.push_back(ControlPoints[i][8] + leg4);
	points.push_back(ControlPoints[i][9] + leg4);
	points.push_back(ControlPoints[i][9]);
	points.push_back(ControlPoints[i][10]);
	points.push_back(ControlPoints[i][10] + neckPerp * 0.15f);
	points.push_back(Skeleton[i][7] + neckPerp * 0.15f);
	points.push_back(Skeleton[i][7]);
	points.push_back(Skeleton[i][7] + headPerp * 0.2f);
	points.push_back(Skeleton[i][8] + headPerp * 0.2f);
	points.push_back(Skeleton[i][8] + headPerp * -0.2f);
	points.push_back(Skeleton[i][7] + headPerp * -0.2f);
	points.push_back(Skeleton[i][7]);
	points.push_back(Skeleton[i][7] + neckPerp * -0.15f);
	points.push_back(ControlPoints[i][10] + neckPerp * -0.15f);
	points.push_back(ControlPoints[i][10]);
	points.push_back(ControlPoints[i][1]);



	return points;
}

void Robot::DrawOutline(HDC hdc, std::vector<POINT> points)
{
	Polyline(hdc, &points[0], points.size());
}
