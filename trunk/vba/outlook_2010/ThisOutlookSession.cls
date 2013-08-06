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


 
 



Private Sub Application_ItemLoad(ByVal Item As Object)
    ' MsgBox "world"

End Sub

Private Sub Application_ItemSend(ByVal Item As Object, Cancel As Boolean)

End Sub


Private Sub Application_NewMail()
    MsgBox "You got an email !!!"
End Sub

Private Sub Application_Quit()
    ' MsgBox "r u going to quit?"
End Sub

Private Sub Application_Startup()
    ' MsgBox "Hello Outlook 2010"
  Set myOlBar = Application.ActiveExplorer.Panes(1)
  Set myExplorer = Application.ActiveExplorer

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
                MsgBox "You got [" & myEmail.Attachments.Count & "] " & "attachment(s)"
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
 
 Set myOlExp = Application.ActiveExplorer
 
End Sub
 
