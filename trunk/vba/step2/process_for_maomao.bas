Attribute VB_Name = "process_for_maomao"
    Dim sur_name(1 To 100, 1 To 2)

Sub setBgColor()
    With Selection.Interior
        .Pattern = xlSolid
        .PatternColorIndex = xlAutomatic
        .Color = 255
        .TintAndShade = 0
        .PatternTintAndShade = 0
    End With
End Sub
Sub range_setBgColor_green(ByRef myRange)
                myRange.Interior.Pattern = xlSolid
                myRange.Interior.PatternColorIndex = xlAutomatic
                myRange.Interior.Color = 5287936
                myRange.Interior.TintAndShade = 0
                myRange.Interior.PatternTintAndShade = 0

End Sub


Sub range_setBgColor_red(ByRef myRange)
                myRange.Interior.Pattern = xlSolid
                myRange.Interior.PatternColorIndex = xlAutomatic
                myRange.Interior.Color = 155
                myRange.Interior.TintAndShade = 0
                myRange.Interior.PatternTintAndShade = 0

End Sub
Sub range_setBgColor_blue(ByRef myRange)
                myRange.Interior.Pattern = xlSolid
                myRange.Interior.PatternColorIndex = xlAutomatic
                myRange.Interior.Color = 15773696
                myRange.Interior.TintAndShade = 0
                myRange.Interior.PatternTintAndShade = 0

End Sub



Sub process_dalily_report_into_montly_report()
    ' Dim dailyfile As Workbook
    Dim shiplist_files(30) As Workbook
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
    Dim colsCount
    
    date_from_filename = "##################"
    current_sheet = 1
    j = 3



    Application.ScreenUpdating = False

    userInterface.Show vbModeless

    myrow = 0

    ' Set dailyfile = Workbooks.Open("c:\data\daliy\daily_sales_group_performance_in_Jan_2013.xls")
    ' Set proformaInvoice = Workbooks.Open("c:\data\proforma_invoice\2012WF0691weifangwangmei.xls")
    Set final_result = Workbooks.Open("C:\data\step2\result\result-monthly-promotion.xlsx")
    Set c_list = Workbooks.Open("c:\data\step2\c_list\c_list-monthly-promotion.xlsx")   ' open company definition file
    ' Set situation = Workbooks.Open("C:\data\step2\month\monthly sales situation(13) (6)")
    
    
    

    remarkPos = "AX"
    
    rowsCount = c_list.Worksheets(1).UsedRange.rows.Count
    colsCount = final_result.Worksheets(1).UsedRange.Columns.Count
    

    
    For i = 1 To rowsCount
        sur_name(i, 2) = Trim(c_list.Worksheets(1).Range("B" & i))
        sur_name(i, 1) = Trim(c_list.Worksheets(1).Range("C" & i))
        
    Next
    

    For i = 1 To colsCount
        If i > (Asc("Z") - Asc("A")) + 1 Then
            ch = "a" & Chr(Asc("a") + (i Mod 26) - 1)
        Else
            ch = Chr(Asc("a") + i - 1)
        End If
        ' MsgBox ch
        
        ' range_setBgColor_blue final_result.Worksheets(1).Range(ch & "2")
    Next
    
range_setBgColor_blue final_result.Worksheets(1).UsedRange


    final_result.Activate



    
'#################################################################
'#################################################################
'#################################################################
' format all titles
    
    For i = Asc("A") To Asc("Z")
        final_result.Worksheets(1).Range(Chr(i) & 1).EntireColumn.AutoFit
        final_result.Worksheets(1).Range("A" & Chr(i) & 1).EntireColumn.AutoFit
    Next
    

    ' MsgBox "total sheets number = " & sheet_cnt
    'invoice_id = proformaInvoice.Sheets(1).Range("C5")
    
    
' process daily report
    
    i = 1
    shiplist_dir = "C:\data\step2\ship-list\"
    filenames = Dir(shiplist_dir & "*.xls")
    ' MsgBox "filenames=" & filenames
' Exit Sub
    
    Do While filenames <> ""
        Set shiplist_files(i) = Workbooks.Open(shiplist_dir & filenames)
        sheet_cnt = shiplist_files(i).Sheets.Count
        
        
        f_name = filenames
        ' get_daily_file_date_year f_name, r_month, r_year
        ' MsgBox "filename=" & filenames & ":month=" & r_month & ":year=" & r_year
        ' date_from_filename = r_month & "/" & r_year
        For s_c = 1 To sheet_cnt
            process_daily_sheet shiplist_files(i), s_c, final_result, date_from_filename '#######################################################
        Next
        filenames = Dir
        i = i + 1
    Loop

