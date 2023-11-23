/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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

#define NOTIF_FOR 180

static const char* const notif_msg[] = {"É A SUA VEZ", "O JOGO ESTÁ COMEÇANDO", "AÇÃO INVÁLIDA"};
static Vector2 notif_loc[] = {{0}, {0}, {0}};
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
ServerMessage buf[MSG_BUFFER_SIZE] = {0};
static bool is_my_turn = false;
static unsigned char player_coins[4] = {0, 0, 0, 0};
static unsigned char batatas[4] = { 0, 0, 0, 0 };
static int player_positions[4] = {0, 0, 0, 0};
static unsigned char dice_rolled = 0;
static unsigned char minigame_select = 0;
static unsigned char player_count = 0;
static unsigned char player_id = 0;
static unsigned char notif = 0;
static int frame_counter = NOTIF_FOR;
static bool is_ready = false;
static bool buy_batata_available = false;
static bool intersection = false;
static unsigned char current_round = 0;
static unsigned char current_turn = 0;

#define PLAYER_COUNTER_HEIGHT 26
#define PLAYER_COUNTER_WIDTH 358
#define PLAYER_COUNTER_Y 160

#define COIN_COUNTER_X 320
#define BATATA_COUNTER_X 540

#define READY_BOX_X 320
#define READY_BOX_Y 185
#define READY_BOX_WIDTH 64
#define READY_BOX_HEIGHT 64

#define READY_BOX_PAD_X 80
// formas
static int player_counters[4] = {104, 136, 265, 296};
static Rectangle ready_box = { 0 };
static Rectangle unready_box = {0};
static const Color bg_color = { 0xf6, 0x94, 0xe8, 0xff };
static const Color text_color = { 0xc8, 0x2b, 0xb2, 0xff };

