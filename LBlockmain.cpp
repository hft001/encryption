/*
	˵����LBlock�����������㷨�����СΪ64λ������Կ����Ϊ80λ
*/
#include <iostream>
#include <string>
#include <vector>
using namespace std;
//S��
int s[10][16] = {
	{14,9,15,0,13,4,10,11,1,2,8,3,7,6,12,5},
	{4,11,14,9,15,13,0,10,7,12,5,6,2,8,1,3},
	{1,14,7,12,15,13,0,6,11,5,9,3,2,4,8,10},
	{7,6,8,11,0,15,3,14,9,10,12,13,5,2,4,1},
	{14,5,15,0,7,2,12,13,1,8,4,9,11,10,6,3},
	{2,13,11,12,15,14,0,9,7,10,6,3,1,8,4,5},
	{11,9,4,14,0,15,10,13,6,12,5,7,3,8,1,2},
	{13,10,15,0,14,4,9,11,2,1,8,3,7,5,12,6},
	{14,9,15,0,13,4,10,11,1,2,8,3,7,6,12,5},
	{4,11,14,9,15,13,0,10,7,12,5,6,2,8,1,3}
};

//��Կ����
void createKey(vector<int> &Key,vector<vector<int> > &subkey)
{
	int lun = 1;
	vector<int> temp1,temp2;
	vector<int> tmp;
	for (int i = 0; i < 32; i++)
	{
		temp1.push_back(Key[i]);
	}
	for (int i = 0; i < 80; i++)
	{
		temp2.push_back(Key[i]);
	}
	subkey.push_back(temp1);
	temp1.clear();
	while (lun<32)
	{
		//ѭ������29λ
		for (int i = 29; i < 80; i++)
		{
			temp1.push_back(temp2[i]);
		}
		for (int i = 0; i < 29; i++)
		{
			temp1.push_back(temp2[i]);
		}
		//s8,s9��ӳ��
		int a = temp1[0] * 8 + temp1[1] * 4 + temp1[2] * 2 + temp1[3];
		int b = s[9][a];
		int c = temp1[4] * 8 + temp1[5] * 4 + temp1[6] * 2 + temp1[7];
		int d = s[8][c];
		for (int i = 0; i < 4; i++)
		{
			temp1[3-i] = b % 2;
			b /= 2;
			temp1[7-i] = d % 2;
			d /= 2;
		}

		//���ֳ��������
		int e = temp1[29] * 16 + temp1[30] * 8 + temp1[31] * 4 + temp1[32] * 2 + temp1[33];
		int f = e ^ lun;
		for (int i = 0; i < 5; i++)
		{
			temp1[33-i] = f % 2;
			f /= 2;
		}
		//ȡ���32λΪ����Կ
		for (int i = 0; i < 32; i++)
		{
			tmp.push_back(temp1[i]);
		}

		subkey.push_back(tmp);
		temp2.clear();
		for (int i = 0; i < 80; i++)
		{
			temp2.push_back(temp1[i]);
		}
		temp1.clear();
		tmp.clear();
		lun++;
	}
}
//������ת��Ϊ������
void Change(string &input, vector<vector<int>> &output)
{
	int n = 0;
	vector<int> tmp,tmp1;
	vector<int> temp;

	//�������ַ�ת��Ϊ����
	for (int i = 0; i < input.size(); i++)
	{
		int m = (int)input[i];
		tmp.push_back(m);
	}
	//���ָ���Ϊ8�����������ܱ�֤ÿһ������붼��64bits������ĵط���0
	while (tmp.size() % 8 != 0)
	{
		tmp.push_back(0);
	}
	//��tmp�е�����ת��Ϊ�����ƴ洢��tmp1�У�tmp2��tmp1�е�ÿһ�����������ֽ����˷�ת
	for (int i = 0; i < tmp.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp1.push_back(tmp[i] % 2);
			tmp[i] /= 2;
		}
		//���λ�����ұ�
		for (int k = 7; k >=0; k--)
		{
			temp.push_back(tmp1[k]);
		}
		if ((i + 1) % 8 == 0 && i != 0)
		{
			output.push_back(temp);
			temp.clear();
		}
		tmp1.clear();
		
	}
}

//����Կת��Ϊ������
void ChangeKey(string &input, vector<int> &output)
{
	int n = 0;
	vector<int> tmp, tmp1;

	//�������ַ�ת��Ϊ����
	for (int i = 0; i < input.size(); i++)
	{
		int m = (int)input[i];
		tmp.push_back(m);
	}

	//��tmp�е�����ת��Ϊ�����ƴ洢��tmp1�У�tmp2��tmp1�е�ÿһ�����������ֽ����˷�ת
	for (int i = 0; i < tmp.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp1.push_back(tmp[i] % 2);
			tmp[i] /= 2;
		}
		//���λ�����ұ�
		for (int k = 7; k >= 0; k--)
		{
			output.push_back(tmp1[k]);
		}
		tmp1.clear();
	}
}

int main()
{
	string in;//��������
	cout << "��������" << endl;
	getline(cin,in);
	string mKey = "azsdfkjiyq";//�̶���Կ
	vector<vector<int>> res;
	vector<int> Key;
	vector<vector<int> > subkey;
	ChangeKey(mKey, Key);//�����Կ������
	Change(in, res);//������Ķ�����

	//����Կ����
	createKey(Key,subkey);
	for (int i = 0; i < 32; i++)
	{
		cout << "��" << i + 1 << "�ֵ�����ԿΪ��";
		for (int j = 0; j < 32; j++)
			cout << subkey[i][j];
		cout << endl;
	}

	//���Ķ��������
	cout << "����" << endl;
	for (int i = 0; i < res.size(); i++)
	{
		for (int j = 0; j < 64; j++)
		{
				cout << res[i][j];
		}
		cout << endl;
	}
	//��Կ���������
	cout << "��Կ��"<<endl;
	for (int i = 0; i < Key.size(); i++)
	{
		cout << Key[i];
	}
	cout << endl;
/*
	//32�ּ���
	int lun = 32;
	int duan = 0;
	
	vector<int> left, right;
	//ÿһ�ηֱ���м���
	while (duan<res.size())
	{
		//��һ�ֵ��������߷ֱ������ĵ����Ҳ���
		for (int i = 0; i < 32; i++)
		{
			left.push_back(res[duan][i]);
			right.push_back(res[duan][i + 32]);
		}
		//��߲��־����ֺ���F


		while (lun)
		{

		}
	}
	*/
	return 0;
}