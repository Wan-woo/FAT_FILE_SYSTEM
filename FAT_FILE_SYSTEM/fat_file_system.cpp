#include "fat_file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int show_dir() {
	FILE* fp;
	fp = fopen("vdisk", "rb");
	extern int sector_per_FAT_int;
	extern int sector_per_cluster_int;
	extern int dir_index;
	fseek(fp, (long)((64 + sector_per_FAT_int * 2 + sector_per_cluster_int * dir_index)*SECTOR), SEEK_SET);
	int catalog_num = sector_per_cluster_int * SECTOR / sizeof(catalog);
	for (int i = 0;i < catalog_num;i++) {
		catalog c;
		fread(&c, sizeof(c), 1, fp);
		if (c.is_valid == 1) {
			if (i == 0) {
				printf("�򿪵��ļ���\n%12s\t\t\t%s\t%s\t\n", c.filename, c.user_name, c.attribute);
			}
			else if (strcmp(c.filename_extension, "dir") == 0) {
				printf("%12s\t\t\t%s\t%s\t\n", c.filename, c.user_name, c.attribute);
			}
			else
			{
				printf("%8s.%4s\t\t%4d\t%s\t%s\t\n", c.filename, c.filename_extension, c.file_size, c.user_name, c.attribute);
			}

		}
	}
	fclose(fp);
	return 0;
}

/* ר�������ƶ��ļ�ָ����Ҵ� */
int print_cluster(FILE* fp, int sector_per_FAT_int, int sector_per_cluster_int, int cluster_index_int) {
	char* buffer = (char*)malloc(SECTOR*sector_per_cluster_int);
	long offset = SECTOR * 64 + sector_per_FAT_int * 2 * SECTOR + sector_per_cluster_int * SECTOR*cluster_index_int;
	fseek(fp, offset, SEEK_SET);
	fread(buffer, SECTOR*sector_per_cluster_int, 1, fp);
	printf("%s", buffer);
	return 0;
}

int read_file(char* filename) {
	extern int sector_per_FAT_int;
	extern int sector_per_cluster_int;
	extern int dir_index;

	int catalog_index;
	find_file(filename, catalog_index);
	if (catalog_index == -1) {
		return -1;
	}
	/* ���ض�ȡ�ļ����� */
	FILE* fp;
	fp = fopen("vdisk", "rb");
	catalog c;
	fseek(fp, (long)((64 + sector_per_FAT_int * 2 + sector_per_cluster_int * dir_index)*SECTOR + (catalog_index) * sizeof(c)), SEEK_SET);
	fread(&c, sizeof(c), 1, fp);
	/* �����ļ������أ�һֱ���£�ֱ���ҵ��ļ������� */
	char cluster_index[2];
	int cluster_index_int = (c.start_cluster[0] << 8) + c.start_cluster[1];
	fseek(fp, SECTOR * 64 + cluster_index_int * 2, SEEK_SET);
	fread(cluster_index, sizeof(cluster_index), 1, fp);

	/* û�н�cluster_indexת�޷���������ʱ��-1���� */
	while (cluster_index[0] != -1 || cluster_index[1] != -1) {
		print_cluster(fp, sector_per_FAT_int, sector_per_cluster_int, cluster_index_int);
		cluster_index_int = (cluster_index[0] << 8) + cluster_index[1];
		fseek(fp, SECTOR * 64 + cluster_index_int * 2, SEEK_SET);
		fread(cluster_index, sizeof(cluster_index), 1, fp);
	}
	/* ��ʾ���ļ������һ�� */
	print_cluster(fp, sector_per_FAT_int, sector_per_cluster_int, cluster_index_int);
	fclose(fp);
	return 0;
}

