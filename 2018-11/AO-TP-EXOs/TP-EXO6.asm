; TP-EXO6: Reads a four-digit integer and prints the sum of its digits.
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  buf db 4+1, ?, 4 dup(?)   ;  maxLength+1 + actualLength + data
  msg db 'Entrez un nombre: $'
  sum db 13, 10, 'La somme de ses chiffres est XY$'
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
      cmp dl, bl
      je print
      inc bl
    jmp nextDigit
    
    print:
    ; Example: AX=13, BL=10 > AX div BL > AL=1 AH=3
    mov bl, 10
    div bl                  ; => AL = AX div 10; AL = AX mod 10
    add ax, 3030h           ; convert to characters by adding '00'
    mov sum[31], al         ; fill in the "sum" string
    mov sum[32], ah         ; ...
    mov ah, 09h             ; print string function
    mov dx, offset sum
    int 21h
    
    mov ah, 4Ch             ; exit to MS-DOS function
    mov al, 0               ; exit code success
    int 21h
  main endp
code ends

end main
