#ifndef VIEW_H
#define VIEW_H

#include "tetris.h"

void init_block();
void draw_appearance();
void draw_block();
void clear_screen();
void clear_char_buf();
void draw_next_block();
void draw_dialogue(const char*);
void demo();
void assign_block(board_element[BOARD_HEIGHT][BOARD_WIDTH]);
void assign_next_block(block);

#endif
