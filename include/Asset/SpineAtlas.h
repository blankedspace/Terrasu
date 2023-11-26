#pragma once

#include "spine/Atlas.h"
#include <string>
#include "AssetManager.h"


class SpineAtlas
{
public:
	SpineAtlas(const std::string& name, Terrasu::AssetManager& mn);
	~SpineAtlas();

	spine::Atlas* mAtlas;
private:
	std::string		mName;
};