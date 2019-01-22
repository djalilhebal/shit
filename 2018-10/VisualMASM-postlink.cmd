REM 2018-10-05 (Friday)
REM By "Despair Dreamski" AKA "Kaotic Kaito" AKA "Alexandros of Chaos"
REM Fo~ a bette~ MASM16 develo~ment enviro~ment~~~
REM Tested on/with Windows 10 Pro x64, VisualMASM x32 v2.00, DOSBox x32 v0.74

REM Supposing you've installed DOSBox and are using VisualMASM's default settings,
REM from VisualMASM's toolbar, select "Project > Options > Link Events > Post-link"
REM then copy and past this one-liner:
cmd /C ""C:\Program Files (x86)\DOSBox-0.74\dosbox.exe" "%CD%\Projects\Program\Release\program.exe" -noconsole -c "cls"" && exit

REM Now, to run MASM16 programs, simply click 'build' or 'run' and DOSBox will start...
REM PS: You're a big boy/girl, in case your paths are different, change them by yourself!

REM ====================<< EXPLANATION KICHGHL >>====================

REM This longer version is easier to understand and edit.
REM It works fine with CMD, but not with VisualMASM's post-link thing, I wonder why...

rem Path to DOSBox
rem it can be different depending on your software versions, may need to be edited.
SET dosbox="C:\Program Files (x86)\DOSBox-0.74\dosbox.exe"
rem The 16-bit resulting program (this is its default path)
rem (%CD% refers to VisualMASM's `Current working Directory`)
SET program="%CD%\Projects\Program\Release\program.exe"
rem Command to open that program with DOSbox. Also, hide its console and clear its logo.
rem To better tweak DOSBox, read its documentation/manual.
SET command="%dosbox% %program% -noconsole -c "cls""
rem To better tweak the CMD, check `cmd /?` and `start /?`
rem Let's rock!! >_<
cmd /C %command%
rem Done executing the program, exit now
rem (Again, for some reason, VisualMASM doesn't close the post-link console.)
exit
