#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

// Global frame counter to align video frames and audio play timestamps
inline std::uint64_t currentFrameIndex = 0;

#include <csignal>
inline volatile std::sig_atomic_t g_signalStatus = 0;
inline void handleCloseSignal(int signal) {
    g_signalStatus = signal;
}

struct SoundEvent {
    const sf::SoundBuffer* buffer;
    std::uint64_t startFrame;
    std::uint64_t endFrame; // std::numeric_limits<std::uint64_t>::max() if not stopped
    float volume;
    float pitch;
    bool looping;
};

class AudioMixer {
public:
    static AudioMixer& getInstance() {
        static AudioMixer instance;
        return instance;
    }

    std::vector<SoundEvent> soundEvents;

    struct MusicEvent {
        std::string filename;
        std::uint64_t startFrame;
        std::uint64_t endFrame;
        float volume = 100.0f;
    };
    std::vector<MusicEvent> musicEvents;

    void addSoundEvent(const sf::SoundBuffer* buffer, std::uint64_t startFrame, float volume, float pitch, bool looping, std::uint64_t* outEventIndex) {
        SoundEvent ev;
        ev.buffer = buffer;
        ev.startFrame = startFrame;
        ev.endFrame = std::numeric_limits<std::uint64_t>::max();
        ev.volume = volume;
        ev.pitch = pitch;
        ev.looping = looping;
        soundEvents.push_back(ev);
        if (outEventIndex) {
            *outEventIndex = soundEvents.size() - 1;
        }
    }

    void stopSoundEvent(std::uint64_t eventIndex, std::uint64_t endFrame) {
        if (eventIndex < soundEvents.size()) {
            soundEvents[eventIndex].endFrame = endFrame;
        }
    }

    void addMusicEvent(const std::string& filename, std::uint64_t startFrame, float volume) {
        MusicEvent ev;
        ev.filename = filename;
        ev.startFrame = startFrame;
        ev.endFrame = std::numeric_limits<std::uint64_t>::max();
        ev.volume = volume;
        musicEvents.push_back(ev);
    }

    void stopMusicEvent(std::uint64_t startFrame, std::uint64_t endFrame) {
        for (auto& ev : musicEvents) {
            if (ev.startFrame == startFrame && ev.endFrame == std::numeric_limits<std::uint64_t>::max()) {
                ev.endFrame = endFrame;
            }
        }
    }

