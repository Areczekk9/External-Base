#include "Includes.h"

// Flags
#define FL_ON_GROUND 257
#define FL_ON_GROUND_CROUCHED 263
#define FL_IN_AIR_STAND 256
#define FL_IN_AIR_MOVING_TO_STAND 258
#define FL_ON_GROUND_MOVING_TO_STAND 259
#define FL_IN_AIR_MOVING_TO_CROUCH 260
#define FL_ON_GROUND_MOVING_TO_CROUCH 261
#define FL_IN_AIR_CROUCHED 262
#define FL_IN_WATER 1280
#define FL_IN_PUDDLE 1281
#define FL_IN_WATER_CROUCHED 1286
#define FL_IN_PUDDLE_CROUCHED 1287
#define FL_PARTIALGROUND (1 << 18)

// Team IDs
#define TEAM_ID_GOTV 1
#define TEAM_ID_T 2
#define TEAM_ID_CT 3

void bhop()
{
	//If we're holding space and the we're on the ground
	if (GetAsyncKeyState(VK_SPACE) && LocalPlayer::getLocalFlags() == FL_ON_GROUND)
	{
		//Jump!
		LocalPlayer::forceJump();
	}
}

DWORD findPlayer()
{
	D3DXVECTOR3 w2sHead;

	DWORD plrToAim = NULL;
	double lowestDist = 9999;

	for (int i = 1; i < 32; i++)
	{
		auto base = Entity::getEntBase(i);

		if (Entity::getEntTeam(base) != LocalPlayer::getLocalTeam() && Entity::isValid(base) && !Entity::getEntImmunity(base))
		{
			WorldToScreen(Entity::getEntBonePos(base, 8), w2sHead, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

			double dist = sqrt(pow((GetSystemMetrics(SM_CXSCREEN) / 2) - w2sHead.x, 2) + pow((GetSystemMetrics(SM_CYSCREEN) / 2) - w2sHead.y, 2));

			if (dist < lowestDist)
			{
				lowestDist = dist;
				plrToAim = base;
			}
		}
	}

	return plrToAim;
}

D3DXVECTOR3 CalmAngle(D3DXVECTOR3 currAngle)
{
	D3DXVECTOR3 angles = currAngle;
	if (angles.x < -180.f) angles.x += 360.f;
	if (angles.x > 180.f) angles.x -= 360.f;

	if (angles.y < -180.f) angles.y += 360.f;
	if (angles.y > 180.f) angles.y -= 360.f;

	if (angles.x > 89.0f && angles.x <= 180.f)
	{
		angles.x = 89.f;
	}
	return angles;
}

void aimbot(DWORD playerToAimbot)
{
	if (playerToAimbot != NULL)
	{
		if (Entity::getSpotted(playerToAimbot) == 1 && GetAsyncKeyState(VK_LBUTTON))
		{
			D3DXVECTOR3 aimAngles = CalcAngle(LocalPlayer::getLocalPos(), Entity::getEntPos(playerToAimbot));

			aimAngles -= LocalPlayer::getLocalPunchAngles() * 2.0;

			auto currAngles = LocalPlayer::getLocalViewAngles();

			auto delta = aimAngles - currAngles;

			delta = CalmAngle(delta);
			LocalPlayer::setLocalViewAngles(currAngles + delta / 15);
		}
	}
}

int main()
{
	init();
	LocalPlayer::setLocalPlayer();

	while (!GetAsyncKeyState(VK_END))
	{
		bhop();

		aimbot(findPlayer());

		Sleep(1);
	}

	CloseHandle(hProc);
}