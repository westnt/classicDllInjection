# classicDllInjection
Example of classic dll injection.
# Overview
inject.cpp - The injector.  
payload.cpp - dll to be injected.  
target.cpp - dummy program to inject into.  
1) allocate RW memory in target process.
2) write string containing path to dll in just allocated memory (strPtr).
3) Create a remote thread in target process using LoadLibrary as start routine and strPtr as thread argument.
# Installation And Usage
Using vc comandline compiler and vcvars32.bat.

compile payload.dll
```
cl \c src\payload.dll
link payload.obj /DLL
```
compile target and injector
```
cl src\inject.cpp
cl src\target.cpp
```

# Usage
start target process
```
.\target.exe
```
inject payload.exe into target.exe
```
.\inject.exe
```