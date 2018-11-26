#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void	title(t_object const *object)
{
	if (object->title)
	{
		if (object->stupid_nm)
			printf("%s:\n", object->title);
		else
			printf("\n%s:\n", object->title);
	}
}

void	render64(t_tree const *tree)
{
	if (tree->symbol->n_type & N_SECT)
		printf("%016llx ", tree->symbol->value);
	else
		printf("                 ");
}	

void	render32(t_tree const *tree)
{
	if (tree->symbol->n_type & N_SECT)
		printf("%08llx ", tree->symbol->value);
	else
		printf("         ");
}	


void	render(t_tree const *tree, void (*f)(t_tree const *tree))
{
	if (tree)
	{
		render(tree->left, f);
		f(tree);
		printf("%c ", tree->symbol->type);
		printf("%s\n", tree->symbol->name);
		render(tree->right, f);
	}
}