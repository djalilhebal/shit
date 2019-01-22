REM looper.cmd -- 2018-05-04 (Friday)
REM to download a huge file that fails often on my poor internet cnx >.<
REM it is super simple/basic/unimpressive yet it kept me from losing my mind <3

:LOOP

aria2c https://bitport.io/[...] -c

goto :LOOP
