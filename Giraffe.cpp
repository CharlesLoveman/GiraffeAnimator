#include "Giraffe.h"
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>


Giraffe::~Giraffe()
{
	DeleteObject(SavedImage);
}

Giraffe::Giraffe(int width, int height)
{
	radius = 10;
	Scale = { 100, 100 };
	Position = { width / 2.0f, height / 2.0f };
	BoneBrush = CreateSolidBrush(RGB(125, 125, 125));
	HitBrush = CreateSolidBrush(RGB(255, 0, 0));
	HurtBrush = CreateSolidBrush(RGB(0, 0, 255));
	CPBrush = CreateSolidBrush(RGB(175, 175, 175));
	LinePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	RotateBrush = CreateSolidBrush(RGB(255, 255, 0));

	Selected = -1;
	SelectedLayer = -1;
	FrameNum = 0;
	Updated = false;
}

void Giraffe::Draw(HDC hdc, BYTE flags, const RECT &clientRect)
{
	if (flags & DRAW_SKELETON) {
		//Draw Rotator
		SelectObject(hdc, RotateBrush);
		Ellipse(hdc, Position.x + Skeleton[FrameNum][0].x * Scale.x - radius, Position.y + Skeleton[FrameNum][0].y * Scale.y - radius, Position.x + Skeleton[FrameNum][0].x * Scale.x + radius, Position.y + Skeleton[FrameNum][0].y * Scale.y + radius);
		//Draw Skeleton
		SelectObject(hdc, BoneBrush);
		for (int i = Skeleton[FrameNum].size() - 1; i > 0; --i) {
			Ellipse(hdc, Position.x + Skeleton[FrameNum][i].x * Scale.x - radius, Position.y + Skeleton[FrameNum][i].y * Scale.y - radius, Position.x + Skeleton[FrameNum][i].x * Scale.x + radius, Position.y + Skeleton[FrameNum][i].y * Scale.y + radius);
		}
	}
	if (flags & DRAW_CP) {
		//Draw Control Points
		SelectObject(hdc, CPBrush);
		for (int i = ControlPoints[FrameNum].size() - 1; i >= 0; --i) {
			Ellipse(hdc, Position.x + ControlPoints[FrameNum][i].x * Scale.x - radius, Position.y + ControlPoints[FrameNum][i].y * Scale.y - radius, Position.x + ControlPoints[FrameNum][i].x * Scale.x + radius, Position.y + ControlPoints[FrameNum][i].y * Scale.y + radius);
		}
	}
	if (flags & DRAW_OUTLINE) {
		SelectObject(hdc, LinePen);
		std::vector<Vec2> Vpoints = GetPoints(FrameNum);
		std::vector<POINT> points;
		for (int i = 0; i < Vpoints.size(); ++i) {
			points.push_back((Position + Scale * Vpoints[i]).ToPoint());
		}

		DrawOutline(hdc, points);
	}
	if (flags & DRAW_PREV) {
		DisplayImage(hdc, clientRect);
	}
	if (flags & DRAW_HURT) {
		SelectObject(hdc, HurtBrush);
		for (int i = 0; i < Hurtboxes[FrameNum].size(); ++i) {
			Ellipse(hdc, Position.x + (Hurtboxes[FrameNum][i].Position.x - Hurtboxes[FrameNum][i].Radius) * Scale.x, Position.y + (Hurtboxes[FrameNum][i].Position.y - Hurtboxes[FrameNum][i].Radius) * Scale.y, Position.x + (Hurtboxes[FrameNum][i].Position.x + Hurtboxes[FrameNum][i].Radius) * Scale.x, Position.y + (Hurtboxes[FrameNum][i].Position.y + Hurtboxes[FrameNum][i].Radius) * Scale.y);
		}
	}
	if (flags & DRAW_HIT) {
		SelectObject(hdc, HitBrush);
		for (int i = 0; i < Hitboxes[FrameNum].size(); ++i) {
			Ellipse(hdc, Position.x + (Hitboxes[FrameNum][i].Position.x - Hitboxes[FrameNum][i].Radius) * Scale.x, Position.y + (Hitboxes[FrameNum][i].Position.y - Hitboxes[FrameNum][i].Radius) * Scale.y, Position.x + (Hitboxes[FrameNum][i].Position.x + Hitboxes[FrameNum][i].Radius) * Scale.x, Position.y + (Hitboxes[FrameNum][i].Position.y + Hitboxes[FrameNum][i].Radius) * Scale.y);
		}
	}
}

