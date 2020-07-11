/*
 * malloc, from K&R;
 * calloc, checks and bfree added
 */

#include <unistd.h>
#include <stdio.h>

typedef long Align;

union header {
	struct {
		union header *ptr;
		unsigned size;
	} s;
	Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;
static Header *morecore(unsigned);

void init_malloc(void)
{
	base.s.ptr = freep  = &base;
	base.s.size = 0;
}

void *_malloc(unsigned bc)
{
	Header *p, *prevp;
	unsigned nunits;

	if (bc <= 0)		/* too small */
		return NULL;

	nunits = (bc + sizeof(Header) - 1) / sizeof(Header) + 1;

	if (freep == NULL)	/* no list yet */
		init_malloc();

	for (prevp = freep, p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {	/* enough space */
			if (p->s.size == nunits)	/* exactly */
				prevp->s.ptr = p->s.ptr;
			else {
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p + 1);
		}
		if (p == freep)		/* empty list */
			if ((p = morecore(nunits)) == NULL)
				return NULL;
	}
}

#define NALLOC		1024

static Header *morecore(unsigned nu)
{
	void *cp;
	void *sbrk(intptr_t);
	void _free(void *);
	Header *up;

	if (nu < NALLOC)
		nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));
	if (cp == (void *)-1)
		return NULL;
	up = (Header *) cp;
	up->s.size = nu;
	_free((void *)(up + 1));

	return freep;
}

void _free(void *ap)
{
	Header *bp, *p;

	if (freep == NULL)		/* no list yet */
		init_malloc();

	bp = (Header *)ap - 1;		/* find a future place in a free list */
	if (bp->s.size <= 0)		/* too small */
		return;

	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;

	if (bp + bp->s.size == p->s.ptr) {	/* merge with higher neighbour */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else
		bp->s.ptr = p->s.ptr;

	if (p + p->s.size == bp) {		/* merge with lower neighbour */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;
	freep = p;
}

void mmset(void *mm, int c, unsigned bc)
{
	register char *m;

	for (m = mm; bc--; m++)
		*m = c;
}

void *_calloc(unsigned n, unsigned sz)
{
	void *mm;
	unsigned bc = n * sz;

	mm = _malloc(bc);
	if (mm != NULL)
		mmset(mm, 0, bc);
	return mm;
}

void bfree(void *mm, unsigned bc)
{
	Header *p;

	if (bc < 2 * sizeof(*p))	/* too small */
		return;

	/* imitate header */
	p = (Header *)mm;
	p->s.size = bc / sizeof(*p);

	_free(p + 1);	/* free */
}

static int arr[] = { 12, 545, 6, 12, 23, 4, 56, 6, 14, 56 };

int main(void)
{
	int *p;

	bfree(arr, sizeof(arr));
	p = _malloc(sizeof(*p * 3));
	printf("%d %d %d\n", *p, p[1], p[2]);
	_free(p);
	return 0;
}
