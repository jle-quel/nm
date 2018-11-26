#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline t_tree *new_ptr(t_symbol const *symbol)
{
	t_tree *tree = constructor(sizeof(t_tree));

	tree->symbol = (t_symbol *)symbol;
	tree->left = NULL;
	tree->right = NULL;

	return tree;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

t_tree *add_to_tree(t_tree *tree, t_symbol const *symbol)
{
	int result = 0;

	if (tree == NULL)
		return new_ptr(symbol);

	if ((result = strcmp(symbol->name, tree->symbol->name)) < 0)
		tree->left = add_to_tree(tree->left, symbol);
	else if (result == 0)
	{
		if (symbol->value <= tree->symbol->value)
			tree->left = add_to_tree(tree->left, symbol);
		else
			tree->right = add_to_tree(tree->right, symbol);
	}
	else
		tree->right = add_to_tree(tree->right, symbol);

	return tree;
}