void Giraffe::Serialize(LPCSTR filepath)
{
	std::ofstream file(filepath);

	file << "SkelPoints[x] = {";
	std::string str;

	for (int i = 0; i < Skeleton.size(); ++i) {
		std::vector<Vec2> points = GetPoints(i);

		file << "{{";
		for (int p = 0; p < points.size(); ++p) {
			str = std::to_string(points[p].y);
			if (str.find('.', 0) == std::string::npos) {
				str += ".0f,";
			}

			file << "{" << FloatToString(points[p].x) << "," << FloatToString(points[p].y) << "},";
		}
		file << "}},";
	}
	file << "};\n";

	file << "Hurtboxes[x] = {";
	for (int i = 0; i < Hurtboxes.size(); ++i) {
		file << "{{";
		for (int h = 0; h < Hurtboxes[i].size(); ++h) {
			file << "HurtCollider({" << FloatToString(Hurtboxes[i][h].Position.x) << "," << FloatToString(Hurtboxes[i][h].Position.y) << "}," << FloatToString(Hurtboxes[i][h].Radius) << "," << FloatToString(Hurtboxes[i][h].DamageMultiplier) << "),";
		}
		file << "}},";
	}
	file << "};\n";

	file << "Hitboxes[x] = {";
	for (int i = 0; i < Hitboxes.size(); ++i) {
		file << "{";
		for (int h = 0; h < Hitboxes[i].size(); ++h) {
			file << "HitCollider({" << FloatToString(Hitboxes[i][h].Position.x) << "," << FloatToString(Hitboxes[i][h].Position.y) << "}," << FloatToString(Hitboxes[i][h].Radius) << ",{" << FloatToString(Hitboxes[i][h].Force.x) << "," << FloatToString(Hitboxes[i][h].Force.y) << "}," << FloatToString(Hitboxes[i][h].Damage) << "," << FloatToString(Hitboxes[i][h].Knockback) << "," << FloatToString(Hitboxes[i][h].Scale) << "," << Hitboxes[i][h].Fixed <<"),";
		}
		file << "},";
	}
	file << "};\n";
}

void Giraffe::Save(LPCSTR filepath)
{
	std::ofstream file(filepath);
	int maxFrames = Skeleton.size();
	file << maxFrames << "\n" << radius << "\n" << Skeleton[0].size() << "\n" << ControlPoints[0].size() << "\n" << Hurtboxes[0].size() << "\n";

	for (int f = 0; f < maxFrames; ++f) {
		for (int i = 0; i < Skeleton[f].size(); ++i) {
			file << Skeleton[f][i].x << "\n" << Skeleton[f][i].y << "\n";
		}
		for (int i = 0; i < ControlPoints[f].size(); ++i) {
			file << ControlPoints[f][i].x << "\n" << ControlPoints[f][i].y << "\n";
		}
		for (int i = 0; i < Hurtboxes[f].size(); ++i) {
			file << Hurtboxes[f][i].Position.x << "\n" << Hurtboxes[f][i].Position.y << "\n" << Hurtboxes[f][i].Radius << "\n" << Hurtboxes[f][i].DamageMultiplier << "\n";
		}
		file << Hitboxes[f].size() << "\n";
		for (int i = 0; i < Hitboxes[f].size(); ++i) {
			file << Hitboxes[f][i].Position.x << "\n" << Hitboxes[f][i].Position.y << "\n" << Hitboxes[f][i].Radius << "\n" << Hitboxes[f][i].Force.x << "\n" << Hitboxes[f][i].Force.y << "\n" << Hitboxes[f][i].Damage << "\n" << Hitboxes[f][i].Knockback << "\n" << Hitboxes[f][i].Scale << "\n" << Hitboxes[f][i].Fixed << "\n";
		}
	}
}

