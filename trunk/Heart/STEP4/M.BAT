cls
@echo off
del heart.obj
del heart.exe
del $$$$$$$$.$$$
echo ------------------------------------------------------------------------
echo MASMing heart.asm...
ml /c /Zi heart.asm
echo MASM heart.asm done.
echo ------------------------------------------------------------------------
echo LINKing heart.obj...
link /CODEVIEW heart;
echo LINK heart.obj done.
echo Everything is ok.
@echo on
