#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "list_lib.h"


List_t* create_list()
{
	List_t *lst = (List_t*)malloc(sizeof(List_t));
	
	lst -> count = 0;
	lst -> top_node = NULL;
	
	return lst;
}

Node_t *get_node(List_t *list, int num_node)
{
	Node_t *base = list -> top_node;
	
	if(base != NULL){
		for(int i = 0; i<num_node-1; i++){ 				/*num_node-1, ибо если элемент последний, то следующий адрес будет null*/ //@codereview отступ коментария в этом месте луче производить при помощи пробелов
			base = base -> next_node; 
		}
	}
	return base;
}

void push_back(List_t *list, Object_t *object_adr)
{
	Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));				/*создание элемента списка*/
	new_node -> object    = (void*)object_adr;
	new_node -> next_node = NULL;

	Node_t *previous_node = get_node(list, list->count);
	
	if(previous_node == NULL)										/*добавляется первый элемент списка*/
		list -> top_node = new_node;
	else
		previous_node -> next_node = new_node;						/*для предыдущего элемента записывается следующий элемент списка*/
	
	list -> count++;
}

Object_t *create_obj(float weight, char* name)
{
	Object_t *object_adr = (Object_t*)malloc(sizeof(Object_t));   	/*выделение памяти под новый объект и заполнение его параметров*/
	object_adr -> weight = weight;
	strcpy(object_adr -> name, name);
	
	return object_adr;
}

void print_node(List_t *list, int num_node)
{
	/*Node_t *node = get_node(list, num_node);
	Object_t* object_adr = (Object_t*)(node -> object);*/
	
	Object_t *object_adr = (Object_t*)((Node_t*)get_node(list, num_node) -> object);  	/*преобразуем адрес, в тип объекта*/
	
	printf("%3d | %15s | %.3f \n", num_node, object_adr -> name, object_adr -> weight);	

}

void delete_node(List_t *list, int num_node)
{
	Node_t *node = get_node(list, num_node);
	Object_t* object_adr = (Object_t*)(node -> object);
	
	if(list -> count == 1){      								/*элемент один*/
		list -> top_node = NULL;
	}
	else if(num_node == 1){										/*элемент первый*/
		list -> top_node = node -> next_node;
	}
	else{
		Node_t *previous_node = get_node(list, num_node-1);
		
		if(num_node == list -> count)							/*элемент последний*/
			previous_node -> next_node = NULL;
		else
			previous_node -> next_node = node -> next_node;
	}
	
	free(object_adr);
	free(node);
	list -> count--;
}

void print_list(List_t *list)
{
	printf("%3s | %15s | %10s \n", "Num", "Name      ", "Weight, kg");
	for(int i = 0; i < list -> count; i++){
		print_node(list, i+1);
	}
}

/*char* toLower(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}*/


void sort_list(List_t *list, FLAG flg)
{
	int N = list -> count;
	int Swap = 0;
	
	for(int i = 0; i< N-1; i++){
		
		Swap = FALSE;
		
		for(int i1 = 0; i1< N-1; i1++){
			Node_t *node_first  = get_node(list, i1+1);
			Object_t* object_first = (Object_t*)(node_first -> object);
			
			Node_t *node_second = get_node(list, i1+2);
			Object_t* object_second = (Object_t*)(node_second -> object);
			
			if(flg == WEIGHT){
				if(object_first -> weight > object_second -> weight)
					Swap = TRUE;
			}
			else if(flg == NAME){
				if(strcmp(object_first -> name, object_second -> name) > 0)
					Swap = TRUE;
			}
			
			if(Swap == TRUE){                       //@codereview лучше было бы ввести функцию list_swap(list, obj_1, obj_2)
				Swap = FALSE;                       //@codereview тогда ее можно было бы использовать в 119 строке, и код функции сильно уменьшился по вертикали
				node_first -> object  = object_second;
				node_second -> object = object_first;
			}
		}
	}
}