void Giraffe::Load(LPCSTR filepath)
{
	std::ifstream file(filepath);
	std::string line;

	std::getline(file, line);
	int maxFrames = std::stoi(line);

	std::getline(file, line);
	radius = std::stof(line);

	std::getline(file, line);
	int skelSize = std::stoi(line);

	std::getline(file, line);
	int cpSize = std::stoi(line);

	std::getline(file, line);
	int hurtSize = std::stoi(line);

	float x;
	float y;
	float r;
	float x2;
	float y2;
	float dmg;
	float knk = 1.0f;
	float scl = 1.0f;
	bool fix = false;

	Skeleton.clear();
	ControlPoints.clear();
	Hitboxes.clear();
	Hurtboxes.clear();

	for (int f = 0; f < maxFrames; ++f) {
		std::vector<Vec2> skel;
		for (int i = 0; i < skelSize; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			skel.push_back({ x, y });
		}
		Skeleton.push_back(skel);
		
		std::vector<Vec2> cp;
		for (int i = 0; i < cpSize; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			cp.push_back({ x, y });
		}
		ControlPoints.push_back(cp);

		std::array<HurtCollider, 6> hurt;
		for (int i = 0; i < hurtSize; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			std::getline(file, line);
			r = std::stof(line);
			std::getline(file, line);
			dmg = std::stof(line);
			hurt[i] = HurtCollider({ x, y }, r, dmg);
		}
		Hurtboxes.push_back(hurt);

		std::vector<HitCollider> hit;
		int hits;
		std::getline(file, line);
		hits = std::stoi(line);
		for (int i = 0; i < hits; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			std::getline(file, line);
			r = std::stof(line);
			std::getline(file, line);
			x2 = std::stof(line);
			std::getline(file, line);
			y2 = std::stof(line);
			std::getline(file, line);
			dmg = std::stof(line);
			std::getline(file, line);
			knk = std::stof(line);
			std::getline(file, line);
			scl = std::stof(line);
			std::getline(file, line);
			fix = std::stoi(line);

			hit.push_back(HitCollider({ x, y }, r, { x2, y2 }, dmg, knk, scl, fix));
		}
		Hitboxes.push_back(hit);
	}
	FrameNum = 0;
	Selected = -1;
	SelectedLayer = -1;
	Updated = false;
}

void Giraffe::Merge(LPCSTR filepath)
{
	std::ifstream file(filepath);
	std::string line;

	std::getline(file, line);
	int maxFrames = std::stoi(line);

	std::getline(file, line);
	radius = std::stof(line);

	std::getline(file, line);
	int skelSize = std::stoi(line);

	std::getline(file, line);
	int cpSize = std::stoi(line);

	std::getline(file, line);
	int hurtSize = std::stoi(line);

	float x;
	float y;
	float r;
	float x2;
	float y2;
	float dmg;
	float knk;
	float scl;
	bool fix;

	for (int f = 0; f < maxFrames; ++f) {
		std::vector<Vec2> skel;
		for (int i = 0; i < skelSize; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			skel.push_back({ x, y });
		}
		Skeleton.push_back(skel);

		std::vector<Vec2> cp;
		for (int i = 0; i < cpSize; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			cp.push_back({ x, y });
		}
		ControlPoints.push_back(cp);

		std::array<HurtCollider, 6> hurt;
		for (int i = 0; i < hurtSize; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			std::getline(file, line);
			r = std::stof(line);
			std::getline(file, line);
			dmg = std::stof(line);
			hurt[i] = HurtCollider({ x, y }, r, dmg);
		}
		Hurtboxes.push_back(hurt);

		std::vector<HitCollider> hit;
		int hits;
		std::getline(file, line);
		hits = std::stoi(line);
		for (int i = 0; i < hits; ++i) {
			std::getline(file, line);
			x = std::stof(line);
			std::getline(file, line);
			y = std::stof(line);
			std::getline(file, line);
			r = std::stof(line);
			std::getline(file, line);
			x2 = std::stof(line);
			std::getline(file, line);
			y2 = std::stof(line);
			std::getline(file, line);
			dmg = std::stof(line);
			std::getline(file, line);
			knk = std::stof(line);
			std::getline(file, line);
			scl = std::stof(line);
			std::getline(file, line);
			fix = std::stoi(line);

			hit.push_back(HitCollider({ x, y }, r, { x2, y2 }, dmg, knk, scl, fix));
		}
		Hitboxes.push_back(hit);
	}
}

