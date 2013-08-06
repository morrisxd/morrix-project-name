Attribute VB_Name = "Module1"
Const root_path             As String = "C:\monthly_report_processor\"
Const comission_path        As String = "commision\"
Const daily_path            As String = "daily_cash_report\"
Const office_salary_path    As String = "office_salary\"
Const result_path           As String = "result\"
Const resultFileName        As String = "result.xlsx"
Const yearly_result_path    As String = "Yearly_Sales_Cost _n_Expenses_List\"
Const end_of_search         As Long = 100
Const resultStartLine       As Long = 4
Const salesNamePos          As String = "A3"
Const salesNamePosInResult  As String = "N"
Const commissionInResult    As String = "D"
Const commiDate             As String = "H3"
Const commiDateInResult     As String = "B"
Const officeManagerInResult As String = "O"
Const dailyFeeType          As String = "G"

Const myname                As String = "freedom"


Sub start_process()
    Application.ScreenUpdating = False

    beginProcessMonthlyReport
    Application.ScreenUpdating = True
End Sub
Sub close_all_files()
    ' MsgBox "all files closed !"
    Dim wb As Workbook
    
    Application.ScreenUpdating = False
    
    For Each wb In Workbooks
        If wb.name <> ThisWorkbook.name Then
            wb.Close SaveChanges:=False
        End If
    Next
    
    ThisWorkbook.Close SaveChanges:=False
    Application.ScreenUpdating = True
    

End Sub




Sub beginProcessMonthlyReport()
    Dim commission_file(100) As Workbook
    Dim daily_file(100) As Workbook
    Dim result_book As Workbook
    Dim office_file(100) As Workbook
    Dim sheetCnt
    Dim f_name
    Dim i As Long
    Dim s_c
    Dim stopPos
    Dim currentResultLine As Long
    Dim c_date, cDatePos
    
    Dim department
    Dim departPos  As Long
    Dim officeManager
    Dim j
    Dim officeNum
    Dim sheetName
    Dim rngTraffic As Range
    Dim rngEnter As Range
    Dim rngOffice As Range
    Dim rngBusinessTrip As Range
    Dim rngExpress As Range
    
    Dim k
    Dim a, b As Integer
    Dim c
    Dim formula
    Dim d_office
    Dim d_achievment
    
    
    
    Dim d_total
    Dim salary_total_col
    
    
    
    
    
    Set result_book = Workbooks.Open(root_path & result_path & resultFileName)
    
    
    
    
    
    
