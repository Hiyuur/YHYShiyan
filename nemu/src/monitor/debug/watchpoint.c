#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
	WP *p;
	p = free_;
	if(free_ == NULL)
	assert(0);
	free_ = p->next;
	WP *q;
	q = head;
	if(q == NULL) {
		q = p;
		head = q;
	}
	else {
		while(q->next) {
		q = q->next;
		}
		q->next = p;
	}
	p->next = NULL;
	return p;
}

void free_wp(WP *wp) {
	WP *p = head;
	WP *q;
	if(p == wp) {
		q = p;
		head = head->next;
	}
	else {
		while(p->next != NULL && p->next != wp) {
			p = p->next;
		}
		q = p->next;
		p->next = wp->next;
	}
	q->next = NULL;
	q->val = 0;
	q->expr[0] = '\0';
	WP *f = free_;
	if(f == NULL) {
		free_ = q;
	}
	else {
		while(f->next) {
			f = f->next;
		}
		f->next = q;
	}
	q->next = NULL;
}

bool check_wp() {
	bool t = true;
	bool succees;
	WP *p;
	p = head;
	while(p != NULL) {
		uint32_t tmp = expr(p->expr,&succees);
		if(!succees)
		assert(0);
		if(tmp != p->val) {
			t = false;
			printf("The value of watchpoint is changed!\n");
			printf("Before: %d hex: 0x%08x\n",p->val,p->val);
			printf("Now: %d hex: 0x%08x\n",tmp,tmp);
			p->val = tmp;
		}
		p = p->next;
	}
	return t;
}

void delete_wp(int num) {
	WP *p;
	p = &wp_pool[num];
	free_wp(p);
}

void info_w() {
	WP *p;
	p = head;
	while(p) {
		printf("Watchpoint%d expr: %s vale: %d hex: 0x%08x\n",p->NO,p->expr,p->val,p->val);
		p = p->next;
	}
}