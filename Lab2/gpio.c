// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas L, M e K
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

#define GPIO_PORTK   (0x0200) //bit 10
#define GPIO_PORTL   (0x0400) //bit 11
#define GPIO_PORTM   (0x0800) //bit 13     

void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports L, M e K
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = ( GPIO_PORTK | GPIO_PORTL | GPIO_PORTM);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & ( GPIO_PORTK | GPIO_PORTL | GPIO_PORTM) ) != (GPIO_PORTK | GPIO_PORTL | GPIO_PORTM) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTK_AMSEL_R = 0x00;
	GPIO_PORTM_AMSEL_R = 0x00;
	GPIO_PORTL_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTK_PCTL_R = 0x00;
	GPIO_PORTM_PCTL_R = 0x00;
	GPIO_PORTL_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTK_DIR_R = 0xFF; // BITS K0, K1, K2, K3, K4, K5, K6 e K7 (saída pro LCD)
	GPIO_PORTM_DIR_R = 0x07; // BITS M0, M1, M2 (saída pro LCD) e M4, M5, M6, M7 (entrada pro teclado) (antes estava F7)
	GPIO_PORTL_DIR_R = 0x10; // BITS L0, L1, L2, L3 (entrada pro teclado) e L4 (saída pro motor)
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTK_AFSEL_R = 0x00;
	GPIO_PORTM_AFSEL_R = 0x00;
	GPIO_PORTL_AFSEL_R = 0x00;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTK_DEN_R = 0xFF;       // BITS K0, K1, K2, K3, K4, K5, K6 e K7
	GPIO_PORTM_DEN_R = 0xF7;       // BITS M0, M1, M2, M4, M5, M6 e M7
	GPIO_PORTL_DEN_R = 0x1F;			 // BITS L0, L1, L2, L3 e L4
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTL_PUR_R = 0x0F;
	
	// Parte dos temporizadores
	
	// 1. Ativar o clock do temporizador 
	SYSCTL_RCGCTIMER_R = 0x01;     // timer 0
	SYSCTL_RCGCTIMER_R = 0x04;     // timer 2
	// 1. Verificar o bit do temporizador respectivo no registrador PRTIMER para saber se está pronto para o uso
	while (SYSCTL_PRTIMER_R != 0x05) {}; //pro 0 e 2 ao mesmo tempo
		
	// 2. Habilita timer A ou B
	TIMER0_CTL_R = 0x00;					 // Habilitando o 0A
	TIMER2_CTL_R = 0x00;           // Habilitando o 2A
		
	// 3. Configura de quantos bits será a contagem (16 bits = 0x04 e 32 bits = 0x00)
	TIMER0_CFG_R = 0x00;           //Habilitando 32 bits
	TIMER2_CFG_R = 0x00;           //Habilitando 32 bits
		
	// 4. Configura modo de operação (Oneshot = 0x1 e Periódico 0x2)
	TIMER0_TAMR_R = 0x02;           //Habilitando 0A em periódico
	TIMER2_TAMR_R = 0x02;           //Habilitando 2A em periódico
		
	// 5. Valor que o timer começa a vai diminuindo (Se 32 bits usa só o TAILR, se timer B como 16 bits usa TBILR também)
	TIMER0_TAILR_R = 55999999;      
	TIMER2_TAILR_R = 55999999;
		
	// 6. Registrador de preescala, só usar se for 16 bits, 32 deixa zerado. A (TAPR) e B (TBPR)
	TIMER0_TAPR_R = 0x00;
	TIMER2_TAPR_R = 0x00;
		
	// 7. Limpa a interrupção (ACK) (escreve 1 onde quer limpar!!)
	TIMER0_ICR_R = 0x01;            //Limpa pro timer 0A
	TIMER2_ICR_R = 0x01;            //Limpa pro timer 2A
		
	// 8a. Controla a interrupção 
	TIMER0_IMR_R = 0x01;				   //Habilita interrupção no timer 0A
	TIMER2_IMR_R = 0x01;           //Habilita interrupção no timer 2A
	
	// 8b. Seta a prioridade da interrupção
	NVIC_PRI4_R = 4 << 29;         //PRI4 porque tem o numero 19 e o 19 tá no PRI4 e move 29 posições pra setar prioridade 4
	NVIC_PRI5_R = 4 << 29;         //PRI5 porque tem o numero 23 e o 23 tá no PRI5 e move 29 posições pra setar prioridade 4
	
	// 8c. Habilitar a interrupção do timer respectivo no respectivo registrador
	NVIC_EN0_R = 1 << 19;          // 0A numero 19
	NVIC_EN0_R |= 1 << 23;         // | pra manter o que coloquei antes, 2A numero 23
	
	// 9. Coloca 1 pra habilitar e começar a contagem
	TIMER0_CTL_R = 0x01;
	TIMER2_CTL_R = 0x01;
	
}	

