@echo off
for /F "tokens=1,2" %%A in ('git log --format^="%ct %H"') do (
    set timestamp=%%A
    set commit=%%B
    call :process_commit
)
goto :eof

:process_commit
git filter-branch --env-filter "set GIT_COMMITTER_DATE=%timestamp% && set GIT_AUTHOR_DATE=%timestamp%" %commit%
goto :eof