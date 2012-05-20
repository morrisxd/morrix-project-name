;MACROS
;##################################
;PARAMETER(S)
;       none
;RETURN VAL
;       none
;FUNCTION
;       Do some prepration work.
;##################################
Preparation     MACRO
                push    ds
                xor     ax, ax
                push    ax
                push    cs
                pop     ds
                push    cs
                pop     es
                ENDM

;##################################
;PARAMETER(S)
;       _msg    - string variable defined in DATA segment
;RETURN VAL
;       none
;FUNCTION
;       Show message defined in _msg
;##################################
ShowMSG         MACRO   _msg            ; _msg should be defined in DS
                mov     ah, 09h
                lea     dx, _msg
                int     21h             ; show greetings message
                ENDM

;##################################
;PARAMETER(S)
;       _src_str        - source string name
;       _target_str     - source string name
;RETURN VAL
;       none
;FUNCTION
;       move the source string to the target string
;##################################
MoveSTR         MACRO   _target_reg,_src_reg,_num
                mov     si, _src_reg
                mov     di, _target_reg
                mov     cx, _num
                rep     movsb
                ENDM

;##################################
;PARAMETER(S)
;       _file_handler_seek      - handler of file
;       _file_offset_seek       - offset value
;       _method                 - method of seek
;RETURN VAL
;       none
;FUNCTION
;       move the current position to pointed position.
;##################################
SeekFile        MACRO   _file_handler_seek,_file_offset_seek,_method
                LOCAL   _seekerror,_afterseek
                clc
                mov     ah,42h
                mov     bx,_file_handler_seek
                mov     cx,0
                mov     dx,_file_offset_seek
                mov     al,_method
                int     21h
                jc      _seekerror
                jmp     _afterseek
_seekerror:
                ShowMSG _fseekerr
                stc
_afterseek:
                ENDM

;##################################
;PARAMETER(S)
;       _file_name_open_reg        - file name register

;RETURN VAL
;       None
;FUNCTION
;       Open file.
;##################################
OpenFile        MACRO   _filename_open_reg, _file_handler_open
                LOCAL   _afteropen,_fopenerror
                clc
                mov     ah, 3dh
                mov     dx, _filename_open_reg
                mov     al, 02h                 ; read and write
                int     21h
                jc      _fopenerror
                mov     _file_handler_open, ax
                jmp     _afteropen
_fopenerror:
                ShowMSG _fopenerr
                stc
_afteropen:
                ENDM

;##################################
;PARAMETER(S)
;       _new_filename        - source string name
;       _new_filehandler     - source string name
;RETURN VAL
;       none
;FUNCTION
;       Create the file _new_filename and store its handler
;       into _new_filehander
;##################################
CreateFile      MACRO   _file_name_create,_file_handler_create
                LOCAL   _fcreateerror, _aftercreate
                clc
                mov     ah, 3ch
                lea     dx, _file_name_create
                mov     cx, 0
                int     21h
                jc      _fcreateerror
                mov     _file_handler_create, ax
                jmp     _aftercreate
_fcreateerror:
                SHOWMSG _fcreateerr
                stc
_aftercreate:
                ENDM

;##################################
;PARAMETER(S)
;       _file_handler_close        - file handler

;RETURN VAL
;       none
;FUNCTION
;       Close file
;##################################
CloseFile       MACRO   _file_handler_close
                LOCAL   _filecloseerror, _afterclose
                clc
                mov     ah, 3eh
                mov     bx, _file_handler_close
                int     21h
                jc      _filecloseerror
                jmp     _afterclose
_filecloseerror:
                SHOWMSG _filecloseerr
                stc
_afterclose:
                ENDM

;##################################
;PARAMETER(S)
;       _file_handler_write     - file handler
;       _file_buffer_write      - file buffer

;RETURN VAL
;       none
;FUNCTION
;       Write file.
;##################################
WriteFile       MACRO   _file_handler_write,_file_buffer_write,_num
                LOCAL   _writeerror, _afterwrite
                clc
                mov     ah, 40h
                mov     bx, _file_handler_write
                mov     cx, _num
                lea     dx, _file_buffer_write
                int     21h             ; write file
                jc      _writeerror
                jmp     _afterwrite
_writeerror:
                ;SHOWMSG _writeerror
                stc
_afterwrite:
                ENDM

;##################################
;PARAMETER(S)
;       _file_handler_read     - file handler
;       _file_buffer_read   - file buffer
;       _num                    - bytes num to be read

