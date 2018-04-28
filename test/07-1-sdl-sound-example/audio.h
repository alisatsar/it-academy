#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <SDL2/SDL.h>

typedef struct sound
{
	uint32_t length;
	uint32_t length_true;
	uint8_t* buffer;
	uint8_t* buffer_true;
	uint8_t loop;
	uint8_t fade;
	uint8_t free;
	uint8_t volume;

	SDL_AudioSpec audio;

	struct sound* next;
} Audio;


Audio* createAudio(const char* filename, uint8_t loop, int volume);

void freeAudio(Audio* audio);

void playSound(const char* filename, int volume);

void playSoundFromMemory(Audio* audio, int volume);

void playMusicFromMemory(Audio* audio, int volume);

void endAudio(void);

void initAudio(void);

void pauseAudio(void);

void unpauseAudio(void);

#ifdef __cplusplus
}
#endif