' ###################################################################
' ###################################################################
' ###################################################################
' ########################    daily file      #######################
' ###################################################################
' ###################################################################
    currentResultLine = resultStartLine
    
    filenames = Dir(root_path & "" & daily_path & "*.xls")
    
    i = 1
    
    Do While filenames <> ""
        Set daily_file(i) = Workbooks.Open(root_path & "" & daily_path & filenames)
        sheetCnt = daily_file(i).Sheets.Count
        d_office = daily_file(i).name
        ' MsgBox "there are pages=" & sheetCnt
        
        f_name = daily_file(i).name
        ' MsgBox "filename=" & f_name
        For s_c = 1 To sheetCnt ' each sheet represent a month
            sheetName = Trim(daily_file(i).Worksheets(s_c).name)
            d_month = Mid(sheetName, InStr(1, sheetName, "-") + 1, Len(sheetName))
            d_year = Mid(sheetName, 1, InStr(1, sheetName, "-") - 1)
            d_year = "20" & d_year
            validline = daily_file(i).Worksheets(s_c).UsedRange.Rows.Count
            d_name = Trim(daily_file(i).Worksheets(s_c).Range("B4"))
            ' MsgBox "usedRange=" & validLine
            For j = 4 To (validline - 1)
                temp_name = daily_file(i).Worksheets(s_c).Range("B" & j)
                ' MsgBox "B(" & j & ") name = " & temp_name
                If Len(Trim(temp_name)) <> 0 And Trim(temp_name) <> d_name Then
                    d_name = temp_name
                End If
                k = findPersonFeePos(Trim(d_name), Trim(d_month), Trim(d_year), 4, result_book.Worksheets(1))
                c = Trim(daily_file(i).Worksheets(s_c).Range("D" & j))
                If k <> end_of_search Then
                    ' MsgBox "name=" & d_name & ", and mont=" & d_month & ", found at = " & k
                    ' MsgBox "stopPos=" & stopPos & ",b=" & Trim(daily_file(i).Worksheets(s_c).Range("D" & j))
                    b = daily_file(i).Worksheets(s_c).Range("G" & j)
                    If c = "1" Or c = "2" Or c = "3" Or c = "4" Or c = "5" Then
                        result_book.Worksheets(1).Range("A" & k) = d_year
                        result_book.Worksheets(1).Range("B" & k) = d_month
                        result_book.Worksheets(1).Range("P" & k) = d_name
                        result_book.Worksheets(1).Range("V" & k) = d_office
                        result_book.Worksheets(1).Range("V" & k).EntireColumn.AutoFit
                    End If
                    
                    
                    Select Case c
                        Case "1", "2", "3", "4", "5"
                            a = result_book.Worksheets(1).Range(Chr(Asc("G") + Asc(c) - Asc("1")) & k)
                            formula = result_book.Worksheets(1).Range(Chr(Asc("Q") + Asc(c) - Asc("1")) & k)
                            If c = "3" Then
                                ' MsgBox "old=" & a & ",new=" & b & ", all=" & a + b
                            End If
                            result_book.Worksheets(1).Range(Chr(Asc("G") + Asc(c) - Asc("1")) & k) = a + b
                            result_book.Worksheets(1).Range(Chr(Asc("Q") + Asc(c) - Asc("1")) & k) = formula & "+" & b
                            result_book.Worksheets(1).Range(Chr(Asc("Q") + Asc(c) - Asc("1")) & k).EntireColumn.AutoFit
                        Case Else
                    End Select
                    
                Else
                    ' MsgBox "name=" & d_name & ", and mont=" & d_month & ", not found, use line = " & currentResultLine
                    a = daily_file(i).Worksheets(s_c).Range("G" & j)
                    
                    If c = "1" Or c = "2" Or c = "3" Or c = "4" Or c = "5" Then
                        result_book.Worksheets(1).Range("A" & currentResultLine) = d_year
                        result_book.Worksheets(1).Range("B" & currentResultLine) = d_month
                        result_book.Worksheets(1).Range("P" & currentResultLine) = d_name
                        result_book.Worksheets(1).Range("V" & currentResultLine) = d_office
                        result_book.Worksheets(1).Range("V" & currentResultLine).EntireColumn.AutoFit
                    End If
                    
                    Select Case c
                        Case "1", "2", "3", "4", "5"
                            If c = "3" Then
                                ' MsgBox "not found, a = " & a
                            End If
                            result_book.Worksheets(1).Range(Chr(Asc("G") + Asc(c) - Asc("1")) & currentResultLine) = a
                            result_book.Worksheets(1).Range(Chr(Asc("Q") + Asc(c) - Asc("1")) & currentResultLine) = a
                            
                        Case Else
                        
                    End Select
                    
                    If c = "1" Or c = "2" Or c = "3" Or c = "4" Or c = "5" Then
                        currentResultLine = currentResultLine + 1
                    End If
                End If
            Next
        Next
        filenames = Dir
        i = i + 1
    Loop
    
    
    
    
    
' ###################################################################
' ###################################################################
' ###################################################################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ########################    commission file      ##################
' ###################################################################
' ###################################################################
    
    currentResultLine = resultStartLine
    currentResultLine = result_book.Worksheets(1).UsedRange.Rows.Count + 1
    
    
    
    
    filenames = Dir(root_path & "" & comission_path & "*.xls")
    
    
    i = 1
    
    
    Do While filenames <> ""
        Set commission_file(i) = Workbooks.Open(root_path & "" & comission_path & filenames)
        sheetCnt = commission_file(i).Sheets.Count
        ' MsgBox "there are pages=" & sheetCnt
        
        f_name = commission_file(i).name
        ' MsgBox "filename=" & f_name
        For s_c = 1 To sheetCnt
            ' MsgBox "sheetname=" & commission_file(i).Worksheets(s_c).Name
            d_year = ""
            d_month = ""
            d_name = commission_file(i).Worksheets(s_c).Range("A3")
            
            stopPos = search_until_end_flag("total", "A", 6, commission_file(i).Worksheets(s_c))    ' find vertical total number
            col_achiev = findDateInCommisssion("sales achievement", "4", commission_file(i).Worksheets(s_c))
            If (col_achiev & stopPos) = "F100" Then
                d_achievment = "error, not found"
            Else
                d_achievment = commission_file(i).Worksheets(s_c).Range(col_achiev & stopPos)
            End If
            
            ' MsgBox " sales achievment found = " & col_achiev & stopPos & ", text=" & d_achievment
            c_date = findDateInCommisssion("total", "5", commission_file(i).Worksheets(s_c))
            cDatePos = c_date
            c_date = commission_file(i).Worksheets(s_c).Range(c_date & "3")
            getMonthYearFromCommission c_date, d_month, d_year
            d_month = getMonFromDigital(d_month)
            d_office = commission_file(i).name & ":" & commission_file(i).Worksheets(s_c).name
            
            ' MsgBox "c_date=" & c_date & ", d_month=" & d_month & ", d_year=" & d_year & ", d_name=" & d_name
            
            d_total = commission_file(i).Worksheets(s_c).Range(cDatePos & "6").Value
            ' MsgBox "commission total=" & d_total
            k = findPersonFeePos(Trim(d_name), Trim(d_month), Trim(d_year), 4, result_book.Worksheets(1))
            
