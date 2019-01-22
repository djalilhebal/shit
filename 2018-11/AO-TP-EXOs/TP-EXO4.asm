; TP-EXO4: Reads a 2-digit number and prints it on the next line
pile segment para stack 'pile'
  db 256 dup(0)
pile ends

data segment
  buf db 2+1, ?, 2+1 dup(?)   ; maxLength+1 + actualLength + data
                            ; That +1 byte is for reading the enter character.
  num db 13, 10, '00$'      ; template, sorta
data ends

code segment
  main proc far
    assume cs:code, ds:data, ss:pile
    mov ax, data
    mov ds, ax
    
    mov ah, 0Ah             ; read buffer function
    mov dx, offset buf
    int 21h
    mov DL, buf[1]          ; DL = "Data Length"
    cmp DL, 1
    je oneDigit

    ; twoDigits
    mov DH, buf[2]          ; DH = "Data Holder"
    mov num[2], DH
    mov DH, buf[3]
    mov num[3], DH
    jmp print
    
    oneDigit:
    mov DH, buf[2]
    mov num[3], DH

    print:
    mov ah, 09h             ; print string function
    mov dx, offset num
    int 21h
    
    mov ah, 4Ch             ; exit to MS-DOS function
    mov al, 0               ; exit code success
    int 21h
  main endp
code ends

end main
