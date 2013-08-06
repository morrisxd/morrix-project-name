Attribute VB_Name = "Module1"
Sub CloseAllFileWithoutSave()
    Dim wb As Workbook
    
    Application.ScreenUpdating = False
    
    For Each wb In Workbooks
        If wb.Name <> ThisWorkbook.Name Then
            wb.Close SaveChanges:=False
        End If
    Next
    
    ThisWorkbook.Close SaveChanges:=False
    Application.ScreenUpdating = True
    
End Sub
