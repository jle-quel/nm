#ifndef FT_NM_H
#define FT_NM_H

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES
////////////////////////////////////////////////////////////////////////////////

#include "./standard.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/nlist.h>
#include <mach-o/ranlib.h>
#include <ar.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define VALID_ARGV_SIZE 2
#define STATE_MACHINE_SIZE 4
#define X86_64 16777223
#define AR_MAGIC 0x0a3e686372613c21

////////////////////////////////////////////////////////////////////////////////
/// ENUMS
////////////////////////////////////////////////////////////////////////////////

typedef enum				e_code
{
	NO_FILE,
	FILE_NOT_VALID,
	OPEN_FAILED,
	CORRUPTED_FILE,
	STAT_FAILED,
	MMAP_FAILED,
}							t_code;

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

typedef struct				s_stat
{
	size_t					index;
	int						bss;
	int						data;
	int						text;
}							t_stat;

typedef struct				s_symbol
{
	uint64_t				value;
	uint8_t					type;
	uint8_t					n_type;
	char const 				*name;
}							t_symbol;

typedef struct				s_tree
{
	struct s_symbol			*symbol;
	struct s_tree			*left;
	struct s_tree			*right;
}							t_tree;

typedef struct				s_object
{
	void					*ptr;
	void const				*eof;
	char const				*name;
	char					*title;
	size_t 					size;
	bool					stupid_nm;
}							t_object;

typedef struct				s_state_machine
{
	uint32_t const			magic_number;

	void					(*f)(struct s_object *);
}							t_state_machine;

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

/// NM

void						nm(char **av);
__hot_function void			handler(t_object *object);
void						macho64(t_object *object);
void						macho32(t_object *object);
void						fat_little(t_object *object);
void						fat_big(t_object *object);
void						archive(t_object *object);

/// OBJECT.C

__hot_function t_object		*get_object(char *filename, size_t const size);

/// TREE.C

t_tree						*add_to_tree(t_tree *tree, t_symbol const *symbol);

/// ERROR.C

void						*error(t_code const code, char const *filename);
void						set_result(uint8_t const result);
uint8_t						*get_result(void);

/// LIB.C

__overload_function void	release(t_object *object);
__overload_function void	release(t_tree *tree);
size_t						get_size(char **vector);
void						*constructor(size_t const size);

/// RENDER.C

void						title(t_object const *object);
void						render64(t_tree const *tree);
void						render32(t_tree const *tree);
void						render(t_tree const *tree, void (*f)(t_tree const *tree));

/// SYMTAB.C

struct symtab_command		*get_symtab(t_object const *object, struct load_command const *lc, uint32_t const ncmds);
bool						is_corrupted(t_object const *object, struct symtab_command const *symtab);

/// UTILS.C

struct load_command			*get_load_command(t_object const *object, size_t const size);
bool						is_segment_valid(char const *segname, uint32_t const filetype);
void						set_stat(t_stat *stat, char const *sectname);
bool						check_ptr(char const *str, t_object const *object);

/// TYPE.C

char						get_type(t_stat const *stat, uint8_t const n_type, uint8_t const n_sect, uint8_t const n_value);

/// STAT

t_stat						*get_stat64(t_object const *object, struct load_command const *lc, struct mach_header_64 const *header);
t_stat						*get_stat32(t_object const *object, struct load_command const *lc, struct mach_header const *header);

#endif
