#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

#define align4(x) (((((x)-1)>>2)<<2)+4)
int flag = 0;

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
		if(p>base)
			if(p<sbrk(0))
		{
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

t_block find_worst_block ( t_block *last , size_t size ){
	t_block b=base;
	size_t maxsize = 0;
	t_block max = NULL;
	while (b) {
	if(b->free)
	{
		if(maxsize < b->size)
		{
			maxsize = b->size;
			max = b;
		}
	}
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

void *my_malloc_first(size_t size){
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

void print()
{
	t_block b;
	b = base;
	while(b)
	{
		printf("Memory allocated at : %p and free flag is: %d\nSize: %p\nData address: %p\n", b, b->free, &(b->size), &(b->data[0]));
		b=b->next;
	}
}

void *my_malloc_worst(size_t size){
	t_block b,last;
	size_t s;
	s=align4(size);
	if(base){
		last = base;
		b=find_worst_block(&last,s);
		if(b){
			//if((b->size - s) >= (BLOCK_SIZE + 4))
				//split_block(b,s);
			b->free = 0;
		}
		else {
			if (flag == 0)
				b = extend_heap(last, s);
			if(!b)
				return NULL;
		}
	}
	else {
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
	printf("%d\n", BLOCK_SIZE);
	int *p[100];
	int i;
	int count = 0;
	int *q[50];
	time_t t;
	srand(1);
	for(i=0;i<100;i++)
	{
		int ran = rand()%100;
		p[i]=(int*)my_malloc_worst(sizeof(int)*ran);
		if(p[i]!=NULL)
		{
			printf("Pointer address: %p and size = %d\n", p[i],ran);
		}
		else
			printf("Not Allocated! \n");
	}
	for(i=0;i<50;i++)
	{
		int ran = (rand()%100);
		free(p[ran]);
		if(p[ran]==NULL)
		{
			printf("%d is deallocated!\n", ran);
		}			
	}
	printf("Base : %p\n", base);
	print();
	flag = 1;
	for(i=0;i<50;i++)
	{
		int ran = (rand()%100);
		q[i]=(int*)my_malloc_worst(sizeof(int)*ran);
		if(q[ran]!=NULL)
		{
			count++;
		}			
	}
	printf("Count: %d\n", count);	

	//free(p);
	//printf("%d\n",*p);
	return 0;
}
