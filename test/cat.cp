#include "cat.h"
#include <stdio.h>

@namespace test.animals

@import test.animals.Cat as Cat

static void @ns Cat_meow(Cat *self) {
	printf("meow: %i\n", self->msg);
}

Cat @ns Cat_init(int msg) {
	return (Cat){
		.msg = msg,

		.meow = &Cat_meow
	};
}

void @ns Cat_free(Cat *cat) {
	cat->valid = false;
	cat->msg = 0;
}
