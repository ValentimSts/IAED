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