'#################################################################
'#################################################################
'#################################################################
' search all files in the dir

    i = 1
    shiplist_dir = "C:\data\step2\month\"    ' situation
    filenames = Dir(shiplist_dir & "*.xls")
    Do While filenames <> ""
        Set proforma_multi(i) = Workbooks.Open(shiplist_dir & filenames)
        process_sailes_situation proforma_multi(i), final_result, 1, c_list
        'MsgBox "filename=" & filenames
        filenames = Dir
        i = i + 1
    Loop
    
    
    final_result.Activate
    
    Application.ScreenUpdating = True
    
    userInterface.Hide
End Sub



Sub get_company_name_and_revenu(daily_work, ByRef company_name, ByRef revenu)
    pos = InStr(1, daily_work, ":")
    company_name = Mid(daily_work, 1, pos - 1)
    revenu = Mid(daily_work, pos + 1, Len(daily_work))
    
End Sub

Function search_in_final_result(ByVal orderNo, ByRef result)
    Dim rowsCount
    
    search_in_final_result = 0
    
    rowsCount = result.Worksheets(1).UsedRange.rows.Count
    For j = 3 To rowsCount
        If UCase(Trim(result.Worksheets(1).Range("a" & j))) = UCase(Trim(orderNo)) Then
            search_in_final_result = j
            ' MsgBox "search_in_final_result = " & j
            Exit Function
        End If
    Next
    
    search_in_final_result = 0
End Function

Sub process_sailes_situation(ByRef pi As Workbook, ByRef target_book As Workbook, ByVal current_sheet, cmp_name As Workbook)
    Dim start_pos
    Dim i, j
    Dim remark
    Dim company_chinese_name, company_english_name
    Dim line_number
    Dim findRelation
    Dim fontColor
    Dim shipNo As Workbook  ' ship number.xls
    Dim rowsCount
    Dim remarkPos
    Dim sheetCnts
    Dim orderNo
    Dim is_in_result
    Dim comment
    
    comment = "AK"
    
    sheetCnts = pi.Worksheets.Count
    j = target_book.Worksheets(1).UsedRange.rows.Count + 1
    
    For mysheet = 1 To sheetCnts
        rowsCount = pi.Worksheets(mysheet).UsedRange.rows.Count
        For myline = 2 To rowsCount
            orderNo = UCase(Trim(pi.Worksheets(mysheet).Range("A" & myline)))
            If UCase(Trim(orderNo)) = "TOTAL" Then
                GoTo nextSheet
            Else
                If Len(Trim(orderNo)) = 0 Then
                Else
                    is_in_result = search_in_final_result(UCase(Trim(orderNo)), target_book)
                    If is_in_result = 0 Then
                        target_book.Worksheets(1).Range("a" & j) = orderNo
                        target_book.Worksheets(1).Range("a" & j).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("d" & j) = pi.Worksheets(mysheet).Range("c" & myline)
                        target_book.Worksheets(1).Range("d" & j).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("c" & j) = pi.Worksheets(mysheet).Range("e" & myline)
                        target_book.Worksheets(1).Range("c" & j).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range(comment & j) = "NOT FOUND"
                        target_book.Worksheets(1).Range(comment & j).EntireColumn.AutoFit
                        range_setBgColor_red target_book.Worksheets(1).Range(comment & j)
                        target_book.Worksheets(1).Range("al" & j) = pi.Worksheets(mysheet).Name
                        target_book.Worksheets(1).Range("al" & j).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("am" & j) = myline
                        target_book.Worksheets(1).Range("am" & j).EntireColumn.AutoFit
                        
                        j = j + 1
                    Else
                        target_book.Worksheets(1).Range("a" & is_in_result) = orderNo
                        target_book.Worksheets(1).Range("a" & is_in_result).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("d" & is_in_result) = pi.Worksheets(mysheet).Range("c" & myline)
                        target_book.Worksheets(1).Range("d" & is_in_result).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("c" & is_in_result) = pi.Worksheets(mysheet).Range("e" & myline)
                        target_book.Worksheets(1).Range("c" & is_in_result).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range(comment & is_in_result) = "YES! FOUND!"
                        range_setBgColor_green target_book.Worksheets(1).Range(comment & is_in_result)
                        target_book.Worksheets(1).Range(comment & is_in_result).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("al" & is_in_result) = pi.Worksheets(mysheet).Name
                        target_book.Worksheets(1).Range("al" & is_in_result).EntireColumn.AutoFit
                        target_book.Worksheets(1).Range("am" & is_in_result) = myline
                        target_book.Worksheets(1).Range("am" & is_in_result).EntireColumn.AutoFit
                    End If
                End If
            End If
            userInterface.updateProgress (mysheet / sheetCnts)
        Next
