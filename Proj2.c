#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ============================= *
 *	Valentim Santos - ist199343	 *
 * ============================= */


#include "tree.h"
#include "hash.h"


/* Define wall */
#define MAX_CHAR 65536

#define HELP "help"
#define QUIT "quit"
#define SET "set"
#define PRINT "print"
#define FIND "find"
#define LIST "list"
#define SEARCH "search"
#define DELETE "delete"

#define PRINT_HELP "help: Imprime os comandos disponíveis."
#define PRINT_QUIT "quit: Termina o programa."
#define PRINT_SET "set: Adiciona ou modifica o valor a armazenar."
#define PRINT_PRINT "print: Imprime todos os caminhos e valores."
#define PRINT_FIND "find: Imprime o valor armazenado."
#define PRINT_LIST "list: Lista todos os componentes imediatos de um sub-caminho."
#define PRINT_SEARCH "search: Procura o caminho dado um valor."
#define PRINT_DELETE "delete: Apaga um caminho e todos os subcaminhos."

#define NOT_FOUND "not found"
#define NO_DATA "no data"


/* main functions */
void read_input(hlink** table, tlink* root);
void help();
void set(hlink** table, tlink* root, char* path, char* value, char** paths);
void print(tlink* root);
void find(hlink** table);
void list(hlink** table, tlink* root);
void search(tlink* root, char* value);

/* Aux functions */
char* input_parse();
char** sub_paths(char* path);
char* read_value();
void alpha_sort(char **lst, unsigned int len);
void swap(char **a, char **b);



int main() {
	/* initializes the hashtable */
	hlink** table = init_hash();
	tlink* root = init_tree();

	read_input(table, root);
	
	return 0;
}



void read_input(hlink** table, tlink* root) {

	int quit = 0;
	char *command = NULL;
	char clean_str[MAX_CHAR];

	char* path = NULL;
	char* value = NULL;
	char** paths = NULL;


	while(!quit) {

		scanf("%s", clean_str);
		command = (char*) malloc(sizeof(char)*(strlen(clean_str)+1));
		strcpy(command, clean_str);

		if(!strcmp(command, HELP)) {
			help();
		}
		else if(!strcmp(command, QUIT)) {
			quit = 1;
		}
		else if(!strcmp(command, SET)) {

			path = input_parse();
			value = read_value();
			paths = sub_paths(path);

			set(table, root, path, value, paths);
		}
		else if(!strcmp(command, PRINT)) {
			print(root);
		}
		else if(!strcmp(command, FIND)) {
			find(table);
		}
		else if(!strcmp(command, LIST)) {
			list(table, root);
		}
		else if(!strcmp(command, SEARCH)) {
			value = read_value();
			search(root, value);
		}
		else if(!strcmp(command, DELETE)) {
			/*delete();*/
		}

		free(command);
 	}
}




/* Prints all the existing commands as well as their descriptions */
void help() {

	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
			PRINT_HELP, PRINT_QUIT, PRINT_SET,
			PRINT_PRINT, PRINT_FIND, PRINT_LIST,
			PRINT_SEARCH, PRINT_DELETE);

}




