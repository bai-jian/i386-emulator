#include "common.h"
#include "nemu.h"

#include <stdlib.h>

#include <sys/types.h>
#include <regex.h>
/* Compile a regular expression into a form suitable for regexec() searches
 *     int regcomp(regex_t* preg, const char* regex, int cflags);
 * Turn the error codes that can be returned by regcomp() and regexec() into the error message strings
 *     size_t regerror(int errcode, const regex_t* preg, char* errbuf, size_t buf_size);
 * Match Regex
 *     int regexec(const regex_t* preg, const char* string, size_t nmatch, regmatch_t pmatch[], int eflags
 */



/*Token Types and Operators*/
enum
{
/*white space      */		NOTYPE, 
/*number           */		NUM, 
/*parenthese       */		LPA, RPA,

/*regester         */		REG, 
/*derefence        */		DER, 

/*symbol           */       SYM,

/*arithmatic ops   */		DEC, ADD, SUB, MUL, DIV, MOD, 
/*shift ops        */		SHL, SHR,
/*relation ops     */       BEL, BEQ, ABV, AEQ, EQA, NEQ,
/*bit-wisw ops     */		BNT, BAD, BXO, BOR,
/*logical ops      */		LNT, LAD, LOR
};

/*Operator Precdence*/
#define NON_P  0
#define REG_P  1
#define DER_P  1
#define SYM_P  1
#define BNT_P  1
#define LNT_P  1
#define DEC_P  1
#define MUL_P  2
#define DIV_P  2
#define MOD_P  2
#define ADD_P  3
#define SUB_P  3
#define SHL_P  4
#define SHR_P  4
#define BEQ_P  5
#define AEQ_P  5
#define BEL_P  5
#define ABV_P  5
#define EQA_P  6
#define NEQ_P  6
#define BAD_P  7
#define BXO_P  8
#define BOR_P  9
#define LAD_P  10
#define LOR_P  11

static struct rule 
{
	char* regex;
	int token_type;
	int precedence;
} rules[] = 
{
	/*white space*/			{" +", NOTYPE, NON_P},
	/*number     */			{"[0-9]+|0x[0-9A-Fa-f]+",  NUM, NON_P},
	/*symbol     */         {"[a-z]+"/*"*(_|[A-Fa-f])(_|[0-9A-Fa-f])*"*/, SYM, SYM_P},
	/*left par   */			{"\\(", LPA, NON_P},
	/*right par  */			{"\\)", RPA, NON_P},

/*Double Operators*/
	/*shift left */			{"<<",  SHL, SHL_P},
	/*shift right*/			{">>",  SHR, SHR_P},
	/*below/equal*/			{"<=",  BEQ, BEQ_P},
	/*above/equal*/			{">=",  AEQ, AEQ_P},
	/*equal      */			{"==",  EQA, EQA_P},
	/*not equal  */			{"\\!=",NEQ, NEQ_P},
	/*logical and*/			{"\\&&",LAD, LAD_P},
	/*logical or */			{"\\|\\|",LOR, LOR_P},

/*Uncommon Single Operators*/
	/*reg        */			{"\\$(eax|ecx|edx|ebx|esp|ebp|esi|edi)", REG, REG_P},
	/*div        */			{"/",   DIV, DIV_P},
	/*mod        */			{"%",   MOD, MOD_P},
	/*bitwise not*/			{"~",   BNT, BNT_P},
	/*add        */			{"\\+", ADD, ADD_P},
	/*below      */			{"<",   BEL, BEL_P},
	/*above      */			{">",   ABV, ABV_P},
	/*bitwise and*/			{"\\&", BAD, BAD_P},
	/*bitwise xor*/			{"\\^", BXO, BXO_P},
	/*bitwise or */			{"\\|", BOR, BOR_P},
	/*logical not*/			{"\\!", LNT, LNT_P},


/*Common Single Operators*/
	/*dereference*/			{"\\*", DER, DER_P},
	/*mul        */			{"\\*", MUL, MUL_P},

	/*dec        */         {"\\-", DEC, DEC_P},
	/*sub        */         {"\\-", SUB, SUB_P}
};
#define NR_REGEX ( sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

// Initialize regex rules and Judge the regex valid
void init_regex()
{
	int ret;
	char error_msg[128];

	int i;
	for(i = 0; i < NR_REGEX; i ++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) 
 		{
			regerror(ret, &re[i], error_msg, 128);
//			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex); 
 		}
 	} 
}


struct token
{
	int type;
	int prec;
	char str[32];
} tokens[32];
int nr_token;

void make_token(char*);
void mend_token(char*);
size_t eval_expr(int, int);
size_t expr(char *e) 
{
	nr_token = 0;

	make_token(e);
	mend_token(e);

	size_t val = eval_expr(0, nr_token-1);
	return val;
}

