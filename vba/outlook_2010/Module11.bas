Attribute VB_Name = "Module11"
Option Explicit
Dim myDoc As Word.Application


Public Sub SaveAttach(Item As Outlook.MailItem)
    SaveAttachment Item, "c:\mailattachments2\", "*"
End Sub
'########################################

Private Sub SaveAttachment(ByVal Item As Object, path$, Optional condition$ = "*")
    Dim olAtt As Attachment
    Dim i As Integer
    Dim isFolderExists
    Dim myDate
    Dim yourDate
    Dim yourPath
    
    yourDate = Date
    yourDate = Replace(yourDate, "/", "-")
    ' MsgBox "date=" & yourDate

    yourPath = path & "\" & yourDate & "\"
    
    Set isFolderExists = CreateObject("Scripting.FileSystemObject")

    If isFolderExists.FolderExists(yourPath) = True Then
        '
    Else
        ' MsgBox "path=" & path & "\" & yourDate
        MkDir yourPath
        ' MsgBox "Dir created"
    End If
    

    If Item.Attachments.Count > 0 Then
        For i = 1 To Item.Attachments.Count
            Set olAtt = Item.Attachments(i)
            ' save the attachment
            If olAtt.FileName Like condition Then
                olAtt.SaveAsFile yourPath & olAtt.FileName
            End If
        Next
    End If
    Set olAtt = Nothing
End Sub



Sub saveEmailToWord(ByRef mail As MailItem)
    Dim newRange As Word.Range
    Dim M As MailItem, Buf As MSForms.DataObject
    
     
    
    Set myDoc = CreateObject("Word.Application")
    myDoc.Visible = True
    myDoc.Documents.Open "C:\data\step2\email\log.docx"
    ' myDoc.Documents(1).Range.Select
    myDoc.Selection.StartOf Unit:=wdStory, Extend:=wdMove
    ' myDoc.Selection.Paste
    myDoc.Selection.Text = Chr(13) & "[Received Time] =====> " & mail.ReceivedTime & Chr(13) & "[Subject] =====> " & mail.Subject & " <=====" & Chr(13) & "[Content] =====>" & Chr(13) & mail.Body & Chr(13)
    ' ##############################################
    ' ##############################################
    ' ##############################################
    ' ##############################################
    ' ##############################################
    'Set M = mail.Application.ActiveExplorer.Selection.Item(1)
    'Set Buf = New MSForms.DataObject
    'mail.BodyFormat = olFormatHTML
    'Buf.SetText mail.Body
    'Buf.PutInClipboard


    'myDoc.Documents(1).Range.Paste


    myDoc.Documents(1).Range.Select
    myDoc.Documents(1).Paragraphs.LineSpacingRule = wdLineSpaceSingle
    
    myDoc.Documents(1).Save
    myDoc.Quit
End Sub



