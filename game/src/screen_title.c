/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <stdio.h>


#define IP_PAD_Y 12.0
#define IP_SIZ_Y 72.0
#define IP_PAD_X 8.0
#define IP_SIZ_X 33.0*3.0
#define IP_SIZE 4
#define IP_TITLE_LOCY 11
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
// Um byte eh representado por 3 digitos decimais + null
static char ip_addr_arr[IP_SIZE][4] = {{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}};
static unsigned char mouse_on_text = 0;
// codigo da sala
static int codeCount = 0;
static bool mouse_on_room = false;
static Rectangle room_rekt = {0};//{267.0f, 270.0f, 264.0f, 72.0f};
static Vector2 text_code = {0};

static int ip_loc_y = 0;
static int ip_loc_x = 0;
static Rectangle ip_rects[IP_SIZE] = {0, 0, 0, 0 };
static int letterCount[IP_SIZE] = { 0, 0, 0, 0 };
static bool show_wrong_ip_msg = false;
static Vector2 wrong_ip_msg_loc = { 0 };
static const char* wrong_ip_msg = "O ENDEREÇO IP É INVÁLIDO";
static Vector2 ip_title_loc = { 0 };
static const char* ip_title = "Endereço ip do servidor";
static Vector2 room_code_title_loc = { 0 };
static const char* room_code_title = "Código da sala";

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------
bool IpIsValid(void);
// Title Screen Initialization logic
void InitTitleScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    // ip_addr_arr = {0}; //calloc(12, sizeof(char));
    // memset(ip_addr_arr, 0, 16 * sizeof(char)); // 12 numbers, 3 dots AND THE NULL TERMINATOR
    // letterCount = {0};
    mouse_on_text = 0;
    //show_wrong_ip_msg = false;

    float mid_height = GetScreenHeight() / 2.0f;
    float mid_width  = GetScreenWidth()  / 2.0f;

    // posição do titulo da caixa de dialogo
    ip_title_loc = MeasureTextEx(font, ip_title, 64, 4);
    ip_title_loc.y = mid_height - IP_PAD_Y - (IP_SIZ_Y*2);
    ip_title_loc.x = mid_width - (ip_title_loc.x / 2);
    
    // posiciona as caixinhas do ip
    ip_loc_y = mid_height - (IP_PAD_Y / 2) - IP_SIZ_Y;
    ip_loc_x = mid_width - (IP_PAD_X * 1.5f) - (2 * IP_SIZ_X);
    for (int i = 0; i < IP_SIZE; ++i) {
        ip_rects[i].x = ip_loc_x + (i * (IP_PAD_X + IP_SIZ_X));
        ip_rects[i].y = ip_loc_y;
        ip_rects[i].width = IP_SIZ_X;
        ip_rects[i].height = IP_SIZ_Y;
        
    }

    // posição do titulo da caixa de dialogo
    room_code_title_loc = MeasureTextEx(font, room_code_title, 64, 4);
    room_code_title_loc.y = mid_height + (IP_PAD_Y/2);
    room_code_title_loc.x = mid_width - (room_code_title_loc.x / 2);

    // posição da caixa de dialogo
    room_rekt.y = mid_height + IP_PAD_Y + IP_SIZ_Y;
    room_rekt.width = 32 * ROOM_CODE_SIZE;
    room_rekt.x = mid_width - (room_rekt.width / 2);
    room_rekt.height = 64;

    text_code.x = room_rekt.x;
    text_code.y = room_rekt.y;
    
    // mensagem de ip inválido
    wrong_ip_msg_loc = MeasureTextEx(font, wrong_ip_msg, 64, 4);
    wrong_ip_msg_loc.y = mid_height + (1.5f*IP_PAD_Y) + (IP_SIZ_Y*2);
    wrong_ip_msg_loc.x = mid_width - (wrong_ip_msg_loc.x / 2);

}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // Press enter or tap to change to GAMEPLAY screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        //finishScreen = 1;   // OPTIONS
        show_wrong_ip_msg = !IpIsValid();
        if (show_wrong_ip_msg) {
            return;
        } 
        finishScreen = 2;   // GAMEPLAY
        PlaySound(fxCoin);
        // assumes an ipv4
        int len = 0;
        for (int i = 0; i < IP_SIZE; ++i) {
             int x = letterCount[i];
             bool is_most_sig = true;
             for (int j = 0; j < x; ++j) {
                 char v = ip_addr_arr[i][j];
                 if (v == '0' && j != x-1 && is_most_sig) continue;
                 if (v != '0') is_most_sig = true;
                 ip_addr_str[len] = v;
                 len++;
             }
             ip_addr_str[len] = '.';
             len++;
        }
        // I LOVE PROGRAMMING
        // We overide the last dot
        ip_addr_str[len-1] = '\0'; 

        // adiciona padding no codigo da sala
        // será que eu preciso disso??
        //for (int i = codeCount; i<ROOM_CODE_SIZE;i++) {
        //    room_code[i] = '.';
        //}
        //codeCount = ROOM_CODE_SIZE;
        // raylib_game.c agora pode fazer o fork e
        // conectar nesse ip
        // a detecção é pela variavel finishScreen
        LOG_DEBUG("%s\n", ip_addr_str);
        LOG_DEBUG("%s\n", room_code);
        return;
    }
    Vector2 mouse = GetMousePosition();
    mouse_on_text = 0;
    for (int i = 0; i < IP_SIZE; ++i) {
        if (CheckCollisionPointRec(mouse, ip_rects[i])) 
            mouse_on_text = i + 1;
    }
    mouse_on_room = false;
    if (CheckCollisionPointRec(mouse, room_rekt)) mouse_on_room = true;
    
    if (mouse_on_text || mouse_on_room) {
        mouse_on_text--;
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if (mouse_on_room) {
                if ((key >= 32) && (key <= 125) && (codeCount < ROOM_CODE_SIZE))
                {
                    room_code[codeCount] = (char)key;
                    room_code[codeCount+1] = '\0'; // Add null terminator at the end of the string.
                    codeCount++;
                }
            } else if ((key >= 48) && (key <= 57) && (letterCount[mouse_on_text] < 3))
            {
                int c = letterCount[mouse_on_text];
                ip_addr_arr[mouse_on_text][c] = (char)key;
                ip_addr_arr[mouse_on_text][c+1] = '\0';
                letterCount[mouse_on_text]++;
            }
            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (mouse_on_room) {
                codeCount--;
                if (codeCount < 0) {
                    codeCount = 0;
                }
                room_code[codeCount] = '\0';
            }
            else {
            letterCount[mouse_on_text]--;
            if (letterCount[mouse_on_text] < 0) {
                letterCount[mouse_on_text] = 0;
            }
            ip_addr_arr[mouse_on_text][letterCount[mouse_on_text]] = '\0';
            }
        }
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}


