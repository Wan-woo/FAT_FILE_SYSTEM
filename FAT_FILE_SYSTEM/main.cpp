#include<stdio.h>
#include "allocate.h"

/*	* main�ļ����ڲ��Ը���ģ��Ĺ���
	* �ڿ���̨����ѡ����Ҫ�Ĺ���
	* ���ǵ����̴�С������FAT16�ļ�ϵͳ
	*/

int main() {
	while (true)
	{
		int input;
		printf("***************FAT�ļ�ϵͳ*******************\n");
		printf("***************1.��������********************\n");
		printf("***************2.��ȡ�����ļ�****************\n");
		printf("***************0.Exit***********************\n");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			allocate_disk();
			break;
		case 0:
			return 0;
		default:
			break;
		}
	}
	
	return 0;
}