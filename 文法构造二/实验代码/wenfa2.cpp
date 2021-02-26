#include<stdio.h>
#include<stdlib.h>
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
map<char,vector<string>> rule;
map<char,set<char>> first;
map<char,set<char>> follow;
set<char> Vt;
set<char> Vn;

void trim(string &s){
int index = 0;
if( !s.empty()){
    while( (index = s.find(' ',index)) != string::npos)
    s.erase(index,1);
    }
}


void GetRule(EXP *exp){
    for(int i=1;i<=exp[0].num;++i){
        string right = "";
        int j=5;
        while(exp[i].expression[j] != '\0'){
            right += exp[i].expression[j];
            j++;
        }
        trim(right);
        rule[exp[i].expression[0]].push_back(right);
    }
    // for(map<char,vector<string>>::iterator
    // it=rule.begin();it!=rule.end();it++){
    //     cout<<it->first<<"->"<<it->second.size()<<endl;
    // }
}

void make_first(){
    //初始化first集
    for(set<char>::iterator it = Vn.begin();it!=Vn.end();it++){
        set<char> t;
        first.insert(make_pair(*it,t));
    }
    //遍历规则rule
    //2.若X∈Vn,有X→aα,(a∈Vt)或/和X→ε 则a或/和ε∈FIRST(x)
    //ε 用 @ 来代替
    for(map<char,vector<string>>::iterator it = rule.begin();it!=rule
    .end();it++){
        //用tempp记录it->first对应的first集
        set<char> tempp;
        vector<string> right = it->second;
        char left = it->first;
        for(int i=0;i<right.size();i++){
            if(Vt.count(right[i][0])!=0){
                tempp.insert(right[i][0]);
            }
            if(right[i] == "@"){
                tempp.insert('@');
            }
        }
        if(first.at(left).size() == 0){
            first.at(left) = tempp;
        }
        else{
            first.at(left).insert(tempp.begin(),tempp.end());
        }
    }
     //循环结束条件： 直到每一个FIRST(X)不再增大为止
    //3.对X→Y0Y1…….Yk(且Y0 ∈Vn), 反复使用以下直到每一个FIRST(X)不再增大为止
    set<char> temp[9];
    int k=0,first_size=0;
    for(map<char,vector<string>>::iterator it = rule.begin();;it++,k++){
         //用temp[k]记录it->first对应的first集
        vector<string> right = it->second;
        char left = it->first;
         //3.对X→Y0Y1…….Yk(且Y0 ∈Vn), 反复使用以下直到每一个FIRST(X)不再增大为止
        bool flag = true; //检测连续含{ε}的Yj
        for(int i=0;i<right.size();++i){
            for(int j=0;i<right[i].size()&&(Vn.count(right[i][j])>0)&&flag;j++){
                set<char> temp1 = first.at(right[i][j]);
                //i 若Y0∈Vn  则把FIRST(Y0)\{ε}元素加入FIRST(X)中
                if(j == 0){
                     //\{ε}
                    if(temp1.count('@')>0){
                        temp1.erase('@');
                    }
                    //Y0不含{ε}
                    else{
                        flag = false;
                    }
                     //FIRST(Y0)\{ε}
                    temp[k].insert(temp1.begin(),temp1.end());
                }
                /*
                ii 若Y1、Y2、……Y i-1 ∈Vn(2≤i ≤k) 且ε ∈FIRST(Y j) (1≤j ≤i-1)
                    则把FIRST(Yi )\{ε}元素加入FIRST(x)中
                 */
                else if(j<right[i].size()-1){
                     //Y0...Yj-1含{ε}
                    if(temp1.count('@')>0){
                        temp1.erase('@');
                    }
                    //Yj不含{ε}
                    else{
                        flag = false;
                    }
                      //FIRST(Yj)\{ε}
                    temp[k].insert(temp1.begin(),temp1.end());
                }
                /*
                iii 若Y0、Y1、……Yk ∈Vn且ε ∈FIRST(Yj)(0≤j ≤k)
                    则把ε元素加入FIRST(x)中
                 */
                else{
                    temp[k].insert('@');
                }
            }
        }
        if(first.at(left).size() == 0){
            first.at(left) = temp[k];
        }
        else{
            first.at(left).insert(temp[k].begin(),temp[k].end());
        }

        if(k == first.size()-1){
            int cnt = 0;
            for(int kk=0;kk<=k;kk++){
                cnt += temp[kk].size();
            }
             //和上一次大小一样，循环终止
            if(cnt == first_size) break;
            else{
                k=0;
                it = rule.begin();
                first_size = cnt;
            }
        }
    }

    cout<<endl;
    cout<<endl;
    cout<<"First Set:"<<endl;
    for(auto it = Vn.begin();it!=Vn.end();++it){
        //cout<<first[*it].size()<<" ";
        cout<<"First("<<*it<<"):" ;
        for(auto itt = first[*it].begin();itt != first[*it].end();itt++){
            cout<<*itt<<" ";
        }
        cout<<endl;
    }
}

