#include "main.h"

int init_devices();
void init_block();
void draw_appearance();
void draw_block();
void demo();
void check_buttons();
void debug_output();

int main(void)
{
	alt_printf("Info: System initialization started!");

	// デバイスの初期化
	if(init_devices() != 0)
	{
		alt_printf("Error: device initialize failed.");
		return 1;
	}

	// 画面を黒に初期化
	alt_up_pixel_buffer_dma_clear_screen(screen, 0);

	init_block();

	score = 0;
	cnt = 0;

	while(1)
	{
		if (alt_up_pixel_buffer_dma_check_swap_buffers_status(screen) == 0)
		{
			// ボタンの取得
			check_buttons();

			if(cnt%10==0)
				demo();

			// 外観の描画
			draw_appearance();

			// ブロックの描画
			draw_block();
			
			debug_output();

			cnt++;
			alt_up_pixel_buffer_dma_swap_buffers(screen);
		}
	}
}

// 基本的な外観を描画する.
void draw_appearance()
{
	int x,y;
	// ブロックが入るスペース
	x=45;
	y=20;
	alt_up_pixel_buffer_dma_draw_box(screen,     x,     y,   x+4, y+205, COLOR_DARKGREY, 0);
	alt_up_pixel_buffer_dma_draw_box(screen, x+106,     y, x+110, y+205, COLOR_DARKGREY, 0);
	alt_up_pixel_buffer_dma_draw_box(screen,   x+5, y+201, x+105, y+205, COLOR_DARKGREY, 0);
	// 次のブロックを表示する場所
	x=165;
	y=20;
	alt_up_pixel_buffer_dma_draw_box(screen,    x,    y, x+60,  y+5, COLOR_DARKGREY, 0);
	alt_up_pixel_buffer_dma_draw_box(screen,    x,    y,  x+5, y+60, COLOR_DARKGREY, 0);
	alt_up_pixel_buffer_dma_draw_box(screen, x+55,    y, x+60, y+60, COLOR_DARKGREY, 0);
	alt_up_pixel_buffer_dma_draw_box(screen,    x, y+55, x+60, y+60, COLOR_DARKGREY, 0);
	alt_up_char_buffer_string (screen_chara, "NEXT", chx(x), chy(y));
	//スコア用のスペース
	x=165;
	y=100;
	char buf[20];
	sprintf(buf, "SCORE: %06d", score);
	alt_up_char_buffer_string (screen_chara, buf, chx(x), chy(y));
}

// デバッグ用の出力. ボタンが押されてるかなどを表示.
void debug_output()
{
	int i,x;
	x = 165;
	int chy_ = chy(100);
	chy_++;
	char buf[20];

	sprintf(buf, "CNT:   %06d", cnt);
	alt_up_char_buffer_string (screen_chara, buf, chx(x), chy_++);
	
	for(i=0; i<NUM_BUTTON; i++)
	{
		sprintf(buf, "BUTTON[%d]:   %04d", i, button[i]);
		alt_up_char_buffer_string (screen_chara, buf, chx(x), chy_++);
	}
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
			if(view_block[y][x].flg)
			{
				// 50, 20はブロックのスペースの右上の座標
				int ulx = 50+x*10;
				int uly = 20+y*10;
				alt_up_pixel_buffer_dma_draw_box(screen, ulx+1, uly+1, ulx+9, uly+9, view_block[y][x].color, 0 );
			}
		}
	}
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
}

// ボタンが入力されているかどうかを取得する.
void check_buttons()
{
	int value = alt_up_parallel_port_read_data(push_button);
	int i;
	// 押されてたら1以上,そうでなければ0とする.
	for(i=0; i<NUM_BUTTON; i++)
	{
		if(((value>>i) & 1) == 1)
			button[i] += 1;
		else
			button[i] = 0;
	}
}

// デバイスの初期化
int init_devices(){
	int error = 0;
	screen = alt_up_pixel_buffer_dma_open_dev( DEV_PATH_PIXEL );
	if(screen == NULL)
	{
		alt_printf("Error: could not open VGA pixel buffer device\n");
		error = 1;
	}
	screen_chara = alt_up_char_buffer_open_dev( DEV_PATH_CHAR );
	if(screen_chara == NULL)
	{
		alt_printf("Error: could not open character buffer device\n");
		error = 1;
	}
	push_button = alt_up_parallel_port_open_dev( DEV_PATH_PUSHBUTTON);
	if(push_button == NULL)
	{
		alt_printf("Error: could not open Pushbuttons\n");
		error = 1;
	}
	// ボタンのバッファもここでリセットしとく
	int i=0;
	for(i=0; i<NUM_BUTTON; i++)
		button[i] = 0;
	return error;
}