void make_token(char* e) 
{
	int position = 0;
	int i;
	regmatch_t pmatch;
	while(e[position] != '\0') 
   	 {  
		for(i = 0; i < NR_REGEX; i ++) 
 	  	 {
			if( regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0 ) 
 	 		{
				char* substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				switch(rules[i].token_type) 
				{
					case NOTYPE: break;

					case NUM:
						tokens[nr_token].type = NUM;
						strncpy(tokens[nr_token].str, e+position, substr_len);
						++nr_token;

						break;

					case SYM:
						tokens[nr_token].type = SYM;
						strncpy(tokens[nr_token].str, e+position, substr_len);
						++nr_token;

						break;
					
					case REG:
						tokens[nr_token].type = REG;
						strncpy(tokens[nr_token].str, e+position+1, substr_len-1);
						++nr_token;

						break;

					default: 
						tokens[nr_token].type = rules[i].token_type; 
						tokens[nr_token].prec = rules[i].precedence;
						++nr_token; 
 	  			}

				position += substr_len;  break;
 	  		}
 	  	}
		if(i == NR_REGEX) 
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return;
		}
	 } 
}

void mend_token(char* e)
{
	int i;
	for (i = 1; i < nr_token; ++i)
 	{
		switch (tokens[i].type)
 		{
			case DER:
				if (tokens[i-1].type == NUM || tokens[i-1].type == RPA) 
					tokens[i].type = MUL, tokens[i].prec = MUL_P;
				break;
			case DEC:
				if (tokens[i-1].type == NUM || tokens[i-1].type == RPA)
					tokens[i].type = SUB, tokens[i].prec = SUB_P;
				break;
		}
	}
	return ;
}

swaddr_t symbol(char*);
bool check_parentheses(int, int);
int dom_op(int, int);
size_t eval_expr(int p, int q)
{
	if (p == q)
	{
		switch (tokens[p].type)
		{
			case NUM:
				return strtol(tokens[p].str, NULL, 0);
			case SYM:
				return symbol(tokens[p].str);
			case REG:
				if ( strcmp( tokens[p].str, "eax" ) ) return cpu.eax;
				if ( strcmp( tokens[p].str, "ecx" ) ) return cpu.ecx;
				if ( strcmp( tokens[p].str, "edx" ) ) return cpu.edx;
				if ( strcmp( tokens[p].str, "ebx" ) ) return cpu.ebx;
				if ( strcmp( tokens[p].str, "ebp" ) ) return cpu.ebp;
				if ( strcmp( tokens[p].str, "esp" ) ) return cpu.esp;
				if ( strcmp( tokens[p].str, "esi" ) ) return cpu.esi;
				if ( strcmp( tokens[p].str, "edi" ) ) return cpu.edi;
 		}
	}
	else
	{
		if (check_parentheses(p, q))
			return eval_expr(p+1, q-1);
		else
		{
			int op = dom_op(p, q);
			size_t lval, rval;
			switch (tokens[op].type)
			{
				case DER:
					rval = eval_expr(op+1, q);
					return swaddr_read(rval, 4);
				case DEC:
					rval = eval_expr(op+1, q);
					return -rval;
				case ADD: 
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval + rval;
				case SUB:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval-rval;
				case MUL:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval*rval;
				case DIV:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval / rval;
				case MOD:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval % rval;
				case SHL:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval << rval;
				case SHR:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval >> rval;
				case BEL:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval < rval;
				case BEQ:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval <= rval;
				case ABV:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval > rval;
				case AEQ:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval >= rval;
				case EQA:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval == rval;
				case NEQ:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval != rval;
				case BNT:
					rval = eval_expr(op+1, q);
					return ~rval;
				case BAD:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval & rval;
				case BXO:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval ^ rval;
				case BOR:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval | rval;
				case LNT:
					rval = eval_expr(op+1, q);
					return !rval;
				case LAD:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval && rval;
				case LOR:
					lval = eval_expr(p, op-1);
					rval = eval_expr(op+1, q);
					return lval || rval;
				default:
					assert(0);
 			}
 		}
 	}
	return 0;
}
bool check_parentheses(int p, int q)
{
	if (tokens[p].type == LPA && tokens[q].type == RPA)
 	{
		int weight = 1, i;
		for (i = p+1; i <= q; ++i)
 		{
			if (tokens[i].type == LPA) ++weight;
			if (tokens[i].type == RPA) --weight;
			if (weight == 0) break;
		}
		return weight == 0 && i == q;
	}
	else
		return false;
}
int dom_op(int p, int q)
{
	int op = p;

	int weight = 0, i;
	for (i = p; i <= q; ++i)
	{
		switch(tokens[i].type)
		{
			case NUM:	break;
			case LPA:	++weight; break;
			case RPA:	--weight; break;
			default:
				if (weight == 0 && tokens[i].prec >= tokens[op].prec)
					op = i;
	 	}
	} 
	return op;
}
