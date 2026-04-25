; gpio.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; Ver 1 19/03/2018
; Ver 2 26/08/2018

; -------------------------------------------------------------------------------
        THUMB                        ; Instruções do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declarações EQU - Defines
; ========================
; ========================
; Definições dos Registradores Gerais
SYSCTL_RCGCGPIO_R	 EQU	0x400FE608
SYSCTL_PRGPIO_R		 EQU    0x400FEA08
; ========================
; Definições dos Ports
; PORT J
GPIO_PORTJ_AHB_LOCK_R    	EQU    0x40060520
GPIO_PORTJ_AHB_CR_R      	EQU    0x40060524
GPIO_PORTJ_AHB_AMSEL_R   	EQU    0x40060528
GPIO_PORTJ_AHB_PCTL_R    	EQU    0x4006052C
GPIO_PORTJ_AHB_DIR_R     	EQU    0x40060400
GPIO_PORTJ_AHB_AFSEL_R   	EQU    0x40060420
GPIO_PORTJ_AHB_DEN_R     	EQU    0x4006051C
GPIO_PORTJ_AHB_PUR_R     	EQU    0x40060510	
GPIO_PORTJ_AHB_DATA_R    	EQU    0x400603FC
GPIO_PORTJ               	EQU    2_000000100000000

GPIO_PORTJ_AHB_IM_R         EQU    0x40060410
GPIO_PORTJ_AHB_IS_R         EQU    0x40060404
GPIO_PORTJ_AHB_IBE_R        EQU    0x40060408
GPIO_PORTJ_AHB_IEV_R 		EQU    0x4006040C
GPIO_PORTJ_AHB_ICR_R        EQU    0x4006041C
GPIO_PORTJ_AHB_MIS_R        EQU    0x40060418
	
; PORT N
GPIO_PORTN_AHB_LOCK_R    	EQU    0x40064520
GPIO_PORTN_AHB_CR_R      	EQU    0x40064524
GPIO_PORTN_AHB_AMSEL_R   	EQU    0x40064528
GPIO_PORTN_AHB_PCTL_R    	EQU    0x4006452C
GPIO_PORTN_AHB_DIR_R     	EQU    0x40064400
GPIO_PORTN_AHB_AFSEL_R   	EQU    0x40064420
GPIO_PORTN_AHB_DEN_R     	EQU    0x4006451C
GPIO_PORTN_AHB_PUR_R     	EQU    0x40064510	
GPIO_PORTN_AHB_DATA_R    	EQU    0x400643FC
GPIO_PORTN               	EQU    2_001000000000000	
	
; PORT A
GPIO_PORTA_AHB_LOCK_R       EQU    0x40058520
GPIO_PORTA_AHB_CR_R         EQU    0x40058524
GPIO_PORTA_AHB_AMSEL_R      EQU    0x40058528
GPIO_PORTA_AHB_PCTL_R       EQU    0x4005852C
GPIO_PORTA_AHB_DIR_R        EQU    0x40058400
GPIO_PORTA_AHB_AFSEL_R      EQU    0x40058420
GPIO_PORTA_AHB_DEN_R        EQU    0x4005851C
GPIO_PORTA_AHB_PUR_R        EQU    0x40058510
GPIO_PORTA_AHB_DATA_R       EQU    0x400583FC
GPIO_PORTA_AHB_DATA_BITS_R  EQU    0x40058000
GPIO_PORTA              EQU    2_000000000000001

; PORT B
GPIO_PORTB_AHB_LOCK_R       EQU    0x40059520
GPIO_PORTB_AHB_CR_R         EQU    0x40059524
GPIO_PORTB_AHB_AMSEL_R      EQU    0x40059528
GPIO_PORTB_AHB_PCTL_R       EQU    0x4005952C
GPIO_PORTB_AHB_DIR_R        EQU    0x40059400
GPIO_PORTB_AHB_AFSEL_R      EQU    0x40059420
GPIO_PORTB_AHB_DEN_R        EQU    0x4005951C
GPIO_PORTB_AHB_PUR_R        EQU    0x40059510
GPIO_PORTB_AHB_DATA_R       EQU    0x400593FC
GPIO_PORTB_AHB_DATA_BITS_R  EQU    0x40059000
GPIO_PORTB              EQU    2_000000000000010

