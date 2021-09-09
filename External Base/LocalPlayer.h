#pragma once

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

namespace LocalPlayer
{
	extern DWORD			getLocalPlayer();
	extern int				getLocalFlags();
	extern int				getLocalHealth();
	extern int				getLocalCrossID();
	extern int				getLocalTeam();
	extern D3DXVECTOR3		getLocalPos();
	extern D3DXMATRIX		getLocalViewMatrix();
	extern D3DXVECTOR3		getLocalViewAngles();
	extern D3DXVECTOR3		getLocalPunchAngles();

	extern void				forceJump(int waitTime = 35);
	extern void				setLocalPlayer();
	extern void				setLocalViewAngles(D3DXVECTOR3 angles);

	extern DWORD			LocalBaseaddress;
}