void set(hlink** table, tlink* root, char* path, char* value, char** paths) {

	int i, key, len;
	hlink* hash_node = NULL, *hash_aux = NULL;
	tlink* tree_node = NULL, *tree_aux = NULL;


	for(i = 0; paths[i] != NULL; i++) {
		
		key = hash(paths[i]);

		/* if that path is in the hashtable */
		if(!isEmpty(table[key])) {

			if(!strcmp(paths[i], path)) {
				hash_node = get_hash(table, paths[i]);

				len = strlen(value)+1;
				hash_node->value = realloc(hash_node->value, sizeof(char)*len);
				strcpy(hash_node->value, value);

				tree_node = hash_node->t_node;
				tree_node->value = realloc(hash_node->value, sizeof(char)*len);
				strcpy(tree_node->value, value);

				return;
			}

			continue;
		}


		/* found a path that is not currently in the hashtable */
		else {

			if(i == 0 && !strcmp(paths[i], path)) {

				hash_node = insert_hlink(table, paths[i], value);
				tree_node = insert_newtlink(root, value, paths[i]);
				hash_node->t_node = tree_node;

				return;
			}

			else if(i == 0) {
				hash_node = insert_hlink(table, paths[i], NULL);
				tree_node = insert_newtlink(root, NULL, paths[i]);
				hash_node->t_node = tree_node;

				set(table, root, path, value, paths);
			}

			/* we reached the last sub-path, in other words, 
			   the path we want to store the value given */
			else if(!strcmp(paths[i], path)) {


				hash_node = insert_hlink(table, paths[i], value);
				hash_aux = get_hash(table, paths[i-1]);
				tree_aux = hash_aux->t_node;
				tree_node = insert_newtlink(tree_aux, value, paths[i]);
				hash_node->t_node = tree_node;

				return;
			}

			
			/* we havent reached the last sub-path yet */
			else {
				hash_node = insert_hlink(table, paths[i], NULL);
				/* gets the hash position of the previous path to the one
				   we're in now */

				hash_aux = get_hash(table, paths[i-1]);
				/* tree_aux has the node containing the previous path */
				tree_aux = hash_aux->t_node;
				/* inserts paths[i] in the tree */
				tree_node = insert_newtlink(tree_aux, NULL, paths[i]);
				/* makes a pointer from the hash of paths[i] to its tree node */
				hash_node->t_node = tree_node;
				set(table, root, path, value, paths);
			}
		}
	}
}




void print(tlink* root) {

	int i;
	if(!root)
		return;

	for(i = 0; i < root->n; i++) {
		printf("%s", root->children[i]->path);

		if(root->children[i]->value != NULL)
			printf(" %s\n", root->children[i]->value);
		else
			printf("\n");

		print(root->children[i]);
	}
}




void find(hlink** table) {

	char* path = input_parse();
	hlink* head = get_hash(table, path);

	if(head == NULL) {
		printf("%s\n", NOT_FOUND);
		return;
	}

	if(head->value == NULL) {
		printf("%s\n", NO_DATA);
		return;
	}

	printf("%s\n", head->value);
}




void list(hlink** table, tlink* root) {

	int i;
	char** raw_lst = NULL;
	hlink* head = NULL;
	tlink* tree_head = NULL;

	/* gets the input from the user */
	char* path = input_parse();

	/* if path is NULL the user wants to list all the root's sub-paths */
	if(path == NULL)
		tree_head = root;

	else {

		head = get_hash(table, path);

		if(head == NULL) {
			printf("%s\n", NOT_FOUND);
			return;
		}
		
		tree_head = head->t_node;
	}
	raw_lst = (char**) malloc(sizeof(char*) * tree_head->n);

	for(i = 0; i < tree_head->n; i++) {
		raw_lst[i] = (char*) malloc(sizeof(char) * strlen(tree_head->children[i]->path));
		/* we copy every path to the raw_lst array,
		   making sure to copy (tree_head->children[i]->path)+1 so the first '/' is not included*/
		strcpy(raw_lst[i], (tree_head->children[i]->path)+1);
	}

	/* sort the array and print its components */
	alpha_sort(raw_lst, i);

	for(i = 0; i < tree_head->n; i++)
		printf("%s\n", raw_lst[i]);

}




void search(tlink* root, char* value) {

	int i;
	if(!root) {
		printf("%s\n", NOT_FOUND);
		return;
	}

	for(i = 0; i < root->n; i++) {

		if(root->children[i]->value == NULL)
			search(root->children[i], value);

		else if(!strcmp(root->children[i]->value,  value)) {
			printf("%s\n", root->children[i]->path);
			return;
		}

		else
			search(root->children[i], value);
	}

}






/* ============================== ( Aux Functions ) ============================== */



