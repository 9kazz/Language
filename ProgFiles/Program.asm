JMP :skip_func_init_fact
fact:
PUSH 0 ; if
PUSHR RAX ; var [n]
PUSH 0 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
PUSH 1 ; num
JA :true_MORE_-1455554224
OUT
OUT
PUSH 0
JMP :false_MORE_-1455554224
true_MORE_-1455554224:
OUT
OUT
PUSH 1
false_MORE_-1455554224:
JE :if_false-1455554288
OUT
OUT
PUSHR RAX ; var [n]
PUSH 0 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
PUSH 1 ; num
SUB
PUSHR RAX ; call func [fact]
PUSH 2 ; count of vars in cur func
ADD
POPR RAX ; new RAM pointer
PUSHR RAX ; 0 parametr
PUSH 0
ADD
POPR RBX
POPM [RBX]
CALL :fact ; function call
PUSHR RAX ; call func [fact]
PUSH 2 ; count of vars in cur func
SUB
POPR RAX ; new RAM pointer
PUSHR RAX ; var [n]
PUSH 0 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
MUL
PUSH 1 ; [f] assignment
PUSHR RAX
ADD
POPR RBX
POPM [RBX]
PUSH 0 ; num
PUSH 2 ; [a] assignment
PUSHR RAX
ADD
POPR RBX
POPM [RBX]
PUSHR RAX ; var [a]
PUSH 2 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
PUSH 1 ; num
ADD
PUSH 2 ; [a] assignment
PUSHR RAX
ADD
POPR RBX
POPM [RBX]
PUSHR RAX ; var [f]
PUSH 1 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
RET
JMP :if_true-1455554288
if_false-1455554288:
OUT
OUT
if_true-1455554288:
PUSHR RAX ; var [n]
PUSH 0 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
RET
skip_func_init_fact:
PUSH 5 ; num
PUSHR RAX ; call func [fact]
PUSH 8 ; count of vars in cur func
ADD
POPR RAX ; new RAM pointer
PUSHR RAX ; 0 parametr
PUSH 0
ADD
POPR RBX
POPM [RBX]
CALL :fact ; function call
PUSHR RAX ; call func [fact]
PUSH 8 ; count of vars in cur func
SUB
POPR RAX ; new RAM pointer
PUSH 0 ; [y] assignment
PUSHR RAX
ADD
POPR RBX
POPM [RBX]
PUSHR RAX ; var [y]
PUSH 0 ; relative_idx
ADD
POPR RBX
PUSHM [RBX]
PRNT
HLT
