@echo off

if "%1"=="-t" (
    @echo found -t
) else (
    dir /s /b . 
)

