#include "Cool.h"

Cool::Cool(int width, int height) : Giraffe(width, height)
{
	Skeleton = { { { {0.0f, -1.0f}, {0.0f, 0.4f}, {-1.0f, -0.4f}, {-0.5f, 1.1f}, {-0.6f, 1.6f}, {-0.35f, 1.2f}, {-0.4f, 1.55f}, {0.35f, 1.1f}, {0.3f, 1.5f}, {0.6f, 1.1f}, {0.6f, 1.6f}, {0.7f, -0.3f}, {0.9f, -1.0f}, {1.1f, -1.3f}, {1.2f, -1.35f} } } };
	ControlPoints = { { { {-0.5f, 0.0f}, {-0.7f, 0.2f}, {-0.65f, 0.8f}, {-0.45f, 0.8f}, {-0.35f, 0.8f}, {-0.15f, 0.8f}, {0.15f, 0.8f}, {0.35f, 0.8f}, {0.45f, 0.8f}, {0.65f, 0.8f}, {0.7f, 0.2f}, {0.6f, 0.1f}, {0.5f, 0.0f} } } };
	Children = { { {2,3,4,5,6,7,8,9,10,11,12,13,14,}, {0,2,3,4,5,6,7,8,9,10,11,12,13,14}, {}, {4}, {}, {6}, {}, {8}, {}, {10}, {}, {}, {}, {14}, {}, } };
	Parents = { { 1, 0, -1, -2, 3, -4, 5, -6, 7, -8, 9, 0, 0, 0, 13} };
	Distance = { { 1.0f, 0.0f, 0.36f, 0.5f, 0.8f, 0.5f, 0.8f, 0.5f, 0.8f, 0.5f, 0.8f, 0.5f, 0.8f, 1.0f, 0.3f} };

	Hurtboxes = { {{HurtCollider({1.2f, -1.35f}, 0.25f, 2.0f), HurtCollider({0.9f, -1.0f}, 0.3f, 1.5f), HurtCollider({0.7f, -0.3f}, 0.3f, 1.2f),HurtCollider({0.0f, 0.4f}, 0.7f, 1.0f),HurtCollider({-0.5f, 1.1f}, 0.3f, 0.5f),HurtCollider({0.35f, 1.1f}, 0.3f, 0.5f)}} };
	Hitboxes = { {} };
}

