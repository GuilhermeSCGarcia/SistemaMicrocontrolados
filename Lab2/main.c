// main.c
// Desenvolvido para a placa EK-TM4C1294XL
//
//
//
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include "tm4c1294ncpdt.h"

// Constantes
#define DISPLAY_X 16 // tamanho X do display
#define DISPLAY_Y 2  // tamanho Y do display

// Funcoes basicas
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);

// Funcoes do LCD
void LCD_Init(void);
void LCD_Reset(void);
void escreveLCD(int valor);
void pulaLinha();
void retornarCursor();
void printLCD(char c[]);
void mostrarInformacao(char c1[], char c2[]);

// Funcoes do Sistema
void colocarEstado(int tecla);
void calculaDuty(int angulo);
void intToStr(int N, char *str);

// Funcao do Teclado
int varreTeclado();

// Variaveis Globais do gpio
extern int angulo;
extern int duty;

// Estado da maquina de estados
typedef enum estados
{
    ModoIdle,
    ModoPosicao,
    ModoScan
} Modos;

// Variaveis Globais
Modos modo = ModoIdle; // inicia a maquina de estados
Modos modoAnterior;
int tecla;             // guarda a tecla
char posS[10];
char angS[10];
int pos;
int ultimaTecla = '*';

int main(void)
{
    PLL_Init();
    SysTick_Init();
    GPIO_Init(); // ja faz o init dos temporizadores tambem!!
    LCD_Init();

    while (1)
    {
        // modo atual
        // maquina de estados
        tecla = varreTeclado(); // busca a tecla escolhida no teclado
				if (tecla == 'F')
				{
						tecla = ultimaTecla; // Se não foi pressionado nenhuma tecla, mantêm a a ultima opção clicada
				}
				else
				{
						ultimaTecla = tecla; // Senão, troca a ultima tecla pela que foi apertada
				}
				
	
        colocarEstado(tecla);	// Muda o estado
				if (modo != modoAnterior) //Verifica se trocou de modo
					{
						if (modo == ModoScan) //Se trocou de modo, e o modo escolhido foi o scan, precisamos deixar o angulo em zero para começar a varredura
						{
							angulo = 0;
						}
						modoAnterior = modo; //Preserva o modo escolhido em ultimo modo
					}
					
        switch (modo)
        {
            case ModoIdle:
                mostrarInformacao("-", "-");
                ultimaTecla = tecla;
                // desliga o timer 2A
                TIMER2_CTL_R &= ~0x01;

                angulo = 90;
                calculaDuty(angulo);
                break;

            case ModoPosicao:
                // desliga o timer 2A
                TIMER2_CTL_R &= ~0x01;

                if (tecla == '0')
                {
                    angulo = 180;
                }
                else
                {
                    angulo = ((tecla - '0') - 1) * 20;
                }

                calculaDuty(angulo);
                pos = ((0.5 + angulo * 0.01) * 1000);
                sprintf(posS, "%d", pos); //Transformo o angulo e a duração do pulso em char para mostrar no display LCD
                sprintf(angS, "%d", angulo);
                mostrarInformacao(angS, posS);
                break;

            case ModoScan:
                calculaDuty(angulo);

                // ligo o timer 2A
                TIMER2_CTL_R |= 0x01;

                pos = ((0.5 + angulo * 0.01) * 1000);
                sprintf(posS, "%d", pos); //Transformo o angulo e a duração do pulso em char para mostrar no display LCD
                sprintf(angS, "%d", angulo);
                mostrarInformacao(angS, posS);
                break;
        }
    }
}

// -------------------------------------------------------------------------------
// Funcao escreveLCD
// Lida com a escrita no LCD
void printLCD(char c[])
{
    for (int i = 0; c[i] != '\0'; i++)
    {
        escreveLCD((int)c[i]);
    }
}

// -------------------------------------------------------------------------------
// Funcao escreveLCD
// Lida com a escrita no LCD
void mostrarInformacao(char pos[], char pulso[])
{
    LCD_Init();

    if (modo == ModoIdle) // se o modo escolhido for idle, so mostra idle
    {
        printLCD("Modo Idle");
        return;
    }

    if (modo == ModoPosicao) // se o modo escolhido for o de posicao, mostra a posicao e o pulso
    {
        printLCD("Modo Posicao");
        pulaLinha();
        printLCD("Pos:");
        printLCD(pos);
        printLCD("/");
        printLCD(pulso);
        printLCD(" us");
        return;
    }

    if (modo == ModoScan)
    {
        printLCD("Modo Scan"); // se o modo escolhido foi o scan, mostra o scan
        pulaLinha();
        printLCD("Pos:");
        printLCD(pos);
        printLCD("/");
        printLCD(pulso);
        printLCD(" us");
        return;
    }

    
}

void colocarEstado(int tecla) // funcao que le a tecla do teclado e escolhe um estado
{
    if (tecla == '*') // se a tecla for *, coloca em Idle
    {
        modo = ModoIdle;
    }
    else if (tecla == 'A') // se for A, coloca em modo scan
    {
        modo = ModoScan;
    }
    else if (tecla >= '0' && tecla <= '9') // se a tecla tiver no raio entre 0 e 9, coloca no modo posicao
    {
        modo = ModoPosicao;
    }
    else
    {
        return; // se nao foi clicado nada, mantem o ultimo estado
    }
}

void calculaDuty(int angulo)
{
    // 0.5 + (angulo * 0.011) de 0.5 ms ate 2.5 ms
    duty = 40000 + (angulo * 889);
}