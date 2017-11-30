#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_parallel_port.h"
#include "sys/alt_stdio.h"
#include <stdio.h>
#include <stdlib.h>

// 画面サイズの定義
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define CHARA_WIDTH   80
#define CHARA_HEIGHT  60

// デバイスファイルのパス
#define DEV_PATH_PIXEL      "/dev/VGA_Pixel_Buffer"
#define DEV_PATH_CHAR       "/dev/VGA_Char_Buffer"
#define DEV_PATH_PUSHBUTTON "/dev/Pushbuttons"

// ボタンの数
#define NUM_BUTTON 4

// デバイスへのポインタ
alt_up_pixel_buffer_dma_dev *screen;
alt_up_char_buffer_dev *screen_chara;
alt_up_parallel_port_dev *push_button;

// 色の定義
// ここを参考にした: https://stackoverflow.com/questions/13720937/c-defined-16bit-high-color
#define COLOR_BLACK           0x0000
#define COLOR_BLUE            0x001F
#define COLOR_GREEN           0x07E0
#define COLOR_RED             0xF800
#define COLOR_GREENYELLOW     0xAFE5
#define COLOR_YELLOW          0xFFE0
#define COLOR_CYAN            0x07FF
#define COLOR_MAGENTA         0xF81F
#define COLOR_ORANGE          0xFD20
#define COLOR_PINK            0xF81F
#define COLOR_NAVY            0x000F
#define COLOR_DARKGREEN       0x03E0
#define COLOR_DARKCYAN        0x03EF
#define COLOR_MAROON          0x7800
#define COLOR_PURPLE          0x780F
#define COLOR_OLIVE           0x7BE0
#define COLOR_LIGHTGREY       0xC618
#define COLOR_DARKGREY        0x7BEF
#define COLOR_WHITE           0xFFFF

// ブロックを表示するときの縦横
#define TETRIS_VIEW_WIDTH  10
#define TETRIS_VIEW_HEIGHT 20

// テトリス内部で使う変数
int score;
int cnt;
int button[4];

typedef struct
{
    int flg;
    int color;
}view_block_struct;

view_block_struct view_block[TETRIS_VIEW_HEIGHT][TETRIS_VIEW_WIDTH];

// 座標xから文字の位置へ変換する関数. 大体の座標.
int chx(int x)
{
	return ((float)x/SCREEN_WIDTH)*CHARA_WIDTH;
}
int chy(int y)
{
	return ((float)y/SCREEN_HEIGHT)*CHARA_HEIGHT;
}