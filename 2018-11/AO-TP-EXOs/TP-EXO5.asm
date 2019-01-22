; TP-EXO5: Reads user's name and prints "Bonjour, NAME." on the next line.
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  buf db 25+1, ?, 25+1 dup(?) ; maxLength+1 + actualLength + data+1
  msg db 'Entrez votre nom: $'
  bjr db 13, 10, 'Bonjour, $'
data ends

code segment
  main proc far
    assume cs:code, ds:data, ss:pile
    mov ax, data
    mov ds, ax
    
    mov ah, 09h             ; print string function
    mov dx, offset msg
    int 21h

    mov ah, 0Ah             ; read buffer function
    mov dx, offset buf
    int 21h
    mov bx, 0
    mov bl, buf[1]          ; "Data Length"
    mov buf[bx+2], '$'
    
    mov ah, 09h             ; print string function
    mov dx, offset bjr
    int 21h

    mov ah, 09h             ; print string function
    mov dx, offset buf + 2
    int 21h

    mov ah, 02h             ; print character function
    mov dl, '.'
    int 21h
    
    mov ah, 4Ch             ; exit to MS-DOS function
    mov al, 0               ; exit code success
    int 21h
  main endp
code ends

end main