;RETURN VAL
;       none
;FUNCTION
;       Read file.
;##################################
ReadFile        MACRO   _file_handler_read,_file_buffer_read,_num
                LOCAL   _afterread,_freaderror
                clc
                mov     ah,3fh
                mov     bx,_file_handler_read
                mov     cx,_num
                mov     dx, OFFSET _file_buffer_read
                int     21h
                jc      _freaderror
                jmp     _afterread
_freaderror:
                ShowMSG _freaderr
                stc
_afterread:
                ENDM

;##################################
;PARAMETER(S)
;       none
;RETURN VAL
;       none
;FUNCTION
;       Read in the EXE_HEAD of file fit during searching
;       progress.
;##################################
READEXEHEAD     MACRO
                LOCAL   _freaderror,_fopen_error,_search_error,_afterreadhead
                clc
                mov     ah, 4eh
                lea     dx, _fitfilename
                mov     cx, 00h
                int     21h             ; search for first fit file
                jc      _search_error
                mov     ah, 51h
                int     21h             ; get PSP in BX
                mov     _SegmentPSP, bx   

                lea     bx, _filename
                push    ds              ; store DS
                mov     ax, _SegmentPSP
                mov     ds, ax          ; set DS = PSP
                mov     dx, 80h + 30
                MoveSTR bx, dx, 13      ; DS:SI -> ES:DI
                pop     ds              ; restore DS

                mov     cx, OFFSET _filename
                OpenFile        cx, _handler_source
                jc      _fopen_error

                ReadFile        _Handler_Source,_exHeader,SIZE _exHeader
                jc      _freaderror
                jmp     _afterreadhead
_freaderror:
                jmp     _afterreadhead
_fopen_error:
                jmp     _afterreadhead
_search_error:
                SHOWMSG _searcherr
                jmp     _afterreadhead
_afterreadhead:
                ENDM


;##################################
;PARAMETER(S)
;       none
;RETURN VAL
;       none
;FUNCTION
;       Modify the relocate items in the old file.
;##################################
ModifyRelocates MACRO
                LOCAL   _writenextitem,_aftermodify
                mov     ax,_exHeader.exRelocItems
                mov     _counter,ax
                mov     ax,_exHeader.exRelocTable
                mov     _currpos,ax
_writenextitem:
                SeekFile _Handler_Temp,_currpos,FROMTOP
                ReadFile _Handler_Temp,_buffer_d,SIZE _buffer_d
                add     WORD PTR _buffer_d,OFFSETVAL
                SeekFile _Handler_Temp,_currpos,FROMTOP
                WriteFile _Handler_Temp,_buffer_d,SIZE _buffer_d
                add     _currpos,4
                dec     _counter
                cmp     _counter,0
                je      _aftermodify
                jmp     _writenextitem
_aftermodify:
                ENDM
          
;##################################
;PARAMETER(S)
;       none
;RETURN VAL
;       none
;FUNCTION
;       Write the new EXE header into the new file.
;##################################
WriteNewHeader  MACRO
                ENDM

;##################################
;PARAMETER(S)
;       none
;RETURN VAL
;       none
;FUNCTION
;       Write 'My Heart' into the new 'Body'
;##################################
WriteHeart      MACRO
                push    ds
                mov     ax,_heartcode
                mov     ds,ax
                WriteFile _Handler_Temp,_frontheart,_heartsize
                pop     ds
                ENDM

;##################################
;NAME
;       CopyBody
;PARAMETER(S)
;       none
;RETURN VAL
;       none
;FUNCTION
;       Write the rest of the old program's code into new file
;##################################
CopyBody        MACRO   _destfile_handler, _srcfile_handler,_buffer_copy
                LOCAL   _again,_afterwrite
_again:
                ReadFile        _srcfile_handler,_buffer,SIZE _buffer_copy
                cmp     ax,SIZE _buffer_copy
                jb      _afterwrite
                WriteFile       _destfile_handler,_buffer,SIZE _buffer_copy
                jmp     _again
_afterwrite:
                ENDM

GetFileSize     MACRO
                mov     ax,_SegmentPSP
                mov     es,ax
                mov     di,80h + 26
                mov     ax,WORD PTR es:[di]
                mov     WORD PTR _filesize,ax
                mov     ax,WORD PTR es:[di][2]
                mov     WORD PTR _filesize[2],ax
                ENDM

SetNewStart     MACRO
                mov     dx,WORD PTR _filesize[2]
                mov     ax,WORD PTR _filesize
                mov     cx,16
                div     cx
                mov     _exHeader.exInitCS,ax
                mov     _exHeader.exInitIP,dx
                mov     ax,_exHeader.exHeaderSize
                sub     _exHeader.exInitCS,ax
                ENDM

