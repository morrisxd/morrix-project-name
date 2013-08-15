Attribute VB_Name = "Module11"
Option Explicit
Dim myDoc As Word.Application


Public Sub SaveAttach(Item As Outlook.MailItem)
    SaveAttachment Item, "c:\mailattachments2\", "*"
End Sub
'########################################

Private Sub SaveAttachment(ByVal Item As Outlook.MailItem, path$, Optional condition$ = "*")
    Dim olAtt As Attachment
    Dim i As Integer
    Dim isFolderExists
    Dim mydate
    Dim yourDate
    Dim yourPath
    

    yourPath = checkDirectory(path)

    If Item.Attachments.Count > 0 Then
        For i = 1 To Item.Attachments.Count
            Set olAtt = Item.Attachments(i)
            ' save the attachment
            ' If olAtt.FileName Like condition Then
                olAtt.SaveAsFile yourPath & olAtt.FileName
            ' End If
        Next
    End If
    Set olAtt = Nothing
    Set isFolderExists = Nothing
End Sub



Sub saveEmailToWord(ByRef mail As MailItem)
    Dim newRange As Word.Range
    Dim M As MailItem, Buf As MSForms.DataObject
    Dim insp As Outlook.Inspector
    Dim doc As Word.Document
    Dim app As Word.Application
    Dim Format
    Dim ActiveExplorer As Outlook.Explorer
    Dim docFilename
    Dim mydate, mytime, mysubject
    Dim path
    Dim isFolderExists
    Dim yourDate
    Dim yourPath
    
    
    path = "C:\data\step2\email\"
    yourPath = checkDirectory(path)
    
    
    Set app = CreateObject("word.application")
    Format = Word.WdSaveFormat.wdFormatDocumentDefault
    Set ActiveExplorer = Application.Explorers.Application.ActiveExplorer
    
     ' Word.Application app = New Word.Application
     ' Object Format = Word.WdSaveFormat.wdFormatDocumentDefault
     ' Outlook.Explorer ActiveExplorer = this.Application.Explorers.Application.ActiveExplorer()

    
     
    
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
    ' Set M = mail.Application.ActiveExplorer.Selection.Item(1)
    ' Set Buf = New MSForms.DataObject
    ' mail.BodyFormat = olFormatHTML
    ' Buf.SetText mail.Body
    ' Buf.SetText mail.GetInspector.WordEditor
    ' Buf.PutInClipboard
    Set insp = mail.GetInspector
    Set doc = insp.WordEditor

    mydate = Replace(Date, "/", "-")
    mytime = Replace(Time, ":", "-")
    mysubject = mail.Subject
    mysubject = Replace(mysubject, "/", "-")
    mysubject = Replace(mysubject, ":", "-")
    mysubject = Replace(mysubject, "*", "-")
    mysubject = Replace(mysubject, "?", "-")
    mysubject = Replace(mysubject, "<", "[")
    mysubject = Replace(mysubject, ">", "]")
    mysubject = Replace(mysubject, "|", "!")
    mysubject = Replace(mysubject, "*", "x")
    ' docFilename = Replace(docFilename, ":", "=")
    
    If mail.Attachments.Count > 0 Then
        docFilename = yourPath & "\" & mydate & "_" & mytime & "_[ATT]_" & mysubject & ".docx"
    Else
        docFilename = yourPath & "\" & mydate & "_" & mytime & "_" & mysubject & ".docx"
    End If
    
    ' MsgBox docFilename
    doc.SaveAs2 docFilename
    'myDoc.Documents(1).Range.Paste


    myDoc.Documents(1).Range.Select
    myDoc.Documents(1).Paragraphs.LineSpacingRule = wdLineSpaceSingle
    
    myDoc.Documents(1).Save
    myDoc.Quit
    
    Set app = Nothing
    Set ActiveExplorer = Nothing
    Set myDoc = Nothing
    Set insp = Nothing
    Set doc = Nothing
End Sub



Function checkDirectory(ByVal myDir)
    Dim yourDate
    Dim yourPath
    Dim isFolderExists
    
    
    yourDate = Date
    yourDate = Replace(yourDate, "/", "-")
    ' MsgBox "date=" & yourDate

    myDir = myDir & "\"
    yourPath = myDir & "\" & yourDate & "\"
    checkDirectory = yourPath
    ' MsgBox "yourpath=" & yourPath
    
    Set isFolderExists = CreateObject("Scripting.FileSystemObject")
    
    If isFolderExists.FolderExists(yourPath) = True Then
        ' do nothing
    Else
        ' MsgBox "path=" & path & "\" & yourDate
        MkDir yourPath
        ' MsgBox "Dir created"
    End If
    
    Set isFolderExists = Nothing
    
    checkDirectory = yourPath
End Function

