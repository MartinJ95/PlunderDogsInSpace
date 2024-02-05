set list= default2DShader default3DShader clickIndicator HealthBarShader selectionCircle

(for %%a in (%list%) do (
call :DistributeShader %%a
))

:DistributeShader
robocopy ".\Graphics" ".\Game" %~1.vs
robocopy ".\Graphics" ".\Game" %~1.fs

robocopy ".\Graphics" ".\x64\Debug" %~1.vs
robocopy ".\Graphics" ".\x64\Debug" %~1.fs
EXIT /B