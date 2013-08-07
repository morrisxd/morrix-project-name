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
Public WithEvents myOlItems As Outlook.Items
Attribute myOlItems.VB_VarHelpID = -1

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

    Set myItem = Application.ActiveExplorer.Selection.Item(1)
    
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
        MsgBox "NewMailEx: Subject=" & objItem.Subject & ", attachments=[" & objItem.Attachments.Count & "]"
        Module1.SaveAttach objItem
        'objItem.BodyFormat = olFormatPlain
        'emailshow.emailBody.Text = objItem.Body
        'emailshow.Show
    Next
    
End Sub

Private Sub Application_Quit()
    ' MsgBox "r u going to quit?"
End Sub

Private Sub Application_Startup()
    Dim olApp As Outlook.Application
    Dim objNS As Outlook.NameSpace
    
    ' MsgBox "Hello Outlook 2010"
    Set myOlBar = Application.ActiveExplorer.Panes(1)
    Set myExplorer = Application.ActiveExplorer
    ' Set myOlItems = Application.GetNamespace("MAPI").GetDefaultFolder(olFolderContacts).Items
    Set olApp = Outlook.Application
    Set objNS = olApp.GetNamespace("MAPI")
  ' default local Inbox
    Set myOlItems = objNS.GetDefaultFolder(olFolderInbox).Items
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
    If Shortcut.name = "Notes" Then
        MsgBox "You cannot open the Notes folder."
        Cancel = True
    End If
End Sub



 
Public Sub Initialize_handler()
    MsgBox "hi"
    Set myOlExp = Application.ActiveExplorer

 
End Sub
 


Private Sub myOlItems_ItemAdd(ByVal Item As Object)
    MsgBox "ItemAdd"
End Sub

Private Sub myOlItems_ItemChange(ByVal Item As Object)
    ' MsgBox "ItemChange"
End Sub
