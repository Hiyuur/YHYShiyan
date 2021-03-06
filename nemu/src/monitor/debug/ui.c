#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"


#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
	int num = 0;
	if(args == NULL) {
		num = 1;
	}
	else{
		sscanf(args,"%d",&num);
	}
	cpu_exec(num);
	return 0;
}

static int cmd_info(char *args) {
	char parm;
	int i;
	sscanf(args,"%c",&parm);
	if(parm == 'r') {
		for(i = R_EAX;i <= R_EDI;i++) {
			printf("%s\t0x%08x\n",regsl[i],cpu.gpr[check_reg_index(i)]._32);
		}
		printf("eip\t0x%08x\n",cpu.eip);
	}
	else if(parm == 'w') {
	info_w();
	}
	return 0;
}

static int cmd_x(char *args) {
	int num;
	hwaddr_t address;
	int i;
	char *c = strtok(args," ");
	sscanf(c,"%d",&num);
	args = c + strlen(c) + 1;
	char *c2;
	c2 = strtok(args,"x");
	args = c2 + strlen(c2) + 1;
	sscanf(args,"%x",&address);
	printf("0x%08x:\n",address);
	for(i = 0;i < num;i++) {
		printf("0x%08x\n",swaddr_read(address,4));
		address = address + 4;
	}
	return 0;
}

static int cmd_p(char *args) {
	uint32_t num;
	bool success;
	num = expr(args,&success);
	if(success) {
		printf("%d\thex: 0x%08x\n",num,num);
	}
	else {
		printf("ERROR!\n");
		assert(0);
	}
	return 0;
}

static int cmd_w(char *args) {
	bool success;
	WP *p;
	p = new_wp();
	p->val = expr(args,&success);
	if(!success) {
		printf("Expr illegal!\n");
		assert(0);
	}
	strcpy(p->expr,args);
	printf("Watchpoint%d is working\n",p->NO);
	printf("value: %d hex: 0x%08x\n",p->val,p->val);
	return 0;
}

static int cmd_d(char *args) {
	int num;
	sscanf(args,"%d",&num);
	delete_wp(num);
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Pauses after the program stepping N instructions.N defaults to 1", cmd_si },
	{ "info", "r : Print register state\nw : Print watchpoint information", cmd_info},
	{ "x", "Scan memory", cmd_x},
	{ "p", "Evaluate expression", cmd_p},
	{ "w", "Make watchpoint", cmd_w},
	{ "d", "Delete watchpoint", cmd_d},

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
