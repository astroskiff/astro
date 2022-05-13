# IR

The IR for this compiler will be a stack based representation
of operations that can be easily converted into the target language


```
; Entry function
label "main"
assign argc   ; Assign parameters
assign argv
pop 8 argc    ; Pop n-bytes into variables
pop 10 argv


; let my_var:u8 = 10 + 25;
push 8 10
push 8 25
add
assign my_var
pop 8 my_var


; let some_float:double = 3.14159 + 2.0;
push 8 3.14159
push 8 2.0
add
assign some_float
pop 8 some_float

asm "mov i0 @0"   ; Direcet asm injection

```