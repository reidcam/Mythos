#include "AudioDB.h"

/// <summary>
/// returns true if the audio exists and errors if it doesn't
/// </summary>
/// <param name="audio_name"></param>
/// <param name="file_path"></param>
/// <returns></returns>
bool AudioDB::AudioExists(const std::string& audio_name, const std::string& file_path)
{
	if (!std::filesystem::exists(file_path)) {
		std::cout << "error: failed to play audio clip " + audio_name;
		exit(0);
	}
	return true;
}

void AudioDB::Play(const int channel, const std::string& clip_name, const bool does_loop)
{
	if (channel < 0 || channel > 49) {
		std::cout << "channel number is not between 0 and 49 inclusive. Channel provided: " << channel << std::endl;
	}

	int loop_var = 0; // set to not loop by default
	LoadAudioFile(clip_name); // check that the file exists, and load it if it isn't loaded

	if (AudioHelper::Mix_OpenAudio498(44100, AUDIO_S16SYS, 1, 2048) < 0) { // start the mixer
		std::cout << Mix_GetError();
		exit(0);
	}

	if (does_loop) { // check for looping. true = pass -1, otherwise 0
		loop_var = -1;
	}

	AudioHelper::Mix_PlayChannel498(channel, loaded_audio.at(clip_name), loop_var);
}

void AudioDB::Halt(const int channel)
{
	AudioHelper::Mix_HaltChannel498(channel);
}

void AudioDB::SetVolume(const int channel, const int volume)
{
	if (volume < 0 || volume > 128) {
		std::cout << "volume is not between 0 and 128 inclusive. Volume provided: " << volume << std::endl;
	}

	AudioHelper::Mix_Volume498(channel, volume);
}

/// <summary>
/// figures out which one of the two file formats the audio file is in and returns the corresponding path
/// </summary>
/// <param name="music_name"></param>
/// <returns></returns>
std::string AudioDB::GetAudioFilePath(const std::string& music_name)
{
	std::string path = "resources/audio/" + music_name + ".wav";
	if (std::filesystem::exists(path))
		return path;
	else
		return "resources/audio/" + music_name + ".ogg";
}

void AudioDB::LoadAudioFile(const std::string file_name)
{
	if (AudioExists(file_name, GetAudioFilePath(file_name))) { // make sure the file exists
		if (loaded_audio.find(file_name) != loaded_audio.end()) { // file already loaded
			return;
		}
		else { // insert the mix chunk into the map
			loaded_audio.insert({ file_name, AudioHelper::Mix_LoadWAV498(GetAudioFilePath(file_name).c_str()) });
		}
	}
}
