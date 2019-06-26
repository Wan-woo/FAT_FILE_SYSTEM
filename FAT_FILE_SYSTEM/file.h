#pragma once
#include "algorithms.h"

/*
char filename[8];				�ļ���
char filename_extension[3];		�ļ���չ��
char attribute[3];				����
char establish_time_precise;	��ȷ����ʱ��
char establish_time[2];			����ʱ�� (Сʱ�����ӡ���)
char establish_date[2];			�������� (�ꡢ�¡���)
char latest_access_date[2];		����޸�����
char unuse2[2];					����
char modify_time[2];			�޸�ʱ��
char modify_date[2];			�޸�����
char start_cluster[2];			��ʼ�غ�
char file_size[4];				�ļ���С
char user_name[10];				�����û�
char former_cluster[2];			��һ���غ�(���ڷ�����һ��Ŀ¼)
char unuse3[8];				����
*/

/*  struct�к�int�Զ�����4�ֽڱ��� */
struct catalog
{
	char filename[8];
	char filename_extension[4];
	char attribute[3];
	char establish_time[3];
	char establish_date[3];
	char latest_access_date[3];
	char is_valid;
	char modify_time[3];
	char modify_date[3];
	char start_cluster[2];
	char user_name[10];
	char former_cluster[2];
	char unuse2[11];
	/* �Ϸ��չ�60�ֽ� Ϊ4�ı��� */
	int file_size;
	int establish_time_precise;
};


int file_param_initial();

int create_dir(char** filename);
int create_file(char** filename);

//int show_dir();
int show_file();

int modify_file();

int delete_dir(char* filename);
int delete_file(char* filename);

int move_dir();

int move_file(char** args);

int read_dir(char* filename);

int return_dir();

int write_file(char** args);

int find_file(char* filename, int& catalog_index);

int test_file(char** filename);

int rename(char* filename, char* newname);