    void mixAndSaveWav(const std::string& filename, std::uint64_t totalFrames, unsigned int sampleRate = 44100) {
        std::uint64_t totalSamples = totalFrames * (sampleRate / 60);
        if (totalSamples == 0) return;

        std::vector<float> masterL(totalSamples, 0.0f);
        std::vector<float> masterR(totalSamples, 0.0f);

        // Mix music events first
        for (const auto& musicEv : musicEvents) {
            sf::SoundBuffer musicBuf;
            if (!musicBuf.loadFromFile(musicEv.filename)) {
                std::cerr << "Warning: Could not load music file for offline mixing: " << musicEv.filename << std::endl;
                continue;
            }
            mixBuffer(masterL, masterR, musicBuf, musicEv.startFrame, musicEv.endFrame, musicEv.volume, 1.0f, true, sampleRate);
        }

        // Mix sound events
        for (const auto& soundEv : soundEvents) {
            if (!soundEv.buffer) continue;
            mixBuffer(masterL, masterR, *soundEv.buffer, soundEv.startFrame, soundEv.endFrame, soundEv.volume, soundEv.pitch, soundEv.looping, sampleRate);
        }

        // Save to WAV file
        saveWav(filename, masterL, masterR, sampleRate);
    }

private:
    void mixBuffer(std::vector<float>& masterL, std::vector<float>& masterR, const sf::SoundBuffer& buffer,
                   std::uint64_t startFrame, std::uint64_t endFrame, float volume, float pitch, bool looping, unsigned int sampleRate) {
        
        const std::int16_t* samples = buffer.getSamples();
        std::uint64_t sampleCount = buffer.getSampleCount();
        unsigned int channels = buffer.getChannelCount();
        unsigned int srcRate = buffer.getSampleRate();

        if (sampleCount == 0 || channels == 0) return;

        std::uint64_t srcFrames = sampleCount / channels;
        std::uint64_t startSample = startFrame * (sampleRate / 60);
        std::uint64_t endSample = (endFrame == std::numeric_limits<std::uint64_t>::max()) ? masterL.size() : endFrame * (sampleRate / 60);

        if (startSample >= masterL.size()) return;
        if (endSample > masterL.size()) endSample = masterL.size();

        float rateFactor = (static_cast<float>(srcRate) / static_cast<float>(sampleRate)) * pitch;

        for (std::uint64_t i = startSample; i < endSample; ++i) {
            float srcPos = static_cast<float>(i - startSample) * rateFactor;

            if (looping) {
                srcPos = std::fmod(srcPos, static_cast<float>(srcFrames));
            } else {
                if (srcPos >= static_cast<float>(srcFrames - 1)) {
                    break; // Finished playing
                }
            }

            std::uint64_t idx = static_cast<std::uint64_t>(srcPos);
            float frac = srcPos - static_cast<float>(idx);
            std::uint64_t idxNext = idx + 1;
            if (looping && idxNext >= srcFrames) {
                idxNext = idxNext % srcFrames;
            } else if (idxNext >= srcFrames) {
                idxNext = idx; // clamp to last
            }

            float sampleL = 0.0f;
            float sampleR = 0.0f;

            if (channels == 1) {
                float s1 = samples[idx] / 32768.0f;
                float s2 = samples[idxNext] / 32768.0f;
                float interp = (1.0f - frac) * s1 + frac * s2;
                sampleL = interp;
                sampleR = interp;
            } else if (channels == 2) {
                float s1L = samples[idx * 2] / 32768.0f;
                float s2L = samples[idxNext * 2] / 32768.0f;
                sampleL = (1.0f - frac) * s1L + frac * s2L;

                float s1R = samples[idx * 2 + 1] / 32768.0f;
                float s2R = samples[idxNext * 2 + 1] / 32768.0f;
                sampleR = (1.0f - frac) * s1R + frac * s2R;
            }

            masterL[i] += sampleL * (volume / 100.0f);
            masterR[i] += sampleR * (volume / 100.0f);
        }
    }

    void saveWav(const std::string& filename, const std::vector<float>& left, const std::vector<float>& right, unsigned int sampleRate) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        std::uint32_t numSamples = left.size();
        std::uint32_t numChannels = 2;
        std::uint32_t bitsPerSample = 16;
        std::uint32_t byteRate = sampleRate * numChannels * (bitsPerSample / 8);
        std::uint32_t blockAlign = numChannels * (bitsPerSample / 8);
        std::uint32_t subChunk2Size = numSamples * numChannels * (bitsPerSample / 8);
        std::uint32_t chunkSSize = 36 + subChunk2Size;

        file.write("RIFF", 4);
        file.write(reinterpret_cast<const char*>(&chunkSSize), 4);
        file.write("WAVE", 4);

        file.write("fmt ", 4);
        std::uint32_t subChunk1Size = 16;
        file.write(reinterpret_cast<const char*>(&subChunk1Size), 4);
        std::uint16_t audioFormat = 1; // PCM
        file.write(reinterpret_cast<const char*>(&audioFormat), 2);
        file.write(reinterpret_cast<const char*>(&numChannels), 2);
        file.write(reinterpret_cast<const char*>(&sampleRate), 4);
        file.write(reinterpret_cast<const char*>(&byteRate), 4);
        file.write(reinterpret_cast<const char*>(&blockAlign), 2);
        file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

        file.write("data", 4);
        file.write(reinterpret_cast<const char*>(&subChunk2Size), 4);

