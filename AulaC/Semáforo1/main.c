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
	while (1)
	{		
		switch(estado)
		{
			case VermelhoVermelho1:
				estado = VermelhoVerde;
				if (pedestre == 1)
				{
					Pisca_leds();
					Pisca_leds();
					Pisca_leds();
					Pisca_leds();
					pedestre = 0;
				}
			  PortN_Output (0x03); //liga os dois leds do N
			  PortF_Output (0x11); //liga os dois leds do F
			  SysTick_Wait1ms (2000);
				break;
			
			case VermelhoVerde:
				estado = VermelhoAmarelo;
				PortN_Output (0x03); //liga os dois leds do N
				PortF_Output (0x10); //liga só o PF4
				SysTick_Wait1ms (8000);
			break;
			
			case VermelhoAmarelo:
				estado = VermelhoVermelho2;
				PortN_Output (0x03); //liga os dois leds do N
			  PortF_Output (0x01); //liga só o PF0
				SysTick_Wait1ms (4000);
				break;
			
			case VermelhoVermelho2:
				estado = VerdeVermelho;
				PortN_Output (0x03); //liga os dois leds do N
			  PortF_Output (0x11); //liga os dois leds do F
				SysTick_Wait1ms (2000);
				break;
			
			case VerdeVermelho:
				estado = AmareloVermelho;
			  PortN_Output (0X02); //liga só PN1
				PortF_Output (0x11); //liga os dois leds do F
				SysTick_Wait1ms (8000);
				break;
			
			case AmareloVermelho:
				estado = VermelhoVermelho1;
				PortN_Output (0X01); //liga só PN0
				PortF_Output (0x11); //liga os dois leds do F
				SysTick_Wait1ms (4000);
				break;
	
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
