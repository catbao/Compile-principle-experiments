#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char prog[80],token[8];
char ch;
int syn,p,row,sum=0,m;
char word[27][20] = {"main","if","then","else","while","do","repeat","until","for","from",
"to","step","switch","of","case","default","return","int","real","char","bool",
"and","or","not","mod","read","write"};//27

//判断是不是字母 
int isChar(char ch)
{
	if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
	return 1;
	else return 0;
}
//判断是不是数字 
int isNum(char ch)
{
	if(ch>='0' && ch<='9')
	return 1;
	else
	return 0;
}

void scanner(){
	for(int i=0;i<8;++i){
		token[i] = '\0';
	}
	ch = prog[p++];
	while(ch == ' '){ //略去开头的空格
		ch = prog[p];
		p++;
	}
	if(isChar(ch)){ //可能是标识符或者变量名，不能以数字开头
		m = 0;
		while(isNum(ch) || isChar(ch)){ //首字符之后的可以是字符或者数字了
			token[m++] = ch;
			ch = prog[p++];
		}
		token[m++] = '\0'; 
		p--;
		syn = 100; //除了上述关键字外的其他关键字或者标识符都用100表示
		for(int n=0;n<27;++n){
			if(strcmp(token,word[n])==0){ //给每一个关键字对应的标号
				syn = n+1;  
				break;
			}
		}
	}
	else if(isNum(ch)){   //如果是数字的话
		while(isNum(ch)){
			sum = sum*10 + ch-'0';
			ch = prog[p++];
		}
		p--;
		syn = 28;
		if(sum>32767){
			syn = -1;
		}
	}
	else switch(ch){  //给每一个运算符以及分隔符各自的标号
		case '<':
			m=0;
			token[m++] = ch;
			ch = prog[p++];
			if(ch == '='){  
				syn = 29;
				token[m++] = ch;
			}
			else{
				syn = 30;
				p--;
			}
			break;
		case '>':
			m = 0;
			token[m++] = ch;
			ch = prog[p++];
			if(ch == '='){
				syn = 31;
				token[m++] = ch;
			}
			else{
				syn = 32;
				p--;
			}
			break;
		case '!': syn = 33;token[0] = ch;token[1] = prog[p++];break;
		case '+': syn = 34;token[0] = ch;break;
		case '*': syn = 35;token[0] = ch;break;
		case '/': syn = 36;token[0] = ch;break;
		case ',': syn = 37;token[0] = ch;break;
		case ';': syn = 38;token[0] = ch;break;
		case ':': syn = 39;token[0] = ch;break;
		case '{': syn = 40;token[0] = ch;break;
		case '}': syn = 41;token[0] = ch;break;
		case '[': syn = 42;token[0] = ch;break;
		case ']': syn = 43;token[0] = ch;break;
		case '(': syn = 44;token[0] = ch;break;
		case ')': syn = 45;token[0] = ch;break;
		case '=': syn = 46;token[0] = ch;break;
		case '@': syn = 0;token[0] = ch;break; //表示结束标志
		case '\n': syn = -2;break;
		default: syn = -1;break; 
	}
}

int main(){
	p = 0;
	row = 1;
	printf("Please input string:\n");
	do{
		ch = getchar();
		prog[p++] = ch;		
	}
	while(ch != '@');
	p = 0;
	do{
		scanner();
		switch(syn){
			case 28 :
				printf("%c%d%c%d%c\n",'(',syn,',',sum,')');break;
			case -1:
				printf("%s%d%c%s%c\n","Error in row",row,'!'," Find unamed symbol:",ch);break;
			case -2:
				row++;break;
			default:
				printf("%c%d%c%s%c\n",'(',syn,',',token,')');break;
		}
	}while(syn!=0);
	system("pause");
	return 0;
}
