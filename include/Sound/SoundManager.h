#pragma once

#include "AssetManager.h"

namespace Terrasu {

	float* GetBuffer();
	int GetBufferSize();
	void PauseSoundTr();
	int PlaySoundTr(AudioData* data);
	void setTimeStamp(float time_in_seconds);
	inline int Lenght;
	int getTimeStamp();
	std::vector<float> GetFreqs();
	std::vector<float> GetFreqs5plus();

};
