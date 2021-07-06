
#ifndef OPCODES
#define OPCODES

enum OPERATION_CODES {
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
	LG     = 14
};

const int OPERATIONS_LENGTH[] = {
	0,
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
	2
};

const char TEXT_OPERATIONS[][9] = {
	"",
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
	"lg"
};

const int COUNT_OF_ARGS[] = {
	0,
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
	1
};

#endif
