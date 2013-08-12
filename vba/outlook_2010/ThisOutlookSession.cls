VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "ThisOutlookSession"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Option Explicit
Dim WithEvents myOlBar As Outlook.OutlookBarPane
Attribute myOlBar.VB_VarHelpID = -1
Dim WithEvents myExplorer As Outlook.Explorer
Attribute myExplorer.VB_VarHelpID = -1
Dim WithEvents myEmailItem As Outlook.MailItem
Attribute myEmailItem.VB_VarHelpID = -1
Dim WithEvents myOlItems As Outlook.Items
Attribute myOlItems.VB_VarHelpID = -1
Dim WithEvents myOlExp As Outlook.Explorer
Attribute myOlExp.VB_VarHelpID = -1
'####################################################
'####################################################
'####################################################
Dim myExcel As Excel.Application

'####################################################
'####################################################
'####################################################
Dim xlBook As Excel.Workbook
Dim xlSheet As Excel.Worksheet
Dim excelsheet
Dim myFolder As Outlook.Folder


Private Sub Application_ItemLoad(ByVal Item As Object)
    ' MsgBox "world"

End Sub

Private Sub Application_ItemSend(ByVal Item As Object, Cancel As Boolean)

End Sub


Private Sub Application_MAPILogonComplete()
    ' MsgBox "logoned"
End Sub

Private Sub Application_NewMail()
    Dim myItem As Outlook.MailItem

    ' Set myItem = Application.ActiveExplorer.Selection.Item(1)
    
    ' MsgBox "You got an email. Subject=[" & myItem.Subject & "], body=[" & myItem.Body & "]"
End Sub

Private Sub Application_NewMailEx(ByVal EntryIDCollection As String)
    ' MsgBox "name=" & EntryIDCollection
    Dim varEntryIDs
    Dim objItem As MailItem
    
    Dim i As Integer
    
    
    varEntryIDs = Split(EntryIDCollection, ",")
    
    For i = 0 To UBound(varEntryIDs)
        Set objItem = Application.Session.GetItemFromID(varEntryIDs(i))
        ' MsgBox "NewMailEx: Subject=" & objItem.Subject & ", attachments=[" & objItem.Attachments.Count & "]"
        Module11.SaveAttach objItem
        'objItem.BodyFormat = olFormatPlain
        'emailshow.emailBody.Text = objItem.Body
        'emailshow.Show
        If UCase(Trim(objItem.Subject)) = "AUTOMATION PLEASE" Then
            ' start_excel
        End If
        
        Module11.saveEmailToWord objItem
    Next
    
    ' #######################################################
    ' #######################################################
    ' #######################################################
    ' #######################################################
    ' #######################################################
    ' #######################################################
    ' MsgBox "Now we have all (" & objItem.Application.ActiveExplorer.CurrentFolder.Items.Count & ") emails. foldername=[" & objItem.Application.ActiveExplorer.CurrentFolder.FolderPath & "]"
    ' MsgBox "count=" & myOlItems.Count
End Sub

Private Sub Application_Quit()
    ' MsgBox "r u going to quit?"
End Sub

Private Sub Application_Startup()
    Initialize_handler
End Sub



Private Sub myEmailItem_Open(Cancel As Boolean)
    ' Set myEmailItem = Application.ActiveExplorer.CurrentItem


    MsgBox "You have opened an email"
End Sub

Private Sub myExplorer_Activate()
    ' MsgBox "mail activated"
End Sub

Private Sub myExplorer_SelectionChange()
    Dim myEmail As MailItem
    
    If ActiveExplorer.Selection.Count = 0 Then
        Exit Sub
    Else
        For Each myEmail In ActiveExplorer.Selection
            If myEmail.Attachments.Count = 0 Then
            Else
                ' MsgBox "You got [" & myEmail.Attachments.Count & "] " & "attachment(s)"
            End If
        Next
        Set myEmail = Nothing
    End If
End Sub

Private Sub myOlBar_BeforeNavigate(ByVal Shortcut As OutlookBarShortcut, Cancel As Boolean)
    If Shortcut.Name = "Notes" Then
        MsgBox "You cannot open the Notes folder."
        Cancel = True
    End If
End Sub



 
Public Sub Initialize_handler()
    Dim olApp As Outlook.Application
    Dim objNS As Outlook.NameSpace
    Dim folderArray As Variant
    Dim folderPath
    Dim i
    
    ' MsgBox "Hello Outlook 2010"
    Set myOlBar = Application.ActiveExplorer.Panes(1)
    Set myExplorer = Application.ActiveExplorer
    Set myOlItems = Application.GetNamespace("MAPI").GetDefaultFolder(olFolderInbox).Items
    ' folderPath = "\\984745824@qq.com (1)]Inbox"
    ' folderPath = "Inbox"
    ' folderPath = Right(folderPath, Len(folderPath) - 2)
    ' folderArray = Split(folderPath, "\")
    ' Set myFolder = Application.Session.Folders.Item(folderArray(0))
    
    'If Not myFolder Is Nothing Then
    '    For i = 1 To UBound(folderArray, 1)
    '        Dim subfolders As Outlook.Folders
    '        Set subfolders = myFolder.Folders
    '        Set myFolder = subfolders.Item(folderArray(i))
    '        ' MsgBox "name=" & myFolder.Name
    '        If UCase(Trim(myFolder.Name)) = "INBOX" Then
    '            Set myOlItems = myFolder.Items
    '        End If
    '    Next
    '
    '    If Not myOlItems Is Nothing Then
    '        ' MsgBox "Email(s) inside Inbox =[" & myOlItems.Count & "]"
    '    End If
    'Else
    '    MsgBox "Can't get folder object"
    'End If
    
    Set myOlExp = Application.ActiveExplorer
End Sub
 


Private Sub myOlExp_Activate()
    ' kjl
End Sub

Private Sub myOlExp_SelectionChange()
    ' msgbox
End Sub

Private Sub myOlItems_ItemAdd(ByVal Item As Object)
    ' MsgBox "ItemAdd: subject=" & Item.Subject
    
End Sub

Private Sub myOlItems_ItemChange(ByVal Item As Object)
    ' MsgBox "ItemChange: new item subject=" & Item.Subject
End Sub

Sub show_all_folders()
    Set allFolders = Application.GetNamespace("MAPI").GetFolderFromID()
End Sub

Sub start_excel()
    Set myExcel = CreateObject("Excel.Application")
    myExcel.Visible = True
    myExcel.Workbooks.Open ("C:\Users\morris\SkyDrive\lib\VBA\daily_report_processor\merge_for_maomao_monthly_promotion.xlsm")
    myExcel.Workbooks.Open ("C:\Users\dengmorr\SkyDrive\lib\VBA\daily_report_processor\merge_for_maomao_monthly_promotion.xlsm")
    myExcel.Run "process_dalily_report_into_montly_report"
    
    For Each excelsheet In myExcel.Workbooks
        excelsheet.Close (False)
    Next
    
    
    myExcel.Quit
    Set myExcel = Nothing
End Sub