bool Giraffe::Select(int x, int y, BYTE flags, bool click)
{

	Vec2 clickPos = { (float)x, (float)y };
	clickPos -= Position;
	clickPos /= Scale;

	LastClick = click;


	//Check Hitboxes, then Hurtboxes, then Skeleton
	if (flags & DRAW_HIT) {
		for (int i = 0; i < Hitboxes[FrameNum].size(); ++i) {
			if ((Hitboxes[FrameNum][i].Position - clickPos).Length() < Hitboxes[FrameNum][i].Radius) {
				Selected = i;
				SelectedLayer = 1;
				return true;
			}
		}
	}
	if (flags & DRAW_HURT) {
			for (int i = 0; i < Hurtboxes[FrameNum].size(); ++i) {
				if ((Hurtboxes[FrameNum][i].Position - clickPos).Length() < Hurtboxes[FrameNum][i].Radius) {
					Selected = i;
					SelectedLayer = 2;
					return true;
				}
			}
		}
	if (!click) {
		if (flags & DRAW_SKELETON) {
			for (int i = 0; i < Skeleton[FrameNum].size(); ++i) {
				if ((Skeleton[FrameNum][i] - clickPos).Length() < 0.1) {
					Selected = i;
					SelectedLayer = 3;
					return true;
				}
			}
		}
	}

	return false;
}

void Giraffe::Deselect()
{
	Selected = -1;
	SelectedLayer = -1;
}

