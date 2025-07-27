//
// Created by Sanjay on 7/23/2025.
//
#pragma once
#include "Component.h"
#include "SDL3/SDL.h"

class SpriteComponent : public Component
{
protected:
	SpriteComponent(class Actor* actor)
		:Component(actor)
	{

	}
	friend class Actor;

public:
	virtual void SetTexture(SDL_Texture* texture) { mTexture = texture; }
	const SDL_Texture* GetTexture() const { return mTexture; }

	int GetDrawOrder() const { return mDrawOrder; }
	void SetDrawOrder(int drawOrder) { mDrawOrder = drawOrder; }

private:
	SDL_Texture* mTexture = nullptr;
	int mDrawOrder = 0;
};
