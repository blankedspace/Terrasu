#pragma once
#include "PlayerScript.h"
#include "CameraScript.h"
#include "BulletScript.h"
#include "EnemyScript.h"
#include "EnemySpawnerScript.h"
#include "WeaponPickupScript.h"
#include "EndScreenScript.h"
#include "WeaponSpawnerScript.h"
#include "HealthPackScript.h"
#define ELEMENT(SCRIPT) {typeid(SCRIPT).name(), []() { return static_cast<NativeScript*>(new SCRIPT()); }}
namespace Terrasu {

	static std::map<std::string, NativeScript* (*)()> ScriptFactory = {
	  ELEMENT(NativeScript),
	  ELEMENT(PlayerScript),
	  ELEMENT(BulletScript),
	  ELEMENT(EnemyScript),
	  ELEMENT(CameraScript),
	  ELEMENT(EnemySpawnerScript),
	  ELEMENT(WeaponPickupScript),
	  ELEMENT(EndScreenScript),
	  ELEMENT(WeaponSpawnerScript),
	  ELEMENT(HealthPackScript),


	};
}