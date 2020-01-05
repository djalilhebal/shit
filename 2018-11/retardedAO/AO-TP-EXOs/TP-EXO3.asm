; TP-EXO3: Reads a lowercase letter and prints it in uppercase on the next line
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
    
    mov ah, 01h ; read character with echo (result in AL)
    int 21h
    mov bl, al  ; Save it in BL, since calling int21h/02h sets `AL = DL`
    sub bl, 32  ; 'x' - 'X' = 32 -> 'x' - 32 = 'X'

    mov ah, 02h ; output character
    mov dl, 13  ; '\r'
    int 21h
    mov dl, 10  ; '\n'
    int 21h
    mov dl, bl
    int 21h
    
    mov al, 0   ; exit code success
    mov ah, 4Ch
    int 21h
  main endp
code ends

end main