int file_system() {
	FILE* fp;
	fp = fopen("vdisk", "r");
	if (fp == NULL) {
		printf("���ȴ��������ļ�\n");
		return 0;
	}
	fclose(fp);
	file_param_initial();
	int is_valified = 0;
	while (true)
	{
		if (!is_valified) {
			printf("***************FAT�ļ�ϵͳ*******************\n");
			printf("***************�û���½(login)***************\n");
			printf("***************������һ��Ŀ¼(cd ..)**********\n");
			getchar();
			char command[COMMAND_MAX_LENGTH];
			gets_s(command);
			char *args[ARGS_MAX_NUM] = { 0 };
			int num = 0;
			split(command, " ",args, &num);
			if (num > 0) {
				if (strncmp(args[0], "login", 5) == 0) {
					is_valified = valify_user(args);
				}
				else if (strncmp(args[0], "cd", 2) == 0) {
					if (strncmp(args[1], "..", 2) == 0) {
						return 0;
					}
				}
			}
		}
		if (is_valified) {
			extern char active_user[];
			printf("��ǰ���û���%s\n", active_user);
			printf("----------------FAT�ļ�ϵͳ------------------\n");
			/*
			printf("***************1.�û��л�********************\n");
			printf("***************2.�����û�********************\n");
			printf("***************3.ɾ���û�********************\n");
			printf("***************4.�鿴�ļ���Ŀ¼**************\n");
			printf("***************5.�����ļ�********************\n");
			printf("***************6.ɾ���ļ�********************\n");
			printf("***************7.�ƶ��ļ�********************\n");
			printf("***************8.�ļ�����********************\n");
			printf("***************9.������һ��******************\n");
			*/
			char command[COMMAND_MAX_LENGTH];
			gets_s(command);
			char *args[ARGS_MAX_NUM] = { 0 };
			int num = 0;
			split(command, " ",args, &num);
			if (num > 0) {
				if (strncmp(args[0], "runas", 5) == 0) {
					is_valified = valify_user(args);
				}
				else if (strncmp(args[0], "dir", 3) == 0) {
					show_dir();
				}
				else if (strncmp(args[0], "net", 3) == 0 && strncmp(args[1], "user", 3) == 0 && strncmp(args[4], "/add", 3) == 0) {
					create_user(args);
				}
				else if (strncmp(args[0], "type", 4) == 0 && strncmp(args[1], "nul>", 4) == 0) {
					if (num >= 3) {
						if (create_file(args) == -1) {
							printf("ֻ��root�û��ſ��Դ����µ��û�,����ѯroot�û��Դ���");
						}
					}
				}
				else if (strncmp(args[0], "type", 4) == 0) {
					if (read_file(args[1]) == -1) {
						printf("��������ļ�����������������");
					}
					else
					{
						printf("\n");
					}
				}
				else if (strncmp(args[0], "write", 5) == 0) {
					if (num == 3) {
						if (write_file(args) == -1) {
							printf("�������·������������������");
						}
					}
				}
				else if (strncmp(args[0], "cd", 2) == 0) {
					if (strncmp(args[1], "..", 2) == 0) {
						extern int dir_index;
						if (dir_index == 2) {
							return 0;
						}
						return_dir();
						show_dir();
					}
					else
					{
						if (read_dir(args[1]) == -1) {
							printf("û���ҵ����ļ���");
						}
						else if (read_dir(args[1]) == -2) {
							printf("��û��Ȩ�޲������ļ���");
						}
						show_dir();
					}
				}
				else if (strncmp(args[0], "md", 2) == 0) {
					create_dir(args);
				}
				else if (strncmp(args[0], "rename", 6) == 0) {
					if (rename(args[1], args[2]) == -1) {
						printf("û���ҵ��ļ����ļ���");
					}
					else if (rename(args[1], args[2]) == -2) {
						printf("��û��Ȩ�޲������ļ����ļ���");
					}
				}
				else if (strncmp(args[0], "del", 3) == 0) {
					if (delete_file(args[1]) == -1) {
						printf("û���ҵ��ļ�");
					}
					else if (delete_file(args[1]) == -2) {
						printf("û��Ȩ�޲������ļ�");
					}
				}
				else if (strncmp(args[0], "rd", 2) == 0) {
					if (delete_dir(args[1]) == -1) {
						printf("û���ҵ��ļ���");
					}
					else if (delete_dir(args[1]) == -2) {
						printf("û��Ȩ�޲������ļ���");
					}
				}
				else if (strcmp(args[0], "move") == 0) {
					if (num < 3) {
						printf("��������");
					}
					if (move_file(args) == -1) {
						printf("�Ҳ����ļ���·������");
					}
				}
			}
		}
		else
		{
			printf("��½ʧ��");
			return 0;
		}
		printf("--------------------------------------------\n");
	}
	return 0;
}

/* ͬʱչʾFAT���ڲ��ṹ */
int check_disk() {
	FILE* fp;
	fp = fopen("vdisk", "rb");

	unsigned char sector_per_cluster[1];
	fseek(fp, (long)(SECTOR * 63 + 0x0d), SEEK_SET);
	fread(sector_per_cluster, sizeof(sector_per_cluster), 1, fp);
	int sector_per_cluster_int = hex_char_dex(sector_per_cluster, sizeof(sector_per_cluster));

	unsigned char _size[2];
	fread(_size, sizeof(_size), 1, fp);
	int size = (int)(_size[0] << 8) + (int)_size[1];
	int sector_per_FAT_int = (size * BUFFER_SIZE) / (SECTOR*(int)sector_per_cluster[0] * 1 << 8);

	int cluster_taken = 0;

	fseek(fp, (long)(SECTOR * 64 + 4), SEEK_SET);
	char cluster_status[2];
	for (int i = 0;i < sector_per_FAT_int*SECTOR / 2 - 2;i++) {
		fread(cluster_status, sizeof(cluster_status), 1, fp);
		if (cluster_status[0] != 0 || cluster_status[1] != 0) {
			cluster_taken++;
			int cluster_index = (cluster_status[0] << 8) + cluster_status[1];
			printf("��%d�ر�ռ��,����Ϊ%d\n", i + 2, cluster_index);
		}
	}

	int sector_left = size * BUFFER_SIZE / SECTOR - 64 - cluster_taken * 2 * sector_per_cluster_int;

	printf("Ŀǰ����%d�ر�ռ��\n", cluster_taken);
	printf("ʣ���������Ϊ%d\n", sector_left);
	printf("ʣ��ռ�Ϊ%d(��λM)\n", sector_left*SECTOR / BUFFER_SIZE);
	fclose(fp);
	return 0;
}