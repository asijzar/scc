@echo off
for %%i in (*.c) do (
	..\..\proj\Debug\scc.exe -l %%i
	fc %%~ni.ans lex.out
	if ERRORLEVEL 1 ( 
		echo %%i - Fail 
	) else ( 
		echo %%i - Success 
	)
)
del lex.out