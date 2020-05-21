#ifndef _GIRAFFE_H_
#define _GIRAFFE_H_

#include <Windows.h>
#include "Vec2.h"
#include "Collider.h"
#include <vector>
#include <array>

enum DrawInputs {
	DRAW_SKELETON = (1 << 1),
	DRAW_CP = (1 << 2),
	DRAW_HIT = (1 << 3),
	DRAW_HURT = (1 << 4),
	DRAW_OUTLINE = (1 << 5),
	DRAW_PREV = (1 << 6),
};

class Giraffe {
public:
	Giraffe() {};
	~Giraffe();
	Giraffe(int width, int height);
	void Draw(HDC hdc, BYTE flags, const RECT &clientRect);
	void Serialize(LPCSTR filepath);
	void Save(LPCSTR filepath);
	void Load(LPCSTR filepath);
	void Merge(LPCSTR filepath);
	bool Select(int x, int y, BYTE flags, bool click);
	void Deselect();
	void Update(int x, int y);
	bool NextFrame(HWND hwnd, const RECT &clientRect);
	bool PrevFrame(HWND hwnd, const RECT& clientRect);
	bool SetFrame(int i);
	void CopyFrameToNext();
	void InsertFrame(HWND hwnd, const RECT& clientRect);
	void DeleteFrame();
	int GetFrameNum();
	int GetMaxFrames();
	void AddRemoveHitbox(int x, int y);
	int CheckIntersect(int x, int y, BYTE flags, int &index);
	void SetHurtboxData(float DmgMult, int i);
	float GetHurtboxData(int i);
	void SetHitboxData(float Damage, Vec2 Force, float Knockback, float Scale, bool Fixed, int i);
	void GetHitboxData(float& Damage, Vec2& Force, float& Knockback, float& Scale, bool& Fixed, int i);
	void FlipX();
	void FlipY();

private:
	Vec2 GetParent(int i);
	bool CaptureImage(HWND hWnd, const RECT& clientRect);
	bool DisplayImage(HDC hdc, const RECT& clientRect);
	std::string FloatToString(float f);
	HBITMAP SavedImage;

	std::vector<std::array<Vec2, 15>> Skeleton;
	std::vector<std::array<Vec2, 9>> ControlPoints;
	std::vector<std::vector<HitCollider>> Hitboxes;
	std::vector<std::array<HurtCollider, 6>> Hurtboxes;
	//std::vector<std::array<float, 15>> HurtRadii;

	std::array<std::vector<int>, 15> Children;
	std::array<int, 15> Parents;
	std::array<float, 15> Distance;


	float radius;
	Vec2 Scale;
	Vec2 Position;
	int Selected;
	int SelectedLayer;
	int FrameNum;
	bool Updated;
	bool LastClick;


	HBRUSH BoneBrush;
	HBRUSH HitBrush;
	HBRUSH HurtBrush;
	HBRUSH CPBrush;
	HBRUSH RotateBrush;
	HPEN LinePen;
};

#endif // !_GIRAFFE_H_