        for (std::uint32_t i = 0; i < numSamples; ++i) {
            float valL = left[i];
            if (valL > 1.0f) valL = 1.0f;
            if (valL < -1.0f) valL = -1.0f;
            std::int16_t sampleL = static_cast<std::int16_t>(valL * 32767.0f);
            file.write(reinterpret_cast<const char*>(&sampleL), 2);

            float valR = right[i];
            if (valR > 1.0f) valR = 1.0f;
            if (valR < -1.0f) valR = -1.0f;
            std::int16_t sampleR = static_cast<std::int16_t>(valR * 32767.0f);
            file.write(reinterpret_cast<const char*>(&sampleR), 2);
        }
    }
};

class RecorderSound {
public:
    RecorderSound() : m_buffer(nullptr), m_volume(100.0f), m_pitch(1.0f), m_looping(false), m_hasEventIndex(false), m_eventIndex(0) {}
    RecorderSound(const sf::SoundBuffer& buffer) : m_buffer(&buffer), m_volume(100.0f), m_pitch(1.0f), m_looping(false), m_hasEventIndex(false), m_eventIndex(0) {}
    
    void setBuffer(const sf::SoundBuffer& buffer) {
        m_buffer = &buffer;
    }
    
    const sf::SoundBuffer* getBuffer() const {
        return m_buffer;
    }

    void play() {
        if (!m_buffer) return;
        // If already playing, stop the previous event
        if (m_hasEventIndex) {
            AudioMixer::getInstance().stopSoundEvent(m_eventIndex, currentFrameIndex);
        }
        AudioMixer::getInstance().addSoundEvent(m_buffer, currentFrameIndex, m_volume, m_pitch, m_looping, &m_eventIndex);
        m_hasEventIndex = true;
    }

    void stop() {
        if (m_hasEventIndex) {
            AudioMixer::getInstance().stopSoundEvent(m_eventIndex, currentFrameIndex);
            m_hasEventIndex = false;
        }
    }

    void setVolume(float volume) {
        m_volume = volume;
    }

    float getVolume() const {
        return m_volume;
    }

    void setPitch(float pitch) {
        m_pitch = pitch;
    }

    float getPitch() const {
        return m_pitch;
    }

    void setLooping(bool loop) {
        m_looping = loop;
    }

    bool getLooping() const {
        return m_looping;
    }

    sf::Sound::Status getStatus() const {
        return m_hasEventIndex ? sf::Sound::Status::Playing : sf::Sound::Status::Stopped;
    }

private:
    const sf::SoundBuffer* m_buffer;
    float m_volume;
    float m_pitch;
    bool m_looping;
    bool m_hasEventIndex;
    std::uint64_t m_eventIndex;
};

class RecorderMusic {
public:
    RecorderMusic() : m_filename(""), m_volume(100.0f), m_looping(true), m_playing(false), m_startFrame(0) {}
    RecorderMusic(const std::string& filename) : m_filename(filename), m_volume(100.0f), m_looping(true), m_playing(false), m_startFrame(0) {}

    bool openFromFile(const std::string& filename) {
        m_filename = filename;
        return true;
    }

    void play() {
        m_playing = true;
        m_startFrame = currentFrameIndex;
        AudioMixer::getInstance().addMusicEvent(m_filename, m_startFrame, m_volume);
    }

    void stop() {
        if (m_playing) {
            AudioMixer::getInstance().stopMusicEvent(m_startFrame, currentFrameIndex);
            m_playing = false;
        }
    }

    void setVolume(float volume) {
        m_volume = volume;
    }

    float getVolume() const {
        return m_volume;
    }

    void setLooping(bool loop) {
        m_looping = loop;
    }

    bool isLooping() const {
        return m_looping;
    }

private:
    std::string m_filename;
    float m_volume;
    bool m_looping;
    bool m_playing;
    std::uint64_t m_startFrame;
};

class SignalRegisterer {
public:
    SignalRegisterer() {
        std::signal(SIGINT, handleCloseSignal);
        std::signal(SIGTERM, handleCloseSignal);
    }
};

class RecorderWindow : private SignalRegisterer, public sf::RenderWindow {
public:
    // Inherit all base class constructors
    using sf::RenderWindow::RenderWindow;

