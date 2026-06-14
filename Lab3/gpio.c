// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTJ  (0x0100) //bit 8
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTF  (0x0020) //bit 5
#define GPIO_PORTA  (0x0003) //bit 0 e 1
#define GPIO_PORTE  (0x0010) //bit 4
#define GPIO_PORTH  (0x0080) //bit 8

int leitura_conversor;
// -------------------------------------------------------------------------------
// Fun��o GPIO_Init
// Inicializa os ports J, N e F
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: N�o tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF | GPIO_PORTA | GPIO_PORTE | GPIO_PORTH);
	//1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF | GPIO_PORTA | GPIO_PORTE) ) != (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTF | GPIO_PORTA | GPIO_PORTE | GPIO_PORTH) ){};
	
	// 2. Limpar o AMSEL para desabilitar a anal�gica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTF_AHB_AMSEL_R = 0x00;
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTE_AHB_AMSEL_R = 0x10;
	GPIO_PORTH_AHB_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTF_AHB_PCTL_R = 0x00;
	GPIO_PORTA_AHB_PCTL_R = 0X11;
	GPIO_PORTH_AHB_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for sa�da
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1 
	GPIO_PORTF_AHB_DIR_R = 0x11; //BIT 0 | BIT4
	GPIO_PORTE_AHB_DIR_R = 0x00; // entrada
	GPIO_PORTH_AHB_DIR_R = 0X04; // BITS H0, H1, H2, H3 saida para controlar o motor de passos
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun��o alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTF_AHB_AFSEL_R = 0x00;
	GPIO_PORTA_AHB_AFSEL_R = 0x03;
	GPIO_PORTE_AHB_AFSEL_R = 0x10;
	GPIO_PORTH_AHB_AFSEL_R = 0x00;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;   //Bit0 e bit1
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTF_AHB_DEN_R = 0x11;   //Bit 0 e bit4
	GPIO_PORTA_AHB_DEN_R = 0x03;
	GPIO_PORTE_AHB_DEN_R = 0x00;
	GPIO_PORTH_AHB_DEN_R = 0x04;   // BITS H0, H1, H2 E H3	
	
	//passos extras
	SYSCTL_RCGCADC_R = 0x01;
	
	while (SYSCTL_RCGCADC_R == 0x00)
	 
	ADC0_PC_R = 7;
	
	ADC0_SSPRI_R = 0x123;
	
	ADC0_ACTSS_R = 0x00;
	
	ADC0_EMUX_R =  0x00;

	ADC0_SSMUX3_R = 9;
	
	ADC0_SSCTL3_R = 0x06;
	
	ADC0_ACTSS_R = 0x08;
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //Bit0 e bit1
	
	//8. Habilita a interrup��o
	GPIO_PORTJ_AHB_IM_R = 0x00;
	
	//9. Borda ou n�vel
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

void UART_Init(void)
{
		SYSCTL_RCGCUART_R = 0x01;              // Habilita o UART 0
		while(SYSCTL_PRUART_R != 0x01) {}
			
		UART0_CTL_R = 0x00;
			
		UART0_IBRD_R = 43;                     // BAUD RATE, parte inteira da conta 80000000/(16*115200)
		UART0_FBRD_R = 51;										// Parte fracion�ria
		
		UART0_LCRH_R = 0x72;		              // Controla as coisas, tipo n� de bits = 8 bits, paridade �mpar e 1 stop bit
			
		UART0_CC_R = 0x00;
			
		UART0_CTL_R = 0x301;
}

// -------------------------------------------------------------------------------
// Fun��o PortJ_Input
// L� os valores de entrada do port J
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: o valor da leitura do port
uint32_t PortJ_Input(void)
{
	return GPIO_PORTJ_AHB_DATA_R;
}

// -------------------------------------------------------------------------------
// Fun��o PortN_Output
// Escreve os valores no port N
// Par�metro de entrada: Valor a ser escrito
// Par�metro de sa�da: n�o tem
void PortN_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amig�vel nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
    //agora vamos fazer o OR com o valor recebido na fun��o
    temp = temp | valor;
    GPIO_PORTN_DATA_R = temp; 
}

// -------------------------------------------------------------------------------
// Fun��o PortF_Output
// Escreve os valores no port F
// Par�metro de entrada: Valor a ser escrito
// Par�metro de sa�da: n�o tem
void PortF_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amig�vel nos bits 0 e 1
    temp = GPIO_PORTF_AHB_DATA_R & 0xEE;
    //agora vamos fazer o OR com o valor recebido na fun��o
    temp = temp | valor;
    GPIO_PORTF_AHB_DATA_R = temp; 
}

uint32_t PORTF_data()
{
	return GPIO_PORTF_AHB_DATA_R;
}

uint32_t PORTN_data()
{
	return GPIO_PORTN_DATA_R;
}

//Interrup��o


int receber_dados()
{
	while((UART0_FR_R & 0x10) == 0x10)
	{
	}
	
	return UART0_DR_R;
}

int receber_dados_nao_bloqueante()
{
		//se a fila n�o tiver dado, ele n�o vai ter nada escrito na fifo, retorna -1 para a variavel que controla se tem dado
    if (UART0_FR_R & 0x10) {
        return 0 ;
    }
    return UART0_DR_R ;
}

void enviar_dados(int mensagem)
{
	while((UART0_FR_R & 0x20) == 0x20)
	{
	}
	
	UART0_DR_R = mensagem;
}

int converter_ad()
{
	ADC0_PSSI_R = 0x0008;
	
	while(ADC0_RIS_R != 0x0008)
	{
		
	}
	
	leitura_conversor = ADC0_SSFIFO3_R;
	ADC0_ISC_R = 0x00;
	
	return leitura_conversor;
}

