#pragma once
#include "algorithms.h"


/* һ��userfileռ��64���ֽ� */
struct user_file
{
	char num;
	char address[32] = {0};
	char unuse[31];
};


/* һ��userռ��64�ֽ� */

struct user
{
	char user_name[10];
	char password[16];
	char is_valid;
	char is_system;
	char establish_date[3];
	char establish_time[3];
	char unuse[30];
};

/* �����Ҫ����ؽ���ļ��Ĺ�������Ҫʹ�ö���Ŀ¼�����й��� */

int valify_user(char** args);
int create_user(char** args);
int delete_user();