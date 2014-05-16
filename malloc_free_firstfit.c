#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

#define align4(x) (((((x)-1)>>2)<<2)+4)
//# define BLOCK_SIZE 17
void *base = NULL;
typedef struct s_block *t_block;

struct s_block {
	size_t size;
	t_block next;
	int free;
	t_block prev;
	void *ptr;
	char data[1];
};

# define BLOCK_SIZE (sizeof(struct s_block))

t_block get_block(void *p)
{
	char *tmp;
	tmp = p;
	
	tmp+=0x4;
	tmp-=BLOCK_SIZE;
	p=tmp;
	return (p);
}

int valid_addr(void *p)
{
	if(base)
	{
		//printf("1");
		if(p>base)
			if(p<sbrk(0))
		{
			printf("adfadfadf");
			return(p==(get_block(p))->ptr);
		}
	}
return (0);	

}

t_block extend_heap(t_block last, size_t s){
	t_block b;
	b = sbrk(0);
	if(sbrk(BLOCK_SIZE + s) == (void *)-1)
		return NULL;
	b->size = s;
	b->next = NULL;
	b->ptr = b->data;
	if(last)
		last->next = b;
	b->free = 0;
	
	return (b);
}

t_block find_block ( t_block *last , size_t size ){
	t_block b=base;
	while (b && !(b->free && b->size >= size )) {
	*last = b;
	b = b->next;
	
	}
	return (b);
}

void split_block ( t_block b, size_t s){
	t_block new;
	new = b->data + s;
	new->size = b->size -s -BLOCK_SIZE;
	new->next = b->next;
	new->ptr = new->data;
	new->free = 1;
	b->size = s;
	b->next = new;
}

void *my_malloc(size_t size){
	t_block b,last;
	size_t s;
	s=align4(size);
	if(base){
		last = base;
		b=find_block(&last,s);
		if(b){
			if((b->size - s) >= (BLOCK_SIZE + 4))
				split_block(b,s);
			b->free = 0;
		}else {
			b = extend_heap(last, s);
			if(!b)
				return NULL;
		}
	}else {
		b=extend_heap(NULL,s);
		if(!b)
			return NULL;
		base = b;				
	}
	return (b->data);
}

t_block fusion(t_block  b){
	if(b->next && b->next->free){
		b->size += BLOCK_SIZE + b->next->size;
		b->next = b->next->next;
		if(b->next)
			b->next->prev = b;
	}
	return (b);
}

void free(void *p){
	t_block b;
	if(valid_addr(p))
	{
		b=get_block(p);
		b->free = 1;
		if(b->prev && b->prev->free)
			b=fusion(b->prev);
		if(b->next)
			fusion(b);
		else
		{
			if(b->prev)
				b->prev->next = NULL;
			else
				base = NULL;
			brk(b);	
		}
	}
}

int main()
{
int *p;
p=(int *)my_malloc(1);
if(p!=NULL){
	*p=1;
	printf("%d\n",*p);}
else
	printf("Error u r dead\n");
free(p);
printf("%d",*p);
return 0;
}