void make_follow(){
    for(set<char>::iterator it = Vn.begin();it != Vn.end();++it){
        set<char> t;
         //令#∈FOLLOW(S) S为文法开始符号
        if(*it==start){
            t.insert('#');
        }
        follow.insert(make_pair(*it,t));
    }

     //遍历规则rule
    //2.对A→ αBβ,且β ≠ε, 则将 FIRST(β)\{ε}加入FOLLOW(B)中
    for(map<char,vector<string>>::iterator it = rule.begin();it!=rule.end();++it){
        vector<string> right = it->second;
        char left = it->first;
        for(int i=0;i<right.size();++i){
            for(int j=0;j<right[i].size();j++){
                if(Vn.count(right[i][j]) != 0){
                    set<char> tt;
                      //β属于Vt
                    if(Vt.count(right[i][j+1]) != 0){
                        tt.insert(right[i][j+1]);
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                    //β属于Vn,FIRST(β)\{ε}加入FOLLOW(B)中
                    if(Vn.count(right[i][j+1]) != 0){
                        tt = first.at(right[i][j+1]);
                        tt.erase('@');
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                }
            }
        }
    }

        // cout<<endl;
        // cout<<endl;
        // cout<<"Follow Set"<<endl;
        // for(auto it = Vn.begin();it!=Vn.end();++it){
        //     //cout<<follow[*it].size()<<" ";
        //     cout<<"Follow("<<*it<<"):" ;
        //     for(auto itt = follow[*it].begin();itt != follow[*it].end();itt++){
        //         cout<<*itt<<" ";
        //     }
        //     cout<<endl;
        // }

     // 3.反复, 直至每一个FOLLOW(A)不再增大
    //   对A→ αB或A→ αBβ(且ε ∈ FIRST(β))
    //   则FOLLOW(A)中的全部元素加入FOLLOW(B)
        int k=0;
        int follow_size = 0;
        for(map<char,vector<string>>::iterator it = rule.begin();;it++,k++){
            vector<string> right = it->second;
            char left = it->first;
            // for(int i=0;i<right.size();++i){
            //     for(int j=0;j<right[i].size();++j){
            //         cout<<right[i][j]<<" ";
            //     }
            //     cout<<endl;
            // }
            for(int i=0;i<right.size();++i){
                for(int j=0;j<right[i].size();j++){
                    //cout<<endl<<right[i].size();
                    if(Vn.count(right[i][j]) != 0){
                        //cout<<"  yes"<<endl;
                        set<char> tt;
                         //到规则的最后一个字符
                        
                        if(j == right[i].size() - 1){
                            //最后一个字符是Vn，即A→αB，则FOLLOW(A)中的全部元素加入FOLLOW(B)
                            tt = follow.at(left);
                            follow.at(right[i][j]).insert(tt.begin(),tt.end());
                        }
                          //没到最后一个,即A→ αBβ(且ε∈FIRST(β)) 这种情况
                        else {
                            bool flag = true;
                            //检索ε∈FIRST(β)是否成立
                            for(int jj=j+1;jj<right[i].size();jj++){
                                 //right[i][jj]是Vn且包含ε
                                if(Vn.count(right[i][jj])!=0 && first.at(right[i][jj]).count('@')!=0){
                                    continue;
                                }
                                  //right[i][jj]不是Vn或不包含ε
                                else{
                                    flag = false;
                                }
                            }
                              //A→ αBβ(且ε∈FIRST(β))情况满足
                            if(flag){
                                tt = follow.at(left);
                                follow.at(right[i][j]).insert(tt.begin(),tt.end());
                            }
                        }
                    }
                }
            }
            if(k == follow.size() - 1){
                int cnt = 0;
                for(map<char,set<char>>::iterator it1 = follow.begin();it1!=follow.end();it1++){
                    cnt += it1->second.size();
                }
                 //和上一次大小一样，循环终止
                if(cnt == follow_size) break;
                else{
                    k = 0;
                    it = rule.begin();
                    follow_size = cnt;
                }
            }
        }
    
        cout<<endl;
        cout<<endl;
        cout<<"Follow Set"<<endl;
        for(auto it = Vn.begin();it!=Vn.end();++it){
            //cout<<follow[*it].size()<<" ";
            cout<<"Follow("<<*it<<"):" ;
            for(auto itt = follow[*it].begin();itt != follow[*it].end();itt++){
                cout<<*itt<<" ";
            }
            cout<<endl;
        }
}

string cal_select(string temp){
    string a = "";
    if(Vt.count(temp[3])!=0){  //直接
        a += temp[3];
        return a;
    }
    else if(temp[3] == '@'){
        set<char> t = follow[temp[0]];
        for(auto it = t.begin();it!=t.end();it++){
            a += " ";
            a += (*it);
        }
    }
    else{
        set<char> t = first[temp[3]];
        if(auto itt = t.find('@') != t.end()){
            t.erase('@');
            for(auto it = t.begin();it!=t.end();it++){
                a += " ";
                a += (*it);
            }
            set<char> tt = first[temp[0]];
            for(auto ittt = t.begin();ittt!=t.end();ittt++){
                a += " ";
                a += (*ittt);
            }
        }
        else{
            for(auto it = t.begin();it!=t.end();it++){
                a += " ";
                a += (*it);
            }
        }
    }
}

void make_select(){
    cout<<endl;
    cout<<"Select Set:"<<endl;
    for(auto it = rule.begin();it!=rule.end();it++){
        vector<string> temp = it->second;
        char c = it->first;
        string str;
        stringstream stream;
        stream << c;
        str = stream.str();  //将字符转为字符串形式
        string tem = str + "->";
        for(int i=0;i<temp.size();++i){
            tem += temp[i];
            cout<<"SELECT("<<tem<<")"<<": "<<cal_select(tem)<<endl;
            tem = str + "->";
        }
        temp.clear();
    }
}

void output(UNFINAL nf,FINAL yf,EXP *exp,char start){

	//打印非终结符
	printf("非终结符：\n");
    printf("%s\n", nf.unfinal);

    //打印终结符
    printf("终结符：\n");
    printf("%s\n", yf.final);

    //打印产生式规则
    printf("产生式:\n");
    int k = 0;
    int expnum = exp[k].num;
    for (k = 1; k <= expnum; k++){
        printf("%d：%s\n", k-1,exp[k].expression);
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
        Vn.insert(nf.unfinal[i]);
        i++;
        nf.unfinal[i] = ' ';
    }

    //输入终结符
    scanf("%d", &yf.num);
    for (i = 0; i < yf.num * 2; i++){
        scanf("%s", &yf.final[i]);
        Vt.insert(yf.final[i]);
        i++;
        yf.final[i] = ' ';
    }

    //输入产生式规则
    scanf("%d", &exp[0].num);
    getchar();
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

    GetRule(exp);

    //输入开始符号
    scanf("%s", &start);

    //输出文法
    output(nf,yf,exp,start);

    //OutPutFirstSet(nf,yf,exp);
}

int main(){
    //空字用@符号表示
    input();
    make_first();
    make_follow();
    make_select();
    cout<<endl;
    system("pause");
    return 0;
}