static Vector2 ready_txt;
static char ready_str[] = "READY/UNREADY";
static Vector2 roll_txt = { 0 };
static Vector2 buy_txt = { 0 };
static char roll_str[] = "Rolar o dado";
static char buy_str[] = "Comprar batata";


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    is_my_turn = false;
    dice_rolled = 0;
    minigame_select = 0;
    player_count = 1;
    player_id = 0;
    notif = 0;
    current_round = 0;
    frame_counter = NOTIF_FOR;
    int r = start_conn(room_code, ip_addr_str);
    if (r == -5) {
        // manda o usuario para a tela do ip
        finishScreen = 2;
        ZeroMemory(ip_addr_str, 16);
        return;
    }
    else if (r < 0) {
		// termina o jogo
		finishScreen = 1;
		return;
    }
    float mid_height = GetScreenHeight() / 2;
    float mid_width  = GetScreenWidth()  / 2;
    for (int i = 0; i < 3; ++i) {
        Vector2 size = MeasureTextEx(font, notif_msg[i], 64, 4);
        notif_loc[i].y = mid_height - (size.y / 2);
        notif_loc[i].x = mid_width  - (size.x / 2);
	}
	ready_box.x = READY_BOX_X;
	ready_box.y = READY_BOX_Y;
	ready_box.width = READY_BOX_WIDTH;
	ready_box.height = READY_BOX_HEIGHT;
    unready_box.x = READY_BOX_X + READY_BOX_PAD_X;
    unready_box.y = READY_BOX_Y;
    unready_box.width = READY_BOX_WIDTH;
    unready_box.height = READY_BOX_HEIGHT;
    ready_txt = MeasureTextEx(font, ready_str, 20.0f, 4.0f);
    roll_txt = MeasureTextEx(font, roll_str, 20.0f, 4.0f);
    buy_txt = MeasureTextEx(font, buy_str, 20.0f, 4.0f);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    int nof_msg = recv_server_msg(buf);
    if (nof_msg < 0) {
		// termina o jogo
		finishScreen = 1;
		return;
    }
    else if (nof_msg != 0) {
        // processa as mensagens
        for (int i = 0; i < nof_msg; i++) {
            switch (buf[i].msg_id)
            {
            case n_game_start:
                // inicia o jogo
                current_round = 1;
                notif = 2;
				break;
            case n_turn_start:
				// é a vez do jogador
                current_turn = (current_turn + 1)%player_count;
				is_my_turn = current_turn == player_id;
                if (is_my_turn) {
					notif = 1;
				}
				break;
            case n_emote:
				// termina o jogo
				PlaySound(fxCoin);
			    break;
            case n_buy_batata:
                buy_batata_available = true;
                break;
            case r_roll_dice:
                dice_rolled = buf[i].data[2];
            case r_move:
            case n_move:
                // move o jogador
                player_positions[current_turn] = buf[i].data[0];
                player_coins[current_turn] = buf[i].data[1];
				break;
            case n_join:
				// adiciona um jogador
				player_count++;
				break;
            case r_roll_dice_intersect:
                intersection = true;
                break;
            case r_can_buy_batata:
                buy_batata_available = true;
			break;
            case e_decode:
            case e_not_your_turn:
            case e_invalid_action:
            case e_game_not_started:
            case e_not_in_minigame:
            case e_join:
                notif = 3;
                break;
            case e_room_not_found:
            case e_room_full:
            case e_player_not_in_room:
            case e_game_already_started:
            case n_minigame:
            case n_minigame_end:
            case r_ok:
                break;
            case n_game_end:
            default:
                finishScreen = 1;
                return;
            }
		}
    }
    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawTexture(bgimg, 0, 0, WHITE);

    for (int i = 0; i < 4; ++i)
    {
        if (i >= player_count) {
            // escondemos o cara
            DrawRectangle(PLAYER_COUNTER_Y, player_counters[i], PLAYER_COUNTER_WIDTH, PLAYER_COUNTER_HEIGHT, bg_color);
            continue;
        }
        // desenha o cara
        char coins_text[4] = {0, 0, 0, 0};
        if (sprintf_s(coins_text, 4, "%.3d", player_coins[i]) == 0) {
            LOG("sprintf_s() falhou\n");
			return;
        }
        Vector2 coins_loc = {COIN_COUNTER_X, player_counters[i]};
        DrawTextEx(font, coins_text, coins_loc, 32.0f, 4.0f, text_color);
        char* batata_text[4] = { 0, 0, 0, 0 };
        sprintf_s(batata_text, 4, "%.3d", batatas[i]);
        Vector2 batata_loc = { BATATA_COUNTER_X, player_counters[i] };
        DrawTextEx(font, batata_text, batata_loc, 32.0f, 4.0f, text_color);
    }
    if (current_round == 0) {
        // desenha o botão de ready
        DrawRectangleRec(ready_box, GREEN);
        DrawRectangleRec(unready_box, RED);
        DrawTextEx(font, ready_str, (Vector2) { unready_box.x - 8.0f - (ready_txt.x / 2), ready_box.y - ready_txt.y - 4.0f }, 20.0f, 4.0f, text_color);
    } else
    if (is_my_turn) {
		// desenha o botão de rolar o dado
        DrawRectangleRec(ready_box, GREEN);
		DrawRectangleRec(unready_box, RED);
		DrawTextEx(font, roll_str, (Vector2) { unready_box.x - 8.0f - (roll_txt.x / 2), ready_box.y - roll_txt.y - 4.0f }, 20.0f, 4.0f, text_color);
	} else 
    if (is_my_turn) {
        // desenha o botão de rolar o dado
        DrawRectangleRec(ready_box, GREEN);
        DrawRectangleRec(unready_box, RED);
        DrawTextEx(font, buy_str, (Vector2) { unready_box.x - 8.0f - (buy_txt.x / 2), ready_box.y - buy_txt.y - 4.0f }, 20.0f, 4.0f, text_color);
    }

    if (notif > 0)
    {
		if (frame_counter == NOTIF_FOR) {
			frame_counter = 0;
			notif = 0;
		}
		else {
			frame_counter++;
		    DrawTextEx(font, notif_msg[notif-1], notif_loc[notif-1], 64.0f, 4.0f, RED);
		}
		
    }
    // TODO: Draw GAMEPLAY screen here!
    //  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!  
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
