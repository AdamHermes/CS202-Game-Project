#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <string>
#include <map>

class AudioManager {
public:
    static AudioManager& getInstance(); // Singleton access


    void playMusic(const std::string& filepath, bool loop = true);
    void stopMusic();

private:
    AudioManager() = default;                // Private constructor
    ~AudioManager() = default;               // Private destructor
    AudioManager(const AudioManager&) = delete;            // Prevent copying
    AudioManager& operator=(const AudioManager&) = delete; // Prevent assignment



    sf::Music music;
};

#endif // AUDIOMANAGER_H
