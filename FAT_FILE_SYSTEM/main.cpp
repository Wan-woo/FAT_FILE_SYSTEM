#include <stdio.h>
#include "initial.h"
#include "version.h"
#include "fat_file_system.h"
#include "algorithms.h"
#include "file.h"
#include <iostream>
using namespace std;

/*	* main�ļ����ڲ��Ը���ģ��Ĺ���
	* �ڿ���̨����ѡ����Ҫ�Ĺ���
	* ���ǵ����̴�С������FAT16�ļ�ϵͳ
	*/

/*	һ��Ҫע��ĵ㣺
	��ͨ���ַ������ƿ�����strcpy()����������ַ����к�0�����Զ�ֹͣ����ʹ��string_copy()
	0xFFΪ-1*/
int main() {
	FILE* fp = fopen("vdisk", "rb");
	if (fp == NULL) {
		printf("���ȳ��Ի�����");
		initial();
	}
	while (true)
	{
		int input;
		printf("***************FAT�ļ�ϵͳ*******************\n");
		printf("***************1.��ʼ��ϵͳ******************\n");
		printf("***************2.����FAT�ļ�ϵͳ*************\n");
		printf("***************3.�鿴����ʹ�����************\n");
		printf("***************4.����***********************\n");
		printf("***************0.Exit***********************\n");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			initial();
			break;
		case 2:
			file_system();
			break;
		case 3:
			check_disk();
			break;
		case 4:
			print_version_details();
			break;
		case 0:
			return 0;
		default:
			break;
		}
	}
	
	return 0;
}