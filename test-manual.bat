@echo off
setlocal EnableDelayedExpansion
chcp 65001 >nul
title InputBox - manualny test

rem =============================================================
rem  InputBox.exe - kompletny manualny testovaci beh
rem =============================================================
rem  Ako to funguje:
rem   1. Pre kazdy test sa najprv vypise, co sa ma objavit / stat.
rem   2. Stlacenim ENTER sa test spusti (spusti sa InputBox.exe
rem      s danymi parametrami).
rem   3. Over dialog / konzolu podla popisu a zavri ho.
rem   4. Potvrd vysledok: Y = spravne, N = chyba, S = preskocit.
rem   5. Vysledky sa priebezne zapisuju do test-results.txt.
rem
rem  Pouzitie:
rem   test-manual.bat                 - najde exe sam (Release/Debug)
rem   test-manual.bat "cesta\k.exe"   - pouzije zadanu cestu k exe
rem =============================================================

set "SCRIPT_DIR=%~dp0"
set "EXE="

if not "%~1"=="" set "EXE=%~1"
if not defined EXE if exist "%SCRIPT_DIR%Release\InputBox.exe" set "EXE=%SCRIPT_DIR%Release\InputBox.exe"
if not defined EXE if exist "%SCRIPT_DIR%Debug\InputBox.exe" set "EXE=%SCRIPT_DIR%Debug\InputBox.exe"
if not defined EXE if exist "%SCRIPT_DIR%InputBox.exe" set "EXE=%SCRIPT_DIR%InputBox.exe"

if not defined EXE (
    echo [CHYBA] InputBox.exe sa nenasiel v Release\, Debug\ ani v priecinku skriptu.
    echo Zostav projekt vo Visual Studiu, alebo zadaj cestu k exe ako parameter:
    echo   %~nx0 "C:\cesta\InputBox.exe"
    exit /b 1
)
for %%F in ("%EXE%") do set "EXE=%%~fF"

set "LOG=%SCRIPT_DIR%test-results.txt"

rem Testovacia ikona aplikacie patri do projektu, nie niekam do profilu
rem pouzivatela - pouziva sa priamo tá, co uz je v Resources\.
set "ICONAPP_ARG=%SCRIPT_DIR:\=/%Resources/icon1.ico"

set /a TOTAL=0
set /a PASS=0
set /a FAIL=0
set /a SKIP=0

echo. >> "%LOG%"
echo ============================================================ >> "%LOG%"
echo Test beh: %DATE% %TIME% >> "%LOG%"
echo Exe: %EXE% >> "%LOG%"
echo ============================================================ >> "%LOG%"

cls
echo ============================================================
echo   InputBox.exe - KOMPLETNY MANUALNY TEST
echo ============================================================
echo Testovany subor: %EXE%
echo.
echo Postup pre kazdy test:
echo   1. Precitaj si, co sa ma stat / objavit.
echo   2. Stlac ENTER - test sa spusti.
echo   3. Over dialog / konzolu podla popisu, potom ho zavri.
echo   4. Potvrd vysledok: Y = spravne, N = chyba, S = preskocit.
echo.
echo Vysledky sa priebezne zapisuju do: %LOG%
echo Kedykolvek mozes cely beh ukoncit cez CTRL+C.
echo ============================================================
pause

rem =============================================================
rem A. ZAKLADNE PARAMETRE A CHYBOVE STAVY
rem =============================================================
echo.
echo ############################################################
echo # A. ZAKLADNE PARAMETRE A CHYBOVE STAVY
echo ############################################################

set "ID=A1-help"
set "DESC=Vypise sa kompletny zoznam parametrov a priklad pouzitia do konzoly. Nema sa otvorit ziadne okno. Errorlevel ma byt 0."
call :runtest -help

set "ID=A2-basic"
set "DESC=Objavi sa okno s titulkom Test a spravou Zadaj meno, s prazdnym vstupnym polom. Klikni OK - text z pola sa vypise do konzoly."
call :runtest -t Test -m "Zadaj meno"

set "ID=A3-missing-message"
set "DESC=V konzole sa vypise Error - message is empty. Ziadne okno sa neotvori, errorlevel 0."
call :runtest -t BezSpravy -m ""

set "ID=A4-unknown-param"
set "DESC=V konzole sa vypise Unknown parameter a strucna sprava Run with -help for usage information - NIE cela napoveda. Ziadne okno sa neotvori."
call :runtest -neexistujuciParameter hodnota

rem =============================================================
rem B. VSTUPNE POLE
rem =============================================================
echo.
echo ############################################################
echo # B. VSTUPNE POLE
echo ############################################################

set "ID=B1-default-text"
set "DESC=Vstupne pole je vopred vyplnene textom Predvolena hodnota."
call :runtest -m "Uprav predvyplneny text" -default "Predvolena hodnota"

set "ID=B2-password"
set "DESC=Znaky vo vstupnom poli su maskovane hviezdickami namiesto citatelneho textu."
call :runtest -m "Zadaj heslo" -password

set "ID=B3-cancel"
set "DESC=Stlac ESC alebo klikni Cancel namiesto OK. Do konzoly sa nema vypisat ziadny text."
call :runtest -m "Zrus tento dialog"

rem =============================================================
rem C. VZHLAD TEXTU
rem =============================================================
echo.
echo ############################################################
echo # C. VZHLAD TEXTU
echo ############################################################

set "ID=C1-multiline"
set "DESC=Sprava sa zobrazi na troch riadkoch pod sebou, nie na jednom riadku."
call :runtest -m "Riadok jedna\nRiadok dva\nRiadok tri" -lines 3

