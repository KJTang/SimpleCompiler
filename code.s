op
op
op
op
op
op
op
INT g
PUSH 5
PUSH 3
PUSH 2
DIV
PUSH 1
PUSH 3
ADD
MUL
SUB
SAVE g
FUNC add
LOAD a
LOAD b
ADD
RET
ENDF
FUNC main
INT a
PUSH 0
SAVE a
INT b
PUSH 1
SAVE b
LOAD a
LOAD b
ADD
PUSH 1
