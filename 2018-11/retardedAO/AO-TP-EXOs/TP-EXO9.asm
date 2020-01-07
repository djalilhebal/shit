; TP-EXO9: Reads three 2-digit numbers and prints the max.
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  buf db 2+1, ?, 2 dup(?)
  msg db 13, 10, 'Entrez un nombre: $'
  res db 13, 10, 'Le max: $'
  err db 13, 10, 'ERROR: bad input!$'
data ends

code segment
  main proc far
    assume cs:code, ds:data, ss:pile
    mov ax, data
    mov ds, ax

    mov BX, 3030h             ; "Best maX", initially '00'
    
    mov cx, 3                 ; repeat 3 times
    readNumber:
      mov ah, 09h             ; print string function
      mov dx, offset msg
      int 21h

      mov ah, 0Ah             ; read buffer function
      mov dx, offset buf
      int 21h
      mov DL, buf[1]          ; Data Length
      cmp DL, 2
      jne badInput
      ; Okay, at least the input's length is fine, continue...
      mov ah, buf[2]
      mov al, buf[3]
      cmp ax, BX
      jle @F                  ; AX > BX? The new Best maX is AX!
        mov BX, ax
      @@:
      dec cx
      cmp cx, 0
    jne readNumber
    
    print:
    mov ah, 09h              ; print string function
    mov dx, offset res
    int 21h
    mov ah, 02h
    mov dl, bh
    int 21h
    mov dl, bl
    int 21h
    
    jmp exit
    
    badInput:
    mov ah, 09h             ; print string function
    mov dx, offset err
    int 21h
    
    exit:
    mov ah, 4Ch             ; exit to MS-DOS function
    mov al, 0               ; exit code success
    int 21h
  main endp
code ends

end main
