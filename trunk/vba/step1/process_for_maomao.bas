Attribute VB_Name = "process_for_maomao"
Sub process_dalily_report_into_montly_report()
    ' Dim dailyfile As Workbook
    Dim dailyfile_multi(30) As Workbook
    ' Dim proformaInvoice As Workbook 'invoice sheet
    Dim proforma_multi(30) As Workbook  ' invoice sheet
    Dim final_result As Workbook    ' save the final result
    Dim c_list As Workbook  ' company name list
    
    
    Dim myrow
    Dim i
    Dim invoice_id
    Dim temp
    Dim pos
    Dim company_name
    Dim revenu
    Dim sales_name
    Dim bill
    Dim j
    Dim c_name, c_revenu As String ' c_name is used for saving company name temproraily
    Dim payment_date
    Dim current_sheet
    Dim sheet_cnt
    Dim r_month, r_year
    Dim f_name As String
    Dim date_from_filename As String
    
    
    current_sheet = 1
    j = 3

    myrow = 0

    'Set dailyfile = Workbooks.Open("c:\data\daliy\daily_sales_group_performance_in_Jan_2013.xls")
    'Set proformaInvoice = Workbooks.Open("c:\data\proforma_invoice\2012WF0691weifangwangmei.xls")
    Set final_result = Workbooks.Open("C:\data\result\result.xlsx")
    Set c_list = Workbooks.Open("c:\data\c_list\c_list.xlsx")   ' open company definition file
    
    final_result.Activate


    
'#################################################################
'#################################################################
'#################################################################
' format all titles
    Application.ScreenUpdating = False
    
    For i = Asc("A") To Asc("Z")
        final_result.Worksheets(1).Range(Chr(i) & 1).EntireColumn.AutoFit
        final_result.Worksheets(1).Range("A" & Chr(i) & 1).EntireColumn.AutoFit
    Next
    

    ' MsgBox "total sheets number = " & sheet_cnt
    'invoice_id = proformaInvoice.Sheets(1).Range("C5")
    
    
' process daily report
    
    i = 1
    proforma_dir = "C:\data\daliy\"
    filenames = Dir(proforma_dir & "*.xls")
    Do While filenames <> ""
        Set dailyfile_multi(i) = Workbooks.Open(proforma_dir & filenames)
        sheet_cnt = dailyfile_multi(i).Sheets.Count
        
        
        f_name = filenames
        get_daily_file_date_year f_name, r_month, r_year
        ' MsgBox "filename=" & filenames & ":month=" & r_month & ":year=" & r_year
        date_from_filename = r_month & "/" & r_year
        For s_c = 1 To sheet_cnt
            process_daily_sheet dailyfile_multi(i), s_c, final_result, date_from_filename
        Next
        filenames = Dir
        i = i + 1
    Loop
    
    
'#################################################################
'#################################################################
'#################################################################
' search all files in the dir

    i = 1
    proforma_dir = "C:\data\proforma_invoice\"
    filenames = Dir(proforma_dir & "*.xls")
    Do While filenames <> ""
        Set proforma_multi(i) = Workbooks.Open(proforma_dir & filenames)
        process_proform_invoice proforma_multi(i), final_result, 1, c_list
        'MsgBox "filename=" & filenames
        filenames = Dir
        i = i + 1
    Loop
    
    
    final_result.Activate
    
    Application.ScreenUpdating = True
    
  
End Sub



Sub get_company_name_and_revenu(daily_work, ByRef company_name, ByRef revenu)
    pos = InStr(1, daily_work, ":")
    company_name = Mid(daily_work, 1, pos - 1)
    revenu = Mid(daily_work, pos + 1, Len(daily_work))
    
End Sub



