#ifndef AUDIODB_H
#define AUDIODB_H

#include "SDL2_mixer/SDL_mixer.h"
#include "AudioHelper.h"
#include <filesystem>
#include <iostream>
#include <string>

class AudioDB
{
	static bool AudioExists(const std::string& audio_name, const std::string& file_path);

public:
	inline static void Init() {
		AudioHelper::Mix_AllocateChannels498(50); // allocate channels 0-49 
	};
	static void Play(const int channel, const std::string& clip_name, const bool does_loop);
	static void Halt(const int channel);
	static void SetVolume(const int channel, const int volume);
	static std::string GetAudioFilePath(const std::string& music_name);
	static void LoadAudioFile(const std::string file_name);

	inline static std::unordered_map<std::string, Mix_Chunk*> loaded_audio;
};

#endif