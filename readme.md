# ncs

## USAGE
nemu [-d] [-q] [FILE]...

-d  debug
-q  quiet


q     quit
e     exit

r     run
c     continue
t     terminate

d     debug
si    single instruction

info  show registers information
x     show memory information
p     show breakpoint and watchpoint
bt    show stack frame linked lists
b     create a breakpoint
w     create a watchpoint
d     delete a breakpoint or watchpoint

## COMPATIBILITY

- Support only 80386 protection mode
    - No support for backward compatibility with 80286, etc
    - No support for 80386 real-address mode and virtual 8086 mode
- No support for 16-bit addressing instruction
