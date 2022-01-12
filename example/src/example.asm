.include	"nes.inc"

.define		SPRITE_SPEED_X	6
.define		SPRITE_SPEED_Y	8
.define		BORDER_MIN		32
.define		BORDER_MAX		224

.segment "HEADER"
	.byte   'N', 'E', 'S', $1A
	.byte   $02, $01, $00, $00
	.byte   $00, $00, $00, $00
	.byte   $00, $00, $00, $00

.segment "VECTORS"
	.word nmi
	.word reset
	.word $0000

.segment "CHARS"
	.repeat 8
		.byte $FF
	.endrepeat

.segment "OAM"
oam:		.res 256

.segment "ZEROPAGE"
pos:		.res 2
old_pos:	.res 2
vector:		.res 2
nmi_lock:	.res 1

.segment "RODATA"
palette:	.byte $0F, $11, $21, $31

.segment "CODE"
.proc	reset
	sei
	lda #$40
	sta APU_PAD2
	lda #0
	sta PPU_CTRL1
	sta PPU_CTRL2
	sta APU_CHANCTRL
	sta APU_MODCTRL
	cld
	ldx #$FF
	txs

	bit PPU_STATUS
	:
	bit PPU_STATUS
	bpl :-

	ldx #0
	:
	.repeat 8, index
		sta index * $100, x
	.endrepeat
	inx
	bne :-

	lda #$FF
	.repeat 64, index
		sta oam + index * 4
	.endrepeat

	lda #$3f
	sta	PPU_VRAM_ADDR2
	lda #$10
	sta	PPU_VRAM_ADDR2
	.repeat 4, index
		lda palette + index
		sta PPU_VRAM_IO
	.endrepeat

	lda #128
	sta pos + 0
	sta pos + 1
	lda #SPRITE_SPEED_Y
	sta vector + 0
	lda #SPRITE_SPEED_X
	sta vector + 1

	lda #%10000000
	sta PPU_CTRL1
	lda #%00010000
	sta PPU_CTRL2

	:
	jmp :-
.endproc

.proc	nmi
	pha
	txa
	pha
	tya
	pha
	lda nmi_lock
	bne nmi_end
	inc nmi_lock
	jsr update_oam
	dec nmi_lock
	nmi_end:
	pla
	tay
	pla
	tax
	pla
	rti
.endproc

.proc	update_oam
	jsr move_pos
	jsr check_collision
	lda pos + 0
	sta oam + 0
	lda pos + 1
	sta oam + 3
	lda #.hibyte(oam)
	sta APU_SPR_DMA
	rts
.endproc

.proc	move_pos
	.repeat 2, index
		lda pos + index
		sta old_pos + index
		clc
		adc vector + index
		sta pos + index
	.endrepeat
	rts
.endproc

.proc	check_collision
	.repeat 2, index
		.scope
			lda vector + index
			bpl move_plus

			lda #BORDER_MIN
			cmp pos + index
			bmi end_check
			cmp old_pos + index
			bpl end_check
			.if(index = 0)
				lda #SPRITE_SPEED_Y
			.else
				lda #SPRITE_SPEED_X
			.endif
			sta vector + index
			jmp end_check

			move_plus:
			lda #BORDER_MAX
			cmp pos + index
			beq turn
			bpl end_check
			turn:
			cmp old_pos + index
			bmi end_check
			.if(index = 0)
				lda #.lobyte(SPRITE_SPEED_Y * -1)
			.else
				lda #.lobyte(SPRITE_SPEED_X * -1)
			.endif
			sta vector + index

			end_check:
		.endscope
	.endrepeat
	rts
.endproc
