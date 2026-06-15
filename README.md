# InputBox

A lightweight, customizable input dialog for the Windows command line.

Repository: [https://github.com/pedromagician/CMD_InputBox]

Note: The Microsoft Visual C++ x86 Redistributable is required. See: [https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170]

![bench](https://raw.githubusercontent.com/pedromagician/CMD_InputBox/main/pic/screenshot.png)

# 🎯 Overview

`InputBox.exe` is a small Windows utility that displays a graphical input dialog and returns the user’s input back to the command line.

# 📦 Basic Usage

```bat
InputBox.exe -message "Enter your name:"
```

# 🧩 Batch Script Example

Store the selected value into a variable:

```bat
@echo|set /p="@set mytmp=">1.bat
@inputbox -message "Message ..." >>1.bat
@call 1.bat
@echo %mytmp%
```

Or capture the output directly:

```bat
@SET mytmp=null
@FOR /F "tokens=*" %%i IN ('InputBox.exe -title Title -message "Message..." -default text') DO @SET mytmp=%%i
echo %mytmp%
```

# 🔧 Command‑Line Options

Text & Content:

| Option | Alias | Type | Description |
|--------|-------|------|-------------|
| `-title ...` | `-t` | `string` | Sets the dialog title. |
| `-message ...` | `-m` | `string` | Sets the dialog message. Use \n for new lines, Unicode via \u{XXXX}. |
| `-default ...` | `-d` | `string` | Sets the default text in the input field. |
| `-password` | `-pass` | | Masks the input with * characters. |

Font & Layout:

| Option | Alias | Type | Description |
|--------|-------|------|-------------|
| `-font ...` | `-f` | `string` | Sets the font name. |
| `-fontsize n` | `-fs` | `int` | Sets the font size. |
| `-lines n` | `-l` | `int` | Number of message lines. |
| `-width n` | `-w` | `int` | Dialog width in pixels. |

Colors:

| Option | Alias | Type | Description |
|--------|-------|------|-------------|
| `-pen color` | `-p` | `color` | Pen color. |
| `-background color` | `-b` | `color` | Background color. |
| `-brush color` | `-br` | `color` | Brush color. |
| `-border color` | `-bo` | `color` | Border color. |

___Color formats supported: #RRGGBB___

Positioning & Monitors:

| Option | Alias | Type | Description |
|--------|-------|------|-------------|
| `-monitor ...` | `-mon` | `string` | Selects monitor: Primary, Mouse, MousePointer, or index 0..n. |
| `-position ...` | `-pos` | `enum` | Position: Center, Top, Bottom, Left, Right, Pointer, xy. |
| `-x n` |  | `int` | X‑axis offset. |
| `-y n` |  | `int` | Y‑axis offset. |
| `-topmost` |  |  | Forces the dialog to stay above non‑topmost windows. |

Icons & Behavior

| Option | Alias | Type | Description |
|--------|-------|------|-------------|
| `-iconapp file.ico` |  | `string` | Sets the application icon (ICO/BMP). |
| `-block` |  |  | Blocks the parent process until the dialog closes. |
| `-help` | `-h, -?` |  | Displays help. |

# 📜 License

Free to use.
Amiga Rulez!
