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

VETOR EQU 0x20000400 
MEDIA EQU 0x20000500 
MAIOR EQU 0x20000501 
MENOR EQU 0x20000502 
QTDELEMENTOS EQU 0x20000503 
VARIANCIA EQU 0x20000504 

Start
; Comece o código aqui <======================================================

;vê os valores que não são 0 e coloca eles na pilha
	LDR R0, =VETOR
loop0
	LDRB R1, [R0]
	CMP R1, #0
	IT NE
	PUSHNE {R1}
	ADD R0,#1
	ADD R2, #1
	CMP R2, #20
	BNE loop0
	
;zera as 20 posições da memória RAM
	LDR R0, =VETOR
	MOV R1, #0
	MOV R2, #0
loop1
	STRB R1, [R0]
	ADD R0,#1
	ADD R2, #1
	CMP R2, #20
	BNE loop1
	
;retira da pilha e recoloca na memória RAM
	LDR R0, =VETOR
	LDR R3, =VETOR
loop2
	POP {R4}
	STRB R4, [R0]
	ADD R0,#1
	CMP R13, R3
	BNE loop2
	
;fazendo a média
	LDR R0, =VETOR
loop3
	LDRB R1, [R0]
	CMP R1,#0
	IT EQ
	BEQ sairloop3
	ADD R5, R1
	ADD R10, #1
	ADD R0,#1
	CMP R1, #0
	BNE loop3
	
sairloop3
	UDIV R5, R10
	LDR R1, =MEDIA
	STRB R5, [R1] ; escreve a média na RAM	
	LDR R1, =QTDELEMENTOS
	STRB R10, [R1] ; escreve a quantidade de elementos válido na RAM
	
; achando o maior 
	LDR R0, =VETOR
	LDRB R3, [R0]
	MOV R2, #0
loop4
	LDRB R1, [R0]
	CMP R1, R3
	IT HS
	MOVHS R3, R1
	ADD R2, #1
	ADD R0, #1
	CMP R2, R10
	BNE loop4
	
	LDR R1, =MAIOR
	STRB R3, [R1]

;achando o menor
	LDR R0, =VETOR
	LDRB R3, [R0]
	MOV R2, #0
loop5
	LDRB R1, [R0]
	CMP R1, R3
	IT LO
	MOVLO R3, R1
	ADD R2, #1
	ADD R0, #1
	CMP R2, R10
	BNE loop5
	
	LDR R1, =MENOR
	STRB R3, [R1]
	
;calculando a variância
;PRÉ-loop6
	LDR R6, =QTDELEMENTOS
	LDRB R10, [R6] ; R10 É A QUANTIDADE DE ELEMNTOS VÁLIDOS
	LDR R7, =MEDIA
	LDRB R9, [R7] ; R9 É A MEDIA DOS VALORES
	
	MOV R5,#0 ; R5 É O CONTADOR
	LDR R0, =VETOR
	
LOOP6
	LDRB R1, [R0] ; R1 SÃO OS ELEMENTOS DO VETOR
	ADD R5, #1 ; INCREMENTA O CONTADOR
	SUB R2, R9,R1 ; R2  <== MÉDIA - VALOR_ATUAL
	MUL R3, R2, R2 ; R3 <== R2*R2
	ADD R4,R4,R3 ; R4 <== R3 + R4 /// VAI ACUMULANDO A VARIANCIA PARCIAL
	ADD R0, #1 ; INCREMENTAO ENDEREÇO PARA A PRÓXIMA POSIÇÃO DE MEMÓRIA
	CMP R5,R10 ;<== R5-R10 quando for igual eu não quero continur o loop, pois eu já contei 15 elementos
	BLO LOOP6
	
;calculando a variância
	UDIV R4,R4,R10
	LDR R0, =VARIANCIA
	STRB R4, [R10] ; ESCREVE A VARIÂNCIA NA MEMÓRIA RAM
	
; achando a parte inteira da raiz
; R4 já têm a variância

;pre-loop7
	MOV R0, #1
	
loop7  ;--> ACHANDO A PARTE INTEIRA DA RAIZ
	MUL R1,R0,R0
	ADD R0,#1
	CMP R1,R4
	BLS loop7
	
	;resposta: a parte inteira da raiz é:
	SUB R0,R0,#2 ;/um por causa do ADD depois da multiplicação (para o próximo) e outro porque passsou do valor que eu queria
	

	NOP
    ALIGN                           ; garante que o fim da seção está alinhada 
    END                             ; fim do arquivo
