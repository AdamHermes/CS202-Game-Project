#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

class SoundEffect {
private:
    sf::Sound sound;        // Plays the sound
    std::unordered_map<std::string, sf::SoundBuffer> soundMap; // Map of sound buffers
    std::string currentSound; // Tracks the currently loaded sound name

public:
    // Load a sound and associate it with a name
    void loadSound(const std::string& name, const std::string& filepath) {
        sf::SoundBuffer newBuffer;
        if (newBuffer.loadFromFile(filepath)) {
            soundMap[name] = newBuffer;
        }
    }

    sf::Sound::Status getStatus() const {
        return sound.getStatus();
    }

    void setSound(const std::string& name) {
        if (name != currentSound && soundMap.find(name) != soundMap.end()) {
            sound.setBuffer(soundMap[name]);
            currentSound = name;
        }
    }

    void setLoop(bool loop) {
        sound.setLoop(loop);
    }

    void stop() {
        sound.stop();
    }

    void play() {
        sound.play();
    }
};
