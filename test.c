/* returns the tree node that has the same path has the one given */
tlink* search_path(tlink* root, char* path) {

	int i, j, check;
	char** paths = NULL;
	tlink* temp_node = root;

	if(root->n == 0)
		return NULL;

	paths = sub_paths(path);

	for(i = 0; paths[i] != NULL; i++) {

		for(j = 0; j< temp_node->n; j++) {

			if(!strcmp(temp_node->children[j]->path, paths[i])) {
				temp_node = temp_node->children[j];
				check = 0;
				break;
			}

			check = 1;
		}

		if(!check)
			continue;

		else {
			free_paths(paths);
			free_tlink(temp_node);
			return NULL;
		}

	}

	free_paths(paths);
	return temp_node;
}


void print_tree(tlink* root) {

	int i;

	if(root == NULL)
		return;

	printf("%s\n", root->value);

	for(i = 0; i<root->n; i++) 
		printf("%s\n", root->children[i]->value);

	print_tree(*(root->children++));
}


void free_tree(tlink* root) {

	int i = 0;

}



void insert_hash(hlink* table, char** paths) {




}



void fill_hash_pos(hlink* table, char* path) {

	int hash = hash(path);

	if(isEmpty(table, hash))
		fill_emptyHash
}


/* builds all the necessary tree nodes, from the head given until the last sub-path of paths */
tlink* build_from_root(tlink* root, char** paths, char* val) {

	int i;
	tlink*
	tlink* Next_node = NULL;

	for(i = 0; paths[i] != NULL; i++) {
		
		head->path = paths[i];
		head->children = (tlink**) realloc(head->children, sizeof(tlink*)*((head->n)+1));

		Next_node = New_tlink(NULL, NULL);

		head->children[head->n] = Next_node;
		head->n++;

		head = Next_node;
	}

	head->value = val;
	return head;