void Giraffe::Update(int x, int y)
{
	Updated = true;
	Vec2 mousePos = { (float)x, (float)y };
	mousePos -= Position;
	mousePos /= Scale;

	Vec2 deltaPos;

	if (Selected >= 0) {
		switch (SelectedLayer)
		{
		case 1://Hitboxes
			if (LastClick) {
				Hitboxes[FrameNum][Selected].Radius = (mousePos - Hitboxes[FrameNum][Selected].Position).Length();
			}
			else {
				Hitboxes[FrameNum][Selected].Position = mousePos;
			}
			break;
		case 2://Hurtboxes
			if (LastClick) {
				Hurtboxes[FrameNum][Selected].Radius = (mousePos - Hurtboxes[FrameNum][Selected].Position).Length();
			}
			else {
				Hurtboxes[FrameNum][Selected].Position = mousePos;
			}
			break;
		case 3://Skeleton
			if (Parents[Selected] != 0) {
				Vec2 parent = GetParent(Selected);
				Vec2 delta = mousePos - parent;
				float dist = delta.Length();
				Vec2 dir = (1.0f / dist) * delta;
				Vec2 prevdir = (Skeleton[FrameNum][Selected] - parent).Normalise();
				float dot = dir.x * -prevdir.y + dir.y * prevdir.x;
				float costheta = dir.Dot(prevdir);
				//float theta = acosf(costheta - 0.00001);
				float sintheta = sinf(acosf(costheta - 0.00001));
				if (dot < 0) {
					float theta = -acosf(costheta - 0.00001);
					//costheta = cosf(theta);
					sintheta = sinf(theta);
				}
				for (int i = 0; i < Children[Selected].size(); ++i) {
					Skeleton[FrameNum][Children[Selected][i]] -= parent;
					Skeleton[FrameNum][Children[Selected][i]] = { costheta * Skeleton[FrameNum][Children[Selected][i]].x - sintheta * Skeleton[FrameNum][Children[Selected][i]].y, sintheta * Skeleton[FrameNum][Children[Selected][i]].x + costheta * Skeleton[FrameNum][Children[Selected][i]].y };
					Skeleton[FrameNum][Children[Selected][i]] += parent;
				}
				if (Selected == 0) {
					for (int i = 0; i < ControlPoints[FrameNum].size(); ++i) {
						ControlPoints[FrameNum][i] -= Skeleton[FrameNum][1];
						ControlPoints[FrameNum][i] = { costheta * ControlPoints[FrameNum][i].x - sintheta * ControlPoints[FrameNum][i].y, sintheta * ControlPoints[FrameNum][i].x + costheta * ControlPoints[FrameNum][i].y };
						ControlPoints[FrameNum][i] += Skeleton[FrameNum][1];
					}
					for (int i = 0; i < Hurtboxes[FrameNum].size(); ++i) {
						Hurtboxes[FrameNum][i].Position -= Skeleton[FrameNum][1];
						Hurtboxes[FrameNum][i].Position = { costheta * Hurtboxes[FrameNum][i].Position.x - sintheta * Hurtboxes[FrameNum][i].Position.y, sintheta * Hurtboxes[FrameNum][i].Position.x + costheta * Hurtboxes[FrameNum][i].Position.y };
						Hurtboxes[FrameNum][i].Position += Skeleton[FrameNum][1];
					}
					for (int i = 0; i < Hitboxes[FrameNum].size(); ++i) {
						Hitboxes[FrameNum][i].Position -= Skeleton[FrameNum][1];
						Hitboxes[FrameNum][i].Position = { costheta * Hitboxes[FrameNum][i].Position.x - sintheta * Hitboxes[FrameNum][i].Position.y, sintheta * Hitboxes[FrameNum][i].Position.x + costheta * Hitboxes[FrameNum][i].Position.y };
						Hitboxes[FrameNum][i].Position += Skeleton[FrameNum][1];
					}
				}
				if (dist > Distance[Selected]) {
					delta = dir * Distance[Selected];
				}
				Skeleton[FrameNum][Selected] = parent + delta;
			}
			else {
				deltaPos = mousePos - Skeleton[FrameNum][Selected];
				Skeleton[FrameNum][Selected] = mousePos;
				for (int i = 0; i < Children[Selected].size(); ++i) {
					Skeleton[FrameNum][Children[Selected][i]] += deltaPos;
				}
				if (Selected == 1) {
					for (int i = 0; i < ControlPoints[FrameNum].size(); ++i) {
						ControlPoints[FrameNum][i] += deltaPos;
					}
					for (int i = 0; i < Hurtboxes[FrameNum].size(); ++i) {
						Hurtboxes[FrameNum][i].Position += deltaPos;
					}
					for (int i = 0; i < Hitboxes[FrameNum].size(); ++i) {
						Hitboxes[FrameNum][i].Position += deltaPos;
					}
				}
			}

		default:
			break;
		}
	}
}

bool Giraffe::NextFrame(HWND hwnd, const RECT &clientRect)
{
	if (Updated) {
		CaptureImage(hwnd, clientRect);
	}
	Updated = false;
	if (FrameNum == Skeleton.size() - 1) {
		Skeleton.push_back(Skeleton[FrameNum]);
		ControlPoints.push_back(ControlPoints[FrameNum]);
		Hitboxes.push_back(Hitboxes[FrameNum]);
		Hurtboxes.push_back(Hurtboxes[FrameNum]);
		++FrameNum;
		return true;
	}
	++FrameNum;
	return false;
}

bool Giraffe::PrevFrame(HWND hwnd, const RECT& clientRect)
{
	if (Updated) {
		CaptureImage(hwnd, clientRect);
	}
	Updated = false;
	if (FrameNum > 0) {
		--FrameNum;
		return true;
	}
	return false;
}

bool Giraffe::SetFrame(int i)
{
	if (i >= 0 && i < Skeleton.size()) {
		FrameNum = i;
		return true;
	}
	return false;
}