std::vector<Vec2> Cool::GetPoints(int i)
{
	std::vector<Vec2> points;

	Vec2 tailPerp = (Skeleton[i][2] - ControlPoints[i][1]).GetPerpendicular().Normalise();

	Vec2 neckSeg1Perp = (Skeleton[i][11] - ControlPoints[i][7]).GetPerpendicular().Normalise();
	Vec2 neckSeg2Perp = (Skeleton[i][12] - Skeleton[i][11]).GetPerpendicular().Normalise();
	Vec2 neckSeg3Perp = (Skeleton[i][13] - Skeleton[i][12]).GetPerpendicular().Normalise();

	Vec2 headPerp = (Skeleton[i][14] - Skeleton[i][13]).GetPerpendicular().Normalise();

	//Vec2 l1s1Perp = (Skeleton[i][3] - 0.5f * (ControlPoints[i][2] + ControlPoints[i][3])).GetPerpendicular().Normalise();
	Vec2 l1s1Perp = 0.5f * ((Skeleton[i][4] - Skeleton[i][3]).GetPerpendicular().Normalise() + (Skeleton[i][3] - 0.5f * (ControlPoints[i][2] + ControlPoints[i][3])).GetPerpendicular().Normalise());
	Vec2 l1s2Perp = (Skeleton[i][4] - Skeleton[i][3]).GetPerpendicular().Normalise();

	Vec2 l2s1Perp = 0.5f * ((Skeleton[i][6] - Skeleton[i][5]).GetPerpendicular().Normalise() + (Skeleton[i][5] - 0.5f * (ControlPoints[i][4] + ControlPoints[i][5])).GetPerpendicular().Normalise());
	Vec2 l2s2Perp = (Skeleton[i][6] - Skeleton[i][5]).GetPerpendicular().Normalise();

	Vec2 l3s1Perp = 0.5f * ((Skeleton[i][8] - Skeleton[i][7]).GetPerpendicular().Normalise() + (Skeleton[i][7] - 0.5f * (ControlPoints[i][6] + ControlPoints[i][7])).GetPerpendicular().Normalise());
	Vec2 l3s2Perp = (Skeleton[i][8] - Skeleton[i][7]).GetPerpendicular().Normalise();

	Vec2 l4s1Perp = 0.5f * ((Skeleton[i][10] - Skeleton[i][9]).GetPerpendicular().Normalise() + (Skeleton[i][9] - 0.5f * (ControlPoints[i][8] + ControlPoints[i][9])).GetPerpendicular().Normalise());
	Vec2 l4s2Perp = (Skeleton[i][10] - Skeleton[i][9]).GetPerpendicular().Normalise();

	points.push_back(ControlPoints[i][0]);
	points.push_back(ControlPoints[i][1]);
	points.push_back(Skeleton[i][2] + -0.3f * tailPerp);
	points.push_back(Skeleton[i][2] + 0.3f * tailPerp);
	points.push_back(ControlPoints[i][1]);
	points.push_back(ControlPoints[i][2]);
	points.push_back(Skeleton[i][3] + l1s1Perp * 0.1f);
	points.push_back(Skeleton[i][4] + l1s2Perp * 0.1f);
	points.push_back(Skeleton[i][4] - l1s2Perp * 0.1f);
	points.push_back(Skeleton[i][3] - l1s1Perp * 0.1f);
	points.push_back(ControlPoints[i][3]);
	points.push_back(ControlPoints[i][4]);
	points.push_back(Skeleton[i][5] + l2s1Perp * 0.1f);
	points.push_back(Skeleton[i][6] + l2s2Perp * 0.1f);
	points.push_back(Skeleton[i][6] - l2s2Perp * 0.1f);
	points.push_back(Skeleton[i][5] - l2s1Perp * 0.1f);
	points.push_back(ControlPoints[i][5]);
	points.push_back(ControlPoints[i][6]);
	points.push_back(Skeleton[i][7] + l3s1Perp * 0.1f);
	points.push_back(Skeleton[i][8] + l3s2Perp * 0.1f);
	points.push_back(Skeleton[i][8] - l3s2Perp * 0.1f);
	points.push_back(Skeleton[i][7] - l3s1Perp * 0.1f);
	points.push_back(ControlPoints[i][7]);
	points.push_back(ControlPoints[i][8]);
	points.push_back(Skeleton[i][9] + l4s1Perp * 0.1f);
	points.push_back(Skeleton[i][10] + l4s2Perp * 0.1f);
	points.push_back(Skeleton[i][10] - l4s2Perp * 0.1f);
	points.push_back(Skeleton[i][9] - l4s1Perp * 0.1f);
	points.push_back(ControlPoints[i][9]);
	points.push_back(ControlPoints[i][10]);
	points.push_back(Skeleton[i][11] + 0.2f * neckSeg1Perp);
	points.push_back(Skeleton[i][12] + 0.2f * neckSeg2Perp);
	points.push_back(Skeleton[i][13] + 0.2f * neckSeg3Perp);
	points.push_back(Skeleton[i][14] + 0.2f * headPerp);
	points.push_back(2 * Skeleton[i][14] - Skeleton[i][13]);
	points.push_back(Skeleton[i][14] + -0.2f * headPerp);
	points.push_back(Skeleton[i][13] + -0.2f * neckSeg3Perp);
	points.push_back(Skeleton[i][12] + -0.2f * neckSeg2Perp);
	points.push_back(Skeleton[i][11] + -0.2f * neckSeg1Perp);
	points.push_back(ControlPoints[i][12]);
	points.push_back(ControlPoints[i][0]);



	points.push_back(points[36]);
	for (int j = 1; j < 10; ++j) {
		Vec2 bp = Bezier(&points[36], j / 10.0f);
		points.push_back(0.5f * (bp + points[39 + 2 * j]) + 0.2f * (bp - points[39 + 2 * j]).GetPerpendicular().Normalise());
		points.push_back(bp);
	}

	return points;
}

void Cool::DrawOutline(HDC hdc, std::vector<POINT> points)
{
	Polyline(hdc, &points[0], 30);
	PolyBezier(hdc, &points[29], 4);
	Polyline(hdc, &points[32], 5);
	PolyBezier(hdc, &points[36], 4);
	Polyline(hdc, &points[39], 2);
	Polyline(hdc, &points[41], 19);
}

Vec2 Cool::Bezier(Vec2* P, float t)
{
	return (1 - t) * (1 - t) * (1 - t) * P[0] + 3 * (1 - t) * (1 - t) * t * P[1] + 3 * (1 - t) * t * t * P[2] + t * t * t * P[3];
}

Vec2 Cool::BezierNorm(Vec2* P, float t)
{
	return (-3 * (1 - t) * (1 - t) * P[0] + 3 * (1 - t) * (1 - 3 * t) * P[1] + (1 - t) * t * t * P[2] + 3 * t * t * P[3]).GetPerpendicular().Normalise();
}