; PORT P
GPIO_PORTP_AHB_LOCK_R       EQU    0x40065520
GPIO_PORTP_AHB_CR_R         EQU    0x40065524
GPIO_PORTP_AHB_AMSEL_R      EQU    0x40065528
GPIO_PORTP_AHB_PCTL_R       EQU    0x4006552C
GPIO_PORTP_AHB_DIR_R        EQU    0x40065400
GPIO_PORTP_AHB_AFSEL_R      EQU    0x40065420
GPIO_PORTP_AHB_DEN_R        EQU    0x4006551C
GPIO_PORTP_AHB_PUR_R        EQU    0x40065510
GPIO_PORTP_AHB_DATA_R       EQU    0x400653FC
GPIO_PORTP_AHB_DATA_BITS_R  EQU    0x40065000
GPIO_PORTP              EQU    2_010000000000000

; PORT Q
GPIO_PORTQ_AHB_LOCK_R       EQU    0x40066520
GPIO_PORTQ_AHB_CR_R         EQU    0x40066524
GPIO_PORTQ_AHB_AMSEL_R      EQU    0x40066528
GPIO_PORTQ_AHB_PCTL_R       EQU    0x4006652C
GPIO_PORTQ_AHB_DIR_R        EQU    0x40066400
GPIO_PORTQ_AHB_AFSEL_R      EQU    0x40066420
GPIO_PORTQ_AHB_DEN_R        EQU    0x4006651C
GPIO_PORTQ_AHB_PUR_R        EQU    0x40066510
GPIO_PORTQ_AHB_DATA_R       EQU    0x400663FC
GPIO_PORTQ_AHB_DATA_BITS_R  EQU    0x40066000
GPIO_PORTQ              EQU    2_100000000000000
	
NVIC_EN1_R 					EQU    0xE000E104
NVIC_PRI12_R				EQU	   0xE000E430
	

; -------------------------------------------------------------------------------
; Área de Código - Tudo abaixo da diretiva a seguir será armazenado na memória de 
;                  código
        AREA    |.text|, CODE, READONLY, ALIGN=2
		
tabela
		DCB     0x3F    ; 0
        DCB     0x06    ; 1
        DCB     0x5B    ; 2
        DCB     0x4F    ; 3
        DCB     0x66    ; 4
        DCB     0x6D    ; 5
        DCB     0x7D    ; 6
        DCB     0x07    ; 7
        DCB     0x7F    ; 8
        DCB     0x6F    ; 9

		; Se alguma função do arquivo for chamada em outro arquivo	
        EXPORT GPIO_Init            ; Permite chamar GPIO_Init de outro arquivo
		EXPORT PortN_Output			; Permite chamar PortN_Output de outro arquivo
		EXPORT PortJ_Input          ; Permite chamar PortJ_Input de outro arquivo
		
		EXPORT GPIOPortJ_Handler	
		EXPORT Escreve_Display
		IMPORT  SysTick_Wait1ms

;--------------------------------------------------------------------------------
; Função GPIO_Init
; Parâmetro de entrada: Não tem
; Parâmetro de saída: Não tem
GPIO_Init
;=====================
; ****************************************
; Escrever função de inicialização dos GPIO
; Inicializar as portas A, B, J, N, P e Q
; ****************************************

            LDR     R0, =SYSCTL_RCGCGPIO_R  		;Carrega o endereço do registrador RCGCGPIO
			MOV		R1, #2_111000100100011			;Setando os bits das portas A, B, J, N, P e Q
            STR     R1, [R0]						;Move para a memória os bits das portas no endereço do RCGCGPIO
 
            LDR     R0, =SYSCTL_PRGPIO_R			;Carrega o endereço do PRGPIO para esperar os GPIO ficarem prontos