    template <typename... Args>
    void create(Args&&... args) {
        sf::RenderWindow::create(std::forward<Args>(args)...);
        std::signal(SIGINT, handleCloseSignal);
        std::signal(SIGTERM, handleCloseSignal);
    }

    ~RecorderWindow() {
        closeRecording();
    }

    void setFramerateLimit(unsigned int limit) {
        (void)limit;
        // Render as fast as possible for offline export.
        setVerticalSyncEnabled(false);
    }

    void display() {
        if (g_signalStatus != 0) {
            std::cout << "[Offline Recorder] Signal " << g_signalStatus << " received. Closing window cleanly..." << std::endl;
            close();
            return;
        }

        sf::RenderWindow::display();

        if (m_recordingFinished) {
            return;
        }

        if (!m_recordingStarted) {
            initRecording();
        }

        if (m_ffmpegPipe) {
            m_captureTexture.update(*this);
            sf::Image img = m_captureTexture.copyToImage();
            sf::Vector2u sz = img.getSize();
            const std::uint8_t* pixels = img.getPixelsPtr();
            size_t written = fwrite(pixels, 1, sz.x * sz.y * 4, m_ffmpegPipe);
            if (written != sz.x * sz.y * 4) {
                std::cerr << "Warning: FFmpeg pipe write mismatch or failed." << std::endl;
            }
        }
        currentFrameIndex++;
#ifdef AUTO_EXIT_FRAME_LIMIT
        if (currentFrameIndex >= AUTO_EXIT_FRAME_LIMIT) {
            close();
        }
#endif
    }

    void close() {
        sf::RenderWindow::close();
        closeRecording();
    }

private:
    FILE* m_ffmpegPipe = nullptr;
    sf::Texture m_captureTexture;
    std::uint32_t m_width = 0;
    std::uint32_t m_height = 0;
    bool m_recordingStarted = false;
    bool m_recordingFinished = false;

    void initRecording() {
        m_recordingStarted = true;
        m_width = getSize().x;
        m_height = getSize().y;
        m_captureTexture = sf::Texture({m_width, m_height});

        const unsigned encW = m_width + (m_width & 1u);
        const unsigned encH = m_height + (m_height & 1u);

        std::string cmd = "./bin/ffmpeg -y -f rawvideo -pix_fmt rgba -s " +
                          std::to_string(m_width) + "x" + std::to_string(m_height) +
                          " -r 60 -i - -vf scale=" + std::to_string(encW) + ":" + std::to_string(encH) +
                          " -c:v libx264 -pix_fmt yuv420p temp_video.mp4";

        std::cout << "[Offline Recorder] Initializing video encoding pipe..." << std::endl;
        m_ffmpegPipe = popen(cmd.c_str(), "w");
        if (!m_ffmpegPipe) {
            std::cerr << "Error: Failed to open FFmpeg pipe!" << std::endl;
        }
    }

    void closeRecording() {
        if (m_ffmpegPipe) {
            std::cout << "[Offline Recorder] Closing video encoding pipe. Finalizing frames..." << std::endl;
            pclose(m_ffmpegPipe);
            m_ffmpegPipe = nullptr;
            m_recordingFinished = true;

            std::cout << "[Offline Recorder] Mixing audio tracks..." << std::endl;
            AudioMixer::getInstance().mixAndSaveWav("temp_audio.wav", currentFrameIndex, 44100);

            std::cout << "[Offline Recorder] Muxing video and audio into output.mp4..." << std::endl;
            std::string muxCmd = "./bin/ffmpeg -y -i temp_video.mp4 -i temp_audio.wav -c:v copy -c:a aac -shortest output.mp4";
            int ret = system(muxCmd.c_str());
            if (ret == 0) {
                std::cout << "[Offline Recorder] Successfully created output.mp4!" << std::endl;
            } else {
                std::cerr << "Error: Muxing failed with code " << ret << std::endl;
            }

            // Cleanup temp files
            std::remove("temp_video.mp4");
            std::remove("temp_audio.wav");
        }
    }
};

// Macro overrides to intercept types cleanly
#define Sound RecorderSound
#define Music RecorderMusic
#define RenderWindow RecorderWindow
