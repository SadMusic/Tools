 for %%i in (*.proto) do (    
echo %%i  
"protoc.exe" --plugin=protoc-gen-lua="plugin\protoc-gen-lua.bat" --lua_out=. %%i  
)  
echo end  
pause