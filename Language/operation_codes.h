
#ifndef OPCODES
#define OPCODES

enum OPERATION_CODES {
	SEMICOLON = 0,
	ADD = 1,
	SUB = 2,
	MUL = 3,
	DIV = 4,
	POW = 5,
	SIN = 6,
	COS = 7,
	TG  = 8,
	CTG = 9,
	ARCSIN = 10,
	ARCCOS = 11,
	ARCTG  = 12,
	ARCCTG = 13,
	LN     = 14,
	LG 	   = 15,
	EQUAL = 16,
};

const int OPERATIONS_LENGTH[] = {
	1,
	1,
	1,
	1,
	1,
	1,
	3,
	3,
	2,
	3,
	6,
	6,
	5,
	6,
	2,
	2,
	1
};

const char TEXT_OPERATIONS[][9] = {
	";",
	"+",
	"-",
	"*",
	"/",
	"^",
	"sin",
	"cos",
	"tg",
	"ctg",
	"arcsin",
	"arccos",
	"arctg",
	"arcctg",
	"ln",
	"lg",
	"="
};

const int COUNT_OF_ARGS[] = {
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	2
};

const int LENGTH_PHRASE_TO_VARIABLE_DECLARATION = 52;
const char PHRASE_TO_VARIABLE_DECLARATION[128] = "Introducing a new word into the Newspeak Dictionary:";

#endif
