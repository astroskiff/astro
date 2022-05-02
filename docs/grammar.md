```

<entry> := <use>
         | <entry> <function-statement>+
         | <entry> <struct-statement>+

<use> := 'use' <identifier>
      | <use> 'use' <identifer>

<function-statement> := 'fn' <identifier> '(' <parameter-declaration> ')' '->' <identifer> <statement-block>

<parameter-declaration> := <identifier> ':' <identifier>
                          | <<parameter-declaration> ',' <identifier> ':' <identifier>

<statement-block> := '{' <statments>+ '}'

statements := <assignment>
            | <if-statement>
            | <while-statement>
            | <for-statement>
            | <return-statement>
            | <expression>

<assignment> := 'let' <identifier> ':' <identifier> ['*'] '=' <expression> ';'

<if-statement> := 'if' <conditional> <statement-block> [<else-if>+] [<else>]

<else-if> := 'else' 'if' <statement-block>

<else> := 'else' <statement-block>

<while-statement> := 'while' <conditional> <statemebt-block>
 
<conditional> := '(' expression ')'

<for-statement> := 'for' '(' [<assignment>] ';' <expression> ';' [<expression>] ')' <statement-block>

<identifier> := <letter> | <identifier> <letter> | <identifier> <number> | <identifier> '_'

<letter> := ALPHANUM

<number> := DECIMAL_NUMBER

<float> := <number> '.' <number>+

<string> := '"' <chars>+ '"'

<expression> := <prefix> [<infix+>]

<expression-list> := <expression>
                    | ',' <expression-list>

<prefix> := <identifier>
          | <number> 
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
          | <call-expr>
          | <expr-accessors>

<call-expr> := <identifier> '(' [<call_parameter_list>] ')' 

<call_parameter_list> := <expression> 
                       | <call_parameter_list> ',' <expression>
```