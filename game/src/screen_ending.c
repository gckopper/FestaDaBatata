/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Color text_color = {49, 64, 275};
static Vector2 text_start = {0};
static const char const header[] = "ID BATATAS MOEDAS BONUS";
static float font_height = 0;
static const float text_height = 48.0f;
static const float text_spacing = 4.0f;
static const char const format_string[] = "%.2u  %.5u  %.6u %.5u";
static const char* const bonus_text[] = {"Moedas", "Passos", "Emotes"};
static const char const bonus_template[] = "Bonus escolhido: ";
static Vector2 bonus_text_pos = { 0 };
static Vector2 bonus_template_pos = { 0 };
//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    int mid_width  = GetScreenWidth()  / 2;
    int mid_height = GetScreenHeight() / 2;
    Vector2 header_size = MeasureTextEx(font, header, text_height, text_spacing);
    text_start.x = mid_width - (header_size.x / 2);
    text_start.y = 10.0f;
    font_height = header_size.y;
    Vector2 bonus_text_size = MeasureTextEx(font, bonus_text[0], text_height, text_spacing);
    Vector2 bonus_template_size = MeasureTextEx(font, bonus_template, text_height, text_spacing);
    bonus_template_pos.x = mid_width - ((bonus_template_size.x + bonus_text_size.x) / 2);
    bonus_template_pos.y = GetScreenHeight() - bonus_template_size.y;
    bonus_text_pos.x = bonus_template_pos.x + bonus_template_size.x;
    bonus_text_pos.y = GetScreenHeight() - bonus_text_size.y;
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // TODO: Update ENDING screen variables here!

    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(sounds[0]);
    }
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    // TODO: Draw ENDING screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);

    DrawTextEx(font, header, text_start, text_height, text_spacing, DARKBLUE);
    char text[32] = { 0 };
    Vector2 pos = { text_start.x, text_start.y };
    unsigned long long bonus = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (podium[i].player_id == MAX_PLAYERS) {
            break;
        }
        switch (podium[i].bonus_type)
        {
        case 0:
            bonus = podium[i].coins;
            break;
        case 1:
            bonus = podium[i].steps;
            break;
        case 2:
            bonus = podium[i].emotes;
            break;
        default:
            break;
        }
        sprintf_s(text, 32, format_string, podium[i].player_id, podium[i].batatas, podium[i].current_coins, bonus);
        pos.y += text_height;
        DrawTextEx(font, text, pos, text_height, text_spacing, DARKBLUE);
    }
    DrawTextEx(font, bonus_text[podium[0].bonus_type], bonus_text_pos, text_height, text_spacing, DARKBLUE);
    DrawTextEx(font, bonus_template, bonus_template_pos, text_height, text_spacing, DARKBLUE);
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}
