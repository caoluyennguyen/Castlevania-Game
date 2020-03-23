#include "Brick.h"
#include "Textures.h"
#include "Define.h"

void CBrick::LoadResource()
{
	CTextures* textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);

	CSprites* sprites = CSprites::GetInstance();
	sprites->Add(20001, 408, 225, 424, 241, texMisc);

	LPANIMATION ani;
	ani = new CAnimation(100);		// brick
	ani->Add(20001);

	CAnimations* animations = CAnimations::GetInstance();
	animations->Add(601, ani);

	AddAnimation(601);
}

void CBrick::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}