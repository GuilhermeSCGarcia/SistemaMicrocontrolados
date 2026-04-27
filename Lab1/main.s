; main.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; Ver 1 19/03/2018
; Ver 2 26/08/2018
; Este programa deve esperar o usuário pressionar uma chave.
; Caso o usuário pressione uma chave, um LED deve piscar a cada 1 segundo.

; -------------------------------------------------------------------------------
        THUMB                        ; Instruções do tipo Thumb-2
; -------------------------------------------------------------------------------
		
; Declarações EQU - Defines
;<NOME>         EQU <VALOR>
; ========================
; Definições de Valores
QUANT_CICLOS			EQU	   8
NIVEL_ATUAL				EQU    10
NIVEL_DESEJADO			EQU    50

; -------------------------------------------------------------------------------
; Área de Dados - Declarações de variáveis
		AREA  DATA, ALIGN=2
		; Se alguma variável for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a variável <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma variável de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posição da RAM		

; -------------------------------------------------------------------------------
; Área de Código - Tudo abaixo da diretiva a seguir será armazenado na memória de 
;                  código
        AREA    |.text|, CODE, READONLY, ALIGN=2

		; Se alguma função do arquivo for chamada em outro arquivo	
        EXPORT Start                ; Permite chamar a função Start a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma função externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; função <func>
		IMPORT  PLL_Init
		IMPORT  SysTick_Init
		IMPORT  SysTick_Wait1ms			
		IMPORT  GPIO_Init
        IMPORT  PortN_Output
        IMPORT  PortJ_Input	
		IMPORT  Escreve_Display


; -------------------------------------------------------------------------------
; Função main()
Start  		
	BL PLL_Init                  ;Chama a subrotina para alterar o clock do microcontrolador para 80MHz
	BL SysTick_Init              ;Chama a subrotina para inicializar o SysTick
	BL GPIO_Init                 ;Chama a subrotina que inicializa os GPIO
	MOV R10, #QUANT_CICLOS        ; R10 para ser a ITERADOR de ciclo (conta o atraso)
	MOV R9, #NIVEL_ATUAL          ; R9 para ser o valor do nível atual
	MOV R8, #NIVEL_DESEJADO       ; R8 para ser o valor do nível desejado
	
	BL Escreve_Display            ; o R10 deve vir com o valor a ser exibido no display; TODO
	SUB R10, R10, #1              ;decrementa o R10

MainLoop
	CMP R9, R8 
	IT LO  ; R9 < R8?
	ADDLO R9, R9, #1                ;aumenta pra mais perto do valor desejado
	
	CMP R9, R8
	IT GT  ; R9 > R8?
	SUBGT R9, R9, #1                ;diminui pra mais perto do valor desejado
	
	CMP R9, R8
	
	IT LO                             ;R9 < R8
	MOVLO R0, #2_00000001             ;quero acender o PN0
	IT GT                             ;R9 > R8
	MOVGT R0, #2_00000010             ;quero acender o PN1
	IT EQ                             ;R9 = R8
	MOVEQ R0, #2_00000011             ;quero acender o PN0 e PN1
	
	BL PortN_Output                   ;função que acende o(s) led(s)
	
DECREMENTA_CONTADOR
	MOV R0, #10                       ;Chamar a rotina para esperar 10ms
	BL SysTick_Wait1ms
	SUB R10, R10, #1
	CMP R10, #1
	BNE DECREMENTA_CONTADOR
	MOV R0, R9                         ;move o nível atual pro R0 pra mostrar no display
	BL Escreve_Display
	MOV R10, #QUANT_CICLOS

	B MainLoop 
	

; -------------------------------------------------------------------------------------------------------------------------
; Fim do Arquivo
; -------------------------------------------------------------------------------------------------------------------------	
    ALIGN                        ;Garante que o fim da seção está alinhada 
    END                          ;Fim do arquivo
