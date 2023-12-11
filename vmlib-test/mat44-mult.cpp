#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first!

TEST_CASE( "4x4 matrix by matrix multiplication", "[mat44]" )
{
	//TODO: implement your own tests here

	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	//done to see that it can multiply the basic identity by itself and return the correct answer (itself)
	SECTION( "Simple" )
	{
		auto const proj =  kIdentity44f * kIdentity44f;

		REQUIRE_THAT( proj(0,0), WithinAbs( kIdentity44f(0,0), kEps_ ) );
		REQUIRE_THAT( proj(0,1), WithinAbs( kIdentity44f(0,1), kEps_ ) );
		REQUIRE_THAT( proj(0,2), WithinAbs( kIdentity44f(0,2), kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( kIdentity44f(0,3), kEps_ ) );

		REQUIRE_THAT( proj(1,0), WithinAbs( kIdentity44f(1,0), kEps_ ) );
		REQUIRE_THAT( proj(1,1), WithinAbs( kIdentity44f(1,1), kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( kIdentity44f(1,2), kEps_ ) );
		REQUIRE_THAT( proj(1,3), WithinAbs( kIdentity44f(1,3), kEps_ ) );

		REQUIRE_THAT( proj(2,0), WithinAbs( kIdentity44f(2,0), kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( kIdentity44f(2,1), kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( kIdentity44f(2,2), kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( kIdentity44f(2,3), kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( kIdentity44f(3,0), kEps_ ) );
		REQUIRE_THAT( proj(3,1), WithinAbs( kIdentity44f(3,1), kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( kIdentity44f(3,2), kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( kIdentity44f(3,3), kEps_ ) );

	}

	//done to see that it can multiply two separate 4x4 matrices with floats less than ten and greater than minus ten.
	SECTION( "standard" )
	{
		Mat44f testA = { {
			4.f, 7.f, 6.f, -5.f,
			3.f, -4.f, 3.f, 9.f,
			-6.f, 5.f, 2.f, 8.f,
			8.f, 7.f, -1.f, 6.f
		} };

		Mat44f testB = { {
			8.f, -6.f, 7.f, 3.f,
			8.f, 3.f, -4.f, 7.f,
			3.f, 2.f, 4.f, -6.f,
			-5.f, 1.f, 7.f, 2.f
		} };

		auto const proj =  testA * testB;

		//WithinAbs cannot use negative values, so i had to reverse it to accomadate
		REQUIRE_THAT( proj(0,0), WithinAbs( 131.0, kEps_) );
		REQUIRE_THAT( proj(0,1), WithinAbs( 4.0, kEps_ ) );
		REQUIRE_THAT( -1 * proj(0,2), WithinAbs( 11.0, kEps_ ) );
		REQUIRE_THAT( proj(0,3), WithinAbs( 15.0, kEps_ ) );

		REQUIRE_THAT( -1 * proj(1,0), WithinAbs( 44.0, kEps_ ) );
		REQUIRE_THAT( -1 * proj(1,1), WithinAbs( 15.0, kEps_ ) );
		REQUIRE_THAT( proj(1,2), WithinAbs( 112.0, kEps_ ) );
		REQUIRE_THAT( -1 * proj(1,3), WithinAbs( 19.0, kEps_ ) );

		REQUIRE_THAT( -1 * proj(2,0), WithinAbs( 42.0, kEps_ ) );
		REQUIRE_THAT( proj(2,1), WithinAbs( 63.0, kEps_ ) );
		REQUIRE_THAT( proj(2,2), WithinAbs( 2.0, kEps_ ) );
		REQUIRE_THAT( proj(2,3), WithinAbs( 21.0, kEps_ ) );

		REQUIRE_THAT( proj(3,0), WithinAbs( 87.0, kEps_ ) );
		REQUIRE_THAT( -1 * proj(3,1), WithinAbs( 23.0, kEps_ ) );
		REQUIRE_THAT( proj(3,2), WithinAbs( 66.0, kEps_ ) );
		REQUIRE_THAT( proj(3,3), WithinAbs( 91.0, kEps_ ) );
	}
}

TEST_CASE( "4x4 matrix by vector multiplication", "[mat44][vec4]" )
{
	//TODO: implement your own tests here
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	//done to see that it can multiply the basic identity by a simple vector and return the correct answer (the simple vector)
	SECTION( "Simple" )
	{
		Vec4f testA = {9.f, -2.f, -7.f, 4.f};

		Mat44f testB = { {
			8.f, -6.f, 7.f, 3.f,
			8.f, 3.f, -4.f, 7.f,
			3.f, 2.f, 4.f, -6.f,
			-5.f, 1.f, 7.f, 2.f
		} };

		auto const proj =  testB * testA;

		REQUIRE_THAT( proj[0], WithinAbs( 47.0, kEps_ ) );
		REQUIRE_THAT( proj[1], WithinAbs( 122.0, kEps_ ) );
		REQUIRE_THAT( -1 * proj[2], WithinAbs( 29.0, kEps_ ) );
		REQUIRE_THAT( -1 * proj[3], WithinAbs( 88.0, kEps_ ) );

	}

	SECTION( "Standard" )
	{
		Vec4f testA = {1.f, 1.f, 1.f, 1.f};

		auto const proj =  kIdentity44f * testA;

		REQUIRE_THAT( proj[0], WithinAbs( testA[0], kEps_ ) );
		REQUIRE_THAT( proj[1], WithinAbs( testA[1], kEps_ ) );
		REQUIRE_THAT( proj[2], WithinAbs( testA[2], kEps_ ) );
		REQUIRE_THAT( proj[3], WithinAbs( testA[3], kEps_ ) );

	}
}
