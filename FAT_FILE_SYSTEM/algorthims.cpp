#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "algorithms.h"


int hex_char_dex(unsigned char* hex, int length) {
	int dex = 0;
	for (int i = 0;i < length;i++) {
		dex += (int)hex[i] * (int)pow(1 << 8, length - 1 - i);
	}
	return dex;
}

Time query_time() {
	Time return_time;
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep); //�˺�����õ�tm�ṹ���ʱ�䣬���Ѿ����й�ʱ��ת��Ϊ����ʱ��
	return_time.date[0] = p->tm_year;
	return_time.date[1] = p->tm_mon;
	return_time.date[2] = p->tm_mday;
	return_time.precise_time = 3600 * p->tm_hour + 60 * p->tm_min + p->tm_sec;
	return_time.time[0] = p->tm_hour;
	return_time.time[1] = p->tm_min;
	return_time.time[2] = p->tm_sec;
	return return_time;
	//printf("%d%02d%02d%02d%02d%02d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

int split(char *src, const char *separator, char **dest, int *num) {
	/*
	src Դ�ַ������׵�ַ(buf�ĵ�ַ)
	separator ָ���ķָ��ַ�
	dest �������ַ���������
	num �ָ�����ַ����ĸ���
	*/
	char *pNext;
	int count = 0;
	if (src == NULL || strlen(src) == 0) 
		/* �������ĵ�ַΪ�ջ򳤶�Ϊ0��ֱ����ֹ  */
		return -1;
	if (separator == NULL || strlen(separator) == 0) 
		/* ��δָ���ָ���ַ�����ֱ����ֹ */
		return -1;
	pNext = (char *)strtok(src, separator);
	/* ����ʹ��(char *)����ǿ������ת��(��Ȼ��д�еı������в������ָ�����) */
	while (pNext != NULL) {
		*dest++ = pNext;
		++count;
		pNext = (char *)strtok(NULL, separator);  
		/* ����ʹ��(char *)����ǿ������ת�� */
	}
	*num = count;
	return 0;
}

int not_allocate_FAT_index(FILE* fp, char* cluster_index, int sector_per_FAT_int) {
	char cluster_status[2];
	fseek(fp, (long)(SECTOR * 64 + 4), SEEK_SET);
	for (int i = 2;i < sector_per_FAT_int*SECTOR / 2 ;i++) {
		fread(cluster_status, sizeof(cluster_status), 1, fp);
		if (cluster_status[0] == 0 && cluster_status[1] == 0) {
			cluster_index[0] = i >> 8;
			cluster_index[1] = i;
			cluster_status[0] = 0xFF;
			cluster_status[1] = 0xFF;
			/* һ���ҵ�һ����δ��ռ�õĴأ��Ͱ�FFFFд������ر�� */
			fseek(fp, -(long)sizeof(cluster_status), SEEK_CUR);
			fwrite(cluster_status, sizeof(cluster_status), 1, fp);
			/* ��FAT2��������ͬ������Ŀ */
			fseek(fp, (long)SECTOR*sector_per_FAT_int, SEEK_CUR);
			fwrite(cluster_status, sizeof(cluster_status), 1, fp);
			break;
		}
	}
	return 0;
}


/* �÷����������ȳ����ַ�������п�������Ԥ��֪������ */
int string_copy(char* dest, char* src, int length) {
	for (int i = 0;i < length;i++) {
		dest[i] = src[i];
	}
	return 0;
}

/* ����һ���ַ����飬������������ת�����ַ��� */
int time_to_char(char* dest) {
	Time t = query_time();
	int time = t.precise_time;
	int i = 0;
	while (time!=0)
	{
		dest[i] = time % 10 + '0';
		time = time / 10;
		i++;
	}
	dest[i] = '\0';
	return 0;
}

