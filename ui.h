#pragma once

#include <raylib.h>
#include <string>

enum class WheelAction {None, Prev, PlayPause, Next};

// Desenha o botão e retorna true se foi clicado neste frame
// "active" pinta o botão como ligado 
bool DrawButton(Rectangle r, const char *label, bool active = false);

// Roda de controle: devolve qual parte foi clicada neste frame
// "paused" define se o centro mostra o ícone de play ou de pause
WheelAction DrawClickWheel(Vector2 center, bool paused);

// Barra de volume vertical
// Recebe o volume atual (0 a 1) e devolve o volume atualizado
void DrawVolumeBar(Rectangle r, float volume);

// speed em pixels por segundo: positivo = esquerda para a direita,
// negativo = direita para a esquerda.
void DrawTextMarquee(const std::string &text, Rectangle bounds,
                     int fontSize, float speed, Color color);

