set spath=%path%
set watcom=c:\lang\ms8amd64
set path=%watcom%\bin;%path%
set include=%watcom%\include;\quirt\comps\dll\cqdicom;\quirt\comps\exe\dgate\lua-5.1.4
set lib=%watcom%\lib

cl disas.c

set spath=%path%
