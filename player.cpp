#include "player.h"
#include <algorithm>
#include <numeric>

void Player::LoadFolder(const char *folder) {
    FilePathList files = LoadDirectoryFilesEx(folder, ".mp3", false);
    for (unsigned int i = 0; i < files.count; i++)
        tracks.push_back(files.paths[i]);
    UnloadDirectoryFiles(files);

    std::sort(tracks.begin(), tracks.end());

    order.resize(tracks.size());
    std::iota(order.begin(), order.end(), 0);   // 0, 1, 2, ...
}

void Player::Play(int p) {
    if (tracks.empty()) return;
    if (loaded) UnloadMusicStream(music);

    pos = (p + order.size()) % order.size();
    music = LoadMusicStream(tracks[order[pos]].c_str());
    music.looping = false;
    SetMusicVolume(music, volume);   // música nova volta ao volume padrão
    PlayMusicStream(music);
    loaded = true;
    paused = false;
}

void Player::Next() { Play(pos + 1); }
void Player::Prev() { Play(pos - 1); }

void Player::TogglePause() {
    if (!loaded) return;
    paused = !paused;
    if (paused) PauseMusicStream(music);
    else        ResumeMusicStream(music);
}

void Player::ToggleShuffle() {
    if (tracks.empty()) return;
    shuffle = !shuffle;
    int currentTrack = order[pos];

    if (shuffle) {
        std::shuffle(order.begin(), order.end(), rng);
        // mantém a faixa atual como primeira, sem interromper o som
        auto it = std::find(order.begin(), order.end(), currentTrack);
        std::iter_swap(order.begin(), it);
        pos = 0;
    } else {
        std::iota(order.begin(), order.end(), 0);
        pos = currentTrack;
    }
}

void Player::SetVolume(float v) {
    volume = std::clamp(v, 0.0f, 1.0f);
    if (loaded) SetMusicVolume(music, volume);
}

void Player::ChangeVolume(float delta) { SetVolume(volume + delta); }

void Player::Update() {
    if (!loaded) return;
    UpdateMusicStream(music);

    if (!paused && GetMusicTimePlayed(music) >= GetMusicTimeLength(music) - 0.05f)
        Next();
}

void Player::Unload() {
    if (loaded) {
        UnloadMusicStream(music);
        loaded = false;
    }
}