Sub process_proform_invoice(ByRef pi As Workbook, ByRef target_book As Workbook, ByVal current_sheet, cmp_name As Workbook)
    Dim start_pos
    Dim i
    Dim sur_name(1 To 100, 1 To 2)
    Dim remark
    Dim company_chinese_name, company_english_name
    Dim line_number
    Dim findRelation
    Dim fontColor
    Dim shipNo As Workbook  ' ship number.xls
    Dim rowsCount
    Dim remarkPos
    
    remarkPos = "AX"
    
    Set shipNo = Workbooks.Open("c:\data\c_list\ship_number.xlsx")   ' open company definition file
    rowsCount = shipNo.Worksheets(1).UsedRange.rows.Count
    ' MsgBox "rowsCount=" & rowsCount
    
    For i = 1 To rowsCount
        sur_name(i, 2) = Trim(shipNo.Worksheets(1).Range("B" & i))
        sur_name(i, 1) = Trim(shipNo.Worksheets(1).Range("C" & i))
        ' MsgBox "shipno=(" & sur_name(i, 2) & "), col=(" & sur_name(i, 1) & ")"
        If UCase(Trim(shipNo.Worksheets(1).Range("B" & i))) = "REMARK" Then
            remarkPos = Trim(shipNo.Worksheets(1).Range("C" & i))
            ' MsgBox "We got remarkPos=" & remarkPos
        End If
        
    Next
    
    findRelation = 0
    
    
    
    
    'sur_name(1, 2) = "PW_TO_T15"
    'sur_name(2, 2) = "P5250UC"
    'sur_name(3, 2) = "faucet"
    'sur_name(4, 2) = "PRE-10"
    'sur_name(5, 2) = "P3050CTD"
    'sur_name(6, 2) = "SHF-1"
    'sur_name(7, 2) = "HSF-1"
    'sur_name(8, 2) = "WMF-1"
    'sur_name(9, 2) = "SFRC-1"
    'sur_name(10, 2) = "POE-1017"
    'sur_name(11, 2) = "POE-1035"
    'sur_name(12, 2) = "POE 8"
    'sur_name(13, 2) = "POE 10"
    'sur_name(14, 2) = "POE 12"
    'sur_name(15, 2) = "PSE-22"
    'sur_name(16, 2) = "PSE-08"
    'sur_name(17, 2) = "PSE-18"
    'sur_name(18, 2) = "PSE-25"
    'sur_name(19, 2) = "PSE-12(440)"
    'sur_name(20, 2) = "PSE-12"
    'sur_name(21, 2) = "P8601Q"
    'sur_name(22, 2) = "P8661Q"
    'sur_name(23, 2) = "EASF"
    'sur_name(24, 2) = "P8409UV"
    'sur_name(25, 2) = "PW-GD-82"
    'sur_name(26, 2) = "PW-GD-85"
    'sur_name(27, 2) = "PW-GD-88"
    'sur_name(28, 2) = "PW-TOK-T20"
    'sur_name(29, 2) = "PW-TOK-S10"
    'sur_name(30, 2) = "PW-TOP-TB15"
    'sur_name(31, 2) = "PW-TOP-SS15"
    'sur_name(32, 2) = "PW-TO-T15"
    'sur_name(33, 2) = "PW-TO-S15"
    'sur_name(34, 2) = "PW-ST-S1"
    'sur_name(35, 2) = "P3200C"
    'sur_name(36, 2) = "P3200W"
    'sur_name(37, 2) = "PW750F"
    'sur_name(38, 2) = "PW1250UF"
    'sur_name(39, 2) = "PWR060"
    'sur_name(40, 2) = "REMARK"
    'sur_name(41, 2) = ""
    'sur_name(42, 2) = ""
    'sur_name(43, 2) = ""
    'sur_name(44, 2) = ""
    'sur_name(45, 2) = ""
    'sur_name(46, 2) = ""
    'sur_name(47, 2) = ""
    'sur_name(48, 2) = ""
    'sur_name(49, 2) = ""
    'sur_name(50, 2) = ""
    
    
    'sur_name(1, 1) = "AP"
    'sur_name(2, 1) = "L"
    'sur_name(3, 1) = "M"
    'sur_name(4, 1) = "N"
    'sur_name(5, 1) = "O"
    'sur_name(6, 1) = "P"
    'sur_name(7, 1) = "Q"
    'sur_name(8, 1) = "R"
    'sur_name(9, 1) = "S"
    'sur_name(10, 1) = "T"
    'sur_name(11, 1) = "U"
    'sur_name(12, 1) = "V"
    'sur_name(13, 1) = "W"
    'sur_name(14, 1) = "X"
    'sur_name(15, 1) = "Y"
    'sur_name(16, 1) = "Z"
    'sur_name(17, 1) = "AA"
    'sur_name(18, 1) = "AB"
    'sur_name(19, 1) = "AC"
    'sur_name(20, 1) = "AD"
    'sur_name(21, 1) = "AE"
    'sur_name(22, 1) = "AF"
    'sur_name(23, 1) = "AG"
    'sur_name(24, 1) = "AH"
    'sur_name(25, 1) = "AI"
    'sur_name(26, 1) = "AJ"
    'sur_name(27, 1) = "AK"
    'sur_name(28, 1) = "AL"
    'sur_name(29, 1) = "AM"
    'sur_name(30, 1) = "AN"
    'sur_name(31, 1) = "AO"
    'sur_name(32, 1) = "AP"
    'sur_name(33, 1) = "AQ"
    'sur_name(34, 1) = "AR"
    'sur_name(35, 1) = "AS"
    'sur_name(36, 1) = "AT"
    'sur_name(37, 1) = "AU"
    'sur_name(38, 1) = "AV"
    'sur_name(39, 1) = "AW"
    'sur_name(40, 1) = "AX"
    'sur_name(41, 1) = "AY"
   
    remark = "present: "
   
    
    Line = target_book.Sheets(1).UsedRange.rows.Count
    Line = Line + 1
    'MsgBox "target sheets lines= " & Line
    
    start_pos = "A10"
    'MsgBox pi.Sheets(1).Range(start_pos)
    end_of_invoice = 40
    
    company_chinese_name = pi.Worksheets(current_sheet).Range("D6")
    company_english_name = get_company_eng_name(company_chinese_name, cmp_name)
    ' MsgBox "chinese_name=" & company_chinese_name & ", english_name = " & company_english_name
    
    If Len(Trim(company_chinese_name)) = 0 Then
    Else
        If (company_english_name = "nothing") Then
        Else
            line_number = get_company_line_in_target_book(target_book, company_english_name)
            If line_number = 0 Then
            Else
                findRelation = 1
                Line = line_number
                ' MsgBox "we found line=" & Line
            End If
        End If
    End If
    
    
    For i = 10 To end_of_invoice ' process every line of item in the profoma_invoice table, find out all the item including gifts
        issueid = pi.Worksheets(current_sheet).Range("A" & i)
        issueid = Trim(issueid)
        
        ' MsgBox "issueid = " & issueid
        
        If issueid = "×Ü½ð¶î(Total RMB):" Then ' we reach the end of this profoma_invoice table.
            'add total number
            total_num = WorksheetFunction.Sum(pi.Worksheets(current_sheet).Range("E10:" & "E" & (i - 1)))
            'MsgBox "total number = " & total_num
            target_book.Sheets(1).Range("F" & Line) = total_num
            compnay_ch_name = pi.Worksheets(current_sheet).Range("D6")
            
            
            If findRelation = 1 Then
            Else
                If Len(compnay_ch_name) > 0 Then
                    target_book.Sheets(1).Range("C" & Line) = compnay_ch_name
                Else
                    target_book.Sheets(1).Range("C" & Line) = "[Special Single Person]"
                End If
            End If
            target_book.Sheets(1).Range("I" & Line) = pi.Worksheets(current_sheet).Range("F5")
            
            GoTo end_of_process
        End If

        If Len(issueid) > 0 Then ' process each line in this table. if the cell is NOT blank. there is real content inside this cell, we process it
        
            ' red = 3
            fontColor = pi.Worksheets(current_sheet).Range("A" & i).Cells.Font.ColorIndex

