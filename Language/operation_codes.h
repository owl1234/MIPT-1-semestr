
#ifndef OPCODES
#define OPCODES

const int ALPHABET = 26;

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

enum COMPARISON_SIGNS {
	SIGN_NOTHING		= 0,
	SIGN_EQUAL 			= 1,
	SIGN_LESS 			= 2,
	SIGN_LESS_OR_EQUAL 	= 3,
	SIGN_MORE 			= 4,
	SIGN_MORE_OR_EQUAL  = 5
};

const char TEXT_COMPARISON_SIGNS[][7] = {
	"",
	"==",
	"<",
	"<=",
	">",
	">="
};

const int LENGTH_TEXT_COMPARISON_SIGNS[] = {
	0,
	2,
	1,
	2,
	1,
	2
};

enum CONDITION_TYPES {
	CONDITION = 0,
	ACTIONS   = 1,
};

const int COUNT_OF_COMPARISON_SIGNS = 5;

const int LENGTH_PHRASE_TO_VARIABLE_DECLARATION 		= 52;
const int LENGTH_PHRASE_TO_CONDITION					= 33;
const int LENGTH_PHRASE_BEGIN_CONDITION					= 2;
const int LENGTH_PHRASE_END_CONDITION					= 2;
const int LENGTH_PHRASE_BEGIN_ACTION_1					= 25;
const int LENGTH_PHRASE_BEGIN_ACTION_2					= 39;
const int LENGTH_PHRASE_BEGIN_BLOCK						= 1;
const int LENGTH_PHRASE_END_BLOCK						= 1;
const int LENGTH_PHRASE_PRINT 							= 49;

const char PHRASE_TO_VARIABLE_DECLARATION[128]  = "Introducing a new word into the Newspeak Dictionary:";
const char PHRASE_TO_CONDITION			 [128] 	= "if Big Brother didn't notice that";
const char PHRASE_BEGIN_CONDITION		 [4] 	= "<<";
const char PHRASE_END_CONDITION			 [4] 	= ">>";
const char PHRASE_BEGIN_ACTION_1		 [128]	= "then calmly exhale and do";
const char PHRASE_BEGIN_ACTION_2		 [128] 	= "otherwise you will be taken to room 101";
const char PHRASE_BEGIN_BLOCK		 	 [2] 	= "{";
const char PHRASE_END_BLOCK				 [2] 	= "}";
const char PHRASE_PRINT 				 [64]   = "We will bring praise to Big Brother on TV screens";


#endif
