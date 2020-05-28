#include "Norm.h"

Norm::Norm(int width, int height) : Giraffe(width, height)
{
	Skeleton = { { { {0.0f, -1.0f}, {0.0f, 0.4f}, {-1.0f, -0.4f}, {-0.5f, 1.1f}, {-0.6f, 1.6f}, {-0.35f, 1.2f}, {-0.4f, 1.55f}, {0.35f, 1.1f}, {0.3f, 1.5f}, {0.6f, 1.1f}, {0.6f, 1.6f}, {0.7f, -0.3f}, {0.9f, -1.0f}, {1.1f, -1.3f}, {1.2f, -1.35f} } } };
	ControlPoints = { { { {-0.5f, 0.0f}, {-0.7f, 0.2f}, {-0.5f, 0.8f}, {-0.3f, 0.8f}, {0.3f, 0.8f}, {0.5f, 0.8f}, {0.7f, 0.2f}, {0.6f, 0.1f}, {0.5f, 0.0f} } } };
	Children = { { {2,3,4,5,6,7,8,9,10,11,12,13,14}, {0,2,3,4,5,6,7,8,9,10,11,12,13,14}, {}, {4}, {}, {6}, {}, {8}, {}, {10}, {}, {}, {}, {14}, {} } };
	Parents = { { 1, 0, -1, -2, 3, -3, 5, -4, 7, -5, 9, 0, 0, 0, 13} };
	Distance = { { 1.0f, 0.0f, 0.36f, 0.3f, 0.5f, 0.3f, 0.5f, 0.3f, 0.5f, 0.3f, 0.5f, 0.5f, 0.5f, 1.0f, 0.3f} };

	Hurtboxes = { {{HurtCollider({1.2f, -1.35f}, 0.25f, 2.0f), HurtCollider({0.9f, -1.0f}, 0.3f, 1.5f), HurtCollider({0.7f, -0.3f}, 0.3f, 1.2f),HurtCollider({0.0f, 0.4f}, 0.7f, 1.0f),HurtCollider({-0.5f, 1.1f}, 0.3f, 0.5f),HurtCollider({0.35f, 1.1f}, 0.3f, 0.5f)}} };
	Hitboxes = { {} };
}

std::vector<Vec2> Norm::GetPoints(int i)
{
	std::vector<Vec2> points;

	Vec2 tailPerp = (Skeleton[i][2] - ControlPoints[i][1]).GetPerpendicular().Normalise();

	Vec2 neckSeg1Perp = (Skeleton[i][11] - ControlPoints[i][7]).GetPerpendicular().Normalise();
	Vec2 neckSeg2Perp = (Skeleton[i][12] - Skeleton[i][11]).GetPerpendicular().Normalise();
	Vec2 neckSeg3Perp = (Skeleton[i][13] - Skeleton[i][12]).GetPerpendicular().Normalise();

	Vec2 headPerp = (Skeleton[i][14] - Skeleton[i][13]).GetPerpendicular().Normalise();

	points.push_back(ControlPoints[i][0]);
	points.push_back(ControlPoints[i][1]);
	points.push_back(Skeleton[i][2] + -0.3f * tailPerp);
	points.push_back(Skeleton[i][2] + 0.3f * tailPerp);
	points.push_back(ControlPoints[i][1]);
	points.push_back(ControlPoints[i][1]);
	points.push_back(ControlPoints[i][2]);
	points.push_back(Skeleton[i][3]);
	points.push_back(Skeleton[i][4]);
	points.push_back(Skeleton[i][3]);
	points.push_back(ControlPoints[i][2]);
	points.push_back(ControlPoints[i][3]);
	points.push_back(Skeleton[i][5]);
	points.push_back(Skeleton[i][6]);
	points.push_back(Skeleton[i][5]);
	points.push_back(ControlPoints[i][3]);
	points.push_back(ControlPoints[i][4]);
	points.push_back(Skeleton[i][7]);
	points.push_back(Skeleton[i][8]);
	points.push_back(Skeleton[i][7]);
	points.push_back(ControlPoints[i][4]);
	points.push_back(ControlPoints[i][5]);
	points.push_back(Skeleton[i][9]);
	points.push_back(Skeleton[i][10]);
	points.push_back(Skeleton[i][9]);
	points.push_back(ControlPoints[i][5]);
	points.push_back(ControlPoints[i][6]);
	points.push_back(Skeleton[i][11] + 0.2f * neckSeg1Perp);
	points.push_back(Skeleton[i][12] + 0.2f * neckSeg2Perp);
	points.push_back(Skeleton[i][13] + 0.2f * neckSeg3Perp);
	points.push_back(Skeleton[i][14] + 0.2f * headPerp);
	points.push_back(2 * Skeleton[i][14] - Skeleton[i][13]);
	points.push_back(Skeleton[i][14] + -0.2f * headPerp);
	points.push_back(Skeleton[i][13] + -0.2f * neckSeg3Perp);
	points.push_back(Skeleton[i][12] + -0.2f * neckSeg2Perp);
	points.push_back(Skeleton[i][11] + -0.2f * neckSeg1Perp);
	points.push_back(ControlPoints[i][8]);
	points.push_back(ControlPoints[i][0]);

	return points;
}

void Norm::DrawOutline(HDC hdc, std::vector<POINT> points)
{
	Polyline(hdc, &points[0], 27);
	PolyBezier(hdc, &points[26], 4);
	Polyline(hdc, &points[29], 5);
	PolyBezier(hdc, &points[33], 4);
	Polyline(hdc, &points[36], 2);
}

