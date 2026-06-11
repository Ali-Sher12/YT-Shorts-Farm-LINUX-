#pragma once
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <string>
#include <stdexcept>

class VideoRecorder {
public:
    VideoRecorder(unsigned width, unsigned height, int fps,
                  const std::string& outputFile,
                  const std::string& audioFile = "")
        : m_width(width), m_height(height), m_fps(fps)
    {
        // Build FFmpeg command
        // -f rawvideo -pix_fmt rgba : input is raw RGBA from stdin
        // -r : framerate
        // -i pipe:0 : read from stdin
        // -i audioFile : optional audio
        // -c:v libx264 -pix_fmt yuv420p : H.264, browser/phone compatible
        // -crf 18 : near-lossless quality (lower = better, 0 = lossless)
        // -preset fast : encoding speed vs compression tradeoff

        std::string cmd = "ffmpeg -y"
            " -f rawvideo -pix_fmt rgba"
            " -s " + std::to_string(width) + "x" + std::to_string(height) +
            " -r " + std::to_string(fps) +
            " -i pipe:0";

        if (!audioFile.empty())
            cmd += " -i \"" + audioFile + "\"";

        cmd += " -c:v libx264 -pix_fmt yuv420p"
               " -crf 18 -preset fast";

        if (!audioFile.empty())
            cmd += " -c:a aac -b:a 192k -shortest";

        cmd += " \"" + outputFile + "\"";

        m_pipe = popen(cmd.c_str(), "w");
        if (!m_pipe)
            throw std::runtime_error("Failed to open FFmpeg pipe");
    }

    ~VideoRecorder() {
        if (m_pipe) pclose(m_pipe);
    }

    // Call once per frame after drawing to your RenderTexture
    void writeFrame(const sf::RenderTexture& rt) {
        sf::Image img = rt.getTexture().copyToImage();
        const uint8_t* pixels = img.getPixelsPtr();
        std::size_t size = m_width * m_height * 4; // RGBA
        fwrite(pixels, 1, size, m_pipe);
    }

private:
    unsigned m_width, m_height;
    int m_fps;
    FILE* m_pipe = nullptr;
};