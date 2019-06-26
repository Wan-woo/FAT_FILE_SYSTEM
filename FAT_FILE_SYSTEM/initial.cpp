#include"initial.h"
#include<stdio.h>
#include<math.h>
#include<string.h>
char buffer[BUFFER_SIZE] = {0};

int allocate_disk();


int f_sector_per_cluster(int size) {
	if (size <= 0 || size > 4096) {
		return -1;
	}
	else if(size<33)
	{
		return 4;
	}
	else if(size<65)
	{
		return 4;
	}
	else if (size < 129)
	{
		return 4;
	}
	else if(size<257)
	{
		return 8;
	}
	else if(size<513)
	{
		return 16;
	}
	else if (size < 1025)
	{
		return 32;
	}
	else if(size<2049)
	{
		return 64;
	}
	else
	{
		return 128;
	}
}

int allocate_disk() {
	int size;
	FILE* fp;
	printf("������Ҫ������������̴�С:(��λΪM)");
	scanf("%d", &size);
	fp = fopen("vdisk", "wb");
	for (int i = 0;i < size;i++) {
		fwrite(buffer, BUFFER_SIZE, 1, fp);
	}
	fclose(fp);
	/* ����MBR���� */
	fp = fopen("vdisk", "rb+");
	fseek(fp, (long)SECTOR - 2, SEEK_SET);
	unsigned char end[] = { 0x55, 0xaa };
	fwrite(end, sizeof(end), 1, fp);
	

	/*	* ����DBR���� 
		* ����BPB����
		* ����FAT��ĳ��ȣ�Ҳ���Ǽ���FAT��ռ�õ���������Ȼ����д��BPB����
	*/
	unsigned char bytes_per_sector[] = {0x02, 0x00};
	fseek(fp, (long)(SECTOR*62+0x0b), SEEK_CUR);
	fwrite(bytes_per_sector, sizeof(bytes_per_sector), 1, fp);

	unsigned char sector_per_cluster[] = { (char)f_sector_per_cluster(size) };
	fwrite(sector_per_cluster, sizeof(sector_per_cluster), 1, fp);
	
	unsigned char _size[2];
	_size[0] = (char)size >> 8;
	_size[1] = (char)size;
	fwrite(_size, sizeof(_size), 1, fp);

	unsigned char sector_per_FAT[2];
	int sector_per_FAT_int = (size*BUFFER_SIZE) / (SECTOR*(int)sector_per_cluster[0] * 1 << 8);
	sector_per_FAT[0] = (char)sector_per_FAT_int>>8;
	sector_per_FAT[1] = (char)sector_per_FAT_int;
	fseek(fp, (long)(0x16 - 0x0d), SEEK_CUR);
	fwrite(sector_per_FAT, sizeof(sector_per_FAT), 1, fp);

	/* �����Ƿ���Ҫ����MBR������DBR�����������в���ȷ������Ҫ������Ԥ��ǰ64������ */

	/* * ����FAT�� 
	   * FAT��ǰ����(�ĸ��ֽ�)���б���
	   * һ����ռ�������ֽ�
	*/
	char not_allocate_cluster[] = { 0, 0 };
	char first_cluster[] = {0xFF, 0xF8, 0xFF, 0XFF};
	fseek(fp, (long)(SECTOR * 64), SEEK_SET);
	fwrite(first_cluster, sizeof(first_cluster), 1, fp);
	for (int i = 0;i < sector_per_FAT_int*SECTOR / 2 - 2;i++) {
		fwrite(not_allocate_cluster, sizeof(not_allocate_cluster), 1, fp);
	}
	fwrite(first_cluster, sizeof(first_cluster), 1, fp);
	for (int i = 0;i < sector_per_FAT_int*SECTOR / 2 - 2;i++) {
		fwrite(not_allocate_cluster, sizeof(not_allocate_cluster), 1, fp);
	}

	/*	* ������������Ŀ¼�� 
		* ���������״����ڴ洢�û���Ϣ
		* �״ص�ǰ64���ֽ����ڴ洢ÿ���û����ڵ�λ��
	*/
	/* 
	user_file f;
	f.num = 1;
	f.address[1] = 1;
	fseek(fp, (long)((63 + sector_per_FAT_int * 2)*SECTOR), SEEK_SET);
	fwrite(&f, sizeof(f), 1, fp);
	*/
	user root;
	root.is_system = 1;
	root.is_valid = 1;
	strcpy(root.user_name, "root");
	strcpy(root.password, "admin");
	fseek(fp, (long)((64 + sector_per_FAT_int * 2)*SECTOR), SEEK_SET);
	fwrite(&root, sizeof(root), 1, fp);

	/* �������ĵڶ������ڴ������ļ��� */
	catalog root_c;
	strcpy(root_c.filename, "root");
	strcpy(root_c.filename_extension, "dir");
	strcpy(root_c.user_name, "root");
	root_c.attribute[0] = '7';
	root_c.attribute[1] = '7';
	root_c.attribute[2] = '7';
	root_c.is_valid = 1;
	root_c.file_size = 0;
	char cluster_index[2];
	not_allocate_FAT_index(fp, cluster_index, sector_per_FAT_int);
	string_copy(root_c.start_cluster, cluster_index, sizeof(cluster_index));
	string_copy(root_c.former_cluster, cluster_index, sizeof(cluster_index));
	fseek(fp, (long)((64 + sector_per_FAT_int * 2 + (int)sector_per_cluster[0] * 2)*SECTOR), SEEK_SET);
	fwrite(&root_c, sizeof(root_c), 1, fp);
	fclose(fp);
	return 0;
}

int read_disk() {
	FILE* fp;
	fp = fopen("vdisk", "rb");
	if (fp == NULL) {
		printf("���ȴ��������ļ�\n");
		return 0;
	}
	unsigned char bytes_per_sector[2];
	fseek(fp, (long)(SECTOR * 63 + 0x0b), SEEK_SET);
	fread(bytes_per_sector, sizeof(bytes_per_sector), 1, fp);
	printf("�����Ĵ�С��\t%d\n", hex_char_dex(bytes_per_sector, sizeof(bytes_per_sector)));
	unsigned char sector_per_cluster[1];
	fread(sector_per_cluster, sizeof(sector_per_cluster), 1, fp);
	printf("ÿһ�صĴ�С��\t%d����\n", hex_char_dex(sector_per_cluster, sizeof(sector_per_cluster)));
	fclose(fp);
	return 0;
}

int initial() {
	int input;
	while (true)
	{
		int input;
		printf("***************��ʼ��ϵͳ********************\n");
		printf("***************1.���������ļ�****************\n");
		printf("***************2.�鿴�����ļ���Ϣ************\n");
		printf("***************3.������һ��Ŀ¼**************\n");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			allocate_disk();
			break;
		case 2:
			read_disk();
			break;
		case 3:
			return 0;
		default:
			break;
		}
	}
	return 0;
}