' GoTo skipcomit
            If k = end_of_search Then
                ' MsgBox "stop failed=" & commission_file(i).Worksheets(s_c).Name
                result_book.Worksheets(1).Range(salesNamePosInResult & currentResultLine) = commission_file(i).Worksheets(s_c).Range(salesNamePos)
                tagColorRed result_book.Worksheets(1), salesNamePosInResult & currentResultLine, 255
                result_book.Worksheets(1).Range(salesNamePosInResult & currentResultLine).EntireColumn.AutoFit
                result_book.Worksheets(1).Range(commissionInResult & currentResultLine) = d_total
                result_book.Worksheets(1).Range(commissionInResult & currentResultLine).EntireColumn.AutoFit
                tagColorRed result_book.Worksheets(1), commissionInResult & currentResultLine, 255
                result_book.Worksheets(1).Range("A" & currentResultLine) = d_year
                result_book.Worksheets(1).Range("B" & currentResultLine) = d_month
                result_book.Worksheets(1).Range("W" & currentResultLine) = d_office
                result_book.Worksheets(1).Range("W" & currentResultLine).EntireColumn.AutoFit
                
                currentResultLine = currentResultLine + 1
                
            Else
                ' MsgBox "stop OK=" & commission_file(i).Worksheets(s_c).Name & "at=" & stopPos & ", sum=" & commission_file(i).Worksheets(s_c).Range("D" & stopPos)
                result_book.Worksheets(1).Range(salesNamePosInResult & k) = commission_file(i).Worksheets(s_c).Range(salesNamePos)
                result_book.Worksheets(1).Range(salesNamePosInResult & k).EntireColumn.AutoFit
                result_book.Worksheets(1).Range(commissionInResult & k) = commission_file(i).Worksheets(s_c).Range("D" & stopPos)
                result_book.Worksheets(1).Range(commissionInResult & k).EntireColumn.AutoFit
                
                result_book.Worksheets(1).Range("W" & k) = d_office
                result_book.Worksheets(1).Range("W" & k).EntireColumn.AutoFit
                
                result_book.Worksheets(1).Range("M" & k) = d_achievment
                result_book.Worksheets(1).Range("M" & k).EntireColumn.AutoFit
                
                
                If c_date <> "Z3" Then
                    result_book.Worksheets(1).Range(commiDateInResult & currentResultLine) = c_date
                End If
                result_book.Worksheets(1).Range(commiDateInResult & currentResultLine).EntireColumn.AutoFit
            End If
