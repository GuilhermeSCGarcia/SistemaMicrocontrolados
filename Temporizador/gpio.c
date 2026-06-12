// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTJ  (0x0100) //bit 8
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTF  (0x0020) //bit 5

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void GPIO_Init(void)
{
	// 1.
	SYSCTL_RCGCTIMER_R = 0x04;
	// 1
	while (SYSCTL_PRTIMER_R != 0x04) {}; 
		
	// 2
	TIMER2_CTL_R = 0x00;
		
	// 3
	TIMER2_CFG_R = 0x00;
		
	// 4 - One - Shot 
	TIMER2_TAMR_R = 0x02;
		
	// 5
	TIMER2_TAILR_R = 55999999;
		
	// 6 
	TIMER2_TAPR_R = 0x00;
		
	// 7 
	TIMER2_ICR_R = 0x01;
		
	// 8a
	TIMER2_IMR_R = 0x01;
	
	// 8b
	NVIC_PRI5_R = 4 << 29;
	
	// 8C
	NVIC_EN0_R = 1 << 23;
	
	// 9
	TIMER2_CTL_R = 0x01;
	
		
//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF) ) != (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTF_AHB_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTF_AHB_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1 
	GPIO_PORTF_AHB_DIR_R = 0x11; //BIT 0 | BIT4
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTF_AHB_AFSEL_R = 0x00;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;   //Bit0 e bit1
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTF_AHB_DEN_R = 0x11;   //Bit 0 e bit4
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //Bit0 e bit1	
	
	//8. Habilita a interrupção
	GPIO_PORTJ_AHB_IM_R = 0x00;
	
	//9. Borda ou nível
	GPIO_PORTJ_AHB_IS_R = 0x00;
	
	//10. Uma borda ou ambas as bordas
	GPIO_PORTJ_AHB_IBE_R = 0x00;
	
	//11. Borda de descida ou borda de subida
	GPIO_PORTJ_AHB_IEV_R = 0x00;
	
	//12.
	GPIO_PORTJ_AHB_ICR_R = 0x03;
	
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

void GPIOPortJ_Handler ()
{
	 int atual = TIMER2_CTL_R;
	 if (atual == 0x00)
	 {
		 TIMER2_CTL_R = 0x01;
	 }
	 else 
	 {
		 TIMER2_CTL_R = 0x00;
	 }
	 
	 GPIO_PORTJ_AHB_ICR_R = 0x03;
}

void InvertePino()
{
	 int atual = GPIO_PORTN_DATA_R;
	 if (atual == 0x00)
	 {
		 PortN_Output(0x01);
	 }
	 else 
	 {
		 PortN_Output(0x00);
	 }
}

void Timer2A_Handler () 
{
	//limpando o ack da interrupção
	TIMER2_ICR_R = 0x01;
	InvertePino();
}

