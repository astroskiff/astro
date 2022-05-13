# Middle Earth

Once the parsed trees are completed they are fed to the semantic analyzer. 
The SA is where dreams go to die. Once an instruction is deemed valid from the SA
it will call into generation to have some IR code generated. 

The SA is what calls into the generator as this ensures all information required to 
generate an instruction is as short lived as possible. There is no point in keeping
meta data around and re-iterating things.

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

push 17 "This is my string" ; Push a 17-byte string 

; SKiff target can assume anything > 8 is a string and const encode it

```