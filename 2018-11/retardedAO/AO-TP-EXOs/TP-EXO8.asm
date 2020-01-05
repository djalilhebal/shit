; TP-EXO8: Reads a three-digit binary number and prints it in decimal.
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  buf db 3+1, ?, 3 dup(?)   ; maxLength+1 + actualLength + data
  msg db 'Entrez un nombre en binaire: $'
  res db 13, 10, 'En decimal: $'
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
    mov DL, buf[1]          ; Data Length
    dec DL                  ; because we start from zero
    mov bx, 0               ; index
    mov ax, 0               ; result
    
    nextDigit:
      add al, buf[bx+2]
      sub al, '0'
      cmp DL, bl
      je print
      shl al, 1             ; Shift left by 1 bit (Same as multiplying by 2)
      inc bl
    jmp nextDigit
    
    print:
    mov BL, al
    add BL, '0'
    mov ah, 09h             ; print string function
    mov dx, offset res
    int 21h
    mov dl, BL
    mov ah, 02h
    int 21h
    
    mov ah, 4Ch             ; exit to MS-DOS function
    mov al, 0               ; exit code success
    int 21h
  main endp
code ends

end main
