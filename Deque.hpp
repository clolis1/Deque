#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include <string.h>

#ifndef DEQUE_H
#define DEQUE_H
#define Deque_DEFINE(t)																	\
																			\
/*Forward declarations*/																\
																			\
bool empty_##t (struct Deque_##t * deq);														\
void Deque_##t##_dtor (struct Deque_##t *);														\
size_t size_##t (struct Deque_##t *);															\
struct Deque_##t##_Iterator begin_##t (struct Deque_##t *);												\
struct Deque_##t##_Iterator end_##t (struct Deque_##t *);												\
void push_back_##t (struct Deque_##t *, t);														\
void push_front_##t (struct Deque_##t *, t);														\
void pop_front_##t (struct Deque_##t *);														\
void pop_back_##t (struct Deque_##t *);															\
t& front_##t (struct Deque_##t *);															\
t& back_##t (struct Deque_##t *);															\
t& at_##t (struct Deque_##t *, size_t);															\
void clear_##t (struct Deque_##t *);															\
void sort_##t (struct Deque_##t *, struct Deque_##t##_Iterator, struct Deque_##t##_Iterator);								\
size_t GCF_##t (size_t x, size_t y);															\
int compare_adapter_##t (const void *, const void *, void *);												\
																			\
struct Deque_##t {																	\
	char type_name[sizeof("Deque_") + sizeof(#t) - 1];												\
																			\
	t * array;																	\
	size_t front_i;																	\
	size_t back_i;																	\
	size_t current_length;																\
	size_t current_size;																\
																			\
	bool (*compare) (const t&, const t&);														\
																			\
	bool (*empty)(struct Deque_##t *);														\
																			\
	void (*dtor)(struct Deque_##t *);														\
																			\
	size_t (*size)(struct Deque_##t *);														\
																			\
	struct Deque_##t##_Iterator (*begin) (struct Deque_##t *);											\
	struct Deque_##t##_Iterator (*end) (struct Deque_##t *);											\
																			\
	void (*push_back) (struct Deque_##t *, t);													\
	void (*push_front) (struct Deque_##t *, t);													\
																			\
	void (*pop_front) (struct Deque_##t *);														\
	void (*pop_back) (struct Deque_##t *);														\
																			\
	t& (*front) (struct Deque_##t *);														\
	t& (*back) (struct Deque_##t *);														\
	t& (*at) (struct Deque_##t *, size_t);														\
																			\
	void (*clear) (struct Deque_##t *);														\
																			\
	void (*sort) (struct Deque_##t *, struct Deque_##t##_Iterator, struct Deque_##t##_Iterator);							\
};																			\
																			\
/* Forward declarations of Iterator function pointers */												\
void inc_##t (struct Deque_##t##_Iterator*);														\
void dec_##t (struct Deque_##t##_Iterator*);														\
t& deref_##t (struct Deque_##t##_Iterator*);														\
																			\
struct Deque_##t##_Iterator {																\
	struct Deque_##t * deq_p;															\
																			\
	size_t loc;																	\
																			\
	void (*inc) (struct Deque_##t##_Iterator*);													\
	void (*dec) (struct Deque_##t##_Iterator*);													\
																			\
	t& (*deref) (struct Deque_##t##_Iterator*);													\
};																			\
																			\
void Deque_##t##_ctor(struct Deque_##t * deq, bool (*f)(const t&, const t&))										\
{																			\
	deq->array = (t *) malloc(10*sizeof(t)); /* Allocates space for 10 objects of type t */								\
	deq->front_i = 10;																\
	deq->back_i = 0;																\
	deq->current_length = 10;															\
	deq->current_size = 0;																\
	deq->compare = f; /*sets this function pointer to the one indicated in the constructor.*/							\
	strcpy(deq->type_name, "Deque_");														\
	strcat(deq->type_name, #t);															\
	deq->empty = &empty_##t;															\
	deq->dtor = &Deque_##t##_dtor;															\
	deq->size = &size_##t;																\
	deq->begin = &begin_##t;															\
	deq->end = &end_##t;																\
	deq->push_back = &push_back_##t;														\
	deq->push_front = &push_front_##t;														\
	deq->pop_front = &pop_front_##t;														\
	deq->pop_back = &pop_back_##t;															\
	deq->front = &front_##t;															\
	deq->back = &back_##t;																\
	deq->at = &at_##t;																\
	deq->clear = &clear_##t;															\
	deq->sort = &sort_##t;																\
}																			\
																			\
void Deque_##t##_dtor(struct Deque_##t * deq)														\
{																			\
	free(deq->array);																\
}																			\
																			\
/* size_##t is the size of the deque. That's all. */													\
size_t size_##t(struct Deque_##t * deq)															\
{																			\
	return deq->current_size;															\
}																			\
																			\
/* Returns true if deq is empty. */															\
bool empty_##t(struct Deque_##t * deq)															\
{																			\
	if (deq->current_size == 0)															\
	{																		\
		return true;																\
	}																		\
	return false;																	\
}																			\
																			\
/* Checks the sizes of the deques, then compares to see if the deques have the same contents in the same order. */					\
bool Deque_##t##_equal(struct Deque_##t d1, struct Deque_##t d2)											\
{																			\
	if (empty_##t(&d1) && empty_##t(&d2))														\
	{																		\
		return true;																\
	}																		\
	else if (empty_##t(&d1) || empty_##t(&d2))													\
	{																		\
		return false;																\
	}																		\
																			\
	if (d1.current_size == d2.current_size)														\
	{																		\
		/* Starts at_##t the "front_##t" of d1 and compares to the "front_##t" of d2 */								\
		/* A second Iterator for d2 is shoehorned into the for loop. */										\
		size_t i;																\
		size_t j = d2.front_i;															\
																			\
		size_t stop = d1.back_i + 1; 														\
																			\
		for (i = d1.front_i; i != stop; i++)													\
		{																	\
			if (i == d1.current_length - 1 && stop == d1.current_length &&									\
				(d1.compare(d1.array[i], d2.array[j]) == d2.compare(d2.array[j], d1.array[i])))						\
			{																\
				return true;														\
			}																\
			if (!(d1.compare(d1.array[i], d2.array[j]) == d2.compare(d2.array[j], d1.array[i])))						\
			{																\
				return false;														\
			}																\
																			\
			if (j == d2.current_length - 1)													\
			{																\
				j = 0;															\
			}																\
			else																\
			{																\
				j++;															\
			}																\
																			\
			if (i == d1.current_length - 1)													\
			{																\
				i = -1;															\
			}																\
		}																	\
		return true;																\
	}																		\
	return false;																	\
}																			\
																			\
void clear_##t(struct Deque_##t * deq)															\
{																			\
	if (empty_##t(deq))																\
	{																		\
		return;																	\
	}																		\
	else																		\
	{																		\
		deq->front_i = deq->current_length;													\
		deq->back_i = 0;															\
		deq->current_size = 0;															\
	}																		\
}																			\
																			\
/* All push functions handle if deque is full at start of execution. */											\
void push_back_##t(struct Deque_##t * deq, t mc)													\
{																			\
	/* This if statement allocates memory for a full array. The order of the elements is retained.	 */						\
	if (deq->current_size == deq->current_length)													\
	{																		\
		t * temp = (t *) malloc((deq->current_length*10)*sizeof(t));										\
		if (deq->front_i == 0)															\
		{																	\
			size_t i;															\
			for (i = 0; i < deq->current_length; i++)											\
			{																\
				temp[i] = deq->array[i];												\
			}																\
			deq->current_length = deq->current_length * 10;											\
			free(deq->array);														\
			deq->array = temp;														\
		}																	\
		else																	\
		{																	\
			size_t i;															\
			for (i = (deq->back_i); i < deq->current_length; i--)										\
			{																\
				temp[i] = deq->array[i];												\
			}																\
			for (i = deq->front_i; i < deq->current_length; i++)										\
			{																\
				temp[i+(9*deq->current_length)] = deq->array[i];									\
			}																\
			deq->front_i = deq->front_i + 9*deq->current_length;										\
			deq->current_length = deq->current_length * 10;											\
																			\
			free(deq->array);														\
			deq->array = temp;														\
		}																	\
	}																		\
																			\
	/* If array is empty, set front_i and back_i to 0. */												\
	if (empty_##t(deq))																\
	{																		\
		deq->front_i = 0;															\
		deq->back_i = 0;															\
	}																		\
	/* The array is not full, but the back is at the end of the array. Set back_i to 0. */								\
	else if (deq->back_i == deq->current_length - 1)												\
	{																		\
		deq->back_i = 0;															\
	}																		\
	/* Otherwise, increment the back_i counter by 1. */												\
	else																		\
	{																		\
		deq->back_i++;																\
	}																		\
																			\
	deq->array[deq->back_i] = mc;															\
	deq->current_size++;																\
}																			\
																			\
void push_front_##t(struct Deque_##t * deq, t mc)													\
{																			\
	/* This if statement allocates memory for a full array. The order of the elements is retained.*/						\
	if (deq->current_size == deq->current_length)													\
	{																		\
		t * temp = (t *) malloc((deq->current_length*10)*sizeof(t));										\
		if (deq->front_i == 0)															\
		{																	\
			size_t i;															\
			for (i = 0; i < deq->current_length; i++)											\
			{																\
				temp[i] = deq->array[i];												\
			}																\
			deq->current_length = deq->current_length * 10;											\
			free(deq->array);														\
			deq->array = temp;														\
		}																	\
		else																	\
		{																	\
			size_t i;															\
			for (i = deq->back_i; i < deq->current_length; i--)										\
			{																\
				temp[i] = deq->array[i];												\
			}																\
			for (i = deq->front_i; i < deq->current_length; i++)										\
			{																\
				temp[i+(9*deq->current_length)] = deq->array[i];									\
			}																\
			deq->front_i = deq->front_i + 9*deq->current_length;										\
			deq->current_length = deq->current_length * 10;											\
																			\
			free(deq->array);														\
			deq->array = temp;														\
		}																	\
	}																		\
																			\
	/* If array is empty, set front_i and back_i to 0. */												\
	if (empty_##t(deq))																\
	{																		\
		deq->front_i = 0;															\
		deq->back_i = 0;															\
	}																		\
	/* The array is not full, but the front is at the start of the array. Set front_i to the last entry. */						\
	else if (deq->front_i == 0)															\
	{																		\
		deq->front_i = deq->current_length - 1;													\
	}																		\
	/* Otherwise, decrement the front_i counter by 1. */												\
	else																		\
	{																		\
		deq->front_i--;																\
	}																		\
																			\
	deq->array[deq->front_i] = mc;															\
	deq->current_size++;																\
}																			\
																			\
void pop_front_##t(struct Deque_##t * deq)														\
{																			\
	if (empty_##t(deq))																\
	{																		\
/*		My faithful, stand firm. */														\
		return;																	\
	}																		\
																			\
	if (deq->current_size == 1)															\
	{																		\
		deq->front_i = deq->current_length;													\
		deq->back_i = 0;															\
	}																		\
	else if (deq->front_i == deq->current_length - 1 || deq->front_i == deq->current_length)							\
	{																		\
		deq->front_i = 0;															\
	}																		\
	else																		\
	{																		\
		deq->front_i++;																\
	}																		\
	deq->current_size--;																\
}																			\
																			\
void pop_back_##t(struct Deque_##t * deq)														\
{																			\
	if (empty_##t(deq))																\
	{																		\
/*		Were it so easy... */															\
		return;																	\
	}																		\
																			\
	if (deq->current_size == 1)															\
	{																		\
		deq->front_i = deq->current_length;													\
		deq->back_i = 0;															\
	}																		\
	else if (deq->back_i == 0)															\
	{																		\
		deq->back_i = deq->current_length - 1;													\
	}																		\
	else																		\
	{																		\
		deq->back_i--;																\
	}																		\
	deq->current_size--;																\
}																			\
																			\
t& front_##t(struct Deque_##t * deq)															\
{																			\
	if (empty_##t(deq))																\
	{																		\
/*		Someone... anyone... respond! */													\
		return deq->array[0];															\
	}																		\
	else																		\
	{																		\
		return deq->array[deq->front_i];													\
	}																		\
}																			\
																			\
t& back_##t(struct Deque_##t * deq)															\
{																			\
	if (empty_##t(deq))																\
	{																		\
/*		Truly, if he were any better...	*/													\
		return deq->array[0];															\
	}																		\
	else																		\
	{																		\
		return deq->array[deq->back_i];														\
	}																		\
}																			\
																			\
t& at_##t(struct Deque_##t * deq, size_t x)														\
{																			\
	if (empty_##t(deq))																\
	{																		\
/*		He had something the others didn't-- luck. */												\
		return deq->array[0];															\
	}																		\
																			\
	x = x + deq->front_i;																\
	if (x >= deq->current_length)															\
	{																		\
		x = x % deq->current_length;														\
	}																		\
																			\
	return deq->array[x];																\
}																			\
																			\
/* Creates a Deque_##t##_Iterator that begins by pointing at the start. */										\
struct Deque_##t##_Iterator begin_##t(struct Deque_##t * deq)												\
{																			\
	struct Deque_##t##_Iterator it;															\
	it.loc = 0;																	\
	it.deq_p = deq;																	\
	it.inc = &inc_##t;																\
	it.dec = &dec_##t;																\
	it.deref = &deref_##t;																\
	return it;																	\
}																			\
																			\
/* Creates a Deque_##t##_Iterator that begins by pointing at the end. */										\
struct Deque_##t##_Iterator end_##t(struct Deque_##t * deq)												\
{																			\
	struct Deque_##t##_Iterator it;															\
	it.loc = deq->current_size;															\
	it.deq_p = deq;																	\
	it.inc = &inc_##t;																\
	it.dec = &dec_##t;																\
	it.deref = &deref_##t;																\
	return it;																	\
}																			\
																			\
bool Deque_##t##_Iterator_equal(struct Deque_##t##_Iterator i1, struct Deque_##t##_Iterator i2)								\
{																			\
	if ((i1.loc == i2.loc) && Deque_##t##_equal(*(i1.deq_p), *(i2.deq_p)))										\
	{																		\
		return true;																\
	}																		\
	return false;																	\
}																			\
																			\
void inc_##t(struct Deque_##t##_Iterator* it)														\
{																			\
	if (size_##t(it->deq_p) == 0)															\
	{																		\
/*		Never give up. */															\
		return;																	\
	}																		\
																			\
	if ((size_t) it->loc == size_##t(it->deq_p))													\
	{																		\
		it->loc = 0;																\
	}																		\
	else																		\
	{																		\
		it->loc++;																\
	}																		\
}																			\
																			\
void dec_##t(struct Deque_##t##_Iterator* it)														\
{																			\
	if (size_##t(it->deq_p) == 0)															\
	{																		\
/*		Trust your instincts. */														\
		return;																	\
	}																		\
																			\
	if (it->loc == 0)																\
	{																		\
		it->loc = size_##t(it->deq_p) - 1;													\
	}																		\
	else																		\
	{																		\
		it->loc--;																\
	}																		\
}																			\
																			\
t& deref_##t (Deque_##t##_Iterator* it)															\
{																			\
	return at_##t(it->deq_p, it->loc);														\
}																			\
																			\
/** Sorts the area in between the two iterators */													\
void sort_##t (struct Deque_##t * deq, struct Deque_##t##_Iterator it1, struct Deque_##t##_Iterator it2)						\
{																			\
	if (it1.loc == it2.loc || it1.loc - it2.loc == 1 || it2.loc - it1.loc == 1) /** Empty or Size 1 Array */					\
	{																		\
		return;																	\
	}																		\
	else if (((it1.loc == 0) && (it2.loc == deq->current_length - 1)) ||										\
		 ((it2.loc == 0) && (it1.loc == deq->current_length - 1)))  /** Full array */								\
	{																		\
		qsort_r(deq->array, deq->current_length, sizeof(t), compare_adapter_##t, deq);								\
	}																		\
	else if (it1.loc < it2.loc) /** it1 has the lower bound */											\
	{																		\
		if (deq->front_i < deq->back_i) /** Array is contiguous */										\
		{																	\
			qsort_r(deq->array + deq->front_i + it1.loc, (it2.loc - it1.loc), sizeof(t), compare_adapter_##t, deq);				\
		}																	\
		else	/** Shift array until contiguous, then sort */											\
		{																	\
			size_t shift_amt = deq->current_length - deq->front_i;										\
			size_t num_chains = GCF_##t(shift_amt, deq->current_length);									\
																			\
			size_t chain_num = 0;														\
			/** Swaps values, going around num_chains times. The amount of chains is equal to the GCF of the shift_amt and length */	\
			while (num_chains > 0)														\
			{																\
				t temp1 = deq->array[chain_num + shift_amt]; 			 							\
				deq->array[chain_num + shift_amt] = deq->array[chain_num];								\
				t temp2 = deq->array[chain_num + shift_amt*2];										\
				size_t i;														\
				for (i = (chain_num + shift_amt); i != chain_num; i = (i + shift_amt) % deq->current_length)				\
				{															\
					deq->array[(i + shift_amt) % deq->current_length] = temp1;							\
					temp1 = temp2;													\
					temp2 = deq->array[(i + shift_amt*2) % deq->current_length];							\
				}															\
				chain_num++;														\
				num_chains--;														\
			}																\
			deq->front_i = 0;														\
			deq->back_i = deq->current_size - 1;												\
			qsort_r(deq->array, (it2.loc - it1.loc), sizeof(t), compare_adapter_##t, deq);							\
		}																	\
	}																		\
	else /** it2 has the lower bound */														\
	{																		\
		if (deq->front_i < deq->back_i) /** Array is contiguous */										\
		{																	\
			qsort_r(deq->array + deq->front_i + it2.loc, (it1.loc - it2.loc), sizeof(t), compare_adapter_##t, deq);				\
		}																	\
		else	/** Shift array until contiguous, then sort */											\
		{																	\
			size_t shift_amt = deq->current_length - deq->front_i;										\
			size_t num_chains = GCF_##t(shift_amt, deq->current_length);									\
																			\
			size_t chain_num = 0;														\
			/** Swaps values, going around num_chains times. The amount of chains is equal to the GCF of the shift_amt and length */	\
			while (num_chains > 0)														\
			{																\
				t temp1 = deq->array[chain_num + shift_amt];										\
				deq->array[chain_num + shift_amt] = deq->array[chain_num];								\
				t temp2 = deq->array[chain_num + shift_amt*2];										\
				size_t i;														\
				for (i = (chain_num + shift_amt); i != chain_num; i = (i + shift_amt) % deq->current_length)				\
				{															\
					deq->array[(i + shift_amt) % deq->current_length] = temp1;							\
					temp1 = temp2;													\
					temp2 = deq->array[(i + shift_amt*2) % deq->current_length];							\
				}															\
				chain_num++;														\
				num_chains--;														\
			}																\
			deq->front_i = 0;														\
			deq->back_i = deq->current_size - 1;												\
			qsort_r(deq->array + it2.loc, (it1.loc - it2.loc), sizeof(t), compare_adapter_##t, deq);					\
		}																	\
	}																		\
}																			\
																			\
size_t GCF_##t(size_t x, size_t y)	/** Code borrowed from programiz.com */										\
{																			\
	size_t gcf = 1;																	\
	size_t i;																	\
	for(i = 1; i <= x && i <= y; i++)														\
    	{																		\
        	/** Checks if i is factor of both integers */												\
        	if(x % i == 0 && y % i == 0)														\
        	{																	\
        		gcf = i;															\
        	}																	\
    	}																		\
    	return gcf;																	\
}																			\
																			\
int compare_adapter_##t(const void * x, const void * y, void * deq)											\
{																			\
	if (((struct Deque_##t*) deq)->compare(*(t*) x, *(t*) y) == 0 && ((struct Deque_##t*) deq)->compare(*(t*) y, *(t*) x) == 0)			\
	{																		\
		return 0;																\
	}																		\
	else if (((struct Deque_##t*) deq)->compare(*(t*) x, *(t*) y) == 0)										\
	{																		\
		return 1;																\
	}																		\
	else																		\
	{																		\
		return -1;																\
	}																		\
	/** One who knows nothing can understand nothing. */												\
	return 117;																	\
}
#endif /* DEQUE_H */
