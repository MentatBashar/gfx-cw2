#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first.

TEST_CASE( "Perspective projection", "[mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// "Standard" projection matrix presented in the exercises. Assumes
	// standard window size (e.g., 1280x720).
	//
	// Field of view (FOV) = 60 degrees
	// Window size is 1280x720 and we defined the aspect ratio as w/h
	// Near plane at 0.1 and far at 100
	SECTION( "Standard" )
	{
		auto const proj = make_perspective_projection(
			60.f * 3.1415926f / 180.f,
			1280/float(720),
			0.1f, 100.f
		);

		REQUIRE_THAT( proj(0,0), WithinAbs( 0.974279, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.732051f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( -1.002002f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( -0.200200f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 0.f, kEps_ ) );
	}

	SECTION( "Smallest Screen " )
	{
		auto const proj = make_perspective_projection(
			60.f * 3.1415926f / 180.f,
			320/float(320),
			0.1f, 100.f
		);

		REQUIRE_THAT( proj(0,0), WithinAbs( 1.732051f, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.732051f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( -1.002002f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( -0.200200f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 0.f, kEps_ ) );
	}
}

TEST_CASE( "Making Translation", "[Vec3f]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	SECTION( "Simple" )
	{
		auto const proj = make_translation({1.f, 1.f, 1.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 1.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 1.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 1.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	SECTION( "Positive X" )
	{
		auto const proj = make_translation({10.f, 0.f, 0.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 10.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	SECTION( "Negative X" )
	{
		auto const proj = make_translation({-10.f, 0.f, 0.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( -10.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	SECTION( "Positive Y" )
	{
		auto const proj = make_translation({0.f, 10.f, 0.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 10.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	SECTION( "Negative Y" )
	{
		auto const proj = make_translation({0.f, -10.f, 0.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( -10.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	SECTION( "Positive Z" )
	{
		auto const proj = make_translation({0.f, 0.f, 10.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 10.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	SECTION( "Negative Z" )
	{
		auto const proj = make_translation({0.f, 0.f, -10.f});

		REQUIRE_THAT( proj(0,0), WithinAbs( 1, kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( -10.f, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}