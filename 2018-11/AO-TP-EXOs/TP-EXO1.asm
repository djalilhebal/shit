; TP-EXO1: Displays 'A'
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  ; no variables
data ends

code segment
  main proc far
    assume cs:code
    assume ds:data
    assume ss:pile
    mov ax, data
    mov ds, ax
    
    mov ah, 02h ; output character
    mov dl, 'A' ; or 65
    int 21h
    
    mov al, 0 ; exit code success
    mov ah, 4Ch
    int 21h
  main endp
code ends

end main
