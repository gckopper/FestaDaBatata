#pragma once

#include "MessageTypes.h"
#include <stdlib.h>
#include <memory.h>
#include "Logger.h"
/**
 * Recebe um ponteiro constante pra mensagem para preparar.
 * O buffer de retorno pode ser um byte maior que o necessario.
 * Para saber o tamanho do buffer usar:
 * `((r[8] & is_long) >> 7) + 1 + ROOM_CODE_SIZE`
 * return buffer retorna um buffer contendo a mensagem
 */
unsigned char* const to_send(ClientMessage* const m);

ClientMessage* const new_message();

void set_room_code(ClientMessage* const m, const unsigned char* const room);

void free_message(ClientMessage* m);



ClientMessage* const new_with_room_code(const unsigned char* room);

ClientMessage* const new_roll_dice(const unsigned char* room);

ClientMessage* const new_buy_batata(const unsigned char* room);

ClientMessage* const new_ready(const unsigned char* room);

ClientMessage* const new_unready(const unsigned char* room);

ClientMessage* const new_join_room(const unsigned char* room);

ClientMessage* const new_move(const unsigned char* room, unsigned char op);

ClientMessage* const new_emote(const unsigned char* room, unsigned char op);

ClientMessage* const new_minigame(const unsigned char* room, unsigned char op);
