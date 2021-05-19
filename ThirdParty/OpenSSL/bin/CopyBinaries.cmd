@echo off
robocopy %~dp0 %1 /xo /fft ^
    libcrypto-1_1-x64.dll ^
    libssl-1_1-x64.dll