int filt_list(List_t *list, FLAG flg, float number)
{
	int N = list -> count;
	Object_t *object_adr;
	int first_print_flg = FALSE;
	
	for(int i = 0; i < N; i++){
		object_adr = (Object_t*)((Node_t*)get_node(list, i+1) -> object); 
		
		if(flg == WEIGHT && object_adr -> weight >= number){
			if(first_print_flg == 0){
				printf("%3s | %15s | %10s \n", "Num", "Name      ", "Weight, kg");
				first_print_flg = TRUE;
			}
			
			print_node(list, i+1);
		}
		if(flg == NAME && strlen(object_adr -> name) >= floor(number)){
			if(first_print_flg == 0){
				printf("%3s | %15s | %10s \n", "Num", "Name      ", "Weight, kg");
				first_print_flg = TRUE;
			}
			
			print_node(list, i+1);
		}
	}
	
	return first_print_flg;
}

int read_file(List_t *list, char* file_name)
{
	FILE* file;
	char name[NUM_SYMBOL_NAME];
	float weight = 0;
	unsigned int count = 0;
	Object_t *object_adr;
	
	file = fopen(file_name, "r");
	
	if(file == NULL){	/*ошибка открытия файла*/
		return ERROR;
	}
	
	while(fscanf(file, "%s %f", name, &weight) != EOF){
		object_adr = create_obj(weight, name);
		push_back(list, object_adr);
		count++;
	}
	
	fclose(file);
	
	return count;
}

int write_file(List_t *list, char* file_name)
{
	FILE* file;
	int count = 0;
	
	file = fopen(file_name, "w");         //@codereview имя файла лучше передавать в качестве аргумента в функцию, не использовать для этого "глобальные" переменные
	
	if(file == NULL){	/*ошибка открытия файла*/
		return ERROR;
	}
	
	//Node_t *node; 
	Object_t *object_adr; 
	for(int i = 0; i< list->count; i++){
		//node = get_node(list, i+1);
		//object_adr = (Object_t*)(node -> object); 
		
		object_adr = (Object_t*)((Node_t*)get_node(list, i+1) -> object);
		fprintf(file, "%s %.3f\n", object_adr -> name, object_adr -> weight);
		count ++;
	}
	
	fclose(file);
	
	return count;
}

int delete_all(List_t *list)
{
	
	Node_t 	 *base = list -> top_node;
	Node_t 	 *next;
	Object_t *object_adr; 
	int count = 0;
	
	while(base != NULL){
		object_adr = (Object_t*)(base -> object);
		
		next = base -> next_node;
		
		free(object_adr);
		free(base);
		
		base = next;
		list -> count--;
		count++;
	}
	
	list -> top_node = NULL;
	
	return count;
}

//////////////////////////////////////////////////////////////////////