'##########################################
'##########################################
'##########################################
' lookup position here
            col_pos = lookup_col_pos(issueid, sur_name, remarkPos)
            If fontColor = 3 Then ' if this cell has color, which means it is a gift
                ' MsgBox "color =" & fontColor & ", issueid=(" & issueid & ")"
                ' col_pos = sur_name(40, 1) ' we put it to remark cell as a gift. sur_name(40,1)=AX in fact
                col_pos = remarkPos
            End If
            
            If Trim(col_pos) = remarkPos Then ' and we set all the gift source item in the profoma_invoice table as RED giving a indication
                ' mycell = "G1"
                ' MsgBox "issueid = " & issueid
                pi.Worksheets(current_sheet).Range("A" & i).Interior.Pattern = xlSolid
                pi.Worksheets(current_sheet).Range("A" & i).Interior.PatternColorIndex = xlAutomatic
                pi.Worksheets(current_sheet).Range("A" & i).Interior.Color = 155
                pi.Worksheets(current_sheet).Range("A" & i).Interior.TintAndShade = 0
                pi.Worksheets(current_sheet).Range("A" & i).Interior.PatternTintAndShade = 0

                ' skip
            End If
            
            'MsgBox "col_pos=" & col_pos
'##########################################
'##########################################
'##########################################
' we need to deal with col_pos specially
            w_id = pi.Worksheets(current_sheet).Range("C5")
            If Trim(col_pos) = remarkPos Then ' now we put the gift into the RESULT table as REMARK
                remark = remark & pi.Worksheets(current_sheet).Range("C" & i) & "pcs " & " " & issueid & ", "
                target_book.Sheets(1).Range("" & col_pos & Line) = remark
                target_book.Sheets(1).Range("" & col_pos & Line).EntireColumn.AutoFit
                
                ' MsgBox "remarkPos = " & col_pos
            Else
                target_book.Sheets(1).Range("" & col_pos & Line) = pi.Worksheets(current_sheet).Range("C" & i)
            End If
            
            
            target_book.Sheets(1).Range("" & col_pos & Line).EntireColumn.AutoFit
            'target_book.Sheets(1).Range("" & col_pos & Line).EntireColumn.Width = 40

            
            
            target_book.Sheets(1).Range("A" & Line) = w_id
            
            target_book.Sheets(1).Range("A" & Line).EntireColumn.AutoFit
            
            If findRelation = 1 Then
                ' preserve the english company name when we found it
            Else
                target_book.Sheets(1).Range("C" & Line) = pi.Worksheets(current_sheet).Range("D6")
            End If
            target_book.Sheets(1).Range("I" & Line) = pi.Worksheets(current_sheet).Range("F5")
            target_book.Sheets(1).Range("I" & Line).EntireColumn.AutoFit
            
            
            If InStr(1, w_id, "WF") > 0 Then
                target_book.Sheets(1).Range("B" & Line) = "WF"
            Else
                target_book.Sheets(1).Range("B" & Line) = "W"
            End If
                
            
        Else
            'GoTo end_of_process
        End If
    Next
    
    
    MsgBox "silly boy"
    
    
    
    For i = Asc("A") To Asc("Z")
        MsgBox "i don't understand"
        final_result.Worksheets(1).Range(Chr(i) & 1).Select
        set_bg_color_blue
        final_result.Worksheets(1).Range("A" & Chr(i) & 1).Select
        set_bg_color_blue
    Next
    
    
    For i = 1 To 20
        
    Next
    
    
    Line = Line + 1


