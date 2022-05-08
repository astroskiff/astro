```
"Case Sensitive" = False 
"Start Symbol"   = <Lines>

{String Chars} = {Printable} - ["]
{WS}           = {Whitespace} - {CR} - {LF}

NewLine        = {CR}{LF}|{CR}
Whitespace     = {WS}+

Remark         = REM{Space}{Printable}*
ID             = {Letter}[$%]? 
string         = '"'{String Chars}*'"' 
integer        = {digit}+ 
float          = {digit}+.{digit}+ 

<entry> := <statements>+

<statement> := GOTO ID ';'
             | GOSUB ID ';'
             | RETURN ';'
             | PRINT <expression-list> ';'
             | LET ID '=' <expression>,
             | <for-loop>
            
<for-loop> := FOR ID '=' <expression> 'to' <expression> <statement>+ END
            | FOR ID '=' <expression> 'to' <expression> 'step' <integer> <statement>+ END

<expression> := <prefix> [<infix+>]

<expression-list> := <expression>
                    | ',' <expression-list>

<prefix> := <id>
          | <integer> 
          | <float>
          | <string>
          | <prefix> 
          | '(' <expression> ')'
          | '{' <expression-list> '}'

<infix> := '=' <expression>
          | '=' '=' <expression>
          | '!' '=' <expression>
          | '<' <expression>
          | '>' <expression>
          | '<' '=' <expression>
          | '>' '=' <expression>
          | '+' <expression>
          | '-' <expression>
          | '/' <expression>
          | '*' <expression>
          | '%' <expression>
          | '^' <expression>
```