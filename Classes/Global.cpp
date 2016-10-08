#include "Global.h"

//通过图片目标高度与长度基准比例缩放图片

void setSize(Node * s, float size)
{
	s->setScale(DATUM* size / s->getContentSize().height);
}

//通过存放对应概率的数组获取随机数
int randNum(int size, int probability[])
{
	int sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += probability[i];
	}
	float r = rand_0_1()*sum;

	sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += probability[i];
		if (r < sum)
		{
			return i;
		}
	}
	return -1; //若概率不合法返回-1
}

//通过概率确定是否发生
bool randFlag(int no, int yes)
{
	int sum = no + yes;
	float r = rand_0_1()*sum;
	if (r < no)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//将Sprite添加到父节点中
void addSprite(Sprite* &s, string img, Point pos, float size, Node* n, int zOrder)
{
	s = Sprite::create(img);
	s->setPosition(pos);
	setSize(s, size);
	n->addChild(s, zOrder);
}

//返回10^x
int pow10(int x)
{
	int t = 1;
	for (int i = 0; i < x; i++)
	{
		t *= 10;
	}
	return t;
}

//字符串转整型数
int  stdStoI(string s)
{
	if (s.empty())
	{
		return -1;
	}
	if (s[0] == '-')
	{
		return -stdStoI(string(s.begin() + 1, s.end()));
	}
	int num = 0, temp = s.length() - 1;
	for (string::iterator i = s.begin(); i < s.end(); ++i,--temp)
	{
		if (*i < '0' || *i >'9')
		{
			return -1;
		}
		else
		{
			int x = *i - '0';
			num += x * pow10(temp);
		}
	}
	return num;
}

//整型数字转字符串
string stdItoS(int x)
{
	char text[128];
	sprintf(text, "%d", x);
	string str = text;
	return str;
}

//字符串转浮点数
float stdStoF(string s)
{
	int nc = 0, pc = 0;
	for (string::iterator i = s.begin(); i < s.end(); ++i)
	{
		if (*i >= '0' && *i <= '9')
		{
			nc++;
		}
		else if (*i == '.')
		{
			pc++;
		}
		else
		{
			return -1;
		}
	}
	if (!nc)
	{
		return -1;
	}
	if (pc > 1)
	{
		return -1;
	}
	float temp = 10;
	bool ispnum = true;
	float ans = 0;
	if (s[0] == '_')
	{
		ispnum = false;
		s = string(s.begin() + 1, s.end());
	}
	else if (s[0] == '-')
	{
		s = string(s.begin() + 1, s.end());
	}
	string::iterator i;
	for (i = s.begin(); i < s.end(); ++i)
	{
		if (*i == '.')
			break;
		ans = ans * 10 + (*i - '0');
	}
	for (++i; i < s.end(); ++i)
	{
		ans = ans + (*i - '0') / temp;
		temp *= 10;
	}
	if (ispnum)
		return ans;
	else
		return -ans;
	return ans;
}

//浮点数转换字符串

string stdFtoS(float x)
{
	char text[128];
	sprintf(text, "%.2f", x);
	string str = text;
	return str;
}