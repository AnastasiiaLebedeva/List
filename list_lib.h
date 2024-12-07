#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_SYMBOL_NAME 	 16
#define NUM_SYMBOL_NAME_FILE 50

#define EXIT_COMMAND    	 -1
#define ERROR 				 -2

#define WEIGHT				 1
#define NAME                 2

#define TRUE				 1
#define FALSE				 0

typedef enum{
	FREE		= 0,
	ADD 		= 1,
	DEL 		= 2,
	SHOW_ALL    = 3,
	SHOW_ONE    = 4,
	SORT_LIST   = 5,
	FILT_LIST   = 6,
	READ_FILE   = 7,
	WRITE_FILE  = 8,
	DEL_ALL     = 9,
	HELP 		= 10,
	EXIT 		= 11
}COMAND;

/*typedef enum{
	ERROR = -1
}ER;*/

//структура объекта, записанного в списке
typedef struct Object Object_t;
struct Object
{
	float weight;
	char  name[NUM_SYMBOL_NAME];
};


//элемент списка
typedef struct Node Node_t;
struct Node
{
	void	  *object; 	 //указатель на объект	
	Node_t    *next_node; //указатель на следующий элемент списка
};


//список
typedef struct List List_t;
struct List
{
	Node_t *top_node;  //указатель на первый элемент списка 				
	int count; 	
};


//прототипы функций
void end_prog();
List_t* create_list();
Node_t *get_node(List_t *list, int num_node);
void push_back(List_t *list, float weight, char* name);
void print_node(List_t *list, int num_node);
void delete_node(List_t *list, int num_node);
void print_list(List_t *list);
void sort_list(List_t *list, int flg);
int read_file(List_t *list);
int write_file(List_t *list);
int delete_all(List_t *list);
