@cd %~dp0
for /d %%i in (*) do if exist %%i\*.cpp clang-format.exe -i %%i/*.cpp
for /d %%i in (*) do if exist %%i\*.h clang-format.exe -i %%i/*.h