end_of_process:
End Sub

Function lookup_col_pos(ByVal issue_id, ByRef sur_name, ByVal remarkPos)
    Dim i
    Dim end_of_surname
    
    end_of_surname = 90 ' this is very important
    lookup_col_pos = remarkPos 'sur_name(40, 1)
    
    
    For i = 1 To end_of_surname
        If Trim(issue_id) = sur_name(i, 2) Then
            lookup_col_pos = sur_name(i, 1)
            GoTo end_of_lookup
        Else
        End If
    Next
    
    lookup_col_pos = remarkPos 'sur_name(40, 1)
end_of_lookup:

End Function



Sub set_bg_color_blue()
'
' set_bg_color Macro
'

'
    With Selection.Interior
        .Pattern = xlSolid
        .PatternColorIndex = xlAutomatic
        .ThemeColor = xlThemeColorLight2
        .TintAndShade = 0.599993896298105
        .PatternTintAndShade = 0
    End With
End Sub





Sub process_daily_sheet(ByRef dailyfile As Workbook, ByVal current_sheet, ByRef final_result As Workbook, date_in_filename As String)
    Dim sales_name
    Dim bill
    Dim pos
    Dim c_name, c_revenu
    
    Dim j
    
    j = final_result.Sheets(1).UsedRange.rows.Count + 1

    If dailyfile.Sheets(current_sheet).Range("A1") = "Person/Date" Then
        'MsgBox "current_sheet=" & current_sheet
        For s = Asc("B") To Asc("Z")
            sales_name = dailyfile.Sheets(current_sheet).Range(Chr(s) & "1")
            sales_name = Trim(sales_name)
            
            If Len(sales_name) = 0 Then
                Exit Sub
            End If
            
            
            
            For i = 2 To 40
                bill = dailyfile.Sheets(current_sheet).Range(Chr(s) & i)
                bill = Trim(bill)
                ' MsgBox "bill(B" & i & ")=" & bill
                If (Len(bill) = 0) Then
                    ' MsgBox "1"
                Else
                    pos = InStr(1, bill, ":")
                    If pos = 0 Then
                        ' MsgBox "2"
                    Else
                        get_company_name_and_revenu bill, c_name, c_revenu
                        final_result.Sheets(1).Range("C" & j) = c_name
                        final_result.Sheets(1).Range("C" & j).EntireColumn.AutoFit
                        final_result.Sheets(1).Range("G" & j) = c_revenu
                        final_result.Sheets(1).Range("G" & j).EntireColumn.AutoFit
                        final_result.Sheets(1).Range("E" & j) = sales_name
                        final_result.Sheets(1).Range("J" & j) = dailyfile.Sheets(current_sheet).Range("A" & i) & "/" & date_in_filename
                        final_result.Sheets(1).Range("J" & j).EntireColumn.AutoFit
                        j = j + 1
                        
                    End If
                End If
            Next
        Next
    Else
    End If

