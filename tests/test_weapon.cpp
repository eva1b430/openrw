#include <boost/test/unit_test.hpp>
#include "test_globals.hpp"
#include <objects/CharacterObject.hpp>
#include <data/WeaponData.hpp>
#include <objects/ProjectileObject.hpp>

BOOST_AUTO_TEST_SUITE(WeaponTests)

BOOST_AUTO_TEST_CASE(TestWeaponScan)
{
	{
		// Test RADIUS scan
		auto character = Global::get().e->createPedestrian(1, {0.f, 0.f, 0.f});
		BOOST_REQUIRE( character != nullptr );
		BOOST_REQUIRE( character->model != nullptr);
		BOOST_REQUIRE( character->physObject != nullptr);

		WeaponScan scan( 10.f, {0.f, 0.f, 10.f}, {0.f,0.f, -10.f} );

		Global::get().e->doWeaponScan( scan );

		BOOST_CHECK( character->mHealth < 100.f );

		Global::get().e->destroyObject(character);
	}
}

BOOST_AUTO_TEST_CASE(TestProjectile)
{
	{
		auto character = Global::get().e->createPedestrian(1, {25.f, 0.f, 0.f});
		BOOST_REQUIRE( character != nullptr );

		auto wepdata = Global::get().e->gameData.weaponData["grenade"];

		auto projectile = new ProjectileObject(Global::get().e, {26.f, 1.f, 10.f},
		{
												ProjectileObject::Grenade,
												{0.f, 0.f, -1.f},
												2.0f,
												5.0f,
												wepdata
											});

		Global::get().e->objects.insert( projectile );

		BOOST_CHECK( character->mHealth == 100.f );

		for(float t = 0.f; t <= 5.f; t += 0.016f) {
			Global::get().e->dynamicsWorld->stepSimulation(0.016f, 0, 0);
			projectile->tick(0.016f);
		}

		BOOST_CHECK_LT( glm::distance(character->getPosition(), projectile->getPosition()), 1.f );

		// Grenade should have dentonated by this point
		BOOST_CHECK( character->mHealth < 100.f );

		Global::get().e->destroyObject(projectile);
		Global::get().e->destroyObject(character);
	}

	{
		auto character = Global::get().e->createPedestrian(1, {25.f, 0.f, 0.f});
		BOOST_REQUIRE( character != nullptr );

		auto wepdata = Global::get().e->gameData.weaponData["molotov"];

		auto projectile = new ProjectileObject(Global::get().e, {26.f, 1.f, 10.f},
		{
												ProjectileObject::Molotov,
												{0.f, 0.f, -1.f},
												2.0f,
												10.f,
												wepdata
											});

		Global::get().e->objects.insert( projectile );

		BOOST_CHECK( character->mHealth == 100.f );

		for(float t = 0.f; t <= 9.0f; t += 0.016f) {
			Global::get().e->dynamicsWorld->stepSimulation(0.016f, 0, 0);
			projectile->tick(0.016f);
		}

		BOOST_CHECK( projectile->getPosition().z < 10.f );
		BOOST_CHECK( projectile->getPosition().z > 0.f );

		BOOST_CHECK( character->mHealth < 100.f );

		Global::get().e->destroyObject(projectile);
		Global::get().e->destroyObject(character);
	}
	{
		auto character = Global::get().e->createPedestrian(1, {25.f, 0.f, 0.f});
		BOOST_REQUIRE( character != nullptr );

		auto wepdata = Global::get().e->gameData.weaponData["rocketlauncher"];

		auto projectile = new ProjectileObject(Global::get().e, {26.f, 1.f, 10.f},
		{
												ProjectileObject::RPG,
												{0.f, 0.f, -1.f},
												2.0f,
												10.f,
												wepdata
											});

		Global::get().e->objects.insert( projectile );

		BOOST_CHECK( character->mHealth == 100.f );

		for(float t = 0.f; t <= 9.f; t += 0.016f) {
			Global::get().e->dynamicsWorld->stepSimulation(0.016f, 0, 0);
			projectile->tick(0.016f);
		}

		BOOST_CHECK( projectile->getPosition().z < 10.f );

		BOOST_CHECK( character->mHealth < 100.f );

		Global::get().e->destroyObject(projectile);
		Global::get().e->destroyObject(character);
	}
}

BOOST_AUTO_TEST_SUITE_END()