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

.main:
	
	; First test, add 000f and 0001, write test results to first line of display
	; Lines are 32 ($20) tiles long

	ld hl, $9800;
	push hl ; Address to print to

	ld hl, $000f
	push hl ; First 16b number to add

	ld hl, $0001
	push hl ; Second 16b number to add 
	
	call .SixteenBitAddCheck1

	; Second test, add $fff and $1, write to second line of display

	ld hl, $9820
	push hl ; address to print to

	ld hl, $fff
	push hl ; first operand

	ld hl, $1
	push hl; second operand

	call .SixteenBitAddCheck1



	jr .turnOnScreen




.SixteenBitAddCheck1
; This function adds two 16 bit numbers to see what happens with the half carry flag 
; Arg 1: First 16b number to add
; Arg 2: Second 16b number to add
; Arg 3: Address to pass to the .copyString function
;
;
	pop de ; gets the return address
	pop bc ; gets the first operand
	pop hl ; gets the second operand

	add hl,bc ; perform the add
	push af ; now push the flags to the stack
	pop  bc ; flags from the 16bit adds are now in c
	
	ld a, %00100000 ; We want to get bit 5, which is where the half carry flag lives
	and c ; Extract the HC flag, and shift it into the carry bit. Need 3 shifts
	
	REPT 3
		sla a
	ENDR
	
	pop hl ; Get the address to write to from the stack
	push de ; Push the old return value back to the stack  
	jr c, .set1 ; If the flag was not set load the string saying so

.notSet1:
	ld de, NotSetStr
	jp .prepForStringCopy

.set1:
	ld de, SetStr ; Else, load the string saying that the flag was set.

.prepForStringCopy
	push de
	push hl
	call .copyString
	ret

.copyString
	; The copyString function copies tile data for a string to a particular place in VRAM
	; PARAMS
	; DE: DE contains a pointer to the start of string to be printed
	; HL: HL contains a pointer to the start of where the string is to be printed
	; 
	; This function expects strings to be 0 terminated
	;

	pop bc ; Get the caller function return address from the stack
	pop hl ; Get the address to print to
	pop de ; get the address of the string to print
	push bc; put the cfra back on the stack

.copyStringLoop
	ld a, [de]
	ld [hli], a
	inc de
	and a ; Check if the byte copied is zero
	jr nz, .copyStringLoop ; continue copying the string if its not
	ret

.turnOnScreen:
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



FontTiles:
INCBIN "font.chr"
FontTilesEnd:

SECTION "Result strings", ROM0

SetStr:
	db "HC Flag was set", 0

NotSetStr:
	db "HC Flag was not set", 0

