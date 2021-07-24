msbuild .\build\GDExt_Playtest.sln /p:Configuration=Release /p:PlatformTarget=x86 /m
cp build/Release/GDExt_Playtest.dll "C:/Program Files (x86)/Steam/steamapps/common/Geometry Dash/quickldr/GDExt_Playtest.dll"
start "Geometry Dash" "steam://rungameid/322170"