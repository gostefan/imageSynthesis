FOR /F "tokens=*" %%G IN ('DIR /B /AD /S *build*') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /S *.ncb*') DO DEL "%%G"
del bin\*.exe bin\*.dll bin\*.pdb