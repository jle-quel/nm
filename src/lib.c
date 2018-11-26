#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

size_t		get_size(char **vector)
{
	size_t	size = 0;
	
	if (vector)
	{
		while (vector[size])
			size++;
	}

	return size;
}

void		*constructor(size_t const size)
{
	void	*ptr = NULL;
	
	if ((ptr = malloc(size)) == NULL)
	{
		printf("fatal: out of memory, malloc failed\n");
		exit(1);
	}

	bzero(ptr, size);

	return (ptr);
}

__overload_function void	release(t_object *object)
{
	munmap(object->ptr, object->size);
	free(object);
}

__overload_function void	release(t_tree *tree)
{
	if (tree)
	{
		release(tree->left);
		release(tree->right);
		free(tree->symbol);
		free(tree);
	}
}
