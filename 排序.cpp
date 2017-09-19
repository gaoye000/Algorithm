#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <algorithm>
//#pragma comment(linker,"/STACK:1024000000,1024000000")
using namespace std;
const int MAXM=500010;
const int INF=0x3fffffff;
const int STACKMAX=40000;
int ini_st[MAXM],st[MAXM];//全局变量，原始数据序列和临时数据序列
int n;//全局变量，序列长度
void debug()
{
    printf("YES\n");
}
void swap(int&a,int&b)//交换数据
{
    if(a==b) return;
    else a^=b^=a^=b;
}
int myrand()//随机生成int(-2^31~2^31-1)范围的数
{
    return (rand()<<20)^(rand()<<10)^rand();
}
void show(int*st)//用来debug
{
    for(int i=0;i<n;i++)
        printf("%d\n",st[i]);
    printf("\n");
}
void gototxt()
{
    FILE*p=fopen("initial.txt","w");
    for(int i=0;i<n;i++)
        fprintf(p,"%d\n",ini_st[i]);
    fclose(p);
    //debug();
}
void gotoqstxt()
{
    FILE*p=fopen("out_quicksort.txt","w");
    for(int i=0;i<n;i++)
        fprintf(p,"%d\n",st[i]);
    fclose(p);
    //debug();
}
void gotomstxt()
{
    FILE*p=fopen("out_mergesort.txt","w");
    for(int i=0;i<n;i++)
        fprintf(p,"%d\n",st[i]);
    fclose(p);
    //debug();
}
void gotobstxt()
{
    FILE*p=fopen("out_bubblesort.txt","w");
    for(int i=0;i<n;i++)
        fprintf(p,"%d\n",st[i]);
    fclose(p);
    //debug();
}
void gotocstxt()
{
    FILE*p=fopen("out_choosesort.txt","w");
    for(int i=0;i<n;i++)
        fprintf(p,"%d\n",st[i]);
    fclose(p);
    //debug();
}
bool examine()//检验是否排序为升序序列
{
    int maxm=st[0];
    for(int i=1;i<n;i++)
    {
        if(maxm>st[i]) return 0;
        maxm=st[i];
    }
    return 1;
}
int ini(char*str)//初始化，处理输入
{
    char ccc;
    sscanf(str,"%c%d",&ccc,&n);
    if(n>MAXM-10) return -1;
    if(ccc=='r')
    {
        for(int i=0;i<n;i++)
            {ini_st[i]=myrand();}
        gototxt();
    }
    else if(ccc=='u')
    {
        int start=myrand(),over=myrand();
        if(start>over) swap(start,over);
        int step=over/n-start/n;
        ini_st[0]=start;
        for(int i=1;i<n;i++)
        {
            ini_st[i]=ini_st[i-1]+step;
            if(ini_st[i]<ini_st[i-1]) {printf("%d %d %d\n",start,over,step);show(ini_st);return -1;}//若生成错误，输出生成序列，返回ERROR
        }
        gototxt();
    }
    else if(ccc=='d')
    {
        int start=myrand(),over=myrand();
        if(start<over) swap(start,over);
        int step=start/n-over/n;
        ini_st[0]=start;
        for(int i=1;i<n;i++)
        {
            ini_st[i]=ini_st[i-1]-step;
            if(ini_st[i]>ini_st[i-1]) {printf("%d %d %d\n",start,over,step);show(ini_st);return -1;}//若生成错误，输出生成序列，返回ERROR
        }
        gototxt();
    }
    else if(ccc=='e')
    {
        int start=myrand();
        for(int i=0;i<n;i++)
            {ini_st[i]=start;}
        gototxt();
    }
    else return -1;
    return 0;
}
void bubblesort_go(int l,int r)//冒泡排序
{
    bool flag=1;
    while(flag)//若一轮未交换，则排序完成
    {
        flag=0;
        for(int i=l;i<r;i++)
        {
            if(st[i]>st[i+1])
            {
                swap(st[i],st[i+1]);
                flag=1;
            }
        }
    }
}
void choosesort_go(int l,int r)//直接选择排序
{
    int k;
    for(int i=l;i<r;i++)
    {
        k=i;
        for(int j=i+1;j<=r;j++)
        {
            if(st[j]<st[k]) k=j;//找出部分最大值的位置
        }
        swap(st[i],st[k]);
    }
}
void quicksort_go(int l,int r,int stack_n)//快速排序
{
    if(stack_n>STACKMAX) {choosesort_go(l,r);return;}//爆栈时转换为直接选择排序
    if(l>=r) return;
    int mark=l+rand()%(r-l+1);//随机选择基准点
    swap(st[l],st[mark]);
    int i=l+1,j=r;
    while(i<j)
    {
        while(st[i]<=st[l]&&i<j) i++;
        while(st[j]>=st[l]&&j>i) j--;
        if(i>=j) break;
        swap(st[i],st[j]);
    }
    if(st[l]<st[i]) i--;
    swap(st[l],st[i]);
    //show(st);
    quicksort_go(l,i-1,stack_n+1);//递归左侧
    quicksort_go(i+1,r,stack_n+1);//递归右侧
}
void mergesort_go(int l,int r,int stack_n)//归并排序
{
    if(stack_n>STACKMAX) {choosesort_go(l,r);return;}//爆栈时转换为直接选择排序
    if(l>=r) return;
    int m=(l+r)>>1;
    mergesort_go(l,m,stack_n+1);//递归左侧
    mergesort_go(m+1,r,stack_n+1);//递归右侧
    int i=l,j=m+1,len=r-l+1;
    int*temp=new int[len];//新建临时数组
    for(int k=0;k<len;k++)
    {
        if(i<=m&&j<=r)
        {
            if(st[i]<=st[j]) temp[k]=st[i++];
            else temp[k]=st[j++];
        }
        else if(i<=m) temp[k]=st[i++];
        else temp[k]=st[j++];
    }
    for(int k=0;k<len;k++)//数据转移
        st[l+k]=temp[k];
    delete []temp;//删除临时数组
}
int quicksort()
{
    for(int i=0;i<n;i++)
        st[i]=ini_st[i];//复制数据
    //show(st);
    int t1=clock();//计时
    quicksort_go(0,n-1,1);
    int t=clock()-t1;//计时
    gotoqstxt();
    //debug();
    return t;
}
int mergesort()
{
    for(int i=0;i<n;i++)
        st[i]=ini_st[i];//复制数据
    //show(st);
    int t1=clock();//计时
    mergesort_go(0,n-1,1);
    int t=clock()-t1;//计时
    gotomstxt();
    //debug();
    return t;
}
int bubblesort()
{
    for(int i=0;i<n;i++)
        st[i]=ini_st[i];//复制数据
    int t1=clock();//计时
    bubblesort_go(0,n-1);
    int t=clock()-t1;//计时
    gotobstxt();
    //debug();
    return t;
}
int choosesort()
{
    for(int i=0;i<n;i++)
        st[i]=ini_st[i];//复制数据
    int t1=clock();//计时
    choosesort_go(0,n-1);
    int t=clock()-t1;//计时
    gotocstxt();
    //debug();
    return t;
}
int main()
{
    srand(time(0));
    char str[20];
    //FILE* backup_stdin=stdin;
    printf(" Please write down r for random statics,\n u for upper statics,\n d for downer statics\n and e for equal statics.\n An number can follow without space to define the length.\n");
    while(~scanf("%s",&str))
    {
        int t;
        if(ini(str)==-1) {printf("ERROR\n");continue;}
        t=quicksort();
        if(examine()) {printf(" Quick sort uses %d ms,\n",t);}
        t=mergesort();
        if(examine()) {printf(" merge sort uses %d ms,\n",t);}
        t=bubblesort();
        if(examine()) {printf(" bubble sort uses %d ms\n",t);}
        t=choosesort();
        if(examine()) {printf(" and choice sort uses %d ms.\n\n",t);}
    }
    return 0;
}