void Giraffe::CopyFrameToNext()
{
	if(FrameNum < Skeleton.size() - 2) {
		Skeleton[FrameNum + 1] = Skeleton[FrameNum];
		ControlPoints[FrameNum + 1] = ControlPoints[FrameNum];
		Hitboxes[FrameNum + 1] = Hitboxes[FrameNum];
		Hurtboxes[FrameNum + 1] = Hurtboxes[FrameNum];
	}
	else {
		Skeleton.push_back(Skeleton[FrameNum]);
		ControlPoints.push_back(ControlPoints[FrameNum]);
		Hitboxes.push_back(Hitboxes[FrameNum]);
		Hurtboxes.push_back(Hurtboxes[FrameNum]);
	}
	++FrameNum;
}

void Giraffe::InsertFrame(HWND hwnd, const RECT& clientRect)
{
	if (Updated) {
		CaptureImage(hwnd, clientRect);
	}
	Updated = false;
	Skeleton.emplace(Skeleton.begin() + FrameNum+1, Skeleton[FrameNum]);
	ControlPoints.emplace(ControlPoints.begin() + FrameNum + 1, ControlPoints[FrameNum]);
	Hitboxes.emplace(Hitboxes.begin() + FrameNum + 1, Hitboxes[FrameNum]);
	Hurtboxes.emplace(Hurtboxes.begin() + FrameNum + 1, Hurtboxes[FrameNum]);
	++FrameNum;
}

void Giraffe::DeleteFrame()
{
	if (Skeleton.size() > 1) {
		Skeleton.erase(Skeleton.begin() + FrameNum);
		ControlPoints.erase(ControlPoints.begin() + FrameNum);
		Hitboxes.erase(Hitboxes.begin() + FrameNum);
		Hurtboxes.erase(Hurtboxes.begin() + FrameNum);

		if (FrameNum == Skeleton.size()) {
			--FrameNum;
		}
	}
}

int Giraffe::GetFrameNum()
{
	return FrameNum;
}

int Giraffe::GetMaxFrames()
{
	return Skeleton.size();
}

void Giraffe::AddRemoveHitbox(int x, int y)
{
	Updated = true;
	Vec2 mousePos = { (float)x, (float)y };
	mousePos -= Position;
	mousePos /= Scale;

	for (int i = Hitboxes[FrameNum].size() - 1; i >= 0; --i) {
		if ((Hitboxes[FrameNum][i].Position - mousePos).Length() < Hitboxes[FrameNum][i].Radius) {
			Hitboxes[FrameNum].erase(Hitboxes[FrameNum].begin() + i);
			return;
		}
	}
	

	Hitboxes[FrameNum].push_back(HitCollider(mousePos, 1.0f, Vec2(1.0f, 1.0f), 1.0f, 1.0f, 1.0f, false));
}

int Giraffe::CheckIntersect(int x, int y, BYTE flags, int &index)
{
	Vec2 mousePos = { (float)x, (float)y };
	mousePos -= Position;
	mousePos /= Scale;

	if (flags & DRAW_HIT) {
		for (int i = Hitboxes[FrameNum].size() - 1; i >= 0; --i) {
			if ((Hitboxes[FrameNum][i].Position - mousePos).Length() < Hitboxes[FrameNum][i].Radius) {
				index = i;
				return 1;
			}
		}
	}
	if (flags & DRAW_HURT) {
		for (int i = Hurtboxes[FrameNum].size() - 1; i >= 0; --i) {
			if ((Hurtboxes[FrameNum][i].Position - mousePos).Length() < Hurtboxes[FrameNum][i].Radius) {
				index = i;
				return 2;
			}
		}
	}
	return 0;
}

void Giraffe::SetHurtboxData(float DmgMult, int i)
{
	Hurtboxes[FrameNum][i].DamageMultiplier = DmgMult;
}

float Giraffe::GetHurtboxData(int i)
{
	return Hurtboxes[FrameNum][i].DamageMultiplier;
}

void Giraffe::SetHitboxData(float Damage, Vec2 Force, float Knockback, float Scale, bool Fixed, int i)
{
	Hitboxes[FrameNum][i].Damage = Damage;
	Hitboxes[FrameNum][i].Force = Force;
	Hitboxes[FrameNum][i].Knockback = Knockback;
	Hitboxes[FrameNum][i].Scale = Scale;
	Hitboxes[FrameNum][i].Fixed = Fixed;
}

