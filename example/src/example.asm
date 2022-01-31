.include "nes.inc"

.define BORDER_MIN	32
.define BORDER_MAX	224

.segment "HEADER"
	.byte	'N', 'E', 'S', $1A
	.byte	$02, $01, $00, $00
	.byte	$00, $00, $00, $00
	.byte	$00, $00, $00, $00

.segment "VECTORS"
	.word	nmi
	.word	reset
	.word	$0000

.segment "CHARS"
	.repeat 8
		.byte $FF
	.endrepeat

.segment "OAM"
oam:		.res 256

.segment "ZEROPAGE"
pos:		.res 2
old_pos:	.res 2
nmi_lock:	.res 1
vector_addr:	.res 1

.segment "RODATA"
.define M(val) 		.lobyte(val * -1)
.define VADDR(val)	.lobyte(val - vector_dic)

vector_dic:
vector_dic_00:	.byte	6,	0,	VADDR(vector_dic_24),	VADDR(vector_dic_00)
vector_dic_04:	.byte	5,	3,	VADDR(vector_dic_20),	VADDR(vector_dic_44)
vector_dic_08:	.byte	3,	5,	VADDR(vector_dic_16),	VADDR(vector_dic_40)
vector_dic_12:	.byte	0,	6,	VADDR(vector_dic_12),	VADDR(vector_dic_36)
vector_dic_16:	.byte	M(3),	5,	VADDR(vector_dic_08),	VADDR(vector_dic_32)
vector_dic_20:	.byte	M(5),	3,	VADDR(vector_dic_04),	VADDR(vector_dic_28)
vector_dic_24:	.byte	M(6),	0,	VADDR(vector_dic_00),	VADDR(vector_dic_24)
vector_dic_28:	.byte	M(5),	M(3),	VADDR(vector_dic_44),	VADDR(vector_dic_20)
vector_dic_32:	.byte	M(3),	M(5),	VADDR(vector_dic_40),	VADDR(vector_dic_16)
vector_dic_36:	.byte	0,	M(6),	VADDR(vector_dic_36),	VADDR(vector_dic_12)
vector_dic_40:	.byte	3,	M(5),	VADDR(vector_dic_32),	VADDR(vector_dic_08)
vector_dic_44:	.byte	5,	M(3),	VADDR(vector_dic_28),	VADDR(vector_dic_04)

palette:	.byte $0F, $11, $21, $31

.segment "CODE"
.proc reset
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
	sta PPU_VRAM_ADDR2
	lda #$10
	sta PPU_VRAM_ADDR2
	.repeat 4, index
		lda palette + index
		sta PPU_VRAM_IO
	.endrepeat

	lda #128
	sta pos + 0
	sta pos + 1
	lda #0
	sta vector_addr

	lda #%10000000
	sta PPU_CTRL1
	lda #%00010000
	sta PPU_CTRL2

	:
	jsr control
	jmp :-
.endproc

.proc control
	not_pushed:
	jsr read_joypad_a_key
	bcc not_pushed

	pushing:
	jsr read_joypad_a_key
	bcs pushing

	lda vector_addr
	clc
	adc #4
	cmp #VADDR(vector_dic_44) + 1
	bmi save_id
	lda #0
	save_id:
	sta vector_addr
	rts
.endproc

.proc read_joypad_a_key
	lda #1
	sta APU_PAD1
	lda #0
	sta APU_PAD1
	.repeat 2, index
		lda APU_PAD1 + index
		lsr
		bcs read_end
	.endrepeat
	read_end:
	rts
.endproc

.proc nmi
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

.proc update_oam
	jsr move_pos
	jsr check_collision
	lda pos + 0
	sta oam + 3
	lda pos + 1
	sta oam + 0
	lda #.hibyte(oam)
	sta APU_SPR_DMA
	rts
.endproc

.proc move_pos
	ldx vector_addr
	.repeat 2, index
		lda pos + index
		sta old_pos + index
		clc
		adc vector_dic + index, x
		sta pos + index
	.endrepeat
	rts
.endproc

.proc check_collision
	.repeat 2, index
		.scope
			ldx vector_addr

			lda vector_dic + index, x
			bpl move_plus

			lda #BORDER_MIN
			cmp pos + index
			bmi end_check
			cmp old_pos + index
			bpl end_check
			jmp turn

			move_plus:
			lda #BORDER_MAX - 1
			cmp pos + index
			bpl end_check
			cmp old_pos + index
			bmi end_check

			turn:
			lda vector_dic + 2 + index, x
			sta vector_addr

			end_check:
		.endscope
	.endrepeat
	rts
.endproc
