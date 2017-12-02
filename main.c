#include "common.h"
#include "view.h"
#include "tetris.h"
#include "main.h"

void check_buttons();
void reset_buttons();
int init_devices();

int cnt=0;
int button[4];
states state;
alt_up_pixel_buffer_dma_dev *screen;
alt_up_char_buffer_dev *screen_chara;
alt_up_parallel_port_dev *push_button;

int main(void) 
{
	// デバイスの初期化
	if(init_devices() != 0)
	{
		alt_printf("Error: device initialize failed.");
		return 1;
	}
	
	// 画面を黒に初期化
	clear_screen();

	switch_state(DEMONSTRATION);

	while(1)
	{
		if (alt_up_pixel_buffer_dma_check_swap_buffers_status(screen) == 0)
		{
			check_buttons();
			switch(state)
			{
				case DEMONSTRATION:	
					draw_appearance();
					if(cnt%10==0)
						demo();
					draw_block();
					draw_dialogue("DEMONSTRATION");
					if(button[BUTTON_ROTATE]==1)
						switch_state(PLAYING);
					break;
				case PLAYING:
					tetris_update();
					draw_appearance();
					draw_block();
					draw_next_block();
					break;
				case GAMEOVER:
					draw_appearance();
					draw_block();
					draw_dialogue("GAMEOVER");
					if(button[BUTTON_ROTATE]==1)
						switch_state(DEMONSTRATION);
					break;
			}

			cnt++;
			alt_up_pixel_buffer_dma_swap_buffers(screen);
		}
	}
	return 0 ;
}

// 状態を変える関数. 状態遷移を起こす前の初期化などに使う.
void switch_state(states next_state)
{
	clear_screen();
	clear_char_buf();

    switch(next_state)
    {
        case DEMONSTRATION:
			init_block();
            state = DEMONSTRATION;
            break;
        case PLAYING:
			init_block();
			init_tetris();
            state = PLAYING;
            break;
		case GAMEOVER:
			state = GAMEOVER;
			break;
    }
}

// ボタンの状態をリセットする.
void reset_buttons()
{
	int i;
	for(i=0; i<NUM_BUTTON; i++)
	{
		button[i] = 0;
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
