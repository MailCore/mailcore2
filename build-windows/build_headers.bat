@mkdir include
@mkdir include\MailCore
@for /F "delims=" %%a in (build_headers.list) do @copy "..\src\%%a" include\MailCore
@echo "done" >_headers_depends
