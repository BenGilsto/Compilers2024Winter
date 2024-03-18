/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|              ....                     |
|          ........::.::::::.           |
|        .:........::.:^^^~~~:          |
|        :^^::::::^^^::^!7??7~^.        |
|       .:^~~^!77777~~7?YY?7??7^.       |
|       :.^~!??!^::::^^~!?5PY??!~.      |
|       ~!!7J~.:::^^^^~!!~~?G5J?~       |
|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |
|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |
|       .~77..    . .~^:^^^~7?:         |
|       .^!^~:::.:^!7?~^~!77J:          |
|        ^^!Y~^^^^~?YJ77??7JJ^          |
|       .^7J?~^~~^~7??7??7JY?~:         |
|        ::^^~^7?!^~~!7???J?J7~:.       |
|         ^~~!.^7YPPPP5Y?7J7777~.       |
|        ..:~..:^!JPP5YJ?!777!^.        |
| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |
|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |
|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |
|    :~75 \__ \/ _ \  _| / _` | 5?.     |
|     7~7 |___/\___/_| |_\__,_| Y5?.    |
|    .^~!~.....................P5YY7.   |
|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |
|                                       |
=---------------------------------------=
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */
#define STR_LEN 20  /* String literal length */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 16

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,   /* Error token */
	VID_T,   /* Variable identifier token */
	FID_T,   /* Function identifier token */
	IL_T,    /* Integer literal token */
	SL_T,    /* String literal token */
	LPR_T,	 /* Left parenthesis token */
	RPR_T,	 /* Right parenthesis token */
	LBR_T,	 /* Left brace token */
	RBR_T,	 /* Right brace token */
	KW_T,    /* Keyword token */
	OP_T,    /* Operator token */
	CM_T,    /* Comment token */
	EOS_T,   /* End of statement token */
	RTE_T,   /* Run-time error token */
	LOOP_T,  /* Logical Operator token*/
	AROP_T,  /* Arithmetic operator token */
	CMT_T,   /* Current token*/
	REOP_T,	 /* Relational Operator token */
	SEOF_T   /* Source end-of-file token */
};

/* TO_DO: Define the list of keywords */
static atys_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"LOOP_T",
	"AROP_T",
	"REOP_T",
	"SEOF_T",
	"CMT_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { ADD, SUB, MUL, DIV } AriOperator;
typedef enum RelationalOperators { EQ, NE, GT, LT } RelOperator;
typedef enum LogicalOperators { AND, OR, NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	atys_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	atys_intg intValue;				/* integer literal attribute (value) */
	atys_intg keywordIndex;			/* keyword index in the keyword table */
	//atys_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	atys_real floatValue;				/* floating-point literal attribute (value) */
	atys_char stringValue[STR_LEN + 1];  /* String literal value */
	atys_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	atys_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	atys_byte flags;			/* Flags information */
	union {
		atys_intg intValue;				/* Integer value */
		atys_real floatValue;			/* Float value */
		atys_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	atys_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	atys_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

 /* TO_DO: Define lexeme FIXED classes */
 /* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '&'
#define CHRCOL4 '\''
#define CHRCOL6 '#'

/* These constants will be used on VID / MID function */
#define MNID_SUF '&'
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		8
#define CHAR_CLASSES	6

/* TO_DO: Transition table - type of states defined in separate table */
static atys_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/* [L(A-Z)] [D(0-9)] [U(_)] [M(&)] [O(')] [other] */
	   {        1,       6,   ESNR,  ESNR,      4,   ESNR}, /* S0 */
	   {        1,       1,      1,     2,      3,      3}, /* S1 */
	   {       FS,      FS,     FS,    FS,     FS,     FS}, /* S2 */
	   {       FS,      FS,     FS,    FS,     FS,     FS}, /* S3 */
	   {        4,       4,      4,     4,      5,      4}, /* S4 */
	   {       FS,      FS,     FS,    FS,     FS,     FS}, /* S5 */
	   {       FS,       6,     FS,    FS,     FS,      7}, /* S6 */
	   {       FS,      FS,     FS,    FS,     FS,     FS}  /* S7 */
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	2		/* accepting state with no retract */
#define ASWR	1		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static atys_intg stateType[NUM_STATES] = {
	 NOAS, /* S0 */
	ASNR, /* S1 */
	ASWR, /* S2 */
	ASWR, /* S3 */
	NOAS, /* S4 */
	ASWR, /* S5 */
	NOAS, /* S6 */
	ASNR  /* S7 */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
atys_intg startScanner(BufferPointer psc_buf);
static atys_intg nextClass(atys_char c);  					/* character class function */
static atys_intg nextState(atys_intg, atys_char);		/* state machine function */
//int charClass = nextClass(c);
//int nextState = transitionTable[state][charClass];
atys_void			printScannerData(ScannerData scData);
Token				tokenizer(atys_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(atys_string lexeme);

/* Declare accepting states functions */
Token funcSL(char* lexeme);
Token funcIL(char* lexeme);
Token funcID(char* lexeme);
Token funcCMT(char* lexeme);
Token funcKEY(char* lexeme);
Token funcErr(char* lexeme);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,   /* S0 */
	funcID, /* S1 */
	NULL,   /* S2 */
	NULL,   /* S3 */
	NULL,   /* S4 */
	funcSL, /* S5 */
	NULL,   /* S6 */
	funcIL  /* S7 */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* TO_DO: Define the list of keywords */
static atys_string keywordTable[KWT_SIZE] = {
	"data",		/* KW00 */
	"code",		/* KW01 */
	"int",		/* KW02 */
	"real",		/* KW03 */
	"string",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do"		/* KW09 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

 /* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	atys_char indentationCharType;
	atys_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
atys_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