skipcomit:
        
        Next
        filenames = Dir
        i = i + 1
    Loop
    
    
    
    
    
    
    
    
    
    
    
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' ########################    office salary file   ##################
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    
    currentResultLine = resultStartLine
    currentResultLine = result_book.Worksheets(1).UsedRange.Rows.Count + 1
    
    filenames = Dir(root_path & "" & office_salary_path & "*.xls")
    
    i = 1
    
    Do While filenames <> ""
        Set office_file(i) = Workbooks.Open(root_path & "" & office_salary_path & filenames)
        sheetCnt = office_file(i).Sheets.Count
        ' MsgBox "there are pages=" & sheetCnt
        
        f_name = office_file(i).name
        ' MsgBox "filename=" & f_name
        For s_c = 1 To 1 'sheetCnt
            j = 1
            d_month = office_file(i).Worksheets(s_c).Range("A1")
            ' MsgBox "d_month=" & d_month
            ' d_month = getMonFromDigital(d_month)
            d_year = office_file(i).Worksheets(s_c).Range("B1")
            validline = office_file(i).Worksheets(s_c).UsedRange.Rows.Count
            For officeNum = 1 To validline
                departPos = search_until_end_flag("department", "A", j, office_file(i).Worksheets(s_c))
                
                ' MsgBox "depart=" & departPos & ", officeNum=" & officeNum & ", j=" & j
                
                    officeManager = office_file(i).Worksheets(s_c).Range("B" & (officeNum + 2))
                    If UCase(Trim(officeManager)) = UCase("name") Then
                        GoTo endOfLoop
                    End If
                    
                    If Len(officeManager) = 0 Then
                        GoTo endOfLoop
                    End If
                    
                    ' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    ' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    ' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    ' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    salary_total_col = office_file(i).Worksheets(s_c).Range("U" & (departPos + 2))
                    ' MsgBox "englishName=" & getEnglishName(officeManager)
                    officeManager = getEnglishName(officeManager)
                    d_name = officeManager
                    ' stopPos = findKeywordInColumn(officeManager, "", "N", 4, result_book.Worksheets(1))
                    k = findPersonFeePos(Trim(d_name), Trim(d_month), Trim(d_year), 4, result_book.Worksheets(1))
                    stopPos = k
                    ' MsgBox "mgr=" & officeManager
                    
                    
                    If stopPos <> end_of_search Then
                        result_book.Worksheets(1).Range(officeManagerInResult & stopPos) = officeManager
                        result_book.Worksheets(1).Range(officeManagerInResult & stopPos).EntireColumn.AutoFit
                        result_book.Worksheets(1).Range("C" & stopPos) = office_file(i).Worksheets(s_c).Range("U" & (officeNum + 2))
                        result_book.Worksheets(1).Range("C" & stopPos).EntireColumn.AutoFit
                        result_book.Worksheets(1).Range("X" & stopPos) = f_name
                        result_book.Worksheets(1).Range("X" & stopPos).EntireColumn.AutoFit
                        result_book.Worksheets(1).Range("A" & stopPos) = d_year
                        result_book.Worksheets(1).Range("B" & stopPos) = d_month
                    Else
                        result_book.Worksheets(1).Range(officeManagerInResult & currentResultLine) = officeManager
                        result_book.Worksheets(1).Range(officeManagerInResult & currentResultLine).EntireColumn.AutoFit
                        result_book.Worksheets(1).Range("C" & currentResultLine) = office_file(i).Worksheets(s_c).Range("U" & (officeNum + 2))
                        result_book.Worksheets(1).Range("C" & currentResultLine).EntireColumn.AutoFit
                        result_book.Worksheets(1).Range("X" & currentResultLine) = f_name
                        result_book.Worksheets(1).Range("X" & currentResultLine).EntireColumn.AutoFit
                        
                        result_book.Worksheets(1).Range("A" & currentResultLine) = d_year
                        result_book.Worksheets(1).Range("B" & currentResultLine) = d_month
                        
                        currentResultLine = currentResultLine + 1
                    End If
                
                j = departPos + 1
            
endOfLoop:
            
            Next
        Next
        filenames = Dir
        i = i + 1
    Loop
    
    
    

    
    
    
    
    
    
    result_book.Activate
End Sub

Function findKeywordInColumn(ByVal c_key, ByVal endFlag, ByVal startPoint, ByVal startPointLine, ByRef c_sheet As Worksheet)
    Dim i
    Dim c_cell
    
    
    findKeywordInColumn = end_of_search
    
    For i = 0 To end_of_search
        c_cell = Trim(c_sheet.Range(startPoint & (startPointLine + i)))
        ' MsgBox "c_cell=" & c_cell & ", c_key=" & c_key
        If c_cell = Trim(endFlag) Then
            findKeywordInColumn = i + startPointLine
            Exit Function
        End If
        
        If c_cell = Trim(c_key) Then
            findKeywordInColumn = i + startPointLine
            Exit Function
        End If
    Next
    

End Function



