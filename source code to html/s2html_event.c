/* s2html_event.c - complete implementation of parser state handlers */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "s2html_event.h"

#define SIZE_OF_SYMBOLS (sizeof(symbols))
#define SIZE_OF_OPERATORS (sizeof(operators))
#define WORD_BUFF_SIZE	100

/********** Internal states and event of parser **********/
typedef enum
{
	PSTATE_IDLE,
	PSTATE_PREPROCESSOR_DIRECTIVE,
	PSTATE_SUB_PREPROCESSOR_MAIN,
	PSTATE_SUB_PREPROCESSOR_RESERVE_KEYWORD,
	PSTATE_SUB_PREPROCESSOR_ASCII_CHAR,
	PSTATE_HEADER_FILE,
	PSTATE_RESERVE_KEYWORD,
	PSTATE_NUMERIC_CONSTANT,
	PSTATE_STRING,
	PSTATE_SINGLE_LINE_COMMENT,
	PSTATE_MULTI_LINE_COMMENT,
	PSTATE_ASCII_CHAR
}pstate_e;

/********** global variables **********/

/* parser state variable */
static pstate_e state = PSTATE_IDLE;

/* sub state is used only in preprocessor state */
static pstate_e state_sub = PSTATE_SUB_PREPROCESSOR_MAIN;

/* event variable to store event and related properties */
static pevent_t pevent_data;
static int event_data_idx=0;

static char word[WORD_BUFF_SIZE];
static int word_idx=0;


static char* res_kwords_data[] = {"const", "volatile", "extern", "auto", "register",
   						   "static", "signed", "unsigned", "short", "long", 
						   "double", "char", "int", "float", "struct", 
						   "union", "enum", "void", "typedef", ""
						  };

static char* res_kwords_non_data[] = {"goto", "return", "continue", "break", 
							   "if", "else", "for", "while", "do", 
							   "switch", "case", "default","sizeof", ""
							  };

static char operators[] = {'/', '+', '*', '-', '%', '=', '<', '>', '~', '&', ',', '!', '^', '|'};
static char symbols[] = {'(', ')', '{', '[', ':'};

/********** state handlers **********/
pevent_t * pstate_idle_handler(FILE *fd, int ch);
pevent_t * pstate_single_line_comment_handler(FILE *fd, int ch);
pevent_t * pstate_multi_line_comment_handler(FILE *fd, int ch);
pevent_t * pstate_numeric_constant_handler(FILE *fd, int ch);
pevent_t * pstate_string_handler(FILE *fd, int ch);
pevent_t * pstate_header_file_handler(FILE *fd, int ch);
pevent_t * pstate_ascii_char_handler(FILE *fd, int ch);
pevent_t * pstate_reserve_keyword_handler(FILE *fd, int ch);
pevent_t * pstate_preprocessor_directive_handler(FILE *fd, int ch);
pevent_t * pstate_sub_preprocessor_main_handler(FILE *fd, int ch);

/********** Utility functions **********/

/* function to check if given word is reserved key word */
static int is_reserved_keyword(char *word)
{
	int idx = 0;

	/* search for data type reserved keyword */
	while(*res_kwords_data[idx])
	{
		if(strcmp(res_kwords_data[idx++], word) == 0)
			return RES_KEYWORD_DATA;
	}

	idx = 0; // reset index
	/* search for non data type reserved key word */
	while(*res_kwords_non_data[idx])
	{
		if(strcmp(res_kwords_non_data[idx++], word) == 0)
			return RES_KEYWORD_NON_DATA;
	}

	return 0; // word did not match, return false
}

/* function to check symbols */
static int is_symbol(char c)
{
	int idx;
	for(idx = 0; idx < (int)(sizeof(symbols)/sizeof(symbols[0])); idx++)
	{
		if(symbols[idx] == c)
			return 1;
	}

	return 0;
}

/* function to check operator */
static int is_operator(char c)
{
	int idx;
	for(idx = 0; idx < (int)(sizeof(operators)/sizeof(operators[0])); idx++)
	{
		if(operators[idx] == c)
			return 1;
	}

	return 0;
}

