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

' #######################################################
' #######################################################
' #######################################################
' #######################################################
' #######################################################
' #######################################################
Private Sub Application_NewMailEx(ByVal EntryIDCollection As String)
    Dim varEntryIDs
    Dim objItem As MailItem
    
    Dim i As Integer
    
    
    varEntryIDs = Split(EntryIDCollection, ",")
    ' MsgBox "EntryIDCollection=" & EntryIDCollection & "dimension[" & UBound(varEntryIDs) & "]"
    
    For i = 0 To UBound(varEntryIDs)
        Set objItem = Application.Session.GetItemFromID(varEntryIDs(i))
        ' MsgBox "categories=" & objItem.Categories
        ' MsgBox "NewMailEx: Subject=" & objItem.Subject & ", attachments=[" & objItem.Attachments.Count & "]"
        Module11.saveEmailToWord objItem
        Module11.SaveAttach objItem
        If UCase(Trim(objItem.Subject)) = "AUTOMATION PLEASE" Then
            ' start_excel
        End If
        
        Set objItem = Nothing
    Next
End Sub

Private Sub Application_Quit()
    ' MsgBox "r u going to quit?"
End Sub

Private Sub Application_Startup()
    Initialize_handler
End Sub



Private Sub myEmailItem_Open(Cancel As Boolean)
End Sub

Private Sub myExplorer_Activate()
    ' MsgBox "mail activated"
End Sub

Private Sub myExplorer_SelectionChange()
    Dim myEmail As MailItem
    
End Sub

Private Sub myOlBar_BeforeNavigate(ByVal Shortcut As OutlookBarShortcut, Cancel As Boolean)
    If Shortcut.Name = "Notes" Then
        MsgBox "You cannot open the Notes folder."
        Cancel = True
    End If
End Sub


'##################################################
'##################################################
'##################################################
'##################################################
'##################################################
'##################################################
'##################################################
'##################################################
 
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
    Module11.saveEmailToWord Item
    Module11.SaveAttach Item
    If UCase(Trim(Item.Subject)) = "AUTOMATION PLEASE" Then
        ' start_excel
    End If
    
End Sub

Private Sub myOlItems_ItemChange(ByVal Item As Object)
    ' MsgBox "ItemChange: new item subject=" & Item.Subject
End Sub

Sub show_all_folders()
    Set allfolders = Application.GetNamespace("MAPI").GetFolderFromID()
    Set allfolders = Nothing
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


Sub process_all_emails()
    Dim email
    Dim exp As Explorer
    Dim msg As MailItem
    Dim i As Integer
    Dim att As Attachment
    
    Set myOlItems = Application.GetNamespace("MAPI").GetDefaultFolder(olFolderInbox).Items
    Set exp = Application.ActiveExplorer
    ' MsgBox exp.CurrentFolder.Items.Count & exp.CurrentFolder.Items(400).Subject
    
    ' MsgBox "Email(s) inside Inbox =[" & myOlItems.Count & "] has been processed"
    emailshow.Show vbModeless
    
    
    
    For email = 1 To myOlItems.Count
        ' MsgBox "processing email subject [" & email.Subject & "]"
        If myOlItems.Item(email).Attachments.Count > 0 Then
            Module11.SaveAttach myOlItems.Item(email)
        End If
        emailshow.Caption = "processing [" & email & "]subject=" & myOlItems.Item(email).Subject
        emailshow.Labelprocess.Width = emailshow.Frame1.Width * (email / myOlItems.Count)
        DoEvents
    
    Next
    emailshow.Hide
    Set exp = Nothing
    Set myOlItems = Nothing
End Sub
