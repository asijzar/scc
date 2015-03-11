#include <stdio.h>

void bubble_sort(int* a, int n) {
	int i, j, k;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++) 
				if (a[k] > a[j]) {
					int temp = a[j];
					a[j] = a[k];
					a[k] = temp;
				}
}

int main() {
	int n, i;
	printf("Введите размер массива");
	scanf("%d", &n);
	int* a = (int*)malloc(sizeof(int) * n);
	
	printf("Введите элементы массива");
	for (i = 0; i < n; i++)
		scanf("%d", a + i);
		
	bubble_sort(a, n);
	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
	
	return 0;
}