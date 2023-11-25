#pragma once
#include "ECS/Entity.h"

#define ELEMENT(SCRIPT) {#SCRIPT, []() { return static_cast<NativeScript*>(new SCRIPT()); }}
namespace Terrasu {

	static std::map<std::string, NativeScript* (*)()> ScriptFactory;
}