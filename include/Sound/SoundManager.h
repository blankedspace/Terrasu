#pragma once

#include "AssetManager.h"

namespace Terrasu {


	void PauseSoundTr();
	int PlaySoundTr(AudioData* data);
	void setTimeStamp(float time_in_seconds);
	inline int Lenght;
	int getTimeStamp();
};
