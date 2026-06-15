# InputBox

A lightweight, script‑friendly GUI input dialog for Windows command line.
Designed for batch files, automation, PowerShell scripts, and tools that need simple user input without building a full UI.

# ✨ Features

- Simple invocation from CMD or PowerShell
- Unicode support (\u{XXXX})
- Password masking mode
- Customizable colors, fonts, and window size
- Multi‑monitor support
- Precise window positioning
- Optional modal blocking
- Custom application icon support

# 🚀 Basic Usage

```bat
InputBox.exe -t "Login" -m "Enter your username:"

InputBox.exe -t "Password" -m "Enter password:" -password

InputBox.exe -t "Search" -m "Enter query:" -default "example"
```

## 🧰 Command‑line Options

| Switch | Description |
|--------|-------------|
| `-help`, `-h`, `-?` | Displays help information. |
| `-title`, `-t` | Sets the dialog window title. |
| `-message, -m` | Sets the message text. Supports \n and Unicode escapes. |
| `-default, -d` | Pre‑fills the input field. |
| `-password, -pass` | Masks input characters with *. |
| `-font, -f` | Sets the font name. |
| `-fontsize, -fs` | Sets the font size in points. |
| `-lines, -l` | Number of lines for the message area. |
| `-width, -w` | Dialog width in pixels. |
| `-pen, -p` | Text/foreground color (#RRGGBB). Default: #FFFFFF. |
| `-background, -b` | Background color. Default: #000000. |
| `-brush, -br` | Fill/pattern color. Default: #000000. |
| `-border, -bo` | Frame color. Default: #000000. |
| `-monitor, -mon` | Target monitor: Primary, Mouse, MousePointer, or index (0, 1, 2, ...). |
| `-position, -pos` | Placement: center, top, bottom, left, right, pointer, xy. |
| `-x` | X‑axis offset in pixels. |
| `-y` | Y‑axis offset in pixels. |
| `-topmost` | Keeps the dialog above other windows. |
| `-iconapp` | Path to ICO/BMP icon. |
| `-block` | Blocks the parent process until the dialog closes. |


# 📦 Examples

Custom Colors:
```bat
InputBox.exe -t "Color Test" -m "Nice colors!" -background #202020 -pen #00FFAA
```

Show on Monitor 2
```bat
InputBox.exe -t "Monitor 2" -m "Hello!" -mon 1
```

# 🧩 Batch Script Example

Store the selected value into a variable:

```bat
@echo|set /p="@set mytmp=">tmp.bat
@inputbox -message "Message ..." >>tmp.bat
@call tmp.bat
@echo %mytmp%
```

Or capture the output directly:

```bat
@SET mytmp=null
@FOR /F "tokens=*" %%i IN ('InputBox.exe -title Title -message "Message..." -default text') DO @SET mytmp=%%i
echo %mytmp%
```

# 📜 License

Free to use.
Amiga Rulez!
