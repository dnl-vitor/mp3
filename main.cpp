#include <raylib.h>
#include "player.h"
#include "ui.h"

const int scr_width = 400;
const int scr_height = 400;

int main(int argc, char *argv[]) {
    InitWindow(scr_width, scr_height, "mp3");
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(scr_width, scr_height);
    SetWindowMaxSize(scr_width, scr_height);
    InitAudioDevice();
    SetTargetFPS(60);

    Player player;
    player.LoadFolder(argc > 1 ? argv[1] : "assets");    
    player.Play(0);

    // layout derivado do tamanho da janela
    const float margin = 20;
    const float wheelR = 95;   // raio da roda (definido em ui.cpp)

    Vector2 wheel = {scr_width / 2.0f, scr_height - wheelR - margin};
    Rectangle screen = {margin, margin, scr_width - 2 * margin,
                        wheel.y - wheelR - 2 * margin};
    Rectangle volBar = {screen.x + screen.width - 15 - 22, screen.y + 25,
                        22, screen.height - 50};
    Rectangle title = {screen.x + 15, screen.y + 15,
                       volBar.x - screen.x - 30, 30};
    Rectangle shuffleBtn = {screen.x + 15, screen.y + screen.height - 45, 110, 30};
    Rectangle volDown = {wheel.x - wheelR - 15 - 60, wheel.y - 25, 60, 50};
    Rectangle volUp   = {wheel.x + wheelR + 15,      wheel.y - 25, 60, 50};

    while (!WindowShouldClose()) {
        if (IsWindowMaximized()) RestoreWindow();
        if (IsWindowResized() &&
            (GetScreenWidth() != scr_width || GetScreenHeight() != scr_height))
            SetWindowSize(scr_width, scr_height);

        player.Update();

        BeginDrawing();
            ClearBackground(LIGHTGRAY);

            // tela
            DrawRectangleRec(screen, RAYWHITE);
            DrawRectangleLinesEx(screen, 2, DARKGRAY);

            if (!player.IsEmpty())
                DrawTextMarquee(GetFileNameWithoutExt(player.CurrentTrack().c_str()),
                                title, 20, -50.0f, BLACK);
            else
                DrawText("Nenhuma musica encontrada", (int)title.x, (int)title.y, 20, RED);

            DrawVolumeBar(volBar, player.GetVolume());

            if (DrawButton(shuffleBtn, "Aleatorio", player.IsShuffle()))
                player.ToggleShuffle();

            // painel inferior
            switch (DrawClickWheel(wheel, player.IsPaused())) {
                case WheelAction::Prev:      player.Prev();        break;
                case WheelAction::PlayPause: player.TogglePause(); break;
                case WheelAction::Next:      player.Next();        break;
                default: break;
            }

            if (DrawButton(volDown, "-")) player.ChangeVolume(-0.1f);
            if (DrawButton(volUp, "+"))   player.ChangeVolume(0.1f);
        EndDrawing();
    }

    player.Unload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
