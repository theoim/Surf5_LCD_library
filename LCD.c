#include <stdio.h>
#include "main.h"


void LCD_control_write(unsigned char LCD_ctrl_cmd)					// CLCD 명령 쓰기 함수
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);                            // RS비트 0으로 클리어
	Delay(10);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);							// EN비트 1로 셋
	Delay(10);
	ControlGPIO(LCD_ctrl_cmd);										// 명령어 값 쓰기
	Delay(10);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);						// EN비트 0으로 클리어
	Delay(5000);							// 명령 끝나고 딜레이 안넣으면 제대로 동작 안함;;
}

void LCD_data_write(unsigned char LCD_write_data)					// CLCD 데이터(글자) 쓰기 함수
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);                            // RS비트 0으로 클리어
	Delay(10);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);							// EN비트 1로 셋
	Delay(10);
	// GPIO_SetBitsWithCmd(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9, CLCD_data);;										// 명령어 값 쓰기
    ControlGPIO(LCD_write_data);
    Delay(10);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);						// EN비트 0으로 클리어
	Delay(5000);							

}

void LCD_String(char *str)										// CLCD 문자열 쓰기 함수
{
	while (*str !='\0')											//공백이 올때까지 반복
	{LCD_data_write(*str++);}									// 배열 글자 하나씩 띄우기
}


void ControlGPIO(unsigned char LCD_cmd) {
    // 비트 순회
    for (int i = 0; i < 8; ++i) {
        // 해당 비트가 1이면 GPIO_SetBits 호출, 0이면 GPIO_ResetBits 호출
        if (LCD_cmd & (1 << i)) {
            switch (i) {
                case 0: GPIO_SetBits(GPIOC, GPIO_Pin_0); break;
                case 1: GPIO_SetBits(GPIOC, GPIO_Pin_1); break;
                case 2: GPIO_SetBits(GPIOC, GPIO_Pin_2); break;
                case 3: GPIO_SetBits(GPIOC, GPIO_Pin_3); break;
                case 4: GPIO_SetBits(GPIOC, GPIO_Pin_4); break;
                case 5: GPIO_SetBits(GPIOC, GPIO_Pin_5); break;
                case 6: GPIO_SetBits(GPIOC, GPIO_Pin_8); break;
                case 7: GPIO_SetBits(GPIOC, GPIO_Pin_9); break;
                default: break;
            }
        } else {
            switch (i) {
                case 0: GPIO_ResetBits(GPIOC, GPIO_Pin_0); break;
                case 1: GPIO_ResetBits(GPIOC, GPIO_Pin_1); break;
                case 2: GPIO_ResetBits(GPIOC, GPIO_Pin_2); break;
                case 3: GPIO_ResetBits(GPIOC, GPIO_Pin_3); break;
                case 4: GPIO_ResetBits(GPIOC, GPIO_Pin_4); break;
                case 5: GPIO_ResetBits(GPIOC, GPIO_Pin_5); break;
                case 6: GPIO_ResetBits(GPIOC, GPIO_Pin_8); break;
                case 7: GPIO_ResetBits(GPIOC, GPIO_Pin_9); break;
                default: break;
            }
        }
    }
}


void LCD_initial(void)											// CLCD 초기화 구동 함수 
{
	Delay(20000);
	LCD_control_write(0x38);								//function set
	Delay(5000);
	LCD_control_write(0x38);								//function set
	LCD_control_write(0x38);								//function set
	LCD_control_write(0x08);								
	LCD_control_write(0x01);
	LCD_control_write(0x06);
	LCD_control_write(0x0c);

}

void LCD_busy(void) {
    // PA0, PA1, PA2를 입력으로 설정

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9; // BUS 8 bits
    GPIO_InitStructure.GPIO_Direction = GPIO_Direction_IN;
    GPIO_InitStructure.GPIO_AF = PAD_AF1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    do {
        // RS = 0, RW = 1, E = 1
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);

    } while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) != 0);  // PA7 (7번째 비트)를 통해 "Busy Flag" 확인

    // PA0에서 PA7까지를 출력으로 설정
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9; // BUS 8 bits
    GPIO_InitStructure.GPIO_Direction = GPIO_Direction_OUT;
    GPIO_InitStructure.GPIO_AF = PAD_AF1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 50 밀리초 딜레이
    Delay(50000);
}

void LCD_command(unsigned char cmd)
{
	LCD_busy();
    
	ControlGPIO(cmd);    

    GPIO_ResetBits(GPIOA, GPIO_Pin_0);              // RS = 0
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);

    GPIO_ResetBits(GPIOA, GPIO_Pin_2);              // RW = 0     
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_1);                // E = 1 
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_2);

    GPIO_ResetBits(GPIOA, GPIO_Pin_1);              // E = 0
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_2);

	
	Delay(1);            		// 일정시간 시간 지연 
}       

void LCD_display_position(unsigned char row, unsigned char col)
{
unsigned cmd;
	cmd = 0b10000000 | ((row-1)*0x40+(col-1));
    LCD_command(cmd);
}

void LCD_display_OnOff(int D, int C, int B)
{
unsigned cmd;
	cmd = 0b00001000;
	if (D == 1) cmd = cmd | 0b00000100;
	if (C == 1) cmd = cmd | 0b00000010;
	if (B == 1) cmd = cmd | 0b00000001;
	LCD_command(cmd);
}

void LCD_cursordisplay_shift(int SC, int RL)
{
unsigned cmd;
	cmd = 0b00010000;
	if (SC == 1) cmd = cmd | 0b00001000;
	if (RL == 1) cmd = cmd | 0b00000100;
	LCD_command(cmd);
}

void LCD_display_clear()
{
	LCD_command(0b00000001);
}

void LCD_cursor_home()
{
	LCD_command(0b00000010);
}

void LCD_entry_mode(int ID, int S)
{
unsigned char cmd;
	cmd = 0b00000100;
	if (ID == 1) cmd = cmd | 0b00000010;
	if (S == 1) cmd = cmd | 0b00000001;
	LCD_command(cmd);
}
