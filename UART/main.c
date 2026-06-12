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
void PortN_Output(uint32_t leds);
void PortF_Output(uint32_t leds);

void UART_Init();
int receber_dados();
void enviar_dados(int mensagen);
uint32_t PORTF_data();
uint32_t PORTN_data();

void empacota_string(char mensagem[]);


int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	UART_Init();
	while (1)
	{		
		int dados = receber_dados();
		int valorN;
		int valorF;
		
		valorN = PORTN_data();
		valorF = PORTF_data();
		
		switch(dados)
		{
			case '1':
				empacota_string ("1  ");
			
				if(valorN == 0x00 | valorN == 0x01)
				{
					PortN_Output (valorN | 0x02);
					empacota_string("LED1 ligado\r\n");
				}
				else if (valorN == 0x02 | valorN == 0x03)
				{
					PortN_Output (valorN & 0x01);
					empacota_string("LED1 desligado\r\n");
				}
				break;
			case '2':
				empacota_string ("2  ");
				if(valorN == 0x00 | valorN == 0x02)
				{
					PortN_Output(valorN | 0x01);
					empacota_string("LED2 ligado\r\n");
				}
				else if (valorN == 0x01 | valorN == 0x03)
				{
					PortN_Output (valorN & 0x02);
					empacota_string("LED2 desligado\r\n");
				}
				break;
			case '3': 
				empacota_string ("3  ");
				if(valorF == 0x00 | valorF == 0x01) // se PF4 e PF0 desligado ou PF4 desligado e PF0 ligado
				{
					PortF_Output (valorF | 0x10); // acende o PF4
					empacota_string("LED3 ligado\r\n");
				}
				else if (valorF == 0x10 | valorF == 0x11) // se PF4 tá ligado e PF0 desligado ou PF4 ligado e PF0 desligado
				{
					PortF_Output (valorF & 0x01); // desliga o PF4 deixa 1 no PF0
					empacota_string("LED3 desligado\r\n");
				}
				break;
			case '4':
				empacota_string ("4  ");
				if(valorF == 0x00 | valorF == 0x10) // se PF4 desligado e PF0 desligado ou PF4 ligado e PF0 desligado
				{
					PortF_Output(valorF | 0x01); // acende o PF0
					empacota_string("LED4 ligado\r\n");
				}
				else if (valorF == 0x01 | valorF == 0x11) // se PF4 tá desligado e PF0 tá ligado ou PF4 tá ligado e PF0 ligado
				{
					PortF_Output (valorF & 0x10); // desliga o PF0 e deixa 1 no PF4
					empacota_string("LED4 desligado\r\n");
				}
				break;
			default :
				continue;
		}
	}
}


void empacota_string(char mensagem[])
{
	for(int i = 0; mensagem[i] != '\0'; i++)
	{
		enviar_dados(mensagem[i]);
	}
}
