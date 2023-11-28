#define MA_NO_DEVICE_IO
#define MINIAUDIO_IMPLEMENTATION
#include "../3rdParty/miniaudio/miniaudio.h"
#include "Sound/SoundManager.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>    /* Change this to your include location. Might be <SDL2/SDL.h>. */

#define CHANNELS    2               /* Must be stereo for this example. */
#define SAMPLE_RATE 48000

static ma_engine g_engine;
static ma_sound g_sound;            /* This example will play only a single sound at once, so we only need one ma_sound object. */
static ma_decoder decoder;
static bool initialized = false;

namespace Terrasu {



#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  48000
    SDL_AudioDeviceID deviceID;


    void data_callback(void* pUserData, ma_uint8* pBuffer, int bufferSizeInBytes)
    {
        /* Reading is just a matter of reading straight from the engine. */
        ma_uint32 bufferSizeInFrames = (ma_uint32)bufferSizeInBytes / ma_get_bytes_per_frame(ma_format_f32, ma_engine_get_channels(&g_engine));
        ma_engine_read_pcm_frames(&g_engine, pBuffer, bufferSizeInFrames, NULL);
    }
    void PauseSoundTr() {
        /* Start playback. */
        SDL_CloseAudioDevice(deviceID);
        ma_sound_stop(&g_sound);
    }
    int PlaySoundTr(AudioData* data) {

        AssetManager manager;

        AudioData* audioData = data;
  
        audioData = manager.LoadAudioFile("Assets/sound.mp3");
        ma_result result;
        ma_engine_config engineConfig;
        SDL_AudioSpec desiredSpec;
        SDL_AudioSpec obtainedSpec;


        /*
        We'll initialize the engine first for the purpose of the example, but since the engine and SDL
        are independent of each other you can initialize them in any order. You need only make sure the
        channel count and sample rates are consistent between the two.

        When initializing the engine it's important to make sure we don't initialize a device
        internally because we want SDL to be dealing with that for us instead.
        */
        engineConfig = ma_engine_config_init();
        engineConfig.noDevice = MA_TRUE;      /* <-- Make sure this is set so that no device is created (we'll deal with that ourselves). */
        engineConfig.channels = CHANNELS;
        engineConfig.sampleRate = SAMPLE_RATE;


        result = ma_engine_init(&engineConfig, &g_engine);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize audio engine.");
            return -1;
        }

        /* Now load our sound. */
        result = ma_decoder_init_memory(audioData->data, audioData->dataSize, NULL, &decoder);
        result = ma_sound_init_from_data_source(&g_engine, &decoder, 0, NULL, &g_sound);


        /* Loop the sound so we can continuously hear it. */
        ma_sound_set_looping(&g_sound, MA_TRUE);

        /*
        The sound will not be started by default, so start it now. We won't hear anything until the SDL
        audio device has been opened and started.
        */
        ma_sound_start(&g_sound);


        /*
        Now that we have the engine and sound we can initialize SDL. This could have also been done
        first before the engine and sound.
        */
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
            printf("Failed to initialize SDL sub-system.");
            return -1;
        }


        MA_ZERO_OBJECT(&desiredSpec);
        desiredSpec.freq = ma_engine_get_sample_rate(&g_engine);
        desiredSpec.format = AUDIO_F32;
        desiredSpec.channels = ma_engine_get_channels(&g_engine);
        desiredSpec.samples = 512;
        desiredSpec.callback = data_callback;


        deviceID = SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (deviceID == 0) {
            printf("Failed to open SDL audio device.");
            return -1;
        }
        /* Start playback. */
        SDL_PauseAudioDevice(deviceID, 0);
        initialized = true;
        float time = 0;
        ma_sound_get_length_in_seconds(&g_sound, &time);
        Lenght = int(time);
        return 1;
    }
    int getTimeStamp()
    {
        if (!initialized)
            return 0;

        return ma_engine_get_time_in_milliseconds(&g_engine);
    }
    void setTimeStamp(float time_in_seconds)
    {
        double desiredTime = time_in_seconds;

        // Convert the desired time to PCM frames
        ma_uint64 desiredFrame = (ma_uint64)(desiredTime * decoder.outputSampleRate);
        // Seek to the desired PCM frame
        auto result = ma_sound_seek_to_pcm_frame(&g_sound, desiredFrame);
        ma_engine_set_time_in_pcm_frames(&g_engine, desiredFrame);

    }
}