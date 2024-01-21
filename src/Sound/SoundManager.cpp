#define MA_NO_DEVICE_IO
#define MINIAUDIO_IMPLEMENTATION
#include "../3rdParty/miniaudio/miniaudio.h"
#include "Sound/SoundManager.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>    /* Change this to your include location. Might be <SDL2/SDL.h>. */

#define CHANNELS    2               /* Must be stereo for this example. */
#define SAMPLE_RATE 48000
#include <queue>
#include <iostream>
static ma_engine g_engine;
static ma_sound g_sound;            /* This example will play only a single sound at once, so we only need one ma_sound object. */
static ma_decoder decoder;
static bool initialized = false;



#include "../../pocketfft/pocketfft_hdronly.h"

namespace Terrasu {
    template <class T, size_t N>
    class FixedSizeQueue {
    public:
        FixedSizeQueue() : head(0), tail(0), size(0) {}

        void push(const T& value) {
            if (size < N) {
                data[tail] = value;
                tail = (tail + 1) % N;
                size++;
            }
            else {
                // Queue is full, handle the overflow
                // For example, you could ignore the new element or replace the oldest one
                // Here, we'll just replace the oldest element
                data[head] = value;
                head = (head + 1) % N;
            }
        }

        void push(const T* arr, size_t arrSize) {
            if (arrSize >= N) {
                // If the array size is greater than or equal to the queue size,
                // we'll simply copy the last N elements of the array
                std::copy_backward(arr + arrSize - N, arr + arrSize, data + N);
                head = 0;  // Reset the head index
                tail = N;  // Set the tail index to N
                size = N;  // Update the size of the queue
            }
            else {
                // If the array size is smaller than the queue size, we'll copy all elements and adjust the indices
                size_t spaceAvailable = N - size;  // Calculate the available space in the queue
                size_t elementsToCopy = std::min(arrSize, spaceAvailable);  // Determine how many elements to copy

                // Copy the elements from the array to the queue
                std::copy(arr, arr + elementsToCopy, data + tail);
                tail = (tail + elementsToCopy) % N;  // Update the tail index
                size += elementsToCopy;  // Update the size of the queue

                if (elementsToCopy < arrSize) {
                    // If there are more elements in the array, we'll copy the remaining elements from the beginning
                    std::copy(arr + elementsToCopy, arr + arrSize, data);
                    tail = elementsToCopy;  // Update the tail index
                    size = N;  // Update the size of the queue
                }
            }
        }

        void pop() {
            if (size > 0) {
                head = (head + 1) % N;
                size--;
            }
        }

        T front() const {
            if (size > 0) {
                return data[head];
            }
            // Add error handling here, e.g., throw an exception or return a default value
        }

        bool empty() const {
            return size == 0;
        }

        bool full() const {
            return size == N;
        }

        T* getbuffer()
        {
            return &data[0];
        }
        size_t size;
    private:
        size_t head;
        size_t tail;

        T* data =  new T[N];
    };




#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  48000
    SDL_AudioDeviceID deviceID;
    FixedSizeQueue<float, 1<<14>* lastBuffer = new FixedSizeQueue<float, 1<<14>();
    int lastbufferSizeInBytes;
    float* buffer;
    float* GetBuffer() {

        return lastBuffer->getbuffer();
    }
    int GetBufferSize() {
        return lastBuffer->size;
    }
    std::vector<std::vector<float>> freqs;
    std::vector<float> GetFreqs() {
        if(!initialized)
            return {};
        auto desiredTime = ma_engine_get_time_in_milliseconds(&g_engine);
        ma_uint64 desiredFrame = (ma_uint64)(((double)desiredTime/1000.0) * (double)decoder.outputSampleRate) / 512.0;
        return freqs[desiredFrame];
    }

