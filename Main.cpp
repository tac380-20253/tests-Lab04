#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "catch_reporter_github.hpp"
// Some Windows BS I guess included by Catch?
#ifdef far
#undef far
#endif
#ifdef near
#undef near
#endif
#include "Actor.h"
#include "AnimatedSprite.h"

// This is janky but doing it this way to account for weird include
// dependencies people may have introduced into CollisionComponent.cpp
#include <algorithm>
#include <vector>
// Create dummy implementations for a few SDL functions/macros
#ifdef SDL_assert
#undef SDL_assert
#endif
#define SDL_assert(condition) REQUIRE(condition)

#ifdef SDL_assert_release
#undef SDL_assert_release
#endif
#define SDL_assert_release(condition) REQUIRE(condition)

// Skip over SDL logs completely
static void SDL_Log(...)
{
}

#include "AnimatedSprite.cpp"
const float DELTA_TIME = 0.016f;

TEST_CASE("Animation tests")
{
	SDL_Texture tex1{"First"};
	SDL_Texture tex2{"Second"};
	SDL_Texture tex3{"Third"};
	SDL_Texture tex4{"Fourth"};
	SDL_Texture tex5{"Fifth"};
	SDL_Texture tex6{"Short"};

	Actor act(Vector2(0.0f, 0.0f), 1.0f);
	AnimatedSprite as(&act);
	std::vector<SDL_Texture*> textures{&tex1, &tex2, &tex3, &tex4, &tex5};
	as.AddAnimation("testAnim", textures);
	std::vector<SDL_Texture*> textures2{&tex6};
	as.AddAnimation("testAnim2", textures2);

	SECTION("Unset animation does not set a texture")
	{
		as.Update(DELTA_TIME);
		REQUIRE(as.GetTexture() == nullptr);
	}

	SECTION("Set animation sets the texture")
	{
		as.SetAnimation("testAnim");
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
	}

	SECTION("Pause then set animation still sets the texture")
	{
		as.SetIsPaused(true);
		as.SetAnimation("testAnim");
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
	}

	SECTION("Updates the frame correctly")
	{
		as.SetAnimation("testAnim");
		as.Update(DELTA_TIME * 6);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
		as.Update(DELTA_TIME);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Second");
	}

	SECTION("Updates the frame correctly, with wrapping")
	{
		as.SetAnimation("testAnim");
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Second");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Third");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Fourth");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Fifth");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
	}

	SECTION("Updates the frame correctly at non-default FPS")
	{
		as.SetAnimation("testAnim");
		as.SetAnimFPS(5.0f);
		as.Update(DELTA_TIME * 6);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
		as.Update(DELTA_TIME);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Second");
	}

	SECTION("When paused, update does not change the frame")
	{
		as.SetIsPaused(true);
		as.SetAnimation("testAnim");
		as.Update(DELTA_TIME * 6);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
		as.Update(DELTA_TIME);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "First");
	}

	SECTION("1-frame animation updates correctly")
	{
		as.SetAnimation("testAnim2");
		as.Update(DELTA_TIME * 7);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Short");
	}

	SECTION("When switching from a longer animation to a 1-frame anim, it wraps properly")
	{
		as.SetAnimation("testAnim");
		as.Update(DELTA_TIME * 14);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Third");
		as.SetAnimation("testAnim2");
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Short");
	}

	SECTION("1-frame animation wraps properly when the anim timer is exactly 1.0")
	{
		as.SetAnimation("testAnim2");
		as.Update(0.1f);
		REQUIRE(as.GetTexture() != nullptr);
		REQUIRE(as.GetTexture()->mName == "Short");
	}
}
