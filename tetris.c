#define USE_TI89
#define SAVE_SCREEN

#include <tigcclib.h>
#include "tetris.h"

board global_board;
void *kbq;
volatile unsigned char quit;
INT_HANDLER old_int_5;

unsigned char difficulty;
unsigned char difficulty_progress;
unsigned long int score;
unsigned char frame;
unsigned char game_over;

char level_text[15];
char score_text[15];

unsigned char difficulties[] = {
	16, 14, 13, 11, 9, 8, 6, 4, 3, 2, 2, 2, 2, 2, 1, 1};

void initialize_pieces(){
	I_piece[0] = I_piece1;
	I_piece[1] = I_piece1;
	I_piece[2] = I_piece1;
	I_piece[3] = I_piece1;
	
	J_piece[0] = J_piece1;
	J_piece[1] = J_piece2;
	J_piece[2] = J_piece3;
	J_piece[3] = J_piece4;

	L_piece[0] = L_piece1;
	L_piece[1] = L_piece2;
	L_piece[2] = L_piece3;
	L_piece[3] = L_piece4;

	O_piece[0] = O_piece1;
	O_piece[1] = O_piece1;
	O_piece[2] = O_piece1;
	O_piece[3] = O_piece1;

	S_piece[0] = S_piece1;
	S_piece[1] = S_piece2;
	S_piece[2] = S_piece1;
	S_piece[3] = S_piece2;

	T_piece[0] = T_piece1;
	T_piece[1] = T_piece2;
	T_piece[2] = T_piece3;
	T_piece[3] = T_piece4;

	Z_piece[0] = Z_piece1;
	Z_piece[1] = Z_piece2;
	Z_piece[2] = Z_piece1;
	Z_piece[3] = Z_piece2;

	pieces[0] = I_piece;
	pieces[1] = J_piece;
	pieces[2] = L_piece;
	pieces[3] = O_piece;
	pieces[4] = S_piece;
	pieces[5] = T_piece;
	pieces[6] = Z_piece;
}

void initialize_board(){
	memset(global_board.squares, 0, sizeof(unsigned char)*200);

	randomize();
	global_board.current_piece = rand()%7;
	global_board.current_orientation = rand()%4;
	global_board.next_piece = rand()%7;
	global_board.next_orientation = rand()%4;
	global_board.current_x = 3;
	global_board.current_y = 0;
}

void draw_tile(char x, char y){
	Sprite8(x*5 + 55, y*5, 5, tile, LCD_MEM, SPRT_XOR);
}

void draw_piece(unsigned char piece, unsigned char orientation, char x, char y){
	unsigned char i;
	unsigned char j;
	unsigned char k;

	k = 0;

	if(orientation&1){
		for(i = 0; i < dimensions[piece][0]; i++){
			for(j = 0; j < dimensions[piece][1]; j++){
				if(pieces[piece][orientation][k]){
					draw_tile(j + x, i + y);
				}

				k++;
			}
		}
	} else {
		for(i = 0; i < dimensions[piece][1]; i++){
			for(j = 0; j < dimensions[piece][0]; j++){
				if(pieces[piece][orientation][k]){
					draw_tile(j + x, i + y);
				}

				k++;
			}
		}
	}
}

void draw_preview(){
	char x;
	char y;

	if(dimensions[global_board.next_piece][global_board.next_orientation&1] < 4){
		x = -5;
	} else {
		x = -6;
	}

	if(dimensions[global_board.next_piece][!(global_board.next_orientation&1)] < 4){
		y = 16;
	} else {
		y = 15;
	}

	draw_piece(global_board.next_piece, global_board.next_orientation, x, y);
}

