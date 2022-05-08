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
             | LET ID [<type-specifier>] '=' <expression>,
             | <for-loop>
             | <if>
            
<type-specifier> := ':' ID

<for-loop> := FOR ID '=' <expression> 'to' <expression> <statement_block> 
            | FOR ID '=' <expression> 'to' <expression> 'step' <integer> <statement_block> 

<if> := IF <expression> <statement_block>  [<elseif>+] [<else>] END
<elseif> := ELIF <expression> <statement_block> 
<else> := ELSE <statement_block> 

<statement_block> := '{' <statement>+ '}'

<expression> := <prefix> [<infix+>]

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