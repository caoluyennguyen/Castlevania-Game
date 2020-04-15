#include "Simon.h"
#include <algorithm>
#include "debug.h"

#include "Game.h"
#include "Textures.h"

#include "Portal.h"
#include "Ground.h"
#include "Brick.h"

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny < 0)
			//	{
			//		if (goomba->GetState() != GOOMBA_STATE_DIE)
			//		{
			//			goomba->SetState(GOOMBA_STATE_DIE);
			//			vy = -SIMON_JUMP_DEFLECT_SPEED;
			//		}
			//	}
			//	else if (e->nx != 0)
			//	{
			//		if (untouchable == 0)
			//		{
			//			if (goomba->GetState() != GOOMBA_STATE_DIE)
			//			{
			//				if (level > SIMON_LEVEL_SMALL)
			//				{
			//					level = SIMON_LEVEL_SMALL;
			//					StartUntouchable();
			//				}
			//				else
			//					SetState(SIMON_STATE_DIE);
			//			}
			//		}
			//	}
			//}

			if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
			{
				if (ny != 0)
				{
					if (ny == -1)
					{
						vy = 0;
						isTouchGround = true;
					}
					else
					{
						y += dy;
						isTouchGround = false;
					}
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

//void Simon::LoadResource()
//{
//
//	LPDIRECT3DTEXTURE9 texSimon = CTextures::GetInstance()->Get(ID_TEX_SIMON);
//
//	CSprites* sprites = CSprites::GetInstance();
//	sprites->Add(10001, 0, 0, 60, 64, texSimon); // stand
//
//	sprites->Add(10011, 0, 0, 60, 64, texSimon); // walk
//	sprites->Add(10012, 60, 0, 120, 64, texSimon);
//	sprites->Add(10013, 120, 0, 180, 64, texSimon);
//	sprites->Add(10014, 180, 0, 240, 64, texSimon);
//
//	sprites->Add(10021, 300, 198, 360, 262, texSimon); // sit
//
//	sprites->Add(10031, 240, 0, 300, 64, texSimon); // jump
//
//	CAnimations* animations = CAnimations::GetInstance();
//
//	LPANIMATION ani;
//	ani = new CAnimation(); // Simon idle
//	ani->Add(10001);
//	animations->Add(401, ani);
//
//	ani = new CAnimation(); // Simon walk
//	ani->Add(10011);
//	ani->Add(10012);
//	ani->Add(10013);
//	ani->Add(10014);
//	animations->Add(402, ani);
//
//	ani = new CAnimation(); // Simon sit
//	ani->Add(10021);
//	animations->Add(403, ani);
//
//	ani = new CAnimation(); // Simon jump
//	ani->Add(10031);
//	animations->Add(404, ani);
//
//	AddAnimation(401);
//	AddAnimation(402);
//	AddAnimation(403);
//	AddAnimation(404);
//}

void Simon::Render()
{
	/*int ani;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else {
		if (vx == 0)
		{
			if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
			else ani = SIMON_ANI_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_LEFT;
	}*/

	int alpha = 255;
	if (untouchable) alpha = 128;
	//animations[ani]->Render(x, y, alpha);
	animation_set->at(state)->Render(x, y, alpha);

	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_SIT:
		vx = vy = 0;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		isTouchGround = false;
		animation_set->at(state)->setStartFrameTime(GetTickCount());
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_HIT_STAND:
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STATE_HIT_SIT:
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	}
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}