set "ID=C2-unicode"
set "DESC=Za slovom Hotovo sa zobrazi emoji palec hore."
call :runtest -m "Hotovo \u{1F44D}"

set "ID=C3-font"
set "DESC=Text je vo fonte Arial a viditelne vacsi ako v predchadzajucich testoch - predvolena velkost je 22."
call :runtest -m "Ina pisma a velkost" -font Arial -fontsize 34

set "ID=C4-width"
set "DESC=Okno je znacne sirsie nez predchadzajuce, priblizne 1200 px - predvolena sirka je 600."
call :runtest -m "Siroke okno" -width 1200

rem =============================================================
rem D. FARBY
rem =============================================================
echo.
echo ############################################################
echo # D. FARBY
echo ############################################################

set "ID=D1-pen-color"
set "DESC=Text spravy je cerveny."
call :runtest -m "Cerveny text" -pen FF0000

set "ID=D2-background-color"
set "DESC=Pozadie dialogu je zelene."
call :runtest -m "Zelene pozadie" -background 00CC00

set "ID=D3-border-color"
set "DESC=Ramik okolo dialogu je modry."
call :runtest -m "Modry ramik" -border 0000CC

set "ID=D4-default-colors-regression"
set "DESC=Bez zadanych farieb ma okno vyzerat standardne - biely/systemovy text na normalnom pozadi, nie cierne ci rozbite pozadie."
call :runtest -m "Ziadne farby nezadane"

rem =============================================================
rem E. IKONA APLIKACIE
rem =============================================================
echo.
echo ############################################################
echo # E. IKONA APLIKACIE
echo ############################################################

set "ID=E1-iconapp"
set "DESC=V titulku okna a na paneli uloh je vlastna ikona z Resources\icon1.ico namiesto predvolenej."
call :runtest -iconapp "%ICONAPP_ARG%" -m "Vlastna ikona aplikacie"

rem =============================================================
rem F. POZICOVANIE A MONITORY
rem =============================================================
echo.
echo ############################################################
echo # F. POZICOVANIE A MONITORY
echo ############################################################

set "ID=F1-position-top"
set "DESC=Okno sa zobrazi pri hornom okraji obrazovky, vodorovne v strede."
call :runtest -m "Hore v strede obrazovky" -pos top

set "ID=F2-position-bottom"
set "DESC=Okno sa zobrazi pri spodnom okraji obrazovky, vodorovne v strede."
call :runtest -m "Dole v strede obrazovky" -pos bottom

set "ID=F3-position-xy-offset"
set "DESC=Okno sa zobrazi 100 px od laveho a 100 px od horneho okraja obrazovky."
call :runtest -m "Posunute o 100,100 od laveho horneho rohu" -pos xy -x 100 -y 100

set "ID=F4-monitor-invalid"
set "DESC=V konzole sa vypise Error - invalid monitor: abc. Ziadne okno sa neotvori, errorlevel 1."
call :runtest -mon abc -m "Neplatny monitor"

set "ID=F5-monitor-primary"
set "DESC=Okno sa zobrazi v strede primarneho monitora."
call :runtest -mon primary -pos center -m "Na primarnom monitore"

rem =============================================================
rem G. SPRAVANIE
rem =============================================================
echo.
echo ############################################################
echo # G. SPRAVANIE
echo ############################################################

set "ID=G1-topmost"
set "DESC=Po otvoreni klikni na plochu alebo inu aplikaciu - toto okno ma zostat viditelne navrchu vsetkych ostatnych."
call :runtest -m "Mal by zostat navrchu" -topmost

set "ID=G2-block"
set "DESC=Kym je dialog otvoreny, okno, z ktoreho bol spusteny - toto konzolove okno - by malo byt blokovane / neaktivne."
call :runtest -m "Blokuje rodicovske okno" -block

goto :summary

rem =============================================================
rem Pomocna rutina: spusti jeden test podla premennych ID a DESC,
rem argumenty pre InputBox.exe su prevzate z parametrov call-u.
rem =============================================================
:runtest
set ARGS=%*
set /a TOTAL+=1
cls
echo ============================================================
echo TEST !ID!   (%TOTAL%. v poradi)
echo ------------------------------------------------------------
echo Ocakavane:
echo   !DESC!
echo.
echo Prikaz: "%EXE%" !ARGS!
echo ============================================================
echo Stlac ENTER pre spustenie testu...
pause >nul

"%EXE%" !ARGS!
set "RC=%ERRORLEVEL%"

echo.
echo Dialog sa zatvoril. Navratovy kod (errorlevel): %RC%
choice /c YNS /n /m "Zodpoveda vysledok popisu vyssie? [Y]ano [N]ie [S]kip: "
if errorlevel 3 goto :runtest_skip
if errorlevel 2 goto :runtest_fail

echo [ OK ] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a PASS+=1
goto :runtest_done

:runtest_fail
echo [FAIL] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a FAIL+=1
goto :runtest_done

:runtest_skip
echo [SKIP] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a SKIP+=1

:runtest_done
exit /b 0

:summary
echo.
echo ============================================================
echo VYSLEDOK: %PASS% OK, %FAIL% chyba, %SKIP% preskocenych, spolu %TOTAL%
echo Podrobny log: %LOG%
echo ============================================================
echo Zhrnutie: %PASS% OK, %FAIL% chyba, %SKIP% preskocenych, spolu %TOTAL% >> "%LOG%"
endlocal
pause
exit /b 0
