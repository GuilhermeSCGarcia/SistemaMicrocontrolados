// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// 
// 
// Prof. Guilherme Peron

#include <stdint.h>

// Funções básicas
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);

// Funções do LCD
void LCD_Init (void);
void escreveLCD(int valor);
void pulaLinha();

// Função do Teclado
int varreTeclado();

// Estado da máquina de estados
typedef enum estados
{
	ModoServo,
	ModoPosicao,
	ModoScan
} Modos;


int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	LCD_Init();
	while(1)
	{
		int modo; //modo atual 
		//máquina de estados
		switch (modo)
		{ 
			case ModoServo:
				break;
			
			case ModoPosicao:
				break;
			
			case ModoScan:
				break;
		}
	};
	
}
