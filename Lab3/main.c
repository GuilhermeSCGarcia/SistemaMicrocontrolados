// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include "tm4c1294ncpdt.h"

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);
void Temporizador_Init();
void UART_Init();

// UART
int receber_dados();
int receber_dados_nao_bloqueante();
void enviar_dados(int mensagen);

// ADC
int converter_ad();
void empacota_string(char mensagem[]);
void mostra_ADC_e_pos();

// Variavel externa
extern int pos_atual;

void selecionarModo();
int calcula_angulo (int passos);

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
	Temporizador_Init();
	UART_Init();
		
	while (1)
	{		
		selecionarModo();
		switch (modo)
    {
      case ModoIdle:
				empacota_string("Sistema em IDLE. Esperando comando...\r\n");
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
					// ligo o timer 0A
          TIMER0_CTL_R |= 0x01;
					mostra_ADC_e_pos();
						
          break;

       case ModoTerminal:
					// desliga o timer 0A
          TIMER0_CTL_R = 0x00;
					mostra_ADC_e_pos();
						
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

int calcula_angulo (int passos)
{
	return ((passos * 360) / 4096);
}

void mostra_ADC_e_pos()
{
	char string[50];
	float adc = pos_atual;
	float pos = calcula_angulo(pos_atual);
						
	// coloca em string o que está entre aspas
	sprintf(string, "[POT] ADC: %.2f | Posicao: %.2f", adc, pos);
	// manda pro terminal a string
	empacota_string(string);
	empacota_string("\r\n");
}
