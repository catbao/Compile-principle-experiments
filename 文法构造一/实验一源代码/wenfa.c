#include <stdio.h>
//创建三个结构体分别表示终结符、非终结符、产生式 
//终结符
typedef struct FINAL{
    char final[10];
    int num;
} FINAL;

//非终结符
typedef struct UNFINAL{
    char unfinal[10];
    int num;
} UNFINAL;

//产生式
typedef struct EXP{
    int num;
    char expression[20];
} EXP;

char start; //开始符号

 
void output(UNFINAL nf,FINAL yf,EXP *exp,char start){
	
	//打印非终结符
	printf("非终结符：\n");
    printf("%s\n", nf.unfinal);
    
    //打印终结符
    printf("终结符：\n");
    printf("%s\n", yf.final);
    
    //打印产生式规则
    printf("产生式：\n");
    int k = 0;
    int expnum = exp[k].num;
    for (k = 1; k <= expnum; k++){
        printf("%s\n", exp[k].expression);
    }
    
    //打印开始符号
    printf("开始符号:\n");
    printf("%c", start);
}

void input(){
    int i;
    FINAL yf;
    UNFINAL nf;
    EXP exp[20];
    //输入非终结符
    scanf("%d", &nf.num);
    for (i = 0; i < nf.num * 2; i++){
        scanf("%s", &nf.unfinal[i]);
        i++;
        nf.unfinal[i] = ' ';
    }

    //输入终结符
    scanf("%d", &yf.num);
    for (i = 0; i < yf.num * 2; i++){
        scanf("%s", &yf.final[i]);
        i++;
        yf.final[i] = ' ';
    }

    //输入产生式规则
    scanf("%d", &exp[0].num);
    for (i = 1; i <= exp[0].num; i++){
        char ch[20];
        gets(ch);
        int temp = 0, j = 0;
        while (ch[j] != '\0'){
            if (ch[j] >= '0' && ch[j] <= '9'){
                exp[i].expression[temp] = '-';
                temp++;
                exp[i].expression[temp] = '>';
                temp++;
                j++;
            }
            else{
                exp[i].expression[temp] = ch[j];
                temp++;
                j++;
            }
        }
        exp[i].expression[temp] = '\0';
    }

    //输入开始符号
    scanf("%s", &start);

    //输出文法
    output(nf,yf,exp,start);
}

int main(){
    input();
    return 0;
}

