#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h> 
#include <stdio.h>
#include <memory.h>

#define MEM_HDR_MAGIC (0x4D4D4150)
#define MEM_PAGE_SIZE (0x1000)
#define MEM_DEF_ALIGN_SIZE (0x4)
#define MEM_FILL_PATTERN (0x4D)

typedef struct mem_hdr_s
{
	unsigned int size;
	unsigned int aligned_size;
	unsigned int real_size;
	void *p;
	void *real_p;
	void *caller;
	unsigned int magic;
} mem_hdr_t;

#define DIE(caller) \
do \
{ \
		printf("DIE AT %s %d %p\n", __FUNCTION__, __LINE__, (caller)); \
		fflush(stdout); \
		*(int *)0 = 0; \
} while (0)


#if 1
void *mmap_malloc(unsigned int size)
{
	unsigned int real_size;
	unsigned char *real_p;
	mem_hdr_t *hdr;
	unsigned int aligned_size;
	void *caller = __builtin_return_address(0);
	
	aligned_size = (size + (MEM_DEF_ALIGN_SIZE - 1)) & (~(MEM_DEF_ALIGN_SIZE - 1));
	
	real_size = (((aligned_size + sizeof(mem_hdr_t)) + (MEM_PAGE_SIZE - 1)) & (~(MEM_PAGE_SIZE - 1))) + MEM_PAGE_SIZE;
	real_p = (unsigned char *)mmap (NULL, real_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
	if (real_p == NULL)
	{
		DIE(caller);
	}
	
	if (mprotect (real_p + real_size - MEM_PAGE_SIZE, MEM_PAGE_SIZE, PROT_READ) != 0)
	{
		DIE(caller);
	}
	
	hdr = (mem_hdr_t *)(real_p + real_size - MEM_PAGE_SIZE - aligned_size - sizeof(mem_hdr_t));
	hdr->magic = MEM_HDR_MAGIC;
	hdr->real_size = real_size;
	hdr->real_p = real_p;
	hdr->size = size;
	hdr->aligned_size = aligned_size;
	hdr->p = (void *)(hdr + 1);
	
	hdr->caller = caller;
	
	if (aligned_size != size)
	{
		memset((unsigned char *)hdr->p + size, MEM_FILL_PATTERN, aligned_size - size);
	}
	
	return hdr->p;
}

void mmap_free(void *p)
{
	unsigned int i;
	mem_hdr_t *hdr;
	
	hdr = ((mem_hdr_t *)p) - 1;
	if (hdr->magic != MEM_HDR_MAGIC)
	{
		DIE((void *)0);
	}

	if (hdr->p != p)
	{
		DIE((void *)0);
	}

	for (i=0; i<(hdr->aligned_size - hdr->size); i++)
	{
		if (*(((unsigned char *)p) + hdr->size + i) != MEM_FILL_PATTERN)
		{
			DIE(hdr->caller);
		}
	}
	
	munmap(hdr->real_p, hdr->real_size);
}

void *mmap_realloc(void *p, unsigned int size)
{
	void *new_mem;
	mem_hdr_t *hdr;
	
	hdr = ((mem_hdr_t *)p) - 1;
	if (hdr->magic != MEM_HDR_MAGIC)
	{
		DIE((void *)0);
	}

	new_mem = mmap_malloc(size);
	if (new_mem != NULL)
	{
		memcpy(new_mem, p, hdr->size > size ? size : hdr->size);

		mmap_free(p);
	}

	return new_mem;
}


void *__wrap_malloc(int size)
{	printf("Calling mmap_malloc\n");
	return mmap_malloc(size);
}

void *__wrap_realloc(void *p, int size)
{
	printf("Calling mmap_realloc\n");
	return mmap_realloc(p, size);
}

void *__wrap_calloc(int n, int size)
{
	printf("Calling mmap_malloc\n");
	return mmap_malloc(n * size);
}

void __wrap_free(void *p)
{
	printf("Calling mmap_free\n");
	mmap_free(p);
}


#else
void *mmap_malloc(unsigned int size)
{
	return malloc(size);
}

void *mmap_realloc(void *p, unsigned int size)
{
	return realloc(p,size);
}

void mmap_free(void *p)
{
	free(p);
}
#endif