// Title Screen Draw logic
void DrawTitleScreen(void)
{
    // TODO: Draw TITLE screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    //Vector2 pos = { 20, 10 };
    //DrawTextEx(font, "TITLE SCREEN", pos, font.baseSize*3.0f, 4, DARKGREEN);
    DrawTextEx(font, ip_title, ip_title_loc, 64, 4, DARKGREEN);
    for (int i = 0; i < IP_SIZE; ++i) {
        DrawRectangleRec(ip_rects[i], DARKGREEN);
        Vector2 pos = {ip_rects[i].x + 4, ip_rects[i].y + 4};
        DrawTextEx(font, ip_addr_arr[i], pos, 64, 4, GREEN);
    }
    DrawTextEx(font, room_code_title, room_code_title_loc, 64, 4, DARKGREEN);
    DrawRectangleRec(room_rekt, DARKGREEN);
    DrawTextEx(font, room_code, text_code, 64, 4, GREEN);
    if (show_wrong_ip_msg) {
        
        DrawTextEx(font, wrong_ip_msg, wrong_ip_msg_loc, 64, 4, MAROON);
    }
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
}

bool IpIsValid(void) {
    for (int i = 0; i < IP_SIZE; ++i) {
        int len = letterCount[i];
        if (len == 0) {
            return false;
        }
        int v = 0;
        int pow = 1;
        for (int j = 1; j <= len; ++j) {
            v += (ip_addr_arr[i][len-j] - 48) * pow;
            pow *= 10;
        }
        if (v > 255) {
            return false;
        }
    }
    return true;
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}
