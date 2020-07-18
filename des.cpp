#include "des.h"

void dd(int aa[6], int &hang, int &lie)
{
	hang = aa[0] * 2 + aa[5];
	lie = aa[1] * 8 + aa[2] * 4 + aa[3] * 2 + aa[4];
}

void ToTwo(char zifu[], int erJinZhi[])
{
	for (int i = 0; i < 8; i++)//明文化为二进制
	{
		int temp = (int)zifu[i];
		int z = 0;
		for (z = 7; z >= 0; z--)
		{
			erJinZhi[8 * i + z] = temp % 2;//取最低位的0或1放在最右侧
			temp = temp / 2;
		}
	}
}





