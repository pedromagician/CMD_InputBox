# InputBox
InputBox for Windows command line.

> Redistributable x86 packages [https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170] are required to run the program

![bench](https://raw.githubusercontent.com/pedromagician/CMD_InputBox/main/pic/screenshot.png)

```
@echo|set /p="@set mytmp=">1.bat\
@inputbox -message "Message ..." >>1.bat\
@call 1.bat\
@echo %mytmp%
```
