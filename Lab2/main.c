// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// 
// 
// Prof. Guilherme Peron

#include <stdint.h>
#include "tm4c1294ncpdt.h"

//Constantes
#define DISPLAY_X 16 //tamanho X do display
#define DISPLAY_Y 2 //tamanho Y do display

// Fun��es b�sicas
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);

// Fun��es do LCD
void LCD_Init (void);
void escreveLCD(int valor);
void pulaLinha();
void retornarCursor();
void printLCD(char c[]);
void mostrarInformacao(char c1[], char c2[]);


//Fun��es do Sistema
void colocarEstado(int tecla);

// Fun��o do Teclado
int varreTeclado();

// Variaveis Globais do gpio
extern int porta;
extern int angulo;
extern int duty;
extern int cima_ou_baixo;

// Estado da m�quina de estados
typedef enum estados
{
	ModoIdle,
	ModoPosicao,
	ModoScan
} Modos;

//Variaveis Globais
Modos modo = ModoIdle; // inicia a m�quina de estados
int tecla = '*';  //guarda a tecla, coloca o valor padr�o de *, que � o modo Idle

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init(); // já faz o init dos temporizadores também!!
	LCD_Init();
	
	while(1)
	{
		//modo atual 
		//m�quina de estados
		mostrarInformacao("-","-"); // mostra o modo atual escolhido
		tecla = varreTeclado(); // busca a tecla escolhida no teclado
		colocarEstado(tecla); // muda o estado
		switch (modo)
		{
			case ModoIdle:
				// deliga o timer 2A
				TIMER2_CTL_R &= ~0x01;

				angulo = 90;
				calculaDuty(angulo);
				break;
			
			case ModoPosicao:
				// deliga o timer 2A
				TIMER2_CTL_R &= ~0x01;

				if (tecla == 0)
				{
					angulo = 180;
				}
				else 
				{
					angulo = (tecla - 1) * 20;
				}

				calculaDuty(angulo);
				int pos;
				pos = ((0.5 + angulo * 0.01) * 1000);
				mostrarInformacao(angulo, pos);
				break;
			
			case ModoScan:
				angulo = 0;
				calculaDuty(angulo);

				//ligo o timer 2A
				TIMER2_CTL_R |= 0x01; 

				int pos;
				pos = ((0.5 + angulo * 0.01) * 1000);
				mostrarInformacao(angulo, pos);
				break;
		}
	};
	
}


// -------------------------------------------------------------------------------
//Fun��o escreveLCD
//Lida com a escrita no LCD
void printLCD(char c[])
{	
	for (int i = 0; c[i] != '\0'; i++) {
			escreveLCD((int)c[i]);
		}
}

// -------------------------------------------------------------------------------
//Fun��o escreveLCD
//Lida com a escrita no LCD
void mostrarInformacao(char pos[], char pulso[])
{	
	LCD_Init();
	
	if(modo == ModoIdle) //se o modo escolhi for idle, so mostra idle
	{
	printLCD("Modo Idle");
	return;
	}
	
	if(modo ==  ModoPosicao) // se o modo escolhido for o de posi��o, mostra a posi��o e o pulso
	{
		printLCD("Modo Posicao");
		pulaLinha();
		printLCD("Pos:");
		printLCD(pos);
		printLCD("� /");
		printLCD(pulso);
		printLCD(" us");
		return;
	}
	
	if(modo ==  ModoScan)
	{
		printLCD("Modo Scan"); //se o modo escolhido foi o scan, mostra o scan
		pulaLinha();
		printLCD("Pos:");
		printLCD(pos);
		printLCD("� /");
		printLCD(pulso);
		printLCD(" us");
		return;
	}
	
}

void colocarEstado(int tecla) //fun��o que le a tecla do teclado e escolhe um estado
{
	if( tecla == '*') // Se a tecla for *, coloca em Idle
	{
		modo = ModoIdle;
	}else if (tecla == 'A') // Se for A, coloca em modo scan
	{
	modo = ModoScan;
	}else if(tecla >= '0' && tecla <= '9') // se a tecla tiver no raio entre 0 e 9, coloca no modo posicao
	{
		modo = ModoPosicao;
	}else
	{
		return; // Se n�o foi cilciado nada, mant�m o ultimo estado
	}
}

void calculaDuty(int angulo)
{
	// 0.5 + (angulo * 0.011) de 0.5 ms até 2.5 ms
	duty =  40000 + (angulo * 889);
}