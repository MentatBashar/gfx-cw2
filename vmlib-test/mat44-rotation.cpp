#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// Comparing floating point values isn't straight forward. See
// https://github.com/catchorg/Catch2/blob/devel/docs/comparing-floating-point-numbers.md#top
// for facilities that Catch2 provides.
//
// Note that we still need to decide what kind of errors we want to accept (so,
// absolute, relative or ULP error). The examples take a simple approach of 
// permitting an absolute error of 10^-6. This should be mostly good enough
// for our purposes. (We could likely be stricter; std::sin() and similar are
// likely accurate to 1 ULP for "normal" floats.)


// Note: as before, these checks are not exhaustive. They're meant as examples
// to get you started.

TEST_CASE( "4x4 rotation around X axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_x( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_x( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_x( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 180 degrees = pi radians.
	SECTION( "180 degrees" )
	{
		auto const right = make_rotation_x( 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -180 degrees = pi radians.
	SECTION( "-180 degrees" )
	{
		auto const right = make_rotation_x( -3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 270 degrees = 3pi/2 radians.
	SECTION( "270 degrees" )
	{
		auto const right = make_rotation_x( 3.f * 3.1415926f/ 2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -270 degrees = -3pi/2 radians.
	SECTION( "-270 degrees" )
	{
		auto const right = make_rotation_x( -3.f * 3.1415926f/ 2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 360 degrees = 2pi radians.
	SECTION( "360 degrees" )
	{
		auto const right = make_rotation_x( 2.f * 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -360 degrees = -2pi radians.
	SECTION( "-360 degrees" )
	{
		auto const right = make_rotation_x( -2.f * 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}

TEST_CASE( "4x4 rotation around Y axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_y( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_y( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_y( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 180 degrees = pi radians.
	SECTION( "180 degrees" )
	{
		auto const right = make_rotation_y( 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -180 degrees = -pi radians.
	SECTION( "-180 degrees" )
	{
		auto const right = make_rotation_y( -3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 270 degrees = 3pi/2 radians.
	SECTION( "270 degrees" )
	{
		auto const right = make_rotation_y( 3.f * 3.1415926f/2 );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -270 degrees = -3pi/2 radians.
	SECTION( "-270 degrees" )
	{
		auto const right = make_rotation_y( -3.f * 3.1415926f/2 );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 360 degrees = 2pi radians.
	SECTION( "360 degrees" )
	{
		auto const right = make_rotation_y( 2.f * 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -360 degrees = -2pi radians.
	SECTION( "-360 degrees" )
	{
		auto const right = make_rotation_y( -2.f * 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}

TEST_CASE( "4x4 rotation around Z axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_z( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_z( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_z( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_z( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 180 degrees = pi radians.
	SECTION( "180 degrees" )
	{
		auto const right = make_rotation_z( 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -180 degrees = -pi radians.
	SECTION( "-180 degrees" )
	{
		auto const right = make_rotation_z( -3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 270 degrees = 3pi/2 radians.
	SECTION( "270 degrees" )
	{
		auto const right = make_rotation_z( 3 * 3.1415926f / 2 );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -270 degrees = -3pi/2 radians.
	SECTION( "-270 degrees" )
	{
		auto const right = make_rotation_z( -3 * 3.1415926f / 2 );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 360 degrees = 2pi radians.
	SECTION( "360 degrees" )
	{
		auto const right = make_rotation_z( 2 * 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -360 degrees = -2pi radians.
	SECTION( "-360 degrees" )
	{
		auto const right = make_rotation_z( 2 * 3.1415926f );

		REQUIRE_THAT( right(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}
