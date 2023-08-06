#pragma once

class Component
{
public:
	Component(class Actor* a)
	: mOwner(a)
	{
	}
	class Actor* GetOwner() const { return mOwner; }

	virtual void Update(float deltaTime) {}

protected:
	class Actor* mOwner;
};
