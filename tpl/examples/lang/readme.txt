+ Introduction

This application is a simple language interreptor.

+ Language Definition

<lang> ::= (<assignment> ';')*

<assignment> ::= 
	c_symbol() (
		'=' <assignment> |
		'(' c_symbol() % ',' ')' '=' <assignment> ) | 
	<expr>

<expr> ::= <andexpr> ('||' <andexpr>)*

<andexpr> ::= <term> <compare> ('&&' <term> <compare>)*

<compare> ::= '<=' <term> | '<' <term> | '>=' <term> | '>' <term> | '==' <term> | '!=' <term>

<term> ::= <term2> ('+' <term2> | '-' <term2>)*

<term2> ::= <factor> ('*' <factor> | '/' <factor>)*

<factor> ::=
	real() |
	c_string() |
	'-' <factor> |
	'+' <factor> |
	'(' <assignment> % ',' ')' |
	c_symbol() ('(' <assignment> % ',' ')')? |
	'if' '(' <expr> ')' <assignment> ('elif' '(' <expr> ')' <assignment>)* 'else' <assignment> |
	'case' <expr> 'of'
		((<compare> | <expr>) ':' <assignment>)*
		'default' ':' <assignment>

<skipper> ::=
	skipws() % ('//' find_eol<true>())

+ Example

weekday(n) =
	case n of
		1: "Mon",
		2: "Tue",
		3: "Wed",
		4: "Thu",
		5: "Fri",
		6: "Sat",
		7: "Sun",
		default: "Error";

abs(x) =
	case x of
		< 0: -x,
		default: x;

abs(x) = if (x < 0) -x else x;

factal(n) = if (n <= 1) 1 else n * factal(n-1);

pi = 3.1415926;

x = abs(sin(-pi/4));

print("x=", x);

times(n) =
(
	fun(y) = 
	(
		val = n * y,
		val
	),
	fun
);

times(n) =
	fun(y) =
	(
		val = n * y,
		val
	);
	
times(n) =
	fun(y) = n * y;

twice = times(2);

y = twice(3); 	// y = 6

print("y=", y);