EsperaGPIO  LDR     R1, [R0]						;Lê da memória o conteúdo do endereço do registrador
			MOV     R2, #GPIO_PORTN                 ;Seta os bits correspondentes às portas para fazer a comparação
			ORR     R2, #GPIO_PORTJ                 ;Seta o bit da porta J, fazendo com OR
			ORR		R2, #GPIO_PORTA                 ;Seta o bit da porta A, fazendo com OR
			ORR 	R2, #GPIO_PORTB					;Seta o bit da porta B, fazendo com OR
			ORR 	R2, #GPIO_PORTP					;Seta o bit da porta P, fazendo com OR
			ORR     R2, #GPIO_PORTQ					;Seta o bit da porta Q, fazendo com OR
            TST     R1, R2							;ANDS de R1 com R2
            BEQ     EsperaGPIO					    ;Se o flag Z=1, volta para o laço. Senão continua executando
 
; 2. Limpar o AMSEL para desabilitar a analógica
            MOV     R1, #0x00000000					;Colocar 0 no registrador para desabilitar a função analógica
            LDR     R0, =GPIO_PORTJ_AHB_AMSEL_R     ;Carrega o R0 com o endereço do AMSEL para a porta J
            STR     R1, [R0]						;Guarda no registrador AMSEL da porta J da memória
            LDR     R0, =GPIO_PORTN_AHB_AMSEL_R		;Carrega o R0 com o endereço do AMSEL para a porta N
            STR     R1, [R0]					    ;Guarda no registrador AMSEL da porta N da memória
			
			LDR 	R0, =GPIO_PORTA_AHB_AMSEL_R     ;Carrega o R0 com o endereço do AMSEL para a porta A
			STR		R1, [R0]						;Guarda no registrador AMSEL da porta A da memória
			LDR 	R0, =GPIO_PORTB_AHB_AMSEL_R		;Carrega o R0 com o endereço do AMSEL para a porta B
			STR		R1, [R0]						;Guarda no registrador AMSEL da porta B da memória
			
			LDR 	R0, =GPIO_PORTP_AHB_AMSEL_R     ;Carrega o R0 com o endereço do AMSEL para a porta P
			STR		R1, [R0]						;Guarda no registrador AMSEL da porta P da memória
			LDR 	R0, =GPIO_PORTQ_AHB_AMSEL_R		;Carrega o R0 com o endereço do AMSEL para a porta Q
			STR		R1, [R0]						;Guarda no registrador AMSEL da porta Q da memória
			
; 3. Limpar PCTL para selecionar o GPIO
            MOV     R1, #0x00					    ;Colocar 0 no registrador para selecionar o modo GPIO
            LDR     R0, =GPIO_PORTJ_AHB_PCTL_R		;Carrega o R0 com o endereço do PCTL para a porta J
            STR     R1, [R0]                        ;Guarda no registrador PCTL da porta J da memória
            LDR     R0, =GPIO_PORTN_AHB_PCTL_R      ;Carrega o R0 com o endereço do PCTL para a porta N
            STR     R1, [R0]                        ;Guarda no registrador PCTL da porta N da memória
			
			MOV 	R1, #0x00000000
			LDR 	R0, =GPIO_PORTA_AHB_PCTL_R      ;Carrega o R0 com o endereço do PCTL para a porta A
			STR		R1, [R0]						;Guarda no registrador PCTL da porta A da memória
			MOV 	R1, #0x000000
			LDR 	R0, =GPIO_PORTB_AHB_PCTL_R		;Carrega o R0 com o endereço do PCTL para a porta B
			STR		R1, [R0]						;Guarda no registrador AMSEL da porta B da memória
			
			LDR 	R0, =GPIO_PORTP_AHB_PCTL_R      ;Carrega o R0 com o endereço do PCTL para a porta P
			STR		R1, [R0]						;Guarda no registrador PCTL da porta P da memória
			MOV 	R1, #0x00000
			LDR 	R0, =GPIO_PORTQ_AHB_PCTL_R		;Carrega o R0 com o endereço do PCTL para a porta Q
			STR		R1, [R0]						;Guarda no registrador PCTL da porta Q da memória
			
			
