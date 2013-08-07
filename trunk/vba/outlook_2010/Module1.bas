Attribute VB_Name = "Module1"
Option Explicit

Public Sub SaveAttach(Item As Outlook.MailItem)
    SaveAttachment Item, "c:\mailattachments\", "*"
End Sub

Private Sub SaveAttachment(ByVal Item As Object, path$, Optional condition$ = "*")
    Dim olAtt As Attachment
    Dim i As Integer

    If Item.Attachments.Count > 0 Then
        For i = 1 To Item.Attachments.Count
            Set olAtt = Item.Attachments(i)
            ' save the attachment
            If olAtt.FileName Like condition Then
                olAtt.SaveAsFile path & olAtt.FileName
            End If
        Next
    End If
    Set olAtt = Nothing
End Sub