End Sub


Function get_company_eng_name(ByVal company_ch_name, ByRef c_name As Workbook)
    Dim rows
    Dim i
    
    rows = c_name.Sheets(1).UsedRange.rows.Count
    
    For i = 1 To rows
        If Trim(c_name.Sheets(1).Range("C" & i)) = Trim(company_ch_name) Then
            get_company_eng_name = c_name.Sheets(1).Range("B" & i)
            Exit Function
        End If
    Next

    get_company_eng_name = "nothing"
End Function


Function get_company_ch_name(ByVal company_eng_name, ByRef c_name As Workbook)
    Dim rows
    Dim i
    
    
    rows = c_name.Sheets(1).UsedRange.rows.Count
    
    For i = 1 To rows
        If Trim(c_name.Sheets(1).Range("B" & i)) = Trim(company_ch_name) Then
            get_company_ch_name = c_name.Sheets(1).Range("C" & i)
            Exit Function
        End If
    Next

    get_company_ch_name = "nothing"
End Function

Function get_company_line_in_target_book(ByRef target_book As Workbook, ByVal company_eng_name)
    Dim i
    Dim rows
    
    rows = target_book.Sheets(1).UsedRange.rows.Count
    
    For i = 1 To rows
        If Trim(target_book.Sheets(1).Range("C" & i)) = company_eng_name Then
            get_company_line_in_target_book = i
            Exit Function
        End If
    Next

    i = 0

End Function


Sub get_daily_file_date_year(ByVal fileName As String, ByRef r_month, ByRef r_year)
    Dim posOfPoint, posOfPrep
    
    posOfPoint = InStr(1, fileName, ".")
    posOfPrep = InStrRev(fileName, "on")
    
    If posOfPrep = 0 Then
        posOfPrep = InStrRev(fileName, "in")
    Else
    End If
    ' MsgBox fileName & ":pos of prep 2:" & posOfPrep

    r_month = Mid(fileName, posOfPrep + 3, posOfPoint - posOfPrep - 3)
    r_year = Mid(fileName, posOfPoint + 1, 4)

End Sub





Sub setBgColor()
'
' Macro2 Macro
'

'
    With Selection.Interior
        .Pattern = xlSolid
        .PatternColorIndex = xlAutomatic
        .Color = 255
        .TintAndShade = 0
        .PatternTintAndShade = 0
    End With
End Sub

