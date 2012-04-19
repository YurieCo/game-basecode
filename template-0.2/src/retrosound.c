#include <AL/al.h>
#include <AL/alc.h>

#include "retrosound.h"
#include "retrocommon.h"

#define CHANNELS	16

static ALCcontext *al_context = NULL;
static ALCdevice *al_device = NULL;

static ALuint src_loop;
static ALuint src_channels[CHANNELS];

void RSO_Init(void)
{
    ALint attribs[4] = { 0, 0, 0, 0 };

	if ( al_device )
		return;

	const ALchar* dev = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	al_device = alcOpenDevice(NULL);

	if (al_device == NULL)
	{
		printf("OpenAL: No sound for you :( (Unable to open sound device)\n");
		return;
	}

    al_context = alcCreateContext( al_device, attribs );
    if (al_context == NULL)
    {
        printf("Unable to create OpenAL context\n");
        return;
    }

	alGetError();

	

	if ( alcMakeContextCurrent(al_context) == AL_FALSE )
	{
		printf("OpenAL: Failure to set default context - probably means no sound for you\n");
		return;
	}
	alGenSources(1, &src_loop);
	alGenSources(CHANNELS, src_channels);
}

void RSO_PlaySound(RetroSnd_t *s)
{
	int i;
	ALuint val;
	for(i=0;i<CHANNELS;i++)
	{
		alGetSourcei(src_channels[i], AL_SOURCE_STATE, &val);
		if ( val == AL_PLAYING )
			continue;

		alSourcei(src_channels[i], AL_BUFFER, s->buffer);
		alSourcePlay(src_channels[i]);
	}
}

void RSO_PlayLoop(RetroSnd_t *s)
{
	alSourcei(src_loop, AL_BUFFER, s->buffer);
	alSourcei(src_loop, AL_LOOPING, AL_TRUE);
	alSourcePlay(src_loop);
	puts("Playing loop");
}

void RSO_StopLoop(void)
{
	alSourceStop(src_loop);
	puts("Stopping loop");
}
