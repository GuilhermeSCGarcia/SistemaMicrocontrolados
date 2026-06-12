// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);
void PortF_Output(uint32_t leds);
void Pisca_leds(void);

void escreveLCD(int valor);
void pulaLinha();
int varreTeclado();

void LCD_Init (void);

typedef enum estados
{
	VermelhoVermelho1,
	VermelhoVerde,
	VermelhoAmarelo,
	VermelhoVermelho2,
	VerdeVermelho,
	AmareloVermelho
} Estados;

Estados estado = 	VermelhoVerde;
int pedestre = 0;

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	LCD_Init();
	
//	escreveLCD('U');
//	escreveLCD('T');
//	escreveLCD('F');
//	escreveLCD('P');
//	escreveLCD('R');
//	
//	pulaLinha();
//	
//	escreveLCD('B');
//	escreveLCD('R');
//	escreveLCD('U');
//	escreveLCD('N');
//	escreveLCD('A');
//	escreveLCD(' ');
//	escreveLCD('L');
//	escreveLCD('A');
//	escreveLCD('U');
//	escreveLCD('R');
//	escreveLCD('A');
//	escreveLCD(' ');
//	escreveLCD('G');
//	escreveLCD('U');
//	escreveLCD('I');
	
	int botao = 'F';
		
	while(1)
	{
		botao = varreTeclado();
		if(botao != 'F')
		{	
			SysTick_Wait1ms(200);
			escreveLCD(botao);
		}
	}
	
}

void Pisca_leds(void)
{
			PortN_Output (0x03); //liga os dois leds do N
			PortF_Output (0x11); //liga os dois leds do F
			SysTick_Wait1ms (100);
			PortN_Output (0x00); //liga os dois leds do N
			PortF_Output (0x00); //liga os dois leds do F
			SysTick_Wait1ms (100);
}