nextSheet:
    Next
    

End Sub

Function lookup_col_pos(ByVal issue_id, ByRef sur_name, ByVal remarkPos)
' ####################################################################################
' ####################################################################################
' ####################################################################################
' ####################################################################################
    
    Dim i
    Dim end_of_surname
    
    end_of_surname = 90 ' this is very important
    lookup_col_pos = 0 'sur_name(40, 1)
    
    
    For i = 1 To end_of_surname
        If UCase(Trim(issue_id)) = UCase(sur_name(i, 2)) Then
            lookup_col_pos = sur_name(i, 1)
            GoTo end_of_lookup
        Else
        End If
    Next
    
    lookup_col_pos = 0 'sur_name(40, 1)
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



' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################
' ######################################################################

Sub process_daily_sheet(ByRef dailyfile As Workbook, ByVal current_sheet, ByRef final_result As Workbook, date_in_filename As String)
    Dim sales_name
    Dim bill
    Dim pos
    Dim c_name, c_revenu
    Dim lineNo
    Dim j
    Dim itemName, subItemName
    Dim col_pos
    Dim subItemPos
    Dim subItemStart, subItemEnd
    
    j = final_result.Sheets(1).UsedRange.rows.Count + 1


    If Len(Trim(dailyfile.Sheets(current_sheet).Range("g4"))) = 0 Then
        dailyfile.Sheets(current_sheet).Range("g4") = "###Order No Should Be Here !"
        range_setBgColor_red (dailyfile.Sheets(current_sheet).Range("g4"))
        dailyfile.Sheets(current_sheet).Range("g4").EntireColumn.AutoFit
    Else
        final_result.Worksheets(1).Range("A" & j) = dailyfile.Sheets(current_sheet).Range("g4")
        final_result.Worksheets(1).Range("A" & j).EntireColumn.AutoFit
    End If
    


    If UCase(Trim(dailyfile.Sheets(current_sheet).Range("B4"))) = "货物品名" Then
        'MsgBox "current_sheet=" & current_sheet
        For lineNo = 5 To 1000
            If UCase(Trim(dailyfile.Sheets(current_sheet).Range("A" & lineNo))) = "收货地址：" Then
               Exit Sub
            End If
            
            itemName = UCase(Trim(dailyfile.Sheets(current_sheet).Range("B" & lineNo)))
            subItemPos = InStr(1, itemName, "/", 0)
            
            If subItemPos <> 0 Then
                Do
                    subItemPos = InStr(1, itemName, "/", 0)
                    If subItemPos > 0 Then
                        subItemName = Left(itemName, subItemPos - 1)
                    Else
                        subItemName = itemName
                    End If
                    
                    col_pos = lookup_col_pos(subItemName, sur_name, "######")
                    
                    If (col_pos = 0) Then
                        range_setBgColor_red (dailyfile.Sheets(current_sheet).Range("B" & lineNo))
                    Else
                        final_result.Worksheets(1).Range(col_pos & j) = UCase(Trim(dailyfile.Sheets(current_sheet).Range("C" & lineNo)))
                    End If
                    
                    itemName = Mid(itemName, subItemPos + 1, 256)
                    ' MsgBox "subitemname=" & subItemName & ",itemname=" & itemName & ",subItemPos=" & subItemPos & "ready to go"
                Loop While subItemPos > 0
            Else

                col_pos = lookup_col_pos(itemName, sur_name, "######")
                
                ' MsgBox "we found " & col_pos & j
                If (col_pos = 0) Then
                    range_setBgColor_red (dailyfile.Sheets(current_sheet).Range("B" & lineNo))
                Else
                    final_result.Worksheets(1).Range(col_pos & j) = UCase(Trim(dailyfile.Sheets(current_sheet).Range("C" & lineNo)))
                End If
            End If
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


