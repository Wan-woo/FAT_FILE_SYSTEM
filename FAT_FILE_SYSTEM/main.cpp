#include<stdio.h>
#include "initial.h"
#include<iostream>
using namespace std;

/*	* main�ļ����ڲ��Ը���ģ��Ĺ���
	* �ڿ���̨����ѡ����Ҫ�Ĺ���
	* ���ǵ����̴�С������FAT16�ļ�ϵͳ
	*/

int main() {
	while (true)
	{
		int input;
		printf("***************FAT�ļ�ϵͳ*******************\n");
		printf("***************1.��ʼ��ϵͳ******************\n");
		printf("***************2.����FAT�ļ�ϵͳ*************\n");
		printf("***************3.����***********************\n");
		printf("***************0.Exit***********************\n");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			initial();
			break;
		case 2:
			break;
		case 3:
			break;
		case 0:
			return 0;
		default:
			break;
		}
	}
	
	return 0;
}