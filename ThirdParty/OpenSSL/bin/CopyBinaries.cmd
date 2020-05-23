@echo off
robocopy %~dp0 %1 /xo /fft ^
    libcrypto.dll ^
    libssl.dll    ^
