#ifndef __MALLOC_H__
#define __MALLOC_H__


void alloc_init( void *heap, unsigned len );
void *malloc( unsigned size );
void free( void *ptr );





#endif /* __MALLOC_H__ */