/* to set parser event */
static void set_parser_event(pstate_e s, pevent_e e)
{
	/* terminate string safely */
	if(event_data_idx >= PEVENT_DATA_SIZE) event_data_idx = PEVENT_DATA_SIZE - 1;
	pevent_data.data[event_data_idx] = '\0';
	pevent_data.length = event_data_idx;
	event_data_idx = 0;
	/* reset word buffer as well */
	word_idx = 0;
	word[0] = '\0';
	state = s;
	pevent_data.type = e;
}


/************ Event functions **********/

/* This function parses the source file and generate 
 * event based on parsed characters and string
 */
pevent_t *get_parser_event(FILE *fd)
{
	int ch, pre_ch;
	pevent_t *evptr = NULL;
	/* Read char by char */
	while((ch = fgetc(fd)) != EOF)
	{
#ifdef DEBUG
	//	putchar(ch);
#endif
		switch(state)
		{
			case PSTATE_IDLE :
				if((evptr = pstate_idle_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_SINGLE_LINE_COMMENT :
				if((evptr = pstate_single_line_comment_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_MULTI_LINE_COMMENT :
				if((evptr = pstate_multi_line_comment_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_PREPROCESSOR_DIRECTIVE :
				if((evptr = pstate_preprocessor_directive_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_RESERVE_KEYWORD :
				if((evptr = pstate_reserve_keyword_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_NUMERIC_CONSTANT :
				if((evptr = pstate_numeric_constant_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_STRING :
				if((evptr = pstate_string_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_HEADER_FILE :
				if((evptr = pstate_header_file_handler(fd, ch)) != NULL)
					return evptr;
				break;
			case PSTATE_ASCII_CHAR :
				if((evptr = pstate_ascii_char_handler(fd, ch)) != NULL)
					return evptr;
				break;
			default : 
				printf("unknown state\n");
				state = PSTATE_IDLE;
				break;
		}
	}

	/* end of file is reached, move back to idle state and set EOF event */
	set_parser_event(PSTATE_IDLE, PEVENT_EOF);

	return &pevent_data; // return final event
}


/********** IDLE state Handler **********
 * Idle state handler identifies
 ****************************************/

pevent_t * pstate_idle_handler(FILE *fd, int ch)
{
	int pre_ch;

	/* Use clearer if/else to support isalpha and digits reliably */
	if (ch == '\'') /* ASCII char begin */
	{
		if (event_data_idx) {
			/* we've buffered some regular text; return it first and don't consume this char */
			fseek(fd, -1L, SEEK_CUR);
			set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
			return &pevent_data;
		} else {
			state = PSTATE_ASCII_CHAR;
			pevent_data.data[event_data_idx++] = (char)ch;
			return NULL;
		}
	}
	else if (ch == '/') /* comment or operator */
	{
		pre_ch = ch;
		if((ch = fgetc(fd)) == '*') /* multi line comment begin */
		{
			if(event_data_idx) /* regular text pending */
			{
				fseek(fd, -2L, SEEK_CUR);
				set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
				return &pevent_data;
			}
			else /* begin multi-line comment */
			{
#ifdef DEBUG	
				printf("Multi line comment Begin : /*\n");
#endif
				state = PSTATE_MULTI_LINE_COMMENT;
				pevent_data.data[event_data_idx++] = (char)pre_ch;
				pevent_data.data[event_data_idx++] = (char)ch;
				return NULL;
			}
		}
		else if (ch == '/') /* single line comment begin */
		{
			if(event_data_idx)
			{
				fseek(fd, -2L, SEEK_CUR);
				set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
				return &pevent_data;
			}
			else
			{
#ifdef DEBUG	
				printf("Single line comment Begin : //\n");
#endif
				state = PSTATE_SINGLE_LINE_COMMENT;
				pevent_data.data[event_data_idx++] = (char)pre_ch;
				pevent_data.data[event_data_idx++] = (char)ch;
				return NULL;
			}
		}
		else /* just a '/' operator - we already have next char read */
		{
			/* put back the next char and treat '/' as normal char */
			ungetc(ch, fd);
			pevent_data.data[event_data_idx++] = (char)pre_ch;
			return NULL;
		}
	}
	else if (ch == '#') /* preprocessor */
	{
		if (event_data_idx) {
			/* return accumulated regular text and leave '#' for next call */
			fseek(fd, -1L, SEEK_CUR);
			set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
			return &pevent_data;
		} else {
			state = PSTATE_PREPROCESSOR_DIRECTIVE;
			state_sub = PSTATE_SUB_PREPROCESSOR_MAIN;
			pevent_data.data[event_data_idx++] = (char)ch;
			return NULL;
		}
	}
	else if (ch == '"') /* string */
	{
		if (event_data_idx) {
			fseek(fd, -1L, SEEK_CUR);
			set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
			return &pevent_data;
		} else {
			state = PSTATE_STRING;
			pevent_data.data[event_data_idx++] = (char)ch;
			return NULL;
		}
	}
	else if (isdigit(ch)) /* numeric constant */
	{
		if (event_data_idx) {
			fseek(fd, -1L, SEEK_CUR);
			set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
			return &pevent_data;
		} else {
			state = PSTATE_NUMERIC_CONSTANT;
			pevent_data.data[event_data_idx++] = (char)ch;
			return NULL;
		}
	}
	else if (isalpha(ch) || ch == '_') /* identifier or keyword */
	{
		if (event_data_idx) {
			/* If there's buffered regular expression, return it first and push back this char */
			fseek(fd, -1L, SEEK_CUR);
			set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
			return &pevent_data;
		} else {
			state = PSTATE_RESERVE_KEYWORD;
			word_idx = 0;
			word[word_idx++] = (char)ch;
			pevent_data.data[event_data_idx++] = (char)ch;
			return NULL;
		}
	}
	else /* default - normal char, just accumulate */
	{
		pevent_data.data[event_data_idx++] = (char)ch;
		return NULL;
	}

	return NULL;
}

/* Preprocessor main handler */
pevent_t * pstate_preprocessor_directive_handler(FILE *fd, int ch)
{
	int tch;
	switch(state_sub)
	{
		case PSTATE_SUB_PREPROCESSOR_MAIN :
			return pstate_sub_preprocessor_main_handler(fd, ch);
		case PSTATE_SUB_PREPROCESSOR_RESERVE_KEYWORD :
			return pstate_reserve_keyword_handler(fd, ch);
		case PSTATE_SUB_PREPROCESSOR_ASCII_CHAR :
			return pstate_ascii_char_handler(fd, ch);
		default :
				printf("unknown state\n");
				state = PSTATE_IDLE;
	}

	return NULL;
}

/* Inside preprocessor - similar to idle but newline ends directive */
pevent_t * pstate_sub_preprocessor_main_handler(FILE *fd, int ch)
{
	/* If newline -> end of preprocessor directive */
	if (ch == '\n') {
		/* include newline in directive */
		pevent_data.data[event_data_idx++] = (char)ch;
		set_parser_event(PSTATE_IDLE, PEVENT_PREPROCESSOR_DIRECTIVE);
		return &pevent_data;
	}

	/* header file detection: <...> or "..." */
	if (ch == '<' || ch == '"') {
		/* append opener */
		pevent_data.data[event_data_idx++] = (char)ch;
		/* move into header handler */
		state = PSTATE_HEADER_FILE;
		/* header handler expects to see inside the header next */
		return NULL;
	}

	/* ASCII char inside preprocessor (rare) */
	if (ch == '\'') {
		pevent_data.data[event_data_idx++] = (char)ch;
		state_sub = PSTATE_SUB_PREPROCESSOR_ASCII_CHAR;
		state = PSTATE_PREPROCESSOR_DIRECTIVE;
		/* move to ascii handler */
		return pstate_ascii_char_handler(fd, fgetc(fd));
	}

	/* word starting - include and move to reserve-keyword substate */
	if (isalpha(ch) || ch == '_') {
		/* start collecting in separate word buffer for keyword checking */
		word_idx = 0;
		word[word_idx++] = (char)ch;
		pevent_data.data[event_data_idx++] = (char)ch;
		state_sub = PSTATE_SUB_PREPROCESSOR_RESERVE_KEYWORD;
		state = PSTATE_PREPROCESSOR_DIRECTIVE;
		return NULL;
	}

	/* digits -> numeric constant inside directive */
	if (isdigit(ch)) {
		pevent_data.data[event_data_idx++] = (char)ch;
		state = PSTATE_NUMERIC_CONSTANT; /* numeric handler will emit numeric event inside preprocessor */
		return NULL;
	}

	/* default: store the character inside directive buffer */
	pevent_data.data[event_data_idx++] = (char)ch;
	return NULL;
}

/* header file handler - collects until closing '>' or '"' */
pevent_t * pstate_header_file_handler(FILE *fd, int ch)
{
    static int is_std = 0;  // <...> = std, "..." = user

    // Detect starting symbol only once
    if (pevent_data.length == 1) {
        is_std = (pevent_data.data[0] == '<');
    }

    // Stop when closing symbol is reached
    if ((is_std && ch == '>') || (!is_std && ch == '"')) {

        // terminate collected data
        pevent_data.data[event_data_idx] = '\0';

        // remove the first character (< or ")
        char tmp[PEVENT_DATA_SIZE];
        strcpy(tmp, pevent_data.data + 1);

        // overwrite event data
        strcpy(pevent_data.data, tmp);
        event_data_idx = strlen(pevent_data.data);

        // set property
        pevent_data.property = is_std ? STD_HEADER_FILE : USER_HEADER_FILE;

        // return the event
        set_parser_event(PSTATE_IDLE, PEVENT_HEADER_FILE);
        return &pevent_data;
    }

    // Otherwise store character inside header name
    pevent_data.data[event_data_idx++] = ch;
    return NULL;
}


/* Reserve-keyword / identifier handler */
pevent_t * pstate_reserve_keyword_handler(FILE *fd, int ch)
{
	/* collect letters, digits and underscore in word[] */
	if (isalpha(ch) || isdigit(ch) || ch == '_') {
		/* append to word and to full event buffer */
		if (word_idx < WORD_BUFF_SIZE - 1) word[word_idx++] = (char)ch;
		if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
		return NULL;
	} else {
		/* word ended - terminate word and check */
		word[word_idx] = '\0';
		int kw = is_reserved_keyword(word);
		if (kw) {
			/* emit reserved keyword event */
			pevent_data.property = kw; /* RES_KEYWORD_DATA or RES_KEYWORD_NON_DATA */
			set_parser_event(PSTATE_IDLE, PEVENT_RESERVE_KEYWORD);
			/* push back delimiter to be processed by next call */
			fseek(fd, -1L, SEEK_CUR);
			return &pevent_data;
		} else {
			/* not a reserved keyword: treat everything collected as regular expression so far */
			set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
			fseek(fd, -1L, SEEK_CUR);
			return &pevent_data;
		}
	}
}

/* Numeric constant handler */
pevent_t * pstate_numeric_constant_handler(FILE *fd, int ch)
{
	/* Accept digits, letters (suffixes), dot, x/X, signs inside exponent etc */
	if (isdigit(ch) || isalpha(ch) || ch == '.' || ch == '+' || ch == '-' || ch == 'x' || ch == 'X') {
		if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
		return NULL;
	} else {
		/* numeric literal ended */
		set_parser_event(PSTATE_IDLE, PEVENT_NUMERIC_CONSTANT);
		fseek(fd, -1L, SEEK_CUR);
		return &pevent_data;
	}
}

/* String literal handler */
pevent_t * pstate_string_handler(FILE *fd, int ch)
{
	/* ch is the next character after initial opening quote (or subsequent chars) */
	/* Note: pevent_data already contains the initial opening quote when we entered */
	if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
	/* On escape, consume next char and include it */
	if (ch == '\\') {
		int nx = fgetc(fd);
		if (nx == EOF) {
			/* malformed escape, finalize as string */
			set_parser_event(PSTATE_IDLE, PEVENT_STRING);
			return &pevent_data;
		}
		if (event_data_idx < PEVENT_DATA_SIZE - 2) pevent_data.data[event_data_idx++] = (char)nx;
		return NULL;
	}
	/* Closing quote -> end string */
	if (ch == '"') {
		set_parser_event(PSTATE_IDLE, PEVENT_STRING);
		return &pevent_data;
	}
	/* Newline inside string — include and continue (this is lenient) */
	if (ch == '\n') {
		/* keep newline char in buffer */
		return NULL;
	}
	return NULL;
}

pevent_t * pstate_single_line_comment_handler(FILE *fd, int ch)
{
	int pre_ch;
	switch(ch)
	{
		case '\n' : /* single line comment ends here */
#ifdef DEBUG	
			printf("\nSingle line comment end\n");
#endif
			pre_ch = ch;
			pevent_data.data[event_data_idx++] = (char)ch;
			set_parser_event(PSTATE_IDLE, PEVENT_SINGLE_LINE_COMMENT);
			return &pevent_data;
		default :  // collect single line comment chars
			if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
			break;
	}

	return NULL;
}
pevent_t * pstate_multi_line_comment_handler(FILE *fd, int ch)
{
	int pre_ch;
	switch(ch)
	{
		case '*' : /* comment might end here */
			pre_ch = ch;
			if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
			if((ch = fgetc(fd)) == '/')
			{
#ifdef DEBUG	
				printf("\nMulti line comment End : */\n");
#endif
				pre_ch = ch;
				if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
				set_parser_event(PSTATE_IDLE, PEVENT_MULTI_LINE_COMMENT);
				return &pevent_data;
			}
			else // multi line comment string still continued
			{
				if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
			}
			break;
		case '/' :
			/* go back by two steps and read previous char */
			fseek(fd, -2L, SEEK_CUR); // move two steps back
			pre_ch = fgetc(fd); // read a char
			fgetc(fd); // to come back to current offset

			if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
			if(pre_ch == '*')
			{
				set_parser_event(PSTATE_IDLE, PEVENT_MULTI_LINE_COMMENT);
				return &pevent_data;
			}
			break;
		default :  // collect multi-line comment chars
			if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
			break;
	}

	return NULL;
}

/* ASCII char literal handler */
pevent_t * pstate_ascii_char_handler(FILE *fd, int ch)
{
	/* pevent_data already has the starting quote ' */
	if (event_data_idx < PEVENT_DATA_SIZE - 1) pevent_data.data[event_data_idx++] = (char)ch;
	/* if escape, consume next char */
	if (ch == '\\') {
		int nx = fgetc(fd);
		if (nx == EOF) {
			set_parser_event(PSTATE_IDLE, PEVENT_ASCII_CHAR);
			return &pevent_data;
		}
		if (event_data_idx < PEVENT_DATA_SIZE - 2) pevent_data.data[event_data_idx++] = (char)nx;
		/* after escape expect closing ' */
		int nx2 = fgetc(fd);
		if (nx2 == EOF) {
			set_parser_event(PSTATE_IDLE, PEVENT_ASCII_CHAR);
			return &pevent_data;
		}
		if (event_data_idx < PEVENT_DATA_SIZE - 2) pevent_data.data[event_data_idx++] = (char)nx2;
		if (nx2 == '\'') {
			set_parser_event(PSTATE_IDLE, PEVENT_ASCII_CHAR);
			return &pevent_data;
		} else {
			/* didn't find closing quote — keep going */
			return NULL;
		}
	} else {
		/* if this char is closing quote -> finish, else keep reading until close */
		if (ch == '\'') {
			set_parser_event(PSTATE_IDLE, PEVENT_ASCII_CHAR);
			return &pevent_data;
		} else {
			/* read next char in stream in next call */
			return NULL;
		}
	}
}
/**** End of file ****/
