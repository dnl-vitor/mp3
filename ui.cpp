#include "ui.h" 
#include <raylib.h>
#include <cmath>
#include <algorithm>

bool DrawButton(Rectangle r, const char *label, bool active){
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, r);
    bool down = hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    Color bg = RAYWHITE;
    if (active) bg = SKYBLUE;
    else if (down) bg = GRAY;
    else if (hover) bg = LIGHTGRAY;

    DrawRectangleRec(r, bg);
    DrawRectangleLinesEx(r, 2, DARKGRAY);

    int fontSize = 20;
    int textW = MeasureText(label, fontSize);
    DrawText(label, 
            (int)(r.x + (r.width - textW) / 2),
            (int)(r.y + (r.height - fontSize) / 2),
            fontSize, BLACK);

    return hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

// ---------- ícones ----------

static void DrawPlayIcon(Vector2 p, Color col) {
    DrawTriangle({p.x - 8, p.y - 12}, {p.x - 8, p.y + 12}, {p.x + 12, p.y}, col);
}

static void DrawPauseIcon(Vector2 p, Color col) {
    DrawRectangle((int)p.x - 10, (int)p.y - 12, 7, 24, col);
    DrawRectangle((int)p.x + 3,  (int)p.y - 12, 7, 24, col);
}

static void DrawSkipIcon(Vector2 p, bool next, Color col) {
    if (next) {
        DrawTriangle({p.x - 8, p.y - 10}, {p.x - 8, p.y + 10}, {p.x + 8, p.y}, col);
        DrawRectangle((int)p.x + 8, (int)p.y - 10, 3, 20, col);
    } else {
        DrawTriangle({p.x + 8, p.y - 10}, {p.x - 8, p.y}, {p.x + 8, p.y + 10}, col);
        DrawRectangle((int)p.x - 11, (int)p.y - 10, 3, 20, col);
    }
}

// ---------- roda de controle ----------

WheelAction DrawClickWheel(Vector2 c, bool paused) {
    const float outerR  = 95;   // círculo externo
    const float centerR = 38;   // botão central (play/pause)
    const float sideR   = 24;   // área clicável de anterior/próxima
    const float offset  = 66;   // distância dos botões laterais ao centro

    Vector2 left  = {c.x - offset, c.y};
    Vector2 right = {c.x + offset, c.y};
    Vector2 mouse = GetMousePosition();

    bool overLeft   = CheckCollisionPointCircle(mouse, left,  sideR);
    bool overRight  = CheckCollisionPointCircle(mouse, right, sideR);
    bool overCenter = CheckCollisionPointCircle(mouse, c, centerR);

    // base
    DrawCircleV(c, outerR, RAYWHITE);
    DrawCircleLines((int)c.x, (int)c.y, outerR, DARKGRAY);

    // botão central
    DrawCircleV(c, centerR, overCenter ? LIGHTGRAY : Color{235, 235, 235, 255});
    DrawCircleLines((int)c.x, (int)c.y, centerR, DARKGRAY);
    if (paused) DrawPlayIcon(c, BLACK);
    else        DrawPauseIcon(c, BLACK);

    // anterior e próxima
    DrawSkipIcon(left,  false, overLeft  ? GRAY : BLACK);
    DrawSkipIcon(right, true,  overRight ? GRAY : BLACK);

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (overLeft)   return WheelAction::Prev;
        if (overRight)  return WheelAction::Next;
        if (overCenter) return WheelAction::PlayPause;
    }
    return WheelAction::None;
}

// ---------- barra de volume ----------

void DrawVolumeBar(Rectangle r, float volume) {
    float fillH = r.height * volume;
    DrawRectangleRec(r, RAYWHITE);
    DrawRectangleRec({r.x, r.y + r.height - fillH, r.width, fillH}, BLACK);
    DrawRectangleLinesEx(r, 2, BLACK);
}
// ---------- texto circular -----------
void DrawTextMarquee(const std::string &text, Rectangle bounds,
                     int fontSize, float speed, Color color) {
    // reinicia a rolagem quando a música muda
    static std::string lastText;
    static double startTime = 0;
    if (text != lastText) {
        lastText = text;
        startTime = GetTime();
    }

    const float gap = 60;
    int textW = MeasureText(text.c_str(), fontSize);

    // distância entre uma cópia do texto e a seguinte
    float period = std::max((float)textW, bounds.width) + gap;
    float offset = fmodf((float)(GetTime() - startTime) * fabsf(speed), period);

    float x1 = (speed >= 0) ? bounds.x + offset - period
                            : bounds.x - offset;
    int y = (int)(bounds.y + (bounds.height - fontSize) / 2);

    BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
        DrawText(text.c_str(), (int)x1, y, fontSize, color);
        DrawText(text.c_str(), (int)(x1 + period), y, fontSize, color);
    EndScissorMode();
}
