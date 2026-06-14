// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>

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
int converter_ad();
int receber_dados_nao_bloqueante();


void empacota_string(char mensagem[]);
void selecionarModo();

typedef enum estados
{
    ModoIdle,
    ModoPotenciomentro,
    ModoTerminal
} Modos;


//Variaveis Globais

Modos modo = ModoIdle;
Modos ultimoModo;

int tecla = 0;
int dadoRecebido;



int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	UART_Init();
	
	
	while (1)
	{		
		selecionarModo();
		switch (modo)
        {
            case ModoIdle:
							empacota_string("Sistema em IDLE. Esperando comando�\r\n");
							while( tecla != 't' || tecla != 'p')
								{
									tecla = receber_dados();
								}
								if(tecla == 't')
									modo = ModoTerminal;
								else
									modo = ModoPotenciomentro;
							break;

            case ModoPotenciomentro:
               break;

            case ModoTerminal:
               break;
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

void selecionarModo()
{
	tecla = receber_dados_nao_bloqueante();
	if(tecla)
		{
			switch(tecla)
			{
				case 't':
					modo = ModoTerminal;
					break;
				case 'p':
					modo = ModoPotenciomentro;
					break;
			}
		}
}





//		int leitura;
//		float resultado;
//		char leitura_string[20], resultado_string[20];
//		leitura = converter_ad();
//		SysTick_Wait1ms(1000);
//		resultado = ((float) leitura * 805.66) / 1000000.0;
//		snprintf(leitura_string, sizeof(leitura_string), "%d", leitura);
//		snprintf(resultado_string, sizeof(resultado_string), "%.1f", resultado);
//		empacota_string(leitura_string);
//		empacota_string(" / ");
//		empacota_string(resultado_string);
//		empacota_string("  ");