/* reads the path given by the user ( Ex: ///a//b////c/ ), and "cleans" it, getting
   rid of any unnecessary '/' as well as the blank spaces that come before the inpu */
char* input_parse() {

	int len, i, j = 0, in = 0;
	char raw_str[MAX_CHAR], clean_str[MAX_CHAR], c;
	char* path = NULL;

	if((c = getchar()) == '\n' || c == '\0') {
		/* if the first char read is either a '\n' or '\0' returns NULL */
		return path;
	}

	scanf("%s", raw_str);
	/* adds a '/' to the beggining of the path, if needed */
	if(raw_str[0] != '/') {
		clean_str[j++] = '/';
	}

	len = strlen(raw_str);

	for(i = 0; i<len; i++) {
		/* "cleans" the string, getting rid of any extra '/' found */
		if(raw_str[i] == '/' && !in) {
			clean_str[j++] = '/';
			in = 1;
		}
		else if(raw_str[i] == '/' && in) {
			continue;
		}
		else if(raw_str[i] != '/') {
			clean_str[j++] = raw_str[i];
			in = 0;
		}
	}
	/* checks if the end of the string has a '/' if yes then it "deletes" it */
	if(clean_str[j-1] == '/') {
		clean_str[j-1] = '\0';
	}
	else {
		clean_str[j] = '\0';
	}
	
	path = (char*) malloc(sizeof(char)*(strlen(clean_str)+1));
	strcpy(path, clean_str);
	return path;
}



/* receives a path and returns a pointer to an lstay of pointers, each one containing a
   sub-path of the original sub-path ( Ex: /a/b/c --> /a, /a/b, /a/b/c ) */ 
char** sub_paths(char* path) {

	int i, len, path_counter = 0, dim = 5;

	char buffer[MAX_CHAR];
	char** paths = NULL;

	/* gives paths a size of 5 to start with */
	paths = (char**) malloc(sizeof(char*) * dim);

	/* Ex: path = "/a/b/c" */
	len = strlen(path);
	buffer[0] = '/';

	for(i = 1; i<=len; i++) {

		if(path[i] == '/' || path[i] == '\0') {
			/* if a '/' is found it means that we found the end of a sub-path 
			   and need to store it in the paths lstay */

			if(path_counter == dim) {
				dim *= 2;
				paths = (char**) realloc(paths, sizeof(char*) * dim);
			}

		  	paths[path_counter] = (char*) malloc(sizeof(char)*(strlen(buffer)+1));
			strcpy(paths[path_counter++], buffer);
		}
		
		buffer[i] = path[i];
		/* without this line, the function would ALLWAYS break at iteration 16 and Osize_LY 16
		   dont know why, but doing this fixed it */
		buffer[i+1] = '\0';
	}

	for(i = path_counter; i<dim; i++)
		free(paths[i]);

	return paths;
}




char* read_value() {

	int len;
	char buffer[MAX_CHAR], c;
	char* value;

	/* gets the first string of value, ignoring any blank spaces that come before it */
	scanf("%s", buffer);
	len = strlen(buffer);

	while((c = getchar()) != '\n' && c != '\0') {
		/* fills the rest of the lstays with the remaining chars */
		buffer[len++] = c;
	}
	/* at the end adds a '\0' to end the string */
	buffer[len] = '\0';

	len = strlen(buffer)+1;
	value = (char*) malloc(sizeof(char) * len);
	strcpy(value, buffer);
	return value;
}



void swap(char **a, char **b) {

	char *temp = *a;
	*a = *b;
	*b = temp;
}


void alpha_sort(char **lst, unsigned int len) {

	unsigned int i, piv = 0;

	if(len <= 1) 
		return;
	
	for(i = 0; i < len; i++) {

		if(strcmp(lst[i], lst[len - 1]) < 0)
			swap(lst + i, lst + piv++);		
	}

	swap(lst + piv, lst + len - 1);

	alpha_sort(lst, piv++);
	alpha_sort(lst + piv, len - piv);
}