// -------------------------------------------------------------------------------
// Função LCD_Init
// Inicializa o LCD
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void LCD_Init ()
{
	GPIO_PORTK_DATA_R = 0x38;
	GPIO_PORTM_DATA_R = 0x04;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1us(40);

	GPIO_PORTK_DATA_R = 0x06;
	GPIO_PORTM_DATA_R = 0x04;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1us(40);
	
	GPIO_PORTK_DATA_R = 0x0E;
	GPIO_PORTM_DATA_R = 0x04;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1us(40);
	
	GPIO_PORTK_DATA_R = 0x01;
	GPIO_PORTM_DATA_R = 0x04;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(2);
	
	return;
}

// -------------------------------------------------------------------------------
// Função escreveLCD
// Escreve no LCD
// Parâmetro de entrada: Valor que quer escrever (uma letra/número por vez)
// Parâmetro de saída: Não tem
void escreveLCD(int valor)
{
	GPIO_PORTK_DATA_R = valor;
	GPIO_PORTM_DATA_R = 0x05;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1us(40);
}

// -------------------------------------------------------------------------------
// Função pulaLinha
// Pula pra outra linha do LCD
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void pulaLinha()
{
	GPIO_PORTK_DATA_R = 0xC0;
	GPIO_PORTM_DATA_R = 0x04;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(2);
	
	return;
}

// -------------------------------------------------------------------------------
// Função varreTeclado
// Varre o teclado vendo qual tecla foi pressionada
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Tecla pressionada ou 'F' se nenhuma
int varreTeclado()
{
	GPIO_PORTM_DIR_R &= 0x0F;
	SysTick_Wait1ms(1);
	GPIO_PORTM_DIR_R |= 0x10;
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(1);
	
	int linhas = GPIO_PORTL_DATA_R;
	int botao;
	if (linhas != 0xF)
	{
		switch(linhas)
		{
			case 0xE:
				botao = '1';
				break;
			case 0xD:
				botao = '4';
				break;
			case 0xB:
				botao = '7';
				break;
			case 0x7:
				botao = '*';
				break;
			default: 
				botao = 'F';
		}
		
		return botao;
	}
		
	GPIO_PORTM_DIR_R &= 0x0F;
	SysTick_Wait1ms(1);
	GPIO_PORTM_DIR_R |= 0x20;
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(1);
	
	linhas = GPIO_PORTL_DATA_R;
	if (linhas != 0xF)
	{
		switch(linhas)
		{
			case 0xE:
				botao = '2';
				break;
			case 0xD:
				botao = '5';
				break;
			case 0xB:
				botao = '8';
				break;
			case 0x7:
				botao = '0';
				break;
			default: 
				botao = 'F';
		}
		
		return botao;
	}
	
	GPIO_PORTM_DIR_R &= 0x0F;
	SysTick_Wait1ms(1);
	GPIO_PORTM_DIR_R |= 0x40;
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(1);
	
	linhas = GPIO_PORTL_DATA_R;
	if (linhas != 0xF)
	{
		switch(linhas)
		{
			case 0xE:
				botao = '3';
				break;
			case 0xD:
				botao = '6';
				break;
			case 0xB:
				botao = '9';
				break;
			case 0x7:
				botao = '#';
				break;
			default: 
				botao = 'F';
		}
		
		return botao;
	}
	
	GPIO_PORTM_DIR_R &= 0x0F;
	SysTick_Wait1ms(1);
	GPIO_PORTM_DIR_R |= 0x80;
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(1);
	
	linhas = GPIO_PORTL_DATA_R;
	if (linhas != 0xF)
	{
		switch(linhas)
		{
			case 0xE:
				botao = 'A';
				break;
			case 0xD:
				botao = 'B';
				break;
			case 0xB:
				botao = 'C';
				break;
			case 0x7:
				botao = 'D';
				break;
			default: 
				botao = 'F';
		}
		
		return botao;
	}
	
	return 'F';
	
}

// -------------------------------------------------------------------------------
// Função Iimer0A_Handler
// Lida com a interrupção do timer 0A (do PWM)
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void Timer0A_Handler () 
{
	//limpando o ack da interrupção
	TIMER0_ICR_R = 0x01;
	//faz o que tem que fazer
}

// -------------------------------------------------------------------------------
// Função Timer2A_Handler
// Lida com a interrupção do timer 2A
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void Timer2A_Handler () 
{
	//limpando o ack da interrupção
	TIMER2_ICR_R = 0x01;
	//faz o que tem que fazer
}


