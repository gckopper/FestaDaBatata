#include "client_packer.h"
/**
 * Recebe um ponteiro constante pra mensagem para preparar.
 * O buffer de retorno pode ser um byte maior que o necessario.
 * Para saber o tamanho do buffer usar: 
 * `((r[8] & is_long) >> 7) + 1 + ROOM_CODE_SIZE`
 * return buffer retorna um buffer contendo a mensagem
 */
unsigned char* const to_send(ClientMessage* const m) {
    if (m == 0) {
		LOG_DEBUG("Erro! Função to_send recebeu um ponteiro nulo\n");
        return 0;
    }
	unsigned char longness = (m->msg_id & 0b1000'0000) >> 7;
	unsigned char* result = calloc(ROOM_CODE_SIZE+1+longness, sizeof(char));
    if (result == 0) {
		LOG("Calloc falhou! Abortando!");
		exit(8);
        return 0;
    }
	memcpy_s(result, ROOM_CODE_SIZE, m->room_code, ROOM_CODE_SIZE);
	result[ROOM_CODE_SIZE] = m->msg_id;
	if (longness) {
		result[ROOM_CODE_SIZE+1] = m->optional;
	}
	// fica pro consumidor dar um free nesse treco
	// o consumidor pode dar um result[8] & is_long pra
	// saber se tem 9 ou 10 bytes :)
	return result;
}

ClientMessage* const new_message() {
	ClientMessage* const result = (ClientMessage*)calloc(1, sizeof(ClientMessage));
	if (result == 0) {
		LOG("Calloc falhou! Abortando!");
		exit(8);
		return 0;
	}
	result->room_code = (unsigned char*)calloc(ROOM_CODE_SIZE, sizeof(char));
	result->msg_id = 0;
	result->optional = 0;
	return result;
}

void set_room_code(ClientMessage* const m, const unsigned char* const room) {
	int r = memcpy_s(m->room_code, ROOM_CODE_SIZE, room, ROOM_CODE_SIZE);
	if (r == 0) {
		LOG("memcpy_s falhou! Abortando!");
		exit(8);
		return;
	}
	return;
}

void free_message(ClientMessage* m) {
	free(m->room_code);
	free(m);
	return;
}



ClientMessage* const new_with_room_code(const unsigned char* room) {
	ClientMessage* const result = new_message();
	set_room_code(result, room);
	return result;
}

ClientMessage* const new_roll_dice(const unsigned char* room) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id = roll_dice;
	return result;
}

ClientMessage* const new_buy_batata(const unsigned char* room) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id = buy_batata;
	return result;
}

ClientMessage* const new_ready(const unsigned char* room) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id = ready;
	return result;
}

ClientMessage* const new_unready(const unsigned char* room) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id = unready;
	return result;
}

ClientMessage* const new_join_room(const unsigned char* room) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id   = join; 
	return result;
}

ClientMessage* const new_move(const unsigned char* room, unsigned char op) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id = move;
	result->optional = op;
	return result;
}

ClientMessage* const new_emote(const unsigned char* room, unsigned char op) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id = emote;
	result->optional = op;
	return result;
}

ClientMessage* const new_minigame(const unsigned char* room, unsigned char op) {
	ClientMessage* const result = new_with_room_code(room);
	result->msg_id   = minigame; 
	result->optional = op;
	return result;
}
