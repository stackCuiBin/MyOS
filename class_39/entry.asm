
global _start
global vstr
global vlen
global print

extern main

[section .data]
   vstr    db    "D.T.Software", 0x0A
   vlen    dd    $ - vstr
   

[section .text]
_start:
    mov ebp, 0
    
    call main
    
    call exit
    
    
    
print:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp + 12]
    mov ecx, [ebp + 8]
    mov ebx, 1
    mov eax, 4
    int 0x80
    
    pop ebp
    ret
    
exit:
    mov ebx, 0
    mov eax, 1
    int 0x80