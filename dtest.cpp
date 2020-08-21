#include <stdio.h>
#include "darray.hpp"

int main() {
	printf("Begin\n");
	darray<int> num;
	num.set_size(10);
	num.initialize(5);
	
	printf("%d", num[0]);
	for (int i = 1; i < num.size; i++) {
		printf(", %d", num[i]);
	}
	printf("\n");
	
	num.set_size(5);
	printf("%d", num[0]);
	for (int i = 1; i < num.size; i++) {
		printf(", %d", num[i]);
	}
	printf("\n");
	
	num.set_size(0);
	
	num.set_cap(20);
	num.set_cap(0);
	num.set_size(5);
	
	num.initialize(12);
	printf("%d", num[0]);
	for (int i = 1; i < num.size; i++) {
		printf(", %d", num[i]);
	}
	printf("\n");
	
	return 0;
}