Function search_until_end_flag(ByVal endFlag, ByVal startPoint As String, ByVal startPointLine, ByRef c_sheet As Worksheet)
    Dim i
    Dim c_cell
    
    
    search_until_end_flag = end_of_search
    
    For i = 0 To end_of_search
        c_cell = Trim(c_sheet.Range(startPoint & (startPointLine + i)))
        If c_cell = Trim(endFlag) Then
            search_until_end_flag = i + startPointLine
            Exit For
        End If
    Next
    

End Function


Function findDateInCommisssion(ByVal endFlag, ByVal startPoint As String, ByRef c_sheet As Worksheet)
    Dim i
    Dim c_cell
    
    
    findDateInCommisssion = end_of_search
    
    For i = Asc("A") To Asc("Z")
        c_cell = Trim(c_sheet.Range(Chr(i) & startPoint))
        If c_cell = Trim(endFlag) Then
            findDateInCommisssion = Chr(i)
            ' MsgBox Chr(i)
            Exit For
        End If
    Next
    

End Function

Sub tagColorRed(ByRef c_sheet As Worksheet, ByVal c_rng As String, ByVal c_color As Long)
    c_sheet.Range(c_rng).Interior.Pattern = xlSolid
    c_sheet.Range(c_rng).Interior.PatternColorIndex = xlAutomatic
    c_sheet.Range(c_rng).Interior.Color = c_color
    c_sheet.Range(c_rng).Interior.TintAndShade = 0
    c_sheet.Range(c_rng).Interior.PatternTintAndShade = 0
End Sub





Function getEnglishName(ByVal name As String)
    Dim i
    Dim engName
    Dim c_letter

    engName = ""
    
    For i = 1 To Len(name)
        c_letter = Mid(name, i, 1)
        ' MsgBox "origin name=" & c_letter & ", value=" & Asc(c_letter)
        If Asc(c_letter) >= Asc("a") And Asc(c_letter) <= Asc("z") Then
            engName = engName & c_letter
        End If
        
        If Asc(c_letter) >= Asc("A") And Asc(c_letter) <= Asc("Z") Then
            engName = engName & c_letter
        End If
        
    Next
    getEnglishName = engName

End Function




Function findPersonFeePos(ByVal d_name, ByVal d_month, ByVal d_year, ByVal currentResultLine, ByRef result As Worksheet)
    Dim i
    Dim r_rows
    
    findPersonFeePos = end_of_search
    
    r_rows = result.UsedRange.Rows.Count
    
    For i = currentResultLine To r_rows
        If UCase(Trim(result.Range("P" & i))) = UCase(Trim(d_name)) And UCase(Trim(result.Range("B" & i))) = UCase(Trim(d_month)) And Trim(result.Range("A" & i)) = Trim(d_year) Then
            findPersonFeePos = i
            Exit Function
        End If
    Next

End Function



Function getValue(ByVal c_str)
    Dim i
    Dim r As String
    
    For i = 1 To 100
        r = r & Mid(c_str, i, 1)
    Next

    getValue = CInt(r)

End Function



Sub getMonthYearFromCommission(ByVal c_date, ByRef d_month, ByRef d_year)
    Dim s1, s2
    
    If Len(Trim(c_date)) > 0 Then
        s1 = InStr(c_date, "/")
        s2 = InStr(s1 + 1, c_date, "/")
        ' d_month = Mid(c_date, s1 + 1, s2 - s1 - 1)
        d_month = Mid(c_date, 1, s1 - 1)
        
        d_year = Mid(c_date, s2 + 1, 10)
    End If
    
End Sub


Function getMonFromDigital(ByVal month)

    Select Case month
        Case "1"
            getMonFromDigital = "JAN"
        Case "2"
            getMonFromDigital = "Feb"
        Case "3"
            getMonFromDigital = "Mar"
        Case "4"
            getMonFromDigital = "Apr"
        Case "5"
            getMonFromDigital = "May"
        Case "6"
            getMonFromDigital = "Jun"
        Case "7"
            getMonFromDigital = "July"
        Case "8"
            getMonFromDigital = "Aug"
        Case "9"
            getMonFromDigital = "Sep"
        Case "10"
            getMonFromDigital = "Oct"
        Case "11"
            getMonFromDigital = "Nov"
        Case "12"
            getMonFromDigital = "Dec"
        Case Else
            getMonFromDigital = "Yahoo!" & month
        
    
    End Select
    

End Function



Function getMonFromFilenameSalary(ByVal filename)

    Select Case monInCh
    Case "1yue"
        getMonFromFilenameSalary = "Jan"
    Case Else
    
    

End Function
