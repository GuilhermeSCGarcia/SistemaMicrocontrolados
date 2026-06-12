// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTJ  (0x0100) //bit 8
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTF  (0x0020) //bit 5

#define GPIO_PORTK   (0x0200) //bit 10
#define GPIO_PORTL   (0x0400) //bit 11
#define GPIO_PORTM   (0x0800) //bit 13      000000 1000 0000 0000

extern int pedestre;
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J, N, F, K, N e M
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF | GPIO_PORTK | GPIO_PORTM | GPIO_PORTL);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTF | GPIO_PORTK | GPIO_PORTN | GPIO_PORTM | GPIO_PORTL) ) != (GPIO_PORTJ | GPIO_PORTF | GPIO_PORTK | GPIO_PORTN | GPIO_PORTM | GPIO_PORTL) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTF_AHB_AMSEL_R = 0x00;
	GPIO_PORTK_AMSEL_R = 0x00;
	GPIO_PORTM_AMSEL_R = 0x00;
	GPIO_PORTL_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTF_AHB_PCTL_R = 0x00;
	GPIO_PORTK_PCTL_R = 0x00;
	GPIO_PORTM_PCTL_R = 0x00;
	GPIO_PORTL_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1 
	GPIO_PORTF_AHB_DIR_R = 0x11; //BIT 0 | BIT4
	GPIO_PORTK_DIR_R = 0xFF; // BIT 0-7
	GPIO_PORTM_DIR_R = 0xF7; // BIT 4 - 7 E 0 - 2
	GPIO_PORTL_DIR_R = 0x00; // BIT 0-3
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTF_AHB_AFSEL_R = 0x00;
	GPIO_PORTK_AFSEL_R = 0x00;
	GPIO_PORTM_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00;
	GPIO_PORTL_AFSEL_R = 0x00;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;   //Bit0 e bit1
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTF_AHB_DEN_R = 0x11;   //Bit 0 e bit4
	GPIO_PORTK_DEN_R = 0xFF;       // BIT 0-7
	GPIO_PORTM_DEN_R = 0xF7;       // BIT 4 - 7 E 0 - 2
	GPIO_PORTL_DEN_R = 0x0F;			 // BIT 0-3
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //Bit0 e bit1	
	GPIO_PORTL_PUR_R = 0x0F;
	
	//8. Habilita a interrupção
	GPIO_PORTJ_AHB_IM_R = 0x00;
	
	//9. Borda ou nível
	GPIO_PORTJ_AHB_IS_R = 0x00;
	
	//10. Uma borda ou ambas as bordas
	GPIO_PORTJ_AHB_IBE_R = 0x00;
	
	//11. Borda de descida ou borda de subida
	GPIO_PORTJ_AHB_IEV_R = 0x00;
	
	//12.
	GPIO_PORTJ_AHB_ICR_R = 0x01;
	
	//13. 
	GPIO_PORTJ_AHB_IM_R = 0x01;
	
	//14. 
	NVIC_EN1_R = 1 << 19;
	
	//15. 
	NVIC_PRI12_R = 5 << 29;
	
}	

// -------------------------------------------------------------------------------
// Função PortJ_Input
// Lê os valores de entrada do port J
// Parâmetro de entrada: Não tem
// Parâmetro de saída: o valor da leitura do port
uint32_t PortJ_Input(void)
{
	return GPIO_PORTJ_AHB_DATA_R;
}

// -------------------------------------------------------------------------------
// Função PortN_Output
// Escreve os valores no port N
// Parâmetro de entrada: Valor a ser escrito
// Parâmetro de saída: não tem
void PortN_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
    //agora vamos fazer o OR com o valor recebido na função
    temp = temp | valor;
    GPIO_PORTN_DATA_R = temp; 
}

// -------------------------------------------------------------------------------
// Função PortF_Output
// Escreve os valores no port F
// Parâmetro de entrada: Valor a ser escrito
// Parâmetro de saída: não tem
void PortF_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTF_AHB_DATA_R & 0xEE;
    //agora vamos fazer o OR com o valor recebido na função
    temp = temp | valor;
    GPIO_PORTF_AHB_DATA_R = temp; 
}

//Interrupção

void GPIOPortJ_Handler()
{
	if (GPIO_PORTJ_AHB_MIS_R == 0x01)
	{
		GPIO_PORTJ_AHB_ICR_R = 0x01;
		pedestre = 1;
	}
	return;
}

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

void escreveLCD(int valor)
{
	GPIO_PORTK_DATA_R = valor;
	GPIO_PORTM_DATA_R = 0x05;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1us(40);
}

void pulaLinha()
{
	GPIO_PORTK_DATA_R = 0xC0;
	GPIO_PORTM_DATA_R = 0x04;
	SysTick_Wait1us(10);
	GPIO_PORTM_DATA_R = 0x00;
	SysTick_Wait1ms(2);
	
	return;
}

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