; 4. DIR para 0 se for entrada, 1 se for saída
            LDR     R0, =GPIO_PORTJ_AHB_DIR_R		;Carrega o R0 com o endereço do DIR para a porta J
			MOV     R1, #2_00					    ;Colocar 0 no registrador DIR para funcionar com saída
            STR     R1, [R0]						;Guarda no registrador
			
            LDR     R0, =GPIO_PORTN_AHB_DIR_R		;Carrega o R0 com o endereço do DIR para a porta N
            MOV     R1, #2_000011               	;Colocar 0 no registrador DIR para funcionar com saída
            STR     R1, [R0]						;Guarda no registrador PCTL da porta N da memória
			
			LDR     R0, =GPIO_PORTA_AHB_DIR_R		;Carrega o R0 com o endereço do DIR para a porta A
            MOV     R1, #2_11110000            		;Colocar 0 no registrador DIR para funcionar com saída
            STR     R1, [R0]						;Guarda no registrador PCTL da porta A da memória
			
            LDR     R0, =GPIO_PORTB_AHB_DIR_R		;Carrega o R0 com o endereço do DIR para a porta B
            MOV     R1, #2_110000              		;Colocar 0 no registrador DIR para funcionar com saída
            STR     R1, [R0]						;Guarda no registrador PCTL da porta B da memória
			
            LDR     R0, =GPIO_PORTP_AHB_DIR_R		;Carrega o R0 com o endereço do DIR para a porta P
            MOV     R1, #2_100000              		;Colocar 0 no registrador DIR para funcionar com saída
            STR     R1, [R0]						;Guarda no registrador PCTL da porta P da memória
			
	        LDR     R0, =GPIO_PORTQ_AHB_DIR_R		;Carrega o R0 com o endereço do DIR para a porta Q
            MOV     R1, #2_01111               		;Colocar 0 no registrador DIR para funcionar com saída
            STR     R1, [R0]						;Guarda no registrador PCTL da porta Q da memória
			
; 5. Limpar os bits AFSEL para 0 para selecionar GPIO 
;    Sem função alternativa
            MOV     R1, #0x00						;Colocar o valor 0 para não setar função alternativa
            LDR     R0, =GPIO_PORTN_AHB_AFSEL_R		;Carrega o endereço do AFSEL da porta N
            STR     R1, [R0]						;Escreve na porta
            LDR     R0, =GPIO_PORTJ_AHB_AFSEL_R     ;Carrega o endereço do AFSEL da porta J
            STR     R1, [R0]                        ;Escreve na porta
			LDR     R0, =GPIO_PORTA_AHB_AFSEL_R     ;Carrega o endereço do AFSEL da porta A
            STR     R1, [R0]                        ;Escreve na porta
			MOV 	R1, #2_000000
            LDR     R0, =GPIO_PORTB_AHB_AFSEL_R     ;Carrega o endereço do AFSEL da porta B
            STR     R1, [R0]                        ;Escreve na porta
            LDR     R0, =GPIO_PORTP_AHB_AFSEL_R     ;Carrega o endereço do AFSEL da porta P
            STR     R1, [R0]                        ;Escreve na porta
			MOV		R1, #2_00000
			LDR     R0, =GPIO_PORTQ_AHB_AFSEL_R     ;Carrega o endereço do AFSEL da porta Q
            STR     R1, [R0]                        ;Escreve na porta
			