unsigned char collide_piece(unsigned char piece, unsigned char orientation, char x, char y){
	unsigned char i;
	unsigned char j;
	unsigned char k;

	k = 0;

	if(orientation&1){
		if(x < 0 || x + dimensions[piece][1] > 10 || y < 0 || y + dimensions[piece][0] > 20){
			return 1;
		}

		for(i = 0; i < dimensions[piece][0]; i++){
			for(j = 0; j < dimensions[piece][1]; j++){
				if(pieces[piece][orientation][k] && global_board.squares[j + x][i + y]){
					return 1;
				}

				k++;
			}
		}
	} else {
		if(x < 0 || x + dimensions[piece][0] > 10 || y < 0 || y + dimensions[piece][1] > 20){
			return 1;
		}

		for(i = 0; i < dimensions[piece][1]; i++){
			for(j = 0; j < dimensions[piece][0]; j++){
				if(pieces[piece][orientation][k] && global_board.squares[j + x][i + y]){
					return 1;
				}

				k++;
			}
		}
	}

	return 0;
}

void place_piece(){
	unsigned char i;
	unsigned char j;
	unsigned char k;

	k = 0;

	if(global_board.current_orientation&1){
		for(i = 0; i < dimensions[global_board.current_piece][0]; i++){
			for(j = 0; j < dimensions[global_board.current_piece][1]; j++){
				if(pieces[global_board.current_piece][global_board.current_orientation][k]){
					global_board.squares[j + global_board.current_x][i + global_board.current_y] = 1;
				}

				k++;
			}
		}
	} else {
		for(i = 0; i < dimensions[global_board.current_piece][1]; i++){
			for(j = 0; j < dimensions[global_board.current_piece][0]; j++){
				if(pieces[global_board.current_piece][global_board.current_orientation][k]){
					global_board.squares[j + global_board.current_x][i + global_board.current_y] = 1;
				}

				k++;
			}
		}
	}
}

unsigned char check_row(unsigned char row){
	unsigned char x;
	
	for(x = 0; x < 10; x++){
		if(!global_board.squares[x][row]){
			return 0;
		}
	}

	return 1;
}

void delete_row(unsigned char row){
	unsigned char x;
	unsigned char y;

	for(y = row; y > 0; y--){
		for(x = 0; x < 10; x++){
			if(global_board.squares[x][y]){
				draw_tile(x, y);
			}

			global_board.squares[x][y] = global_board.squares[x][y - 1];

			if(global_board.squares[x][y]){
				draw_tile(x, y);
			}
		}
	}

	for(x = 0; x < 10; x++){
		if(global_board.squares[x][0]){
			draw_tile(x, 0);
		}

		global_board.squares[x][0] = 0;
	}
}

void draw_board(){
	unsigned char x;
	unsigned char y;
	
	DrawLine(54, 0, 54, 100, A_NORMAL);
	DrawLine(105, 0, 105, 100, A_NORMAL);
	DrawLine(23, 73, 46, 73, A_NORMAL);
	DrawLine(46, 73, 46, 96, A_NORMAL);
	DrawLine(46, 96, 23, 96, A_NORMAL);
	DrawLine(23, 96, 23, 73, A_NORMAL);

	for(x = 0; x < 10; x++){
		for(y = 0; y < 20; y++){
			if(global_board.squares[x][y]){
				draw_tile(x, y);
			}
		}
	}

	draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
	draw_preview();
	FontSetSys(F_6x8);
	sprintf(level_text, "Level %d", difficulty);
	DrawStr(109, 3, level_text, A_NORMAL);

	DrawStr(109, 14, "Score", A_NORMAL);
	FontSetSys(F_4x6);
	sprintf(score_text, "%ld", score);
	DrawStr(109, 25, score_text, A_NORMAL);
}

void draw_game_over(){
	SCR_RECT s;
	s = (SCR_RECT) {55,36,104,60};
	ScrRectFill(&s, ScrRect, A_REVERSE);
	DrawLine(55, 36, 104, 36, A_NORMAL);
	DrawLine(55, 60, 104, 60, A_NORMAL);
	FontSetSys(F_6x8);
	DrawStr(68, 40, "Game", A_NORMAL);
	DrawStr(65, 48, "Over!", A_NORMAL);
}

