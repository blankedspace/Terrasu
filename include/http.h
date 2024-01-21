#pragma once
#include <string>
#include "../httplib.h"
#include "../3rdParty/json.hpp"

namespace Terrasu {
	using json = nlohmann::json;
	json search(std::string query, std::function<void(json)> callback);
	std::string download(std::string video_id, std::function<void(std::string)> callback);
}