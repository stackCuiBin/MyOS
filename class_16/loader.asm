%include "inc.asm"

org 0x9000

jmp ENTRY_SEGMENT

[section .gdt]
; GDT definition
;                                 段基址，       段界限，       段属性
GDT_ENTRY       :     Descriptor    0,            0,           0
CODE32_DESC     :     Descriptor    0,    Code32SegLen - 1,    DA_C + DA_32
VIDEO_DESC      :     Descriptor 0xB8000,     0x07FFF,         DA_DRWA + DA_32
STACK32_DESC    :     Descriptor    0,     TopOfStack32,       DA_DRW + DA_32
FUNCTION_DESC   :     Descriptor    0,   FunctionSegLen - 1,   DA_C + DA_32
; Gate Descriptor
; Call Gate                        选择子，         偏移，      参数个数，      属性
FUNC_CG_ADD_DESC      Gate   FunctionSelector,   CG_Add,        0,      DA_386CGate
FUNC_CG_SUB_DESC      Gate   FunctionSelector,   CG_Sub,        0,      DA_386CGate
; GDT end

GdtLen    equ   $ - GDT_ENTRY

GdtPtr:
          dw   GdtLen - 1
          dd   0
          
          
; GDT Selector

Code32Selector     equ (0x0001 << 3) + SA_TIG + SA_RPL0
VideoSelector      equ (0x0002 << 3) + SA_TIG + SA_RPL0
Stack32Selector    equ (0x0003 << 3) + SA_TIG + SA_RPL0
FunctionSelector   equ (0x0004 << 3) + SA_TIG + SA_RPL0
FuncCGAddSelector  equ (0x0005 << 3) + SA_TIG + SA_RPL0
FuncCGSubSelector  equ (0x0006 << 3) + SA_TIG + SA_RPL0

; end of [section .gdt]

TopOfStack16    equ 0x7c00

[section .s16]
[bits 16]
ENTRY_SEGMENT:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, TopOfStack16
    
    ; initialize GDT for 32 bits code segment
    mov esi, CODE32_SEGMENT
    mov edi, CODE32_DESC
    
    call InitDescItem
    
    mov esi, STACK32_SEGMENT
    mov edi, STACK32_DESC
    
    call InitDescItem
    
    mov esi, FUNCTION_SEGMENT
    mov edi, FUNCTION_DESC
    
    call InitDescItem
    
    ; initialize GDT pointer struct
    mov eax, 0
    mov ax, ds
    shl eax, 4
    add eax, GDT_ENTRY
    mov dword [GdtPtr + 2], eax

    ; 1. load GDT
    lgdt [GdtPtr]
    
    ; 2. close interrupt
    cli 
    
    ; 3. open A20
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    
    ; 4. enter protect mode
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax
    
    ; 5. jump to 32 bits code
    jmp dword Code32Selector : 0


; esi    --> code segment label
; edi    --> descriptor label
InitDescItem:
    push eax

    mov eax, 0
    mov ax, cs
    shl eax, 4
    add eax, esi
    mov word [edi + 2], ax
    shr eax, 16
    mov byte [edi + 4], al
    mov byte [edi + 7], ah
    
    pop eax
    
    ret
    
[section .s32]
[bits 32]
CODE32_SEGMENT:
    mov ax, VideoSelector
    mov gs, ax
    
    mov ax, Stack32Selector
    mov ss, ax
    
    mov eax, TopOfStack32
    mov esp, eax
    
    mov ax, 2
    mov bx, 1
    
    call FuncCGAddSelector : 0
    call FuncCGSubSelector : 0

    jmp $

Code32SegLen    equ    $ - CODE32_SEGMENT

[section .func]
[bits 32]
FUNCTION_SEGMENT:

; ax --> a
; bx --> b
; 
; return:
;     cx --> a + b
AddFunc:
    mov cx, ax
    add cx, bx
    retf
    
CG_Add    equ    AddFunc - $$

; ax --> a
; bx --> b
; 
; return:
;     cx --> a - b
SubFunc:
    mov cx, ax
    sub cx, bx
    retf

CG_Sub    equ    SubFunc - $$  

FunctionSegLen   equ  $ - FUNCTION_SEGMENT

[section .gs]
[bits 32]
STACK32_SEGMENT:
    times 1024 * 4 db 0
    
Stack32SegLen equ $ - STACK32_SEGMENT
TopOfStack32  equ Stack32SegLen - 1


