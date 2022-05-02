```

<entry> := <use-statement>
         | <import-statement>
         | <function-statement>
         | <struct-statement>
         | <container-statement>

<use-statement> := 'use' <identifier> ';'
                 | <use-statement> 'use' <identifer> ';'

<import-statement> := 'import' <string> ';'
                    | <import-statement> 'import' <string> ';'

<function-statement> := 'fn' <identifier> '(' <parameter-declaration> ')' '->' <identifer> <statement-block>

<struct-statement> := 'struct' <identifer> '{' [<struct-definition-block>] '}' 

<struct-definition-block> := <identifier> ':' <identifier> ';'
                           | <struct-definition-block> <identifier> ':' <identifier> ';'

<container-statement> := 'container' '<' <identifier> '>' ':' <container-sizing> <identifier> ';'

<container-sizing> := 'fixed'
                    | 'variable'

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