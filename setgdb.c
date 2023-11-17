#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* gdb插件配置 */
struct _gdb
{
	char enable[256];	// 启用插件
	char disable[256];	// 禁用插件
	char name[64];		// 插件名
}gdb[] = {
	{
		"source ~/pwn/pwndbg/gdbinit.py\nsource ~/pwn/Pwngdb/pwngdb.py\nsource ~/pwn/Pwngdb/angelheap/gdbinit.py\n",
		"# source ~/pwn/pwndbg/gdbinit.py\n# source ~/pwn/Pwngdb/pwngdb.py\n# source ~/pwn/Pwngdb/angelheap/gdbinit.py\n",
		"pwndbg"
	},
	{
		"source ~/pwn/peda/peda.py\n",
		"# source ~/pwn/peda/peda.py\n",
		"peda",
	},
	{
		"source ~/pwn/gef/gef.py\n",
		"# source ~/pwn/gef/gef.py\n",
		"gef"
	}
};

int select_n = sizeof(gdb) / sizeof(struct _gdb);

/* gdb配置目录 */
char gdbpath[256] = { 0 };

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
		if(i == select)	// 启用插件
			fwrite(gdb[i-1].enable, strlen(gdb[i-1].enable), 1, file);
		else			// 禁用插件
			fwrite(gdb[i-1].disable, strlen(gdb[i-1].disable), 1, file);
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
void selectgdb()
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
		selectgdb();
		return;
	}

	// 设置
	setgdb((int)c);
}


int main(int argc, char **argv)
{
	// 设置.gdbinit目录
	strcpy(gdbpath, getenv("HOME"));
	strcat(gdbpath, "/.gdbinit");
	
	// 判断是几个参数
	// 如果是一个参数, 提示选择
	// 如果是两个参数, 直接设置
	// 如果是三个及其以上, 那么当作只有一个参数
	if(argc == 1)
		selectgdb();
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
			selectgdb();
		}
	}
	else
	{
		puts("未知参数!");
		selectgdb();
	}
	
	return 0;
}
