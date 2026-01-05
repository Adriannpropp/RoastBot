@echo off
geode build
geode package new .
echo copying that shit...
for %%f in (*.geode) do (
    copy "%%f" "C:\Users\adria\AppData\Local\GeometryDash\geode\mods\" >nul
    copy "%%f" "C:\Users\adria\Games_Bots_Servers\ImageToBlocks2\" >nul
)
echo done bro 🙏
pause