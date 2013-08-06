VERSION 5.00
Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} userInterface 
   Caption         =   "Progress"
   ClientHeight    =   1560
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   8550
   OleObjectBlob   =   "userInterface.frx":0000
   StartUpPosition =   1  'CenterOwner
End
Attribute VB_Name = "userInterface"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim cnt
Private Sub CommandButton1_Click()
    ' merge_for_maomao.process_for_maomao
End Sub

Sub updateProgress(ByVal percent)
    Me.Caption = Format(percent, "0%")
    LabelProgress.Width = percent * (frmProcessing.Width)
    ' Me.Repaint
    DoEvents
End Sub