; 6. Setar os bits de DEN para habilitar I/O digital
            LDR     R0, =GPIO_PORTN_AHB_DEN_R			;Carrega o endereço do DEN
            MOV     R1, #2_000011                       ;Ativa os pinos N0 e N1 como I/O Digital
            STR     R1, [R0]							;Escreve no registrador da memória funcionalidade digital 
 
            LDR     R0, =GPIO_PORTJ_AHB_DEN_R			;Carrega o endereço do DEN
			MOV     R1, #2_00000000                     ;Ativa os pinos PJ0 e PJ1 como I/O Digital      
            STR     R1, [R0]                            ;Escreve no registrador da memória funcionalidade digital
			
			LDR     R0, =GPIO_PORTA_AHB_DEN_R			;Carrega o endereço do DEN
			MOV     R1, #2_11110000                     ;Ativa os pinos A4, A5, A6 e A7 como I/O Digital      
            STR     R1, [R0]                            ;Escreve no registrador da memória funcionalidade digital
			
		    LDR     R0, =GPIO_PORTB_AHB_DEN_R			;Carrega o endereço do DEN
			MOV     R1, #2_110000                       ;Ativa os pinos B4 e B5 como I/O Digital      
            STR     R1, [R0]                            ;Escreve no registrador da memória funcionalidade digital
			
            LDR     R0, =GPIO_PORTP_AHB_DEN_R			;Carrega o endereço do DEN
			MOV     R1, #2_100000                       ;Ativa os pinos P5 como I/O Digital      
            STR     R1, [R0]                            ;Escreve no registrador da memória funcionalidade digital
			
            LDR     R0, =GPIO_PORTQ_AHB_DEN_R			;Carrega o endereço do DEN
			MOV     R1, #2_01111                        ;Ativa os pinos Q0, Q1, Q2 e Q3 como I/O Digital      
            STR     R1, [R0]                            ;Escreve no registrador da memória funcionalidade digital
			
; 7. Para habilitar resistor de pull-up interno, setar PUR para 1
			LDR     R0, =GPIO_PORTJ_AHB_PUR_R			;Carrega o endereço do PUR para a porta J
			MOV     R1, #2_00000011						;Habilitar funcionalidade digital de resistor de pull-up 
                                                        ;nos bits 0 e 1
            STR     R1, [R0]							;Escreve no registrador da memória do resistor de pull-up
			
; coisas das interrupções
; 8. Não sabemos
            LDR     R0, =GPIO_PORTJ_AHB_IM_R
			MOV		R1, #0
			STR 	R1, [R0]
			
; 9. Alguma coisa pra borda
            LDR     R0, =GPIO_PORTJ_AHB_IS_R
			MOV		R1, #0
			STR 	R1, [R0]
			
; 10. Alguma coisa pra borda
            LDR     R0, =GPIO_PORTJ_AHB_IBE_R
			MOV		R1, #0
			STR 	R1, [R0]
			
; 11. Alguma coisa pra borda
            LDR     R0, =GPIO_PORTJ_AHB_IEV_R
			MOV		R1, #0
			STR 	R1, [R0]

; 12. Não sabemos
            LDR     R0, =GPIO_PORTJ_AHB_ICR_R
			MOV		R1, #2_11
			STR 	R1, [R0]
			
; 13. Não sabemos
            LDR     R0, =GPIO_PORTJ_AHB_IM_R
			MOV		R1, #2_11
			STR 	R1, [R0]
			
; 14. Não sabemos
            LDR     R0, =NVIC_EN1_R
			MOV		R1, #1
			LSL     R1, R1, #19
			STR 	R1, [R0]
			
; 15. Não sabemos
            LDR     R0, =NVIC_PRI12_R
			MOV		R1, #5
			LSL		R1, R1, #29
			STR 	R1, [R0]

	BX LR
;--------------------------------------------------------------------------------
;Função de acender o display
Escreve_Display
	PUSH {LR}
