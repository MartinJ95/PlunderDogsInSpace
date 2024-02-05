set list= default2DShader default3DShader clickIndicator HealthBarShader selectionCircle

(for %%a in (%list%) do (
call :CleanShader %%a
))

:CleanShader
del ".\x64\Debug\%~1.vs"
del ".\x64\Debug\%~1.fs"

del ".\Game\%~1.vs"
del ".\Game\%~1.fs"
EXIT /B