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
	/* ����MBR���� */
	fp = fopen("vdisk", "r+");
	fseek(fp, (long)SECTOR - 4, SEEK_CUR);
	char end[] = { '5', '5', 'a', 'a' };
	fwrite(end, sizeof(end), 1, fp);
	

	/*	*����DBR���� */
	fclose(fp);

	/* �����Ƿ���Ҫ����MBR������DBR�����������в���ȷ������Ҫ������Ԥ��ǰ64������ */

	return 0;
}