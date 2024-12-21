#include "AudioManager.h"
#include <iostream>
#include <stdexcept>
#include <thread> 
#include <chrono> 
AudioManager& AudioManager::getInstance() {
    static AudioManager instance; // Guaranteed to be destroyed, instantiated on first use
    return instance;
}

void AudioManager::playMusic(const std::string& filepath, bool loop) {
    if (!music.openFromFile(filepath)) {
        throw std::runtime_error("Failed to load music: " + filepath);
    }
    music.setLoop(loop);
    music.play();
}

void AudioManager::stopMusic() {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(0.2 * 1000)));

    music.stop();
}
