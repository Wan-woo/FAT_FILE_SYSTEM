#include"allocate.h"
#include<stdio.h>
char buffer[BUFFER_SIZE];

int allocate_disk() {
	int size;
	FILE* fp;
	printf("������Ҫ������������̴�С:(��λΪM)");
	scanf("%d", &size);
	fp = fopen("vdisk", "w");
	for (int i = 0;i < size;i++) {
		fwrite(buffer, BUFFER_SIZE, 1, fp);
	}
	fclose(fp);
	return 0;
}