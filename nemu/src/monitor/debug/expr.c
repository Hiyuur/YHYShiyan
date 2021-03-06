#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NEQ,NUMBER,AND,OR,MINUS,POINTER,HEXNUM,REG

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
	int priority;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE, 0},				// spaces
	{"\\+", '+', 4},					// plus
	{"==", EQ, 3},					// equal
	{"/", '/', 5},
	{"-", '-', 4},
	{"\\*", '*', 5},
	{"\\(", '(', 7},
	{"\\)", ')', 7},
	{"!=", NEQ, 3},
	{"&&", AND, 2},
	{"\\|\\|", OR, 1},
	{"!", '!', 6},
	{"0[xX][0-9a-zA-Z]+", HEXNUM, 0},
	{"[0-9]+", NUMBER, 0},
	{"\\$[a-zA-Z]+", REG, 0},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int priority;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				

				switch(rules[i].token_type) {
					case NOTYPE: break;
					case REG: tokens[nr_token].type = rules[i].token_type;
				              tokens[nr_token].priority = rules[i].priority;
							  strncpy(tokens[nr_token].str,substr_start + 1,substr_len - 1);
							  tokens[nr_token].str[substr_len - 1] = '\0';
							  nr_token++;
							  break;
					default: tokens[nr_token].type = rules[i].token_type;
				             tokens[nr_token].priority = rules[i].priority;
							 strncpy(tokens[nr_token].str,substr_start,substr_len);
							 tokens[nr_token].str[substr_len] = '\0';
							 nr_token++;

					//panic("please implement me");
				}
				position = position + substr_len;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

int check_parentheses(int p,int q) {
	int i;
	if(tokens[p].type == '(' && tokens[q].type == ')') {
		int num_l = 0;
		int num_r = 0;
		for(i = p + 1;i < q;i++) {
			if(tokens[i].type == '(') num_l++;
			if(tokens[i].type == ')') num_r++;
			if(num_r > num_l) return false;
		}
		if(num_l == num_r)
		return true;
	}
	return false;
}

int dominent_op(int p,int q) {
	int i,j;
	int op = p;
	int min = 10;
	for(i = p;i <= q;i++) {
		if(tokens[i].type == NUMBER || tokens[i].type == HEXNUM || tokens[i].type == REG) continue;
		int num = 0;
		bool t = true;
		for(j = i - 1;j >= p;j--) {
			if(tokens[j].type == '(' && !num) {
				t = false;
				break;
			}
			if(tokens[j].type == '(') num--;
			if(tokens[j].type == ')') num++;
		}
		if(!t)
		continue;
		if(tokens[i].priority <= min) {
			min = tokens[i].priority;
			op = i;
		}
	}
	return op;
}

uint32_t eval(int p ,int q) {
	if(p > q) {
		return 0;
	}
	else if(p == q) {
		uint32_t num = 0;
		if(tokens[p].type == NUMBER)
		sscanf(tokens[p].str,"%d",&num);
		if(tokens[p].type == HEXNUM)
		sscanf(tokens[p].str,"%X",&num);
		if(tokens[p].type == REG) {
			int len = strlen(tokens[p].str);
			if(len == 3) {
				int i;
				for(i = R_EAX;i <= R_EDI;i++) {
					if(strcmp(tokens[p].str,regsl[i]) == 0)
					break;
				}
				if(i > R_EDI) {
					if(strcmp(tokens[p].str,"eip") == 0) {
						num = cpu.eip;
					}
					else {
						printf("No this register!\n");
						assert(0);
					}
				}
				else {
					num = reg_l(i);
				}
			}
			else if(len == 2) {
				if(tokens[p].str[1] == 'x' || tokens[p].str[1] == 'i' || tokens[p].str[1] == 'p') {
					int i;
					for(i = R_AX;i <= R_DI;i++) {
						if(strcmp(tokens[p].str,regsw[i]) == 0) {
						num = reg_w(i);
						break;
						}
					}
				}
				else if(tokens[p].str[1] == 'h' || tokens[p].str[1] == 'l') {
					int i;
					for(i = R_AL;i <= R_BH;i++) {
						if(strcmp(tokens[p].str,regsb[i]) == 0) {
							num = reg_b(i);
							break;
						}
					}
				}
				else {
					printf("No this register!\n");
					assert(0);
				}
			}
		}
		return num;
	}
	else if(check_parentheses(p,q) == true) {
		return eval(p + 1,q - 1);
	}
	else {
		int op = dominent_op(p,q);
		if(p == op || tokens[op].type == MINUS || tokens[op].type == POINTER || tokens[op].type == '!') {
			uint32_t tmp = eval(p + 1,q);
			switch(tokens[p].type) {
				case MINUS : return -tmp;
				case POINTER : return swaddr_read(tmp,4);
				case '!' : return !tmp;
			}
		}
		u_int32_t val1 = eval(p,op - 1);
		u_int32_t val2 = eval(op + 1,q);
		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case EQ : return val1 == val2;
			case NEQ : return val1 != val2;
			case AND : return val1 && val2;
			case OR : return val1 || val2;
			default: assert(0);
		}
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	int i;
	for(i = 0;i < nr_token;i++) {
		if(tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != NUMBER && tokens[i - 1].type != REG && tokens[i - 1].type != HEXNUM && tokens[i - 1].type != ')'))) {
			tokens[i].type = MINUS;
			tokens[i].priority = 6;
		}
		if(tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != NUMBER && tokens[i - 1].type != REG && tokens[i - 1].type != HEXNUM && tokens[i - 1].type != ')'))) {
			tokens[i].type = POINTER;
			tokens[i].priority = 6;
		}
	}
	
	*success = true;
	return eval(0,nr_token - 1);
}

