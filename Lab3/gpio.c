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
void SysTick_Wait1ms(uint32_t delay);
volatile int pos_atual = 0;

// movimentacao do motor de passo
int calcula_passos (int angulo);
void gira_passo_direita(int angulo);
void gira_passo_esquerda(int angulo);
void gira_meio_passo_direita(int passos);
void gira_meio_passo_esquerda(int passos);
void mostra_pos();

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

void Temporizador_Init()
{
	// Parte dos temporizadores
	
	// 1. Ativar o clock do temporizador 
	SYSCTL_RCGCTIMER_R = 0x05;           // timer 0 e timer 2
	// 1. Verificar o bit do temporizador respectivo no registrador PRTIMER para saber se est? pronto para o uso
	while (SYSCTL_PRTIMER_R != 0x05) {}; //pro 0 e 2 ao mesmo tempo
		
	// 2. Habilita timer A ou B
	TIMER0_CTL_R = 0x00;		   // Habilitando o 0A
	TIMER2_CTL_R = 0x00;           // Habilitando o 2A
		
	// 3. Configura de quantos bits ser? a contagem (16 bits = 0x04 e 32 bits = 0x00)
	TIMER0_CFG_R = 0x00;           //Habilitando 32 bits
	TIMER2_CFG_R = 0x00;           //Habilitando 32 bits
		
	// 4. Configura modo de opera??o (Oneshot = 0x1 e Peri?dico 0x2)
	TIMER0_TAMR_R = 0x02;           //Habilitando 0A em peri?dico
	TIMER2_TAMR_R = 0x02;           //Habilitando 2A em peri?dico
		
	// 5. Valor que o timer come?a a vai diminuindo (Se 32 bits usa s? o TAILR, se timer B como 16 bits usa TBILR tamb?m)
	TIMER0_TAILR_R = 16000000;      // 200ms
	TIMER2_TAILR_R = 40000000;		  // 500ms 
		
	// 6. Registrador de preescala, s? usar se for 16 bits, 32 deixa zerado. A (TAPR) e B (TBPR)
	TIMER0_TAPR_R = 0x00;
	TIMER2_TAPR_R = 0x00;
		
	// 7. Limpa a interrup??o (ACK) (escreve 1 onde quer limpar!!)
	TIMER0_ICR_R = 0x01;            //Limpa pro timer 0A
	TIMER2_ICR_R = 0x01;            //Limpa pro timer 2A
		
	// 8a. Controla a interrup??o 
	TIMER0_IMR_R = 0x01;				   //Habilita interrup??o no timer 0A
	TIMER2_IMR_R = 0x01;           //Habilita interrup??o no timer 2A
	
	// 8b. Seta a prioridade da interrup??o
	NVIC_PRI4_R = 4 << 29;         //PRI4 porque tem o numero 19 e o 19 t� no PRI4 e move 29 posicoes pra setar prioridade 4
	NVIC_PRI5_R = 4 << 29;         //PRI5 porque tem o numero 23 e o 23 t� no PRI5 e move 29 posicoes pra setar prioridade 4
	
	// 8c. Habilitar a interrup??o do timer respectivo no respectivo registrador
	NVIC_EN0_R = 1 << 19;          // 0A numero 19
	NVIC_EN0_R |= 1 << 23;         // | pra manter o que coloquei antes, 2A numero 23
	
	// 9. Coloca 1 pra habilitar e come?ar a contagem
	TIMER0_CTL_R = 0x00;           // come�a desligado
	TIMER2_CTL_R = 0x00;           // come�a desligado
	
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

unsigned int seq_direita_meio[8]={
1000,
1100,
0100,
0110,
0010,
0011,
0001,
1001
};

unsigned int seq_esquerda_meio[8]={
1001,
0001,
0011,
0010,
0110,
0100,
1100,
1000
};

unsigned int seq_direita_inteiro[4]={
1000,
0100,
0010,
0001
};

unsigned int seq_esquerda_inteiro[4]={
0001,
0010,
0100,
1000
};


// -------------------------------------------------------------------------------
// Fun��o gira_meio_passo
// Manda girar o motor x passos de meio passo a partir do 0
// Par�metro de entrada: Passos
// Par�metro de sa�da: n�o tem
void gira_meio_passo(int passos)
{
	int passos_restante;
	
	// se o valor que quero ir � mais longe, calculo quanto falta pra ir
	if (passos > pos_atual)
	{
		passos_restante = passos - pos_atual;
		pos_atual = passos;
		gira_meio_passo_direita(passos_restante);
	}
	// se o valor que quero ir � mais perto, calculo quanto tem que voltar
	else 
	{
		passos_restante = pos_atual - passos;
		pos_atual = passos;
		gira_meio_passo_esquerda(passos_restante);
	}
}


// -------------------------------------------------------------------------------
// Fun��o gira_passo_direita
// Gira o motor x angulo sentido hor�rio
// Par�metro de entrada: Angulo
// Par�metro de sa�da: n�o tem
void gira_passo_direita(int angulo)
{
	static int indice = 0;
	int attPos = 0; //controla o tempo para mostrar a atualiza��o parcial dos passos do motor
	int passos;
	passos = calcula_passos(angulo);
	
	for(int i = 0; i < passos; i++)
	{
		pos_atual = (pos_atual - 1 + 4096) % 4096; //atualiza a pos atual do motor conforme o motor gira
		GPIO_PORTH_AHB_DATA_R = seq_direita_inteiro[indice];
		//pra ir at� o 3 e depois voltar pro 0
		indice = (indice + 1) % 4;
		//espera 5ms pra energizar a pr�xima
		SysTick_Wait1ms(5);
		
		attPos = (attPos + 1) % 200; //Quando completa 200 passos, o valor atualiza��o e no total vai ter passado 1 segundo
		
		if(attPos == 199)
		{
			mostra_pos(); //mostra uma parcial do motor
		}
	
	}

}


// -------------------------------------------------------------------------------
// Fun��o gira_passo_esquerda
// Gira o motor x angulo sentido antihor�rio
// Par�metro de entrada: Angulo
// Par�metro de sa�da: n�o tem
void gira_passo_esquerda(int angulo)
{
	static int indice = 0;
	int attPos = 0; //controla o tempo para mostrar a atualiza��o parcial dos passos do motor
	
	int passos;
	passos = calcula_passos(angulo);
	for(int i = 0; i < passos; i++)
	{
		pos_atual = (pos_atual + 1) % 4096; //atualiza a pos atual do motor conforme o motor gira
		GPIO_PORTH_AHB_DATA_R = seq_esquerda_inteiro[indice];
		//pra ir at� o 3 e depois voltar pro 0
		indice = (indice + 1) % 4;
		//espera 5ms pra energizar a pr�xima
		SysTick_Wait1ms(5);

		attPos = (attPos + 1) % 200; //Quando completa 200 passos, o valor atualiza��o e no total vai ter passado 1 segundo


		if(attPos == 199)
		{
			mostra_pos(); //mostra uma parcial do motor
		}
		
	}
}


// -------------------------------------------------------------------------------
// Fun��o gira_meio_passo_direita
// Gira o motor x passos sentido hor�rio
// Par�metro de entrada: Passos
// Par�metro de sa�da: n�o tem
void gira_meio_passo_direita(int passos)
{
	  static int indice = 0;
		for(int i = 0; i < passos; i++)
		{
			GPIO_PORTH_AHB_DATA_R = seq_direita_meio[indice];
			//pra ir at� o 7 e depois voltar pro 0
			indice = (indice + 1) % 8;
			//espera 5ms pra energizar a pr�xima
			SysTick_Wait1ms(5);
		}
}


// -------------------------------------------------------------------------------
// Fun��o gira_meio_passo_esquerda
// Gira o motor x passos sentido antihor�rio
// Par�metro de entrada: Passos
// Par�metro de sa�da: n�o tem
void gira_meio_passo_esquerda(int passos)
{
	  static int indice = 0;
		for(int i = 0; i < passos; i++)
		{
			GPIO_PORTH_AHB_DATA_R = seq_esquerda_meio[indice];
			//pra ir at� o 7 e depois voltar pro 0
			indice = (indice + 1) % 8;
			//espera 5ms pra energizar a pr�xima
			SysTick_Wait1ms(5);
		}
}


// -------------------------------------------------------------------------------
// Funcao Iimer0A_Handler
// Lida com a interrupcao do timer 0A (do PWM)
// Parametro de entrada: Nao tem
// Parametro de saida: Nao tem
void Timer0A_Handler () 
{
	//limpando o ack da interrup??o
	TIMER0_ICR_R = 0x01;
	
	// leio o potenci�metro
	leitura_conversor = converter_ad();
	// gira o tanto que eu li
	gira_meio_passo(leitura_conversor); 
}

int calcula_passos (int angulo)
{
	return ((angulo * 4096) / 360);
}