    std::vector<float> GetFreqs5plus() {
        if (!initialized)
            return {};
        auto desiredTime = ma_engine_get_time_in_milliseconds(&g_engine);
        ma_uint64 desiredFrame = (ma_uint64)((((double)desiredTime + 1000.0)/ 1000.0) * (double)decoder.outputSampleRate) / 512.0;
        return freqs[desiredFrame];

    }
    void data_callback(void* pUserData, ma_uint8* pBuffer, int bufferSizeInBytes)
    {



        /* Reading is just a matter of reading straight from the engine. */
        ma_uint32 bufferSizeInFrames = (ma_uint32)bufferSizeInBytes / ma_get_bytes_per_frame(ma_format_f32, ma_engine_get_channels(&g_engine));
        lastbufferSizeInBytes = bufferSizeInFrames;
        ma_engine_read_pcm_frames(&g_engine, pBuffer, bufferSizeInFrames, NULL);


   
 
        buffer = reinterpret_cast<float*>(pBuffer);
        for (int ax = 0; ax < bufferSizeInFrames * 2; ax += 2)
        {
            lastBuffer->push(buffer[ax]);
        }
        //lastBuffer->push(buffer, bufferSizeInFrames);



    }
    void PauseSoundTr() {
        /* Start playback. */
        SDL_CloseAudioDevice(deviceID);
        ma_sound_stop(&g_sound);
    }

    bool file_exists(const std::string& name) {
        if (FILE* file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        }
        else {
            return false;
        }
    }

    int PlaySoundTr(AudioData* data) {

        AssetManager manager;

        AudioData* audioData = data;
  
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

        //FFT
    


        size_t ndata = 1;
        FixedSizeQueue<std::complex<float>, 512 * 40> audiofat;

        ma_format outputFormat = ma_format_f32;  // You can choose the desired output format here

        ma_uint64 outputBufferFrames = 512;  // Choose an appropriate buffer size
        ma_uint64 outputBufferSizeInBytes = ma_get_bytes_per_frame(ma_format_f32, engineConfig.channels) * outputBufferFrames;
        float* pOutputBuffer = (float*)malloc(outputBufferSizeInBytes);
        ma_uint64 framesRead;

        std::string filepath = "Assets/FFTs/" + data->filename + ".FFT";
        if (!file_exists(filepath)) {
            FILE* pFile = fopen(filepath.c_str(), "wb");
            while (1) {
                pocketfft::shape_t shape = { (unsigned long long)audiofat.size };
                pocketfft::stride_t strided(shape.size());
                size_t tmpd = sizeof(std::complex<float>);
                for (int i = shape.size() - 1; i >= 0; --i)
                {
                    strided[i] = tmpd;
                    tmpd *= shape[i];
                }

                ma_uint64 frameCount = outputBufferFrames;
                ma_result result = ma_decoder_read_pcm_frames(&decoder, pOutputBuffer, frameCount, &framesRead);
                if (result != MA_SUCCESS) {
                    break;  // End of file or an error occurred
                }

                for (int ax = 0; ax < frameCount * ma_get_bytes_per_frame(outputFormat, engineConfig.channels) / sizeof(float); ax += 2)
                {
                    audiofat.push({ pOutputBuffer[ax], 0 });
                }


                std::vector<std::complex<float>> dat_out(audiofat.size);
                pocketfft::c2c<float>(shape, strided, strided, { 0 }, true, audiofat.getbuffer(), dat_out.data(), 1.0f);

                std::vector<float> freqs;
                float step = 1.05946309435f;
                for (float f = 20.0f; f < dat_out.size(); f *= step) {
                    float freq = 0;
                    for (int i = f; i < dat_out.size() && i < f * step; i++) {
                        auto b = std::abs(dat_out[i]);
                        if (b > freq) freq = b;
                    }
                    freqs.push_back(freq);
                }

                // Write the raw PCM audio data to the output file
                size_t siz = freqs.size();
                fwrite(&siz, 1, sizeof(size_t), pFile);
       
                fwrite(freqs.data(), freqs.size(), sizeof(float), pFile);
            }
            fclose(pFile);
            delete pOutputBuffer;
        }
        //FFT
        FILE* pFile = fopen(filepath.c_str(), "rb");  // Open the file in binary mode for reading


        // Determine the size of the file
        fseek(pFile, 0, SEEK_END);
        long fileSize = ftell(pFile);
        rewind(pFile);

        // Read the file into a buffer
        std::vector<char> buffer(fileSize);

        long cursor = 0;
        size_t size = 0;
        int i = 0;
        while (cursor < fileSize) {
      
            size_t bytesRead = fread(&size, 1, sizeof(size_t), pFile);
            std::vector<float> freq(size);// Read the entire file into the buffer
            bytesRead += fread(freq.data(), size,  sizeof(float), pFile);
            freqs.push_back(freq);
            cursor += 8 + size * sizeof(float);
        }
        fclose(pFile);

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