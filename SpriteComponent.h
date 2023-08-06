#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include <string>

struct SDL_Texture
{
	std::string mName;
};

class SpriteComponent : public Component
{
public:
	// (Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, int drawOrder = 100)
	: Component(owner)
	, mDrawOrder(drawOrder)
	{
	}

	virtual void SetTexture(SDL_Texture* texture) { mTexture = texture; }

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	bool IsVisible() const { return mIsVisible; }
	void SetIsVisible(bool visible) { mIsVisible = visible; }
	SDL_Texture* GetTexture() const { return mTexture; }

protected:
	SDL_Texture* mTexture = nullptr;
	int mDrawOrder;
	int mTexWidth = 1;
	int mTexHeight = 1;
	bool mIsVisible = true;
};
