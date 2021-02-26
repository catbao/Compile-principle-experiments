#include<bits/stdc++.h>
using namespace std;
//创建三个结构体分别表示终结符、非终结符、产生式
//终结符
typedef struct FINAL{
    char final[10]="";
    int num;
} FINAL;

//非终结符
typedef struct UNFINAL{
    char unfinal[10]="";
    int num;
} UNFINAL;

//产生式
typedef struct EXP{
    int num;
    char expression[20]="";
} EXP;

char start; //开始符号
char dot = '.';
map<char,vector<string>> rule;
//map<char,vector<string>> rule2;
map<char,set<char>> first;
map<char,set<char>> follow;
set<char> Vt;
set<char> Vn;
//unordered_set<char> V;
vector<char> V;
vector<char> Vnn;
vector<char> Vtt;
string input_string;
int at[20][20]; //action表
string act[20][20]; //action表输出形式
int gt[20][20]; //goto表
//bool FlagofAction = false;

struct rule2{  //用于closure的构造
    char left;
    string right;
    char future;
}rul[100];

struct rule3{  //表达式，用于最后的分析过程
	char left;
	string right;
}ru[100];
int NumofRu = 0;

vector<rule2> minii_closure;  //用于action表的构造
vector<vector<rule2>> closure(100); //存储自动机closure
int closure_num = 0;  //共有多少个closure
int closure_num_move = 0; //现在进行到了哪个closure
int f=0;
int ff=0;