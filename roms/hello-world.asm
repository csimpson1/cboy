INCLUDE "hardware.inc"

SECTION "Header", ROM0[$100] ; create a section of code which lives in rom bank 0 at $100
;We have 4 bytes before the game's header, we use 3 of them to exit
	di; disable interrupts, we don't care
	jp Start;

REPT $150 - $104
	db 0
ENDR

SECTION "Game Code", ROM0

Start:
	; Turn off the LCD
.waitVBlank
	ld a, [rLY]
	cp 144 ; Check if the LCD is past VBlank
	jr c, .waitVBlank

	xor a ; ld a, 0 ; Only need to reset bit 7, but 0 will work
	ld [rLCDC], a ; Reset bit 7 of the rLCDC register to turn it off

	; Copy the font to VRAM
	ld hl, $9000
	ld de, FontTiles
	ld bc, FontTilesEnd - FontTiles

.copyFont
	ld a, [de] ; Grab 1 byte from the source
	ld [hli], a ; Place this byte in VRAM, and increment HL to point to the next location
	inc de ; Move to the next byte
	dec bc ; decrement count
	ld a, b ; Check if the count is 0, since dec bc doesn't change flags
	or c ; Cheeky, compare the high byte with the low byte. Only way this is 0 is if both are, ie BC == 0
	jr nz, .copyFont

	ld hl, $9800
	ld de, HelloWorldStr

.copyString
	ld a, [de]
	ld [hli], a
	inc de
	and a ; Check if the byte copied is zero
	jr nz, .copyString ; continue copying the string if its not

	; Init display registers
	ld a, %11100100
	ld [rBGP], a

	xor a ; ld a, 0
	ld [rSCY], a
	ld [rSCX], a

	; turn off sound
	ld [rNR52], a

	; turn screen on, display background
	ld a, %10000001
	ld [rLCDC], a

	; Lock up

.lockup
	jr .lockup


SECTION "Font", ROM0

FontTiles:
INCBIN "font.chr"
FontTilesEnd:

SECTION "Hello World string", ROM0

HelloWorldStr:
	db "Hello World!", 0

