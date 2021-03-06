#include "common.h"
#include "view.h"
#include "tetris.h"

// ブロックを表示するときの縦横
#define TETRIS_VIEW_WIDTH  10
#define TETRIS_VIEW_HEIGHT 20

// 次のブロックを表示する場所
#define NEXT_BLOCK_WIDTH 5
#define NEXT_BLOCK_HEIGHT 5

int ch(int);

typedef struct
{
    int flg;
    int color;
}view_block_struct;

view_block_struct view_block[TETRIS_VIEW_HEIGHT][TETRIS_VIEW_WIDTH];
view_block_struct next_block[NEXT_BLOCK_HEIGHT][NEXT_BLOCK_WIDTH];

// 四角形を描画する 移植を容易にするための処置.
void render_box(int x1, int y1, int x2, int y2, int color)
{
	alt_up_pixel_buffer_dma_draw_box(screen, x1, y1, x2, y2, color, 0);
}

// 文字を描画する これも上に同じ
void render_char(int x, int y, const char *string)
{
	alt_up_char_buffer_string (screen_chara, string, ch(x), ch(y));
}

// 座標xから文字の位置へ変換する関数. 大体の座標.
int ch(int x)
{
	return (int)(((float)x/(float)SCREEN_WIDTH)*(float)CHARA_BUF_WIDTH);
}

// 基本的な外観を描画する.
void draw_appearance()
{
	int x,y;
	// ブロックが入るスペース
	x=45;
	y=20;
	render_box(    x,     y,   x+4, y+205, COLOR_DARKGREY);
	render_box(x+106,     y, x+110, y+205, COLOR_DARKGREY);
	render_box(  x+5, y+201, x+105, y+205, COLOR_DARKGREY);
	// 次のブロックを表示する場所
	x=165;
	y=20;
	render_box(   x,    y, x+60,  y+5, COLOR_DARKGREY);
	render_box(   x,    y,  x+5, y+60, COLOR_DARKGREY);
	render_box(x+55,    y, x+60, y+60, COLOR_DARKGREY);
	render_box(   x, y+56, x+60, y+60, COLOR_DARKGREY);
	render_char(x, y, "NEXT");
	//スコア用のスペース
	x=165;
	y=100;
	char buf[20];
	sprintf(buf, "SCORE: %06d", score);
	render_char(x, y, buf);
}

// ダイアログの表示
void draw_dialogue(const char *string)
{
	render_box(0, SCREEN_HEIGHT/2-10, SCREEN_WIDTH, SCREEN_HEIGHT/2+10, COLOR_NAVY);
	int len = strlen(string);
	render_char((SCREEN_WIDTH/2)-(len*CHARA_WIDTH/2), SCREEN_HEIGHT/2, string);
}

// ブロックを全部リセットする.
void init_block()
{
	int x,y;
	for(y=0; y<TETRIS_VIEW_HEIGHT; y++)
	{
		for(x=0; x<TETRIS_VIEW_WIDTH; x++)
		{
			view_block[y][x].flg = 0;
			view_block[y][x].color = COLOR_BLACK;
		}
	}
	for(y=0; y<NEXT_BLOCK_HEIGHT; y++)
	{
		for(x=0; x<NEXT_BLOCK_WIDTH; x++)
		{
			next_block[y][x].flg = 0;
			next_block[y][x].color = COLOR_BLACK;
		}
	}
}

// 文字バッファの消去
void clear_char_buf()
{
	alt_up_char_buffer_clear(screen_chara);
}

// 画面を消す
void clear_screen()
{
	alt_up_pixel_buffer_dma_clear_screen(screen, 0);
}

// 適当な色のブロックを表示しておく.
void demo()
{
	int x,y;
	int colors[] = {COLOR_BLUE, COLOR_GREEN, COLOR_YELLOW, COLOR_RED, COLOR_PURPLE, COLOR_PINK, COLOR_ORANGE, COLOR_WHITE, COLOR_GREENYELLOW};
	for(y=0; y<TETRIS_VIEW_HEIGHT; y++)
	{
		for(x=0; x<TETRIS_VIEW_WIDTH; x++)
		{
			view_block[y][x].flg = 1;
			view_block[y][x].color = colors[rand()%9];
		}
	}
}

// ブロックを描画する.
void draw_block()
{
	int x,y;
	for(y=0; y<TETRIS_VIEW_HEIGHT; y++)
	{
		for(x=0; x<TETRIS_VIEW_WIDTH; x++)
		{
			int ulx = 50+x*10;
			int uly = 20+y*10;
			if(view_block[y][x].flg)
			{
				// 50, 20はブロックのスペースの右上の座標
				render_box(ulx+1, uly+1, ulx+9, uly+9, view_block[y][x].color);
			}
			else
			{
				render_box(ulx+1, uly+1, ulx+9, uly+9, COLOR_BLACK);
			}
		}
	}
}

// 次のブロックの部分を描画する.
void draw_next_block()
{
	int x, y;
	for(y=0; y<NEXT_BLOCK_HEIGHT; y++)
	{
		for(x=0; x<NEXT_BLOCK_WIDTH; x++)
		{
			int ulx = 170+x*10;
			int uly = 25+y*10;
			if(next_block[y][x].flg)
			{
				// 170, 25はブロックのスペースの右上の座標
				render_box(ulx+1, uly+1, ulx+9, uly+9, next_block[y][x].color);
			}
			else
			{
				render_box(ulx+1, uly+1, ulx+9, uly+9, COLOR_BLACK);
			}
		}
	}
}

// ブロックの状態を代入する.
void assign_block(board_element board[BOARD_HEIGHT][BOARD_WIDTH])
{
	int i,n;
	for(i=4; i<BOARD_HEIGHT-1; i++)
	{
		for(n=1; n<BOARD_WIDTH-1; n++)
		{
			if(board[i][n].flg)
			{
				view_block[i-4][n-1].flg = 1;
				view_block[i-4][n-1].color = board[i][n].color;
			}
			else
			{
				view_block[i-4][n-1].flg = 0;
			}
		}
	}
}

// 次のブロックを代入する.
void assign_next_block(block b)
{
	int x,y,i;
	// 初期化する.
	for(y=0; y<NEXT_BLOCK_HEIGHT; y++)
	{
		for(x=0; x<NEXT_BLOCK_WIDTH; x++)
		{
			next_block[y][x].flg = 0;
			next_block[y][x].color = COLOR_BLACK;
		}
	}
	// 原点の位置
	x = 2;
	y = 2;
	for(i=0; i<BLOCK_NUM; i++)
	{
		next_block[b.child[i].y + y][b.child[i].x + x].flg = 1;
		next_block[b.child[i].y + y][b.child[i].x + x].color = b.color; 
	}
}
