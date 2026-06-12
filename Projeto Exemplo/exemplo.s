; Exemplo.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 12/03/2018

; -------------------------------------------------------------------------------
        THUMB                        ; Instruções do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declarações EQU - Defines
;<NOME>         EQU <VALOR>
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

; -------------------------------------------------------------------------------
; Função main()
Start  
; Comece o código aqui <======================================================

;EX1
;	MOV R0, #65
;	
;	MOV R1, #0x1B001B00
;	
;	MOV R2, #0x5678
;	MOVT R2, #0x1234
;	
;	MOV R3, #0x0040
;	MOVT R3, #0x2000
;	STR R0, [R3]
;	
;	STR R1, [R3, #4]
;	
;	STR R2, [R3, #8]
;	
;	MOV R4, #0x0001
;	MOVT R4, #0X000F
;	STR R4, [R3, #12]
;	
;	MOV R5, #205
;	STRB R5, [R3, #6]

;	LDR R7, [R3]
;	
;	LDR R8, [R3, #8]
;	
;	MOV R9, R7

;EX2
;	MOV R0, #2_11110000
;	MOV R1, #2_01010101
;	ANDS R0, R1
;	
;	MOV R0, #2_11001100
;	MOV R1, #2_00110011
;	ANDS R1, R0, R1
;	
;	MOV R0, #2_10000000
;	MOV R1, #2_00110111
;	ORRS R2, R0, R1
;	
;	MOV R0, #0xABCD
;	MOVT R0, #0xABCD
;	MOV R1, #0xFFFF
;	BICS R3, R0, R1

;EX3
;	MOV R0, #701
;	MOV R1, #5
;	LSRS R0, R1
;	
;	MOV R0, #32067
;	NEG R0, R0
;	MOV R1, #4
;	LSRS R0, R1
;	
;	MOV R0, #701
;	MOV R1, #3
;	ASRS R0, R1
;	
;	MOV R0, #32067
;	NEG R0, R0
;	MOV R1, #5
;	ASRS R0, R1
;	
;	MOV R0, #255
;	MOV R1, #8
;	LSLS R0, R1
;	
;	MOV R0, #58982
;	NEG R0, R0
;	MOV R1, #18
;	LSLS R0, R1
;	
;	MOV R0, #0x1234
;	MOVT R0, #0xFABC
;	MOV R1, #10
;	RORS R0, R1
;	
;	MOV R0, #0x4321
;	RRXS R0
;	RRXS R0
;	
;	NOP

;EX4
;	
;	MOV R0, #101
;	MOV R1, #253
;	ADDS R0, R1
;	
;	MOV R0, #1500
;	MOV R1, #40543
;	ADD R0, R1
;	
;	MOV R0, #340
;	MOV R1, #123
;	SUBS R1, R0
;	
;	MOV R0, #1000
;	MOV R1, #2000
;	SUBS R0, R1
;	
;	MOV R0, #54378
;	MOV R1, #4
;	MUL R0, R1
;	
;	MOV R0, #0x3344
;	MOVT R0, #0x1122
;	MOV R1, #0x2211
;	MOVT R1, #0x4433
;	UMULL R4, R3, R0, R1
;	
;	MOV R0, #0x7560
;	MOVT R0, #0xFFFF
;	MOV R1, #1000
;	UDIV R0, R1
;	
;	MOV R0, #0x7560
;	MOVT R0, #0xFFFF
;	SDIV R0, R1

;EX5

;	MOV R0, #10
;	CMP R0, #9
;	ITTE HS
;	MOVHS R1, #50
;	ADDHS R2, R1, #32
;	MOVLO R3, #75
;	
;	CMP R0, #11
;	ITTE HS
;	MOVHS R1, #50
;	ADDHS R2, R1, #32
;	MOVLO R3, #75

;EX6

;	MOV R0, #10
;	MOV R1, #0xCC22
;	MOVT R1, #0xFF11
;    MOV R2, #1234
;	MOV R3, #0x300
;	PUSH {R0}
;	PUSH {R1}
;	PUSH {R2}
;	PUSH {R3}
;	MOV R1, #60
;	MOV R2, #0x1234
;	POP {R3}
;	POP {R2}
;	POP {R1}
;	POP {R0}
;	
;	
;    PUSH {R3, R2, R1, R0}

;EX7
;	MOV R0, #10
;	ADD R0, R0, #5
;	
;	BL loop
;loop CMP R0, #50
;	ITE NE
;	ADDSNE R0, R0, #5
;	BLEQ func
;	BX LR
;	
;func MOV R1, R0
;	CMP R1, #50
;	ITE LO
;	ADDSLO R1, R1, #5
;	MOVHS R1, #-50
;	BX LR

;EX8



	
	NOP

    ALIGN                           ; garante que o fim da seção está alinhada 
    END                             ; fim do arquivo