;	MOV R1, R10
;	MOV R2, R9
;	MOV R3, R8
;	BX LR
	
	LDR R4, =GPIO_PORTP_AHB_DATA_R     ;desliga o port PP5 dos leds
	MOV R5, #2_000000
	STR R5, [R4]
	
	;R0 -- VALOR TODO DO DISPLAY -- NÍVEL ATUAL
	MOV R2, #10
	UDIV R1, R0, R2
	MUL R2, R1, R2
	SUB R2, R0, R2
	;R1 - DEZENA
	;R2 - UNIDADE
	;=============== BLOCO DA DEZENA ============================================
	LDR R3, =tabela                    ;pego o endereço da tabela
	LDR	R4, =GPIO_PORTA_AHB_DATA_R     ;pego o endereço do port A
	LDRB R5, [R3,R1]                   ;coloco no R2 o valor total pra acender 1, COM A VARIÁVEL R1
	AND  R7, R5, #0xF0                 ; estava 0x0F ;sobra no R8 só o últimos 4 bits
	STR R7, [R4]                       ;escreve os 4 bits no port A
	
	LDR R4, =GPIO_PORTQ_AHB_DATA_R     ;pego o endereço do port Q
	AND R7, R5, #0x0F                  ; estava 0xF0;sobra no R7 só os primeiros 4 bits
	;LSR R7, R7, #4                     ;desloco pra ficarem no final, COM A VARIÁVEL R1
	STR R7, [R4]                       ;escreve os 4 bits no port Q
	
	LDR R4, =GPIO_PORTB_AHB_DATA_R     ;pego o endereço do port B
	MOV R5, #2_010000                  
	STR R5, [R4]					   ; ativa o B4
	
	MOV R4, R0
	MOV R0, #1                         ;Chamar a rotina para esperar 1ms, COM O TRANSISTOR ATIVO
	BL SysTick_Wait1ms
	MOV R0, R4 						   ; guardando todo o valor 
	
	LDR R4, =GPIO_PORTB_AHB_DATA_R     ;pego o endereço do port B
	MOV R5, #2_000000                  
	STR R5, [R4]					   ;desligo o transistor
	
	MOV R4, R0
	MOV R0, #1                         ;Chamar a rotina para esperar 1ms, COM O TRANSISTOR DESLIGADO
	BL SysTick_Wait1ms
	MOV R0, R4 
	
	;=============== BLOCO DA UNIDADE ============================================
	LDR R3, =tabela                    ;pego o endereço da tabela
	LDR	R4, =GPIO_PORTA_AHB_DATA_R     ;pego o endereço do port A
	LDRB R5, [R3,R2]                   ;coloco no R2 o valor total pra acender 1, COM A VARIÁVEL R1
	AND  R7, R5, #0xF0                 ; estava 0x0F ;sobra no R7 só o últimos 4 bits
	STR R7, [R4]                       ;escreve os 4 bits no port A
	
	LDR R4, =GPIO_PORTQ_AHB_DATA_R     ;pego o endereço do port Q
	AND R7, R5, #0x0F                  ; estava 0xF0;sobra no R8 só os primeiros 4 bits
	;LSR R7, R7, #4                     ;desloco pra ficarem no final, COM A VARIÁVEL R1
	STR R7, [R4]                       ;escreve os 4 bits no port Q
	
	LDR R4, =GPIO_PORTB_AHB_DATA_R     ;pego o endereço do port B
	MOV R5, #2_100000                  
	STR R5, [R4]					   ; ativa B5
	
	MOV R4, R0
	MOV R0, #1                         ;Chamar a rotina para esperar 1ms, COM O TRANSISTOR ATIVO
	BL SysTick_Wait1ms
	MOV R0, R4 						   ; guardando todo o valor 
	
	LDR R4, =GPIO_PORTB_AHB_DATA_R     ;pego o endereço do port B
	MOV R5, #2_000000                  ;desligo o transistor
	STR R5, [R4]
	
	MOV R4, R0
	MOV R0, #1                         ;Chamar a rotina para esperar 1ms, COM O TRANSISTOR DESLIGADO
	BL SysTick_Wait1ms
	MOV R0, R4 

	;=============== BLOCO DOS LEDS ============================================
	MOV R0, R8						   ; pego o nível desejado e coloco em R0
	LDR	R4, =GPIO_PORTA_AHB_DATA_R     ;pego o endereço do port A
	AND  R7, R0, #0xF0                 ; estava 0x0F ;sobra no R8 só o últimos 4 bits
	STR R7, [R4]                       ;escreve os 4 bits no port A
	
	LDR R4, =GPIO_PORTQ_AHB_DATA_R     ;pego o endereço do port Q
	AND R7, R0, #0x0F                  ; estava 0xF0;sobra no R8 só os primeiros 4 bits
	STR R7, [R4]                       ;escreve os 4 bits no port Q
	
	LDR R4, =GPIO_PORTP_AHB_DATA_R     ;pego o endereço do port P
	MOV R5, #2_100000                  
	STR R5, [R4]					   ; ativa B5
	
	MOV R4, R0
	MOV R0, #1                         ;Chamar a rotina para esperar 
	BL SysTick_Wait1ms
	MOV R0, R4 						   ; guardando todo o valor 
	
	LDR R4, =GPIO_PORTP_AHB_DATA_R     ;pego o endereço do port B
	MOV R5, #2_000000                  
	STR R5, [R4]					   ;desligo o transistor
	
	MOV R4, R0
	MOV R0, #5                         ;Chamar a rotina para esperar 5ms, COM O TRANSISTOR DESLIGADO
	BL SysTick_Wait1ms
	MOV R0, R4 
	
	POP {LR}
	BX LR

