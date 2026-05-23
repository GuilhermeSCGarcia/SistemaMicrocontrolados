// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// 
// 
// Prof. Guilherme Peron

#include <stdint.h>


//Constantes
#define DISPLAY_X 16 //tamanho X do display
#define DISPLAY_Y 2 //tamanho Y do display

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
void retornarCursor();
void printLCD(char c[]);
void mostrarInformacao(char c1[], char c2[]);


//Funções do Sistema
void colocarEstado(int tecla);
// Função do Teclado
int varreTeclado();

// Estado da máquina de estados
typedef enum estados
{
	ModoIdle,
	ModoServo,
	ModoPosicao,
	ModoScan
} Modos;

//Variaveis Globais
Modos modo = ModoIdle; // inicia a máquina de estados
int tecla = '*';  //guarda a tecla, coloca o valor padrão de *, que é o modo Idle

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	LCD_Init();
	//fazer o motor init pra configurar o PWM e essas coisas
	// temporizador init talvez??
	
	while(1)
	{
		//modo atual 
		//máquina de estados
		mostrarInformacao("-","-"); // mostra o modo atual escolhido
		tecla = varreTeclado(); // busca a tecla escolhida no teclado
		colocarEstado(tecla); // muda o estado
		switch (modo)
		{
			case ModoIdle:
				break;
			
			case ModoServo:
				break;
			
			case ModoPosicao:
				break;
			
			case ModoScan:
				break;
		}
	};
	
}



// -------------------------------------------------------------------------------
//Função escreveLCD
//Lida com a escrita no LCD
void printLCD(char c[])
{	
	for (int i = 0; c[i] != '\0'; i++) {
			escreveLCD((int)c[i]);
		}
}

// -------------------------------------------------------------------------------
//Função escreveLCD
//Lida com a escrita no LCD
void mostrarInformacao(char pos[], char pulso[])
{	
	LCD_Init();
	
	if(modo == ModoIdle) //se o modo escolhi for idle, so mostra idle
	{
	printLCD("Modo Idle");
	return;
	}
	
	if(modo ==  ModoPosicao) // se o modo escolhido for o de posição, mostra a posição e o pulso
	{
		printLCD("Modo Posicao");
		pulaLinha();
		printLCD("Pos:");
		printLCD(pos);
		printLCD("° /");
		printLCD(pulso);
		printLCD(" us");
		return;
	}
	
	if(modo ==  ModoScan)
	{
		printLCD("Modo Scan"); //se o modo escolhido foi o scan, mostra o scan
		return;
	}
	
}

void colocarEstado(int tecla) //função que le a tecla do teclado e escolhe um estado
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
		return; // Se não foi cilciado nada, mantêm o ultimo estado
	}
}
