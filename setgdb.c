#include <stdio.h>
#include <string.h>
char gdbstr0[]		= { "source ~/pwn/peda/peda.py" };
char gdbstr1[]		= { "source ~/pwn/pwndbg/gdbinit.py" };

/**
 * \brief 设置~/.gdbinit
 * 
 * \param select 0默认 1pwndbg 2peda
 * 
 * \return 0设置失败, 1设置成功
*/
int setgdb(int select)
{
	FILE *file = NULL;

	// 打开文件
	file = fopen( "/home/tuto/.gdbinit", "w");
	if(file == NULL)
		return 0;

	switch(select)
	{
		// 默认gdb
		case 0:
			fprintf(file, "#%s%c#%s%c", gdbstr0, 10, gdbstr1, 10);
			break;
		// pwndbg
		case 1:
			fprintf(file, "#%s%c%s%c", gdbstr0, 10, gdbstr1, 10);
			break;
		// peda
		case 2:
			fprintf(file, "%s%c#%s%c", gdbstr0, 10, gdbstr1, 10);
			break;
	}

	fclose(file);
	return 1;
}

// 选择gdb模式
void select()
{
	unsigned int c = 0;
	puts("请选择要设置的gdb:");
	puts("输入 0 设置为默认gdb");
	puts("输入 1 设置为pwndbg");
	puts("输入 2 设置为peda");
	puts("输入 3 退出");
	scanf("%d", &c);

	// 检测输入
	if(c == 3)
		return;
	else if(c > 2)
	{
		printf("无法识别命令 %d 请重新输入!\n", c);
		select();
		return;
	}

	// 设置
	if(setgdb(c))
		puts("设置成功!");
	else
		puts("设置失败!");
}


int main(int argc, char **argv)
{
	// 判断是几个参数
	// 如果是一个参数, 提示选择
	// 如果是两个参数, 直接设置
	// 如果是三个及其以上, 那么当作只有一个参数
	if(argc == 1)
		select();
	else if(argc == 2)
	{
		if(!strcmp("gdb", argv[1]))
			if(setgdb(0))
				puts("设置成功!");
			else
				puts("设置失败!");

		else if(!strcmp("pwndbg", argv[1]))
			if(setgdb(1))
				puts("设置成功!");
			else
				puts("设置失败!");

		else if(!strcmp("peda", argv[1]))
			if(setgdb(2))
				puts("设置成功!");
			else
				puts("设置失败!");
		else
		{
			puts("未知参数!");
			select();
		}
	}
	else
	{
		puts("未知参数!");
		select();
	}
	
	return 0;
}