void Giraffe::GetHitboxData(float& Damage, Vec2& Force, float& Knockback, float& Scale, bool& Fixed, int i)
{
	Damage = Hitboxes[FrameNum][i].Damage;
	Force = Hitboxes[FrameNum][i].Force;
	Knockback = Hitboxes[FrameNum][i].Knockback;
	Scale = Hitboxes[FrameNum][i].Scale;
	Fixed = Hitboxes[FrameNum][i].Fixed;
}

void Giraffe::FlipX()
{
	for (int i = 0; i < Skeleton[FrameNum].size(); ++i) {
		Skeleton[FrameNum][i].x *= -1;
	}
	for (int i = 0; i < ControlPoints[FrameNum].size(); ++i) {
		ControlPoints[FrameNum][i].x *= -1;
	}
	for (int i = 0; i < Hurtboxes[FrameNum].size(); ++i) {
		Hurtboxes[FrameNum][i].Position.x *= -1;
	}
	for (int i = 0; i < Hitboxes[FrameNum].size(); ++i) {
		Hitboxes[FrameNum][i].Position.x *= -1;
	}
}

void Giraffe::FlipY()
{
	for (int i = 0; i < Skeleton[FrameNum].size(); ++i) {
		Skeleton[FrameNum][i].y *= -1;
	}
	for (int i = 0; i < ControlPoints[FrameNum].size(); ++i) {
		ControlPoints[FrameNum][i].y *= -1;
	}
	for (int i = 0; i < Hurtboxes[FrameNum].size(); ++i) {
		Hurtboxes[FrameNum][i].Position.y *= -1;
	}
	for (int i = 0; i < Hitboxes[FrameNum].size(); ++i) {
		Hitboxes[FrameNum][i].Position.y *= -1;
	}
}

Vec2 Giraffe::GetParent(int i)
{
	if (Parents[i] >= 0) {
		return Skeleton[FrameNum][Parents[i]];
	}
	else {
		return ControlPoints[FrameNum][abs(Parents[i])];
	}
}

bool Giraffe::CaptureImage(HWND hWnd, const RECT &clientRect)
{

	HDC hdc = GetDC(hWnd);
	//HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdc);

	if (!hdcMemDC)
	{
		MessageBox(NULL, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		return false;
	}

	/*SetStretchBltMode(hdc, HALFTONE);

	if (!StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcScreen, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY)) {
		MessageBox(NULL, L"StretchBlt has failed", L"Failed", MB_OK);
		return false;
	}*/

	if (SavedImage) {
		DeleteObject(SavedImage);
	}
	
	SavedImage = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	if (!SavedImage) {
		MessageBox(NULL, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
		return false;
	}

	SelectObject(hdcMemDC, SavedImage);

	if (!BitBlt(hdcMemDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hdc, 0, 0, SRCCOPY)) {
		MessageBox(NULL, L"BitBlt has failed", L"Failed", MB_OK);
		return false;
	}

	DeleteDC(hdcMemDC);
	//DeleteDC(hdcScreen);
	DeleteDC(hdc);

	return true;
}

bool Giraffe::DisplayImage(HDC hdc, const RECT& clientRect)
{
	if (!SavedImage) {
		return false;
	}
	HDC hdcMemDC = CreateCompatibleDC(hdc);
	if (!hdcMemDC)
	{
		MessageBox(NULL, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		return false;
	}

	SelectObject(hdcMemDC, SavedImage);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 50;
	bf.AlphaFormat = 0;

	if (!AlphaBlend(hdc, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hdcMemDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, bf))
	{
		MessageBox(NULL, L"AlphaBlend has failed", L"Failed", MB_OK);
		return false;
	}

	DeleteDC(hdcMemDC);

	return true;
}

std::string Giraffe::FloatToString(float f)
{
	std::string str = std::to_string(f);
	if (str.find('.', 0) == std::string::npos) {
		str += ".0";
	}
	str += "f";
	return str;
}
