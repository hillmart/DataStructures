#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"

//Martin Hill, 93328778
//hillmart@oregonstate.edu

struct call {
	int id;
	char* name;
	char* reason;
};

//option 1, take in call and enqueue it
void recieve(struct queue* q, int call_id){
	struct call* call = malloc(sizeof(struct call));
	call->id = call_id;
	call->name = malloc(51*sizeof(char));
	call->reason = malloc(101*sizeof(char));
	printf("Enter caller's name: ");
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
	fgets(call->name, 50, stdin);
	call->name[strcspn(call->name, "\n")] = 0;
	printf("Enter caller's reason: ");
	fgets(call->reason, 100, stdin);
	call->reason[strcspn(call->reason, "\n")] = 0;
	queue_enqueue(q, call);
}

//option 2, deque and push
void answer(struct queue* q, struct stack* s){
	if (!queue_isempty(q)){
		struct call* call = queue_dequeue(q);
		stack_push(s, call);
	}
	if (queue_isempty(q)){
		printf("Queue is empty, no un-answered calls\n");
	}
}

//option 3, stack_top
void current_stack(struct stack* s, int calls){
	printf("Number of calls answered: %d\n", calls);
	if (!stack_isempty(s)){
		printf("Details of the last call answered: \n");
		struct call* call = stack_top(s);
		printf("Call ID: %d\n", call->id);
		printf("Caller's name: %s\n", call->name);	
		printf("Call reason: %s\n", call->reason);	
	}	
}

//option 4, queue_front
void current_queue(struct queue* q, int messages){
	printf("Number of calls to be answered: %d\n", messages);
	if (!queue_isempty(q)){
		printf("Details of the next call to be answered: \n");
		struct call* call = queue_front(q);
		printf("Call ID: %d\n", call->id);
		printf("Caller's name: %s\n", call->name);	
		printf("Call reason: %s\n", call->reason);	
	}
}

//option 5, loop through q and s and free all calls
void free_at_end(struct queue* q, struct stack* s){
	while (!queue_isempty(q)) {
		struct call* call = queue_dequeue(q);
		free(call->name);
		free(call->reason);
		free(call);
	}

	while (!stack_isempty(s)) {
		struct call* call = stack_pop(s);
		free(call->name);
		free(call->reason);
		free(call);
	}
}

//print options at start of each while loop iteration
int print_options(){
	printf("1. Receive a new call\n");
	printf("2. Answer a call\n");
	printf("3. Current state of the stack – answered calls\n");
	printf("4. Current state of the queue – calls to be answered\n");
	printf("5. Quit\n");
	printf("Choose an option: ");
	int val;
	scanf("%d", &val);
	return val;
}

int main(int argc, char const *argv[]) {
	//variables
	int loop = 1;
	int val;
	struct queue* q = queue_create();
	struct stack* s = stack_create();
	int call_id = 1;
	int calls = 0;
	int messages = 0;

	//while loop with options
	while(loop){
		val = print_options();
		if (val == 1){
			recieve(q, call_id);
			call_id++;
			messages++;
		}
		if (val == 2){
			answer(q, s);
			calls++;
			messages--;
		}
		if (val == 3){
			current_stack(s, calls);
		}
		if (val == 4){
			current_queue(q, messages);			
		}
		if (val == 5){
			loop = 0;
			free_at_end(q,s);
			stack_free(s);
			queue_free(q);
		}
	}

	return 0;
}
