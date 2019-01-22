; TP-EXO7: Reads two two-digit numbers and prints the sum on the next line.
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  buf db 2+1, ?, 2 dup(?)     ; maxLength+1 + actualLength + data
  msg db 13, 10, 'Entrez un nombre: $'
  res db 13, 10, 'Leur somme: $'
  err db 13, 10, 'ERROR: Lazem deux chiffres yooo!$'
data ends

code segment
  main proc far
    assume cs:code, ds:data, ss:pile
    mov ax, data
    mov ds, ax
    
    mov cx, 2                 ; repeat 2 times
    readNumber:
      mov ah, 09h             ; print string function
      mov dx, offset msg
      int 21h

      mov ah, 0Ah             ; read buffer function
      mov dx, offset buf
      int 21h
      mov DL, buf[1]          ; "Data Length"
      cmp DL, 2
      jne badInput

      mov ah, buf[2]
      mov al, buf[3]
      sub ax, 3030h
      push ax
      dec cx
      cmp cx, 0
    jne readNumber
    
    sumAndPrint:
    pop ax
    pop bx
    add ax, bx
    aaa
    add ax, 3030h
    mov bx, ax              ; Remember: BX contains 'XY'
    mov ah, 09h             ; print string function
    mov dx, offset res
    int 21h
    mov ah, 02h
    cmp bh, '9'
    jle @F
      sub bh, 10
      mov dl, '1'
      int 21h
    @@:
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
