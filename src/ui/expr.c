
#include "common.h"


#include <sys/types.h>
#include <regex.h>
#include <string.h>

enum
{
	NOTYPE, NUM, LPA, RPA,
	REG, DER, BNT, LNT,
	MUL, DIV, MOD,
	ADD, SUB,
	SHL, SHR,
	BEL, BEQ, ABV, AEQ,
	EQA, NEQ,
	BAD, BXO, BOR,
	LAD, LOR
};
static struct rule 
{
	char *regex;
	int token_type;
	int precedence;
}  rules[] = {
	/*white space*/			{" +", NOTYPE, 0},
	/*number     */			{"-?[0-9]+|0x[0-9A-Fa-f]+",NUM, 0},
	/*left par   */			{"\\(", LPA, 0},
	/*right par  */			{"\\)", RPA, 0},

	/*reg        */			{"\\$", REG, 1},
//	/*dereference*/			{"\\*", DER, 1},
	/*bitwise not*/			{"~",   BNT, 1},
	/*logical not*/			{"\\!", LNT, 1},

	/*mul        */			{"\\*", MUL, 2},
	/*dereference*/			{"\\*", DER, 1},
	/*div        */			{"/",   DIV, 2},
	/*mod        */			{"%",   MOD, 2},

	/*add        */			{"\\+", ADD, 3},
	/*sub        */			{"\\-", SUB, 3},

	/*shift left */			{"<<",  SHL, 4},
	/*shift right*/			{">>",  SHR, 4},

	/*below      */			{"<",   BEL, 5},
	/*below/equal*/			{"<=",  BEQ, 5},
	/*above      */			{">",   ABV, 5},
	/*above/equal*/			{">=",  AEQ, 5},

	/*equal      */			{"==",  EQA, 6},
	/*not equal  */			{"\\!=",NEQ, 6},

	/*bitwise and*/			{"\\&", BAD, 7},
	/*bitwise xor*/			{"\\^", BXO, 8},
	/*bitwise or */			{"\\|", BOR, 9},

	/*logical and*/			{"\\&&",LAD, 10},
	/*logical or */			{"\\||",LOR, 11}
};
#define NR_REGEX ( sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];


void init_regex()
{
	int ret;
	char error_msg[128];

	int i;
	for(i = 0; i < NR_REGEX; i ++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex); }
	}
}

/*
struct token 
{
	int type;
	char str[32];
} tokens[32];
int nr_token = 0;

static bool make_token(char *e) 
{
	int position = 0;
	while(e[position]) 
	{
		int i;
		for(i = 0; i < NR_REGEX; ++i) 
		{
			regmatch_t pmatch;
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) 
			{
				char* substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				switch(rules[i].token_type) {
					case NOTYPE: 
						break;
					case NUM: 
						tokens[nr_token].type = NUM;

						char* p = tokens[nr_token++].str;
						int j;
						for (j = 0; j < substr_len; ++j)
							p[j] = e[position+j];
						p[j] = '\0';

						break;
					default: 
						tokens[nr_token++].type = rules[i].token_type; }

				position += substr_len;
				break;
			}
		}
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false; }
	}
	return true; 
}

uint32_t expr(char *e, bool *success) 
{
	if(!make_token(e)) {
		*success = false;
		return 0; }

	assert(0);
	return 0;
}

size_t eval_expr(int p, int q)
{
	if (p == q)
	{
		unsigned val = strtoul(tokens[p].str, NULL, 10);
		return val;
	}
	else
	{
		if (check_parentheses(p, q))
			return eval_expr(p+1, q-1);
		else
			char op = dom_op(p, q);

	}
}

bool check_parentheses(int, int);
bool check_parentheses(int p, int q)
{
	if (tokens[p].type == '(' && tokens[q].type == ')')
	{
		int weight = 1, i;
		for (i = p+1; i <= q; ++i)
		{
			if (tokens[p].type == '(') ++weight;
			if (tokens[q].type == ')') --weight;
			if (weight == 0) break;
		}
		return (i == q) ? true : false;
	}
	else
		return false;
}
*/