int main()
{
	List_t *my_list = create_list();
	COMAND command = HELP;
	
	char Hello_text[] = "Hello my cat list! ";
	printf("%s\n", Hello_text);
	
	do{
		
		switch(command){
			
			case FREE:
				printf("\n");
				printf("Get command for list! \n");
				scanf("%d", &command);
				scanf("%*[^\n]");			/*очищение строки (в случае ввода неправильной команды)*/
				break;
				
			case ADD:
				{
					char name[NUM_SYMBOL_NAME];
					float weight = 0;
					printf("Cat's name (<%d symbols): ", NUM_SYMBOL_NAME-1);
					scanf("%16s", &name);
					printf("Cat's weight: ");
					scanf("%f", &weight);
					
					Object_t *object_adr = create_obj(weight, name);
					push_back(my_list, object_adr);
					
					command = FREE;
				};
				break;
				
			case DEL:
				if(my_list -> count == 0)  
					printf("Warning! The list is empty. Add cat.\n");
				else{
					int number = 0;
					printf("Which cat's number do you want to delete? [%d - exit]\n", EXIT_COMMAND);
					scanf("%d", &number);
						
					if(number != EXIT_COMMAND){
						if(number <= 0)						printf("Error! The cat's number must be > 0.");
						else if(my_list -> count < number)	printf("Error! The number of cats in the list = %d. \n", my_list -> count);
						else{
							delete_node(my_list, number);
							printf("Success! \n");
						}
					}
				}
				
				command = FREE;
				break;
				
			case SHOW_ALL:
				if(my_list -> count == 0)		printf("Warning! The list is empty. Add cat.\n");
				else							print_list(my_list);
				
				command = FREE;
				break;
				
			case SHOW_ONE:
				if(my_list -> count == 0)  
					printf("Warning! The list is empty. Add cat.\n");
				else{
					int number = 0;
					printf("Which cat do you want to see? [%d - exit] \n", EXIT_COMMAND);
					scanf("%d", &number);
					
					/*проверка на наличие элемента указанного номера*/
					if(number != EXIT_COMMAND){
						if(number <= 0)
							printf("Error! The cat's number must be > 0. \n");
						else if(my_list -> count < number)
							printf("Error! The number of cats in the list = %d. \n", my_list -> count);
						else{
							printf("%3s | %15s | %10s \n", "Num", "Name      ", "Weight, kg");
							print_node(my_list, number);
						}
					}
				}
				
				command = FREE;
				break;
				
			case SORT_LIST:
				if(my_list -> count == 0){
					printf("Warning! The list is empty. Add cat.\n");
					command = FREE;
				}		
				else if(my_list -> count == 1){			//не нужно сортировать 1 элемент
					print_node(my_list, 1);
					command = FREE;
				}
				else{
					FLAG flg = 0;
					
					printf("Choose an attribute : 1 - weight, 2 - name\n");
					scanf("%d", &flg);
					scanf("%*[^\n]");
					
					if(flg != WEIGHT && flg != NAME){
						printf("Error! The command does not exist\n");
						command = FREE;
					}
					else{
						sort_list(my_list, flg);
						command = SHOW_ALL;
					}
				}
				
				break;
				
			case FILT_LIST:
				if(my_list -> count == 0){
					printf("Warning! The list is empty. Add cat.\n");
				}
				else{
					FLAG flg = 0;
					
					printf("Choose an attribute : 1 - weight, 2 - name\n");
					scanf("%d", &flg);
					scanf("%*[^\n]");
					
					if(flg != WEIGHT && flg != NAME){
						printf("Error! The command does not exist\n");
					}
					else{
						printf("Enter a number: show cats with ");
						float number = 0;
						
						if(flg == WEIGHT)		printf("a weight >= ");
						else if(flg == NAME)	printf("a name length >= ");
						
						scanf("%f", &number);
						scanf("%*[^\n]");
						
						if(filt_list(my_list, flg, number) == FALSE)
							printf("There are no cats in the list by filter \n");
					}
				}
				
				command = FREE;
				break;
			
			case READ_FILE:
				printf("Write name file (<%d symbols): ", NUM_SYMBOL_NAME_FILE-1);
				char file_name[NUM_SYMBOL_NAME_FILE];
				scanf("%50s", &file_name);
				
				unsigned int num_cats = 0;
				num_cats = read_file(my_list, file_name);
				
				if(num_cats == ERROR)   printf("File is not find :( \n");
				else					printf("%d cats have been added in list! \n", num_cats);
				
				command = FREE;
				break;
				
			case WRITE_FILE:
				if(my_list -> count == 0)		printf("Warning! The list is empty. Add cat.\n");
				else{
					printf("Write name file (<%d symbols): ", NUM_SYMBOL_NAME_FILE-1);
					char file_name[NUM_SYMBOL_NAME_FILE];
					scanf("%50s", &file_name);
					
					unsigned int num_cats = 0;
					num_cats = write_file(my_list, file_name);
					
					if(num_cats == ERROR)		printf("File is not create :(\n");
					else						printf("%d cats were recorded in file! \n", num_cats);
				}
				
				command = FREE;
				break;
				
			case DEL_ALL:
				if(my_list -> count != 0)	printf("Success! The number of deleted cats = %d. \n", delete_all(my_list));
				else						printf("Warning! The list is empty. Add cat.\n");
				
				command = FREE;
				break;
				
			case HELP:
				printf("Available commands: \n");
				printf(" - 1 add new cat \n");
				printf(" - 2 delete one cat  \n");
				printf(" - 3 show all the cat \n");
				printf(" - 4 show the cat by number \n");
				printf(" - 5 sorting the list by attribute \n"); 
				printf(" - 6 filtering the list by attribute \n"); 
				printf(" - 7 add cats from a file \n");    
				printf(" - 8 write all cats to a file \n"); 
				printf(" - 9 delete all cats \n"); 
				printf(" - 10 help \n");
				printf(" - 11 exit \n");
				command = FREE;
				break;
				
			default:
				printf("Error! The command does not exist\n");
				command = FREE;
		}
	}while(command != EXIT);
	
	
	/*выход из бесконечного цикла, завершение работы программы, освобождение выделенной памяти*/
	printf("Bye!\n");
	delete_all(my_list);
	free(my_list);	
	
	return 0;
}