; -------------------------------------------------------------------------------
; Função PortN_Output
; Parâmetro de entrada: 
; Parâmetro de saída: Não tem
PortN_Output
; ****************************************
; Escrever função que acende ou apaga o LED
; ****************************************
	LDR	R1, =GPIO_PORTN_AHB_DATA_R
	LDR R2, [R1]
	BIC R2, #2_00000010
	ORR R0, R0, R2
	STR R0, [R1]
		
	BX LR
; -------------------------------------------------------------------------------
; Função PortJ_Input
; Parâmetro de entrada: Não tem
; Parâmetro de saída: R0 --> o valor da leitura
PortJ_Input
; ****************************************
; Escrever função que lê a chave e retorna 
; um registrador se está ativada ou não
; ****************************************

	LDR	R1, =GPIO_PORTJ_AHB_DATA_R		    ;Carrega o valor do offset do data register
	LDR R0, [R1] 
	BX LR
	
GPIOPortJ_Handler
	LDR R0, =GPIO_PORTJ_AHB_MIS_R 
	LDR R1, [R0] ;vê se alguma chave foi pressionada
	CMP R1, #2_01
	BNE PINOJ1
PINOJO
	LDR R0,=GPIO_PORTJ_AHB_ICR_R ; ACK da interrupção
	MOV R1, #2_01
	STR R1, [R0]
	PUSH{LR}
	BL ACENDELED
	POP{LR}
	B FIMINTERRUPT
PINOJ1
	CMP R1, #2_10
	BNE AMBOSPINOS
	LDR R0,=GPIO_PORTJ_AHB_ICR_R
	MOV R1, #2_10
	STR R1, [R0]
	PUSH{LR}
	BL APAGALED
	POP{LR}
	B FIMINTERRUPT
AMBOSPINOS
	LDR R0,=GPIO_PORTJ_AHB_ICR_R
	MOV R1, #2_11
	STR R1, [R0]
FIMINTERRUPT
	BX LR
		
ACENDELED
	PUSH{LR}
	MOV R0, #2_00000010			 
	BL PortN_Output	
	POP{LR}
	BX LR
	
APAGALED
	PUSH{LR}
	MOV R0, #0                   
	BL PortN_Output	
	POP{LR}
	BX LR
; O QUE FALTA FAZER:
; arrumar a função de interrupção dos botões para aumentar o diminuir o nível desejado
; a função de interrupção vai ter que resetar o iterador do ciclo para QUANT_CICLOS
; e chamar a função de atualizar os displays  -- VAI TER QUE CONFERIR SE O REGISTRADOR R9 NÃO SER PASSADO DURANTE A INTERRUPÇÃO
; ou seja, se o código da interrupção, ele vai ter acesso ao R8 do código principal

; ainda falta a função de acender o led da placa, e chamar no main

    ALIGN                           ; garante que o fim da seção está alinhada 
    END                             ; fim do arquivo