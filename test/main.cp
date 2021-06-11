#include "cat.hp"

@import test.animals.Cat as Cat

int main(int argc, char *argv[]){
	(void)argc; (void)argv;

	Cat cat = @new Cat(69);

	cat.meow(&cat);

	@free cat;

	return 0;
}
