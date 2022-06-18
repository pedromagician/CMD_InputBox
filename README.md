# InputBox
InputBox for Windows command line.

![bench](https://raw.githubusercontent.com/pedromagician/CMD_InputBox/main/pic/screenshot.png)

@echo|set /p="@set mytmp=">1.bat\
@inputbox -message "Message ..." >>1.bat\
@call 1.bat\
@echo %mytmp%
