#include <stdio.h>
#include <math.h>

#define MEM 100                                                     //配列の数
#define EPS_MIN 0.0000001                                           //許容誤差(最小)
#define EPS_MAX 100000000                                           //許容誤差(最大)

double K1A[MEM][MEM]={
    {5,1,1},
    {1,4,1},
    {2,1,3}};
double K1C[MEM]={10,12,13};

double S1A[MEM][MEM]={
 {3,8},
 {2,5}};
 double S1C[MEM]={7,5};

double K2A[MEM][MEM] ={
    {5,2,0,0,0,0,0,0,0,0},
    {2,5,2,0,0,0,0,0,0,0},
    {0,2,5,2,0,0,0,0,0,0},
    {0,0,2,5,2,0,0,0,0,0},
    {0,0,0,2,5,2,0,0,0,0},
    {0,0,0,0,2,5,2,0,0,0},
    {0,0,0,0,0,2,5,2,0,0},
    {0,0,0,0,0,0,2,5,2,0},
    {0,0,0,0,0,0,0,2,5,2},
    {0,0,0,0,0,0,0,0,2,5}};
double K2C[MEM] = {3,1,4,0,5,-1,6,-2,7,-15};

double x[MEM] = {0};                                                //近似値
double x_old,s;                                                       //前回の近似値
double d[MEM] = {0};                                                //対角成分以外の絶対値の和用
double error;                                                       //誤差
double a[MEM][MEM] = {0};                                                //式代入用
double c[MEM] = {0};
int mode[MEM];                                                      //モード処理用
int i,j,p,e;

int main(){
    printf("モードを選択してください。\n[1] サンプルの実行   [2] 手動入力\n => ");
    scanf("%d",&mode[0]);
    if(mode[0]==1){
        printf("\n使用するデータを選択してください。\n[1] 3x3(収束Ver)    [2] 10x10(収束Ver)    [3] 2x2(発散Ver)\n => ");
        scanf("%d",&mode[1]);
        if(mode[1]==1){
            e=3;
             for(i=0;i<e;i++){                                        //行列の要素数分だけループ
                 for(j=0;j<e;j++){
                     a[i][j]=K1A[i][j];
                 }
                 c[i]=K1C[i];
             }
        }else if(mode[1]==2){
            e=10;
            for(i=0;i<e;i++){                                         //行列の要素数分だけループ
                for(j=0;j<e;j++){
                    a[i][j]=K2A[i][j];
                }
                c[i]=K2C[i];
            }
        }else if(mode[1]==3){
            e=2;
            for(i=0;i<e;i++){                                         //行列の要素数分だけループ
                for(j=0;j<e;j++){
                    a[i][j]=S1A[i][j];
                }
                c[i]=S1C[i];
            }
        }
    }else if(mode[0]==2){
        printf("行列の要素数を入力してください。\n => ");
        scanf("%d",&e);
        /*********************/
         for(i=0;i<e;i++){
             for(j=0;j<e;j++){
                 printf("\n行列の要素数 : %d\n",e);
                 printf("[%d]行 [%d]列を入力してください。\n => ",i+1,j+1);
                 scanf("%lf",&a[i][j]);
             }
             printf("[%d]行目の解を入力してください。\n => ",i+1);
             scanf("%lf",&c[i]);
         }
    }
    printf("\n初期値を入力してください。\n => ");
    scanf("%lf",&s);
    for(i=0;i<e;i++){                                         //行列の要素数分だけループ
        x[i]=s;                                                     //初期値
        for(j=0;j<e;j++){                                     //列成分計算
            if(i!=j){                                               //対角成分以外を抽出する条件
                d[i]+=fabs(a[i][j]);                                //対角成分以外の絶対値の和
            }
        }
        if(d[i]>a[i][i]){
            printf("入力された行列が対角優位ではないため解が発散している可能性があります。\n");
        }
    }
    printf("\n-------------------------以下計算過程---------------------------\n\n");
    while(1){
        error=0;
        printf("%3d回目 ",p+=+1);
        for(i=0;i<e;i++){                                     //行列の要素数分だけループ
            x_old = x[i];                                           //xの値を保存
            printf("[x%d] = ",i+1);                                   //xの番号表示
            x[i] = c[i];                                            //計算の最初に行列の解を代入
            for(j=0;j<e;j++){                                 //列成分計算
                if(i!=j){                                           //対角成分以外を抽出する条件
                    x[i] += x[j] * -(a[i][j]);                      //方程式の計算
                }
            }
            x[i] /= a[i][i];                                        //方程式から得られた結果を対角成分で割る
            printf("%lf  ",x[i]);                                   //近似解の表示
            FILE *hoge;
            hoge = popen( "gnuplot -persist","w" ) ;
            fprintf(hoge, "plot '-' with linespoints pointtype 2\n");
            fprintf(hoge, "%lf\n",x[i]);
            pclose(hoge);
            error += fabs(x_old - x[i]);                         //誤差累積計算(近似値-前回の近似値)
        }
        printf("\n");
        if(error <= EPS_MIN){                                    //収束判定(近似値の誤差が指定許容誤差以下になった場合を収束条件とする)
            printf("\n>>>%d回目で式が収束しました。\n",p);
            break;
        }else if(error >= EPS_MAX){                              //発散判定(近似値の誤差が指定許容誤差以上になった場合を発散条件とする)
            printf("\n>>>%d回目で式が発散しました。\n",p);
            break;
        }
    }
}
