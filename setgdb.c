#include <stdio.h>
#include <string.h>

/* gdb插件配置 */
struct
{
	char path[256];	// 插件目录
	char name[64];	// 插件名
}gdb[] = {
	"~/pwn/pwndbg/gdbinit.py",	"pwndbg",
	"~/pwn/peda/peda.py",		"peda",
	"~/pwn/gef/gef.py",			"gef"
};

int select_n = sizeof(gdb) / (256 + 64);

/* gdb配置目录 */
const char gdbpath[] = { "/home/tuto/.gdbinit" };

/**
 * \brief 设置~/.gdbinit
 * 
 * \param select 0为默认, 然后依次按照顺序选择
 * 
 * \return 0设置失败, 1设置成功
*/
int _setgdb(int select)
{
	int i = 0;
	FILE *file = NULL;

	// 打开文件
	if( !( file = fopen( gdbpath, "w") ) )
		return 0;
	
	// 配置gdb
	for(int i = 1; i <= select_n; i++)
	{
		if(i == select)
			fprintf(file, "source %s\n", gdb[i-1].path);
		else
			fprintf(file, "#source %s\n", gdb[i-1].path);
	}

	fclose(file);
	return 1;
}

// 运行setgdb
void setgdb(int c)
{
	if(_setgdb(c))
		puts("设置成功!");
	else
		printf("设置失败 : 无法打开文件 : %s\n", gdbpath);
}

// 选择gdb模式
void select()
{
	unsigned int c = 0, i = 0;
	puts("请选择要设置的gdb:");
	puts("输入 0 设置为默认gdb");
	for(i = 1; i <= select_n; i++)
		printf("输入 %d 设置为%s\n", i, gdb[i-1].name);
	printf("输入 %d 退出\n请输入 : ", i);
	scanf("%d", &c);

	// 检测输入
	if(c == i)
		return;
	else if(c > i)
	{
		printf("无法识别命令 %d 请重新输入!\n", c);
		select();
		return;
	}

	// 设置
	setgdb((int)c);
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
		int i = 0;
		if(!strcmp("gdb", argv[1]))
			setgdb(0);
		else
			for(; i < select_n; i++)
				if(!strcmp(gdb[i].name, argv[1]))
				{
					setgdb(i + 1);
					break;
				}

		if(i == select_n)
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
