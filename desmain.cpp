/*DES算法由加密、解密和子密钥的生成三部分组成。*/
#include<iostream>
#include<string>
#include "des.h"
using namespace std;


//所有预配置的矩阵，包括PC1,PC2,P,E，存储的值都是索引值！！！S盒存的不是索引值


int main()
{
	/*******************明文及密钥预处理******************/
	char mingwen[9] = { "Chinaman" };//明文8字节 mingwen[9]='/0';
	char key[9] = { "cryptogr" };//密钥8字节

	int minint[64];//明文
	int keyint[64];//密钥
	int miwen[64];//密文

	//明文密钥转换为二进制
	int i = 0, j = 0, m = 0, n = 0;
	for (i = 0; i < 64; i++)//用于存储明文化为二进制
	{
		minint[i] = 0;
	}
	for (i = 0; i < 64; i++)//用于存储密钥二进制
		keyint[i] = 0;

	//将字符表示转化为二进制
	ToTwo(mingwen, minint);
	ToTwo(key, keyint);

	

	/******************密钥的置换以及分组*************/

	int pc1key[56]; //用于存放经pc1置换后的密钥
	for (i = 0; i < 56; i++)
	{   
		//进行pc－1置换
		pc1key[i] = keyint[pc1[i]];
		// cout<<pc1key[i]<<" ";
	}

	int leftkey[28];
	int rightkey[28];

	for (j = 0; j < 28; j++)//将密钥分为两部分leftkey，rightkey
	{
		leftkey[j] = pc1key[j];
		rightkey[j] = pc1key[j + 28];
	}

	int lun = 1; //记录所进行的轮数
	//轮密钥左移位数
	int movleft[17] = { 0,1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };







	/***********明文的初始置换以及分组************/

	int ww[64];//辅助数组ww[64]，将明文做初始ip置换
	for (i = 0; i < 64; i++)
	{
		ww[i] = minint[i];
	}
	for (i = 0; i < 64; i++)
		minint[i] = ww[beginip[i]];

	int leftmin[32];//将明文分为两部分leftmin，rightmin
	int rightmin[32];

	for (j = 0; j < 32; j++)
	{
		leftmin[j] = minint[j];
		rightmin[j] = minint[j + 32];
	}




	/*********************16次轮变换*****************/

	int keykey[16][48];
	int s = 0;
	for (s = 0; s < 16; s++)
	{
	/*******************生成子密钥**********************/
		int pc2key[48];
		int temp1[28], temp2[28];//temp1,temp2分别作为辅助数组控制密钥左半部分，右半部分左移  
		for (j = 0; j < 28; j++)
		{
			temp1[j] = leftkey[j];
			temp2[j] = rightkey[j];
		}
		for ( j = 0; j < 28; j++)
		{
			leftkey[j] = temp1[(j + lun + 28) % 28];
			rightkey[j] = temp2[(j + lun + 28) % 28];
		}

		//将左右部分密钥合在一起
		int pc2keytemp[56];
		for (i = 0; i < 28; i++)
		{
			pc2keytemp[i] = leftkey[i];
			pc2keytemp[i + 28] = rightkey[i];
		}
		//进行pc2置换，得到48位子密钥
		cout << "第"<<s+1<<"轮的轮密钥为：" << endl;
		for (i = 0; i < 48; i++)
		{
			pc2key[i] = pc2keytemp[pc2[i]];
			keykey[s][i] = pc2keytemp[pc2[i]];//放入子密钥数组中
			cout << keykey[s][i];
		}
		cout << endl;

		/****************轮变换过程***************/

		int lefttemp[32];//保存左右部分明文
		int righttemp[32];
		for (i = 0; i < 32; i++)
		{
			lefttemp[i] = leftmin[i];
			righttemp[i] = rightmin[i];
		}
		//进行E表扩展置换
		int kk[48];
		for (i = 0; i < 48; i++)
			kk[i] = rightmin[zhihuanE[i]];
		//扩展后的48比特与子密钥异或
		for (i = 0; i < 48; i++)
			kk[i] = pc2key[i] ^ kk[i];
		//得到的比特流分成8个6比特，
		int cc[8][6];
		n = 0;
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 6; j++)
			{
				cc[i][j] = kk[n++];
			}
		}
		int tt[32];
		int hang = 0, lie = 0;
		//经过s盒变换 6位输入，四位输出 
		for (i = 0; i < 8; i++)
		{
			dd(cc[i], hang, lie);
			int xx = ss[i][hang * 16 + lie];
			int z = 3;
			for (z = 3; z >= 0; z--)
			{
				tt[i * 4 + z] = xx % 2;
				xx = xx / 2;
			}
		}
		//开始置换P
		int dd[32];
		for (i = 0; i < 32; i++)
		{
			dd[i] = tt[zhihuanP[i]];
		}


		//leftmin的32位与置换P得到的32位异或
		for (i = 0; i < 32; i++)
			dd[i] = dd[i] ^ leftmin[i];
		cout << "第" << s + 1 << "轮的左右部分分别为：" << endl;
		//异或后得到的32位作为下一轮的右半部分，上一轮的右半部分作为下一轮的左半部分
		for (i = 0; i < 32; i++)
		{
			leftmin[i] = righttemp[i];
			rightmin[i] = dd[i];
			cout << leftmin[i];
		}
		cout << "   ";
		for (i = 0; i < 32; i++)
		{
			cout << rightmin[i];
		}
		cout << endl;


		lun++;
	}

	//经过16次轮变换后将左右两部分交换，因为最后一轮本不应该进行左右交换的
	int o;
	for (i = 0; i < 32; i++)
	{
		o = leftmin[i];
		leftmin[i] = rightmin[i];
		rightmin[i] = o;
	}
	//合并左右部分
	for (i = 0; i < 32; i++)
	{
		minint[i] = leftmin[i];
		minint[i + 32] = rightmin[i];
	}
	//经过逆初始置换得到密文
	for (i = 0; i < 64; i++)
	{
		miwen[i] = minint[zhihuanpp[i]];
	}
	for (i = 0; i < 64; i++)
	{
		cout << miwen[i];
	}
	cout << endl;
	//将二进制转化成char型输出
	char michar[8];
	for (i = 0; i < 8; i++)
	{
		long int qq = 0;
		for (j = 0; j < 8; j++)
		{
			qq = (miwen[j + 8 * i] + qq) * 2;
		}
		qq = qq / 2;
		michar[i] = (char)(qq);
	}
	cout << "加密得到的密文为:  ";
	for (i = 0; i < 8; i++)
		cout << michar[i];
	cout << endl;





	/*************解密过程**********************/
	//解密过程与加密过程算法相同，只是子密钥使用的顺序相反

	lun = 1;
	s = 0;
	for (i = 0; i < 8; i++)
		mingwen[i] = michar[i];

	/*********************16次轮变换*****************/
	for (s = 0; s < 16; s++)
	{
		//对明文进行处理
		int lefttemp[32];//保存左右部分明文
		int righttemp[32];
		for (i = 0; i < 32; i++)
		{
			lefttemp[i] = leftmin[i];
			righttemp[i] = rightmin[i];
		}
		//进行E表扩展置换
		int kk[48];
		for (i = 0; i < 48; i++)
			kk[i] = rightmin[zhihuanE[i]];
		//扩展后的48比特与子密钥异或
		for (i = 0; i < 48; i++)
			kk[i] = keykey[16 - lun][i] ^ kk[i];///****///注意此处密钥顺序跟加密相反///****///
		   //得到的比特流分成8个6比特
		int cc[8][6];
		n = 0;
		for (i = 0; i < 8; i++)//将kk分为8个6位数
		{
			for (j = 0; j < 6; j++)
			{
				cc[i][j] = kk[n++];
			}
		}
		int tt[32];
		int coutt = 0;
		int hang = 0, lie = 0;
		for (i = 0; i < 8; i++)
		{
			dd(cc[i], hang, lie);
			int xx = ss[i][hang * 16 + lie];
			int z = 3;
			for (z = 3; z >= 0; z--)
			{
				tt[i * 4 + z] = xx % 2;
				xx = xx / 2;
			}
		}

		//开始置换P
		int dd[32];
		for (i = 0; i < 32; i++)
		{
			dd[i] = tt[zhihuanP[i]];
		}


		//leftmin的32位与置换P得到的32位异或
		for (i = 0; i < 32; i++)
			dd[i] = dd[i] ^ leftmin[i];
		//异或后得到的32位作为下一轮的右半部分，上一轮的右半部分作为下一轮的左半部分
		for (i = 0; i < 32; i++)
		{
			leftmin[i] = righttemp[i];
			rightmin[i] = dd[i];
		}
		lun++;
	}
	//经过16次轮变换后将左右两部分交换
	for (i = 0; i < 32; i++)
	{
		o = leftmin[i];
		leftmin[i] = rightmin[i];
		rightmin[i] = o;
	}
	//经过逆初始置换得到密文
	for (i = 0; i < 32; i++)
	{
		minint[i] = leftmin[i];
		minint[i + 32] = rightmin[i];
	}
	for (i = 0; i < 64; i++)
	{
		miwen[i] = minint[zhihuanpp[i]];
	}
	//将二进制转化成char型输出
	char jiemichar[8];
	for (i = 0; i < 8; i++)
	{
		long int qq = 0;
		for (j = 0; j < 8; j++)
		{
			qq = (miwen[j + 8 * i] + qq) * 2;
		}
		qq = qq / 2;
		jiemichar[i] = (char)(qq);
	}
	cout << "解密得到的明文为:  ";
	for (i = 0; i < 8; i++)
		cout << jiemichar[i];
	cout << endl;
	
	return 0;
}