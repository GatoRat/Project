This subtree contains the needed build scripts.

OpenSSL                 Scripts
  readme.txt            This file
  *.cmd                 Build command scripts

You must download the following:
  nasm.exe              The netwide assembler 
  openssl               OpenSSL

OpenSSL for Windows platform
----------------------------

1) Install ActivePerl.
2) If this is a new version:
   a) Delete old 'src' directory (if present)
   b) Extract the innermost openssl-* folder under the OpenSSL folder.
   c) Rename the created openssl-* folder to 'src'.
3) Copy nasm.exe to folder.
4) Open a Command Prompt.
5) Run the proper command file
   Note: Ignore the notice that nmake is not in path. (It seems that perl generates this erroneous message.
                                                       It happens around line 1156 of configure and I have
                                                       no idea what that code is doing.)


OpenSSL for Mac platform
--------------------------

[This needs to be corrected]
For conformance reasons, use the standard OpenSSL install process on Mac. Use either an OpenSSL tarball from 
www.openssl.org, or the OpenSSL module.  The make process (linux.mak) picks up the header files and libraries 
from /usr/local/ssl. 


OpenSSL for Linux platform
--------------------------

[This needs to be corrected]
For conformance reasons, use the standard OpenSSL install process on Linux.  Use either an OpenSSL tarball from 
www.openssl.org, or the OpenSSL module.  The make process (linux.mak) picks up the header files and libraries 
from /usr/local/ssl. 
