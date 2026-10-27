#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <random>

class Player{
public:
    void LoadFolder (const char *folder);
    void Play(int pos);
    void Next();
    void Prev();
    void TogglePause();
    void ToggleShuffle();
    void SetVolume(float v);
    void ChangeVolume(float delta);
    void Update();
    void Unload();

    bool IsEmpty() const {return tracks.empty();}
    bool IsPaused() const {return paused;}
    bool IsShuffle() const {return shuffle;}
    float GetVolume() const {return volume;}
    const std::string &CurrentTrack() const {return tracks[order[pos]];}

private:
    std::vector<std::string> tracks;
    std::vector<int> order;
    int pos = 0;
    Music music{};
    bool loaded = false;
    bool paused = false;
    bool shuffle = false;
    float volume = 0.5f;
    std::mt19937 rng{std::random_device{}()};
};