DEFINE_INT_HANDLER (update){
	unsigned int key;
	unsigned char y;
	unsigned char b;
	unsigned char row_count;
	unsigned char new_orientation;

	b = !frame;

	while(!OSdequeue(&key, kbq) || b){
		if(key == KEY_F1 && !game_over){
			if(!global_board.current_orientation){
				new_orientation = 3;
			} else {
				new_orientation = global_board.current_orientation - 1;
			}
			if(!collide_piece(global_board.current_piece, new_orientation, global_board.current_x, global_board.current_y)){
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
				global_board.current_orientation = new_orientation;
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
			}
		} else if(key == KEY_F2 && !game_over){
			if(!collide_piece(global_board.current_piece, (global_board.current_orientation + 1)%4, global_board.current_x, global_board.current_y)){
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
				global_board.current_orientation = (global_board.current_orientation + 1)%4;
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
			}
		} else if(key == KEY_ESC){
			quit = 1;
		} else if((key == KEY_LEFT || _keytest(RR_LEFT)) && !game_over){
			if(!collide_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x - 1, global_board.current_y)){
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
				global_board.current_x -= 1;
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
			}
		} else if((key == KEY_RIGHT || _keytest(RR_RIGHT)) && !game_over){
			if(!collide_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x + 1, global_board.current_y)){
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
				global_board.current_x += 1;
				draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
			}

		}

		b = 0;
	}

	frame = (frame + 1)%difficulties[difficulty];

	if((!frame || _keytest(RR_DOWN)) && !game_over){
		if(!collide_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y + 1)){
			draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
			global_board.current_y++;
			draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
		} else {
			place_piece();
			row_count = 0;
			for(y = global_board.current_y; y < global_board.current_y + dimensions[global_board.current_piece][!(global_board.current_orientation&1)]; y++){
				if(check_row(y)){
					row_count++;
					delete_row(y);
					difficulty_progress = (difficulty_progress + 1)%10;
					if(!difficulty_progress && difficulty < 15){
						FontSetSys(F_6x8);
						DrawStr(109, 3, level_text, A_XOR);
						difficulty++;
						sprintf(level_text, "Level %d", difficulty);
						DrawStr(109, 3, level_text, A_NORMAL);
					}
				}
			}

			FontSetSys(F_4x6);
			DrawStr(109, 25, score_text, A_XOR);
			switch(row_count){
				case 1:
					score += 40;
					break;
				case 2:
					score += 100;
					break;
				case 3:
					score += 300;
					break;
				case 4:
					score += 1200;
			}
			sprintf(score_text, "%ld", score);
			DrawStr(109, 25, score_text, A_NORMAL);

			global_board.current_x = 3;
			global_board.current_y = 0;
			global_board.current_piece = global_board.next_piece;
			global_board.current_orientation = global_board.next_orientation;
			draw_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y);
			draw_preview();
			global_board.next_piece = rand()%7;
			global_board.next_orientation = rand()%4;
			draw_preview();

			if(collide_piece(global_board.current_piece, global_board.current_orientation, global_board.current_x, global_board.current_y)){
				game_over = 1;
				draw_game_over();
			}
		}
	}
}

void _main(){
	unsigned int old_delay;
	clrscr();
	old_delay = OSInitKeyInitDelay(0);

	difficulty = 0;
	difficulty_progress = 0;
	frame = 0;
	score = 0;
	game_over = 0;
	initialize_pieces();
	initialize_board();
	draw_board();
	
	FontSetSys(F_8x10);
	DrawStr(3, 3, "Tetris", A_NORMAL);
	FontSetSys(F_4x6);
	DrawStr(13, 13, "For Ti-89", A_NORMAL);
	DrawStr(3, 40, "By Ben Jones", A_NORMAL);
	DrawStr(27, 65, "Next", A_NORMAL);

	kbq = kbd_queue();
	old_int_5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_5, update);

	quit = 0;

	while(!quit){
	}

	SetIntVec(AUTO_INT_5, old_int_5);
	OSInitKeyInitDelay(old_delay);
}

