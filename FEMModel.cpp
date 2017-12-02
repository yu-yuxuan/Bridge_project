#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "FEMModel.h"
#include <math.h>

using namespace std;

FEMModel::FEMModel()
{
    NodeNumber=0;
    ElementNumber=0;
    Node=nullptr;
    Element=nullptr;
    KReduced=nullptr;
    FReduced=nullptr;
    UReduced=nullptr;
    L_LU=nullptr;
    U_LU=nullptr;
    Stress=nullptr;
    U=nullptr;
    K=nullptr;
    F=nullptr;

}

FEMModel::~FEMModel()
{
    NodeNumber=0;
    ElementNumber=0;
    if(nullptr!=Node)
    {
        delete [] Node;
    }
    if(nullptr!=Element)
    {
        delete [] Element;
    }
    if(nullptr!=KReduced)
    {
        delete [] KReduced;
    }
    if(nullptr!=FReduced)
    {
        delete [] FReduced;
    }
    if(nullptr!=UReduced)
    {
        delete [] UReduced;
    }
    if(nullptr!=L_LU)
    {
        delete [] L_LU;
    }
    if(nullptr!=U_LU)
    {
        delete [] U_LU;
    }
    if(nullptr!=Stress)
    {
        delete [] Stress;
    }
    if(nullptr!=K)
    {
        delete [] K;
    }
    if(nullptr!=F)
    {
        delete [] F;
    }
    if(nullptr!=U)
    {
        delete [] U;
    }
}

bool FEMModel::ReadTXTFiles(const char *inputName)
{

    FILE *input;
    int i, j;
    int x, y;
    int x_i, y_i,z_i;

    input = NULL;
    input = fopen(inputName, "r");
    if(input == NULL)
    {
        return false;
    }

    fscanf(input,"%d %d\n", &NodeNumber, &ElementNumber);

    Node=new int [2*NodeNumber];
    Element=new int [3*ElementNumber];

    const int n = NodeNumber;
    KReduced = new double[4 * (n - 2)*(n - 2)];
    FReduced = new double[2 * n - 4];
    K = new double[4 * n*n];
    F = new double[2 * n];
    U=new double [2*NodeNumber];


    L_LU=new double [(2*NodeNumber-4)*(2*NodeNumber-4)];
    U_LU=new double [(2*NodeNumber-4)*(2*NodeNumber-4)];

    UReduced=new double[2*NodeNumber-4];

    //Read Node information

    for (i=0; i<NodeNumber; ++i)
    {
        fscanf(input, "%d %d", &x, &y);
        Node[2*i]=x;
        Node[2*i+1]=y;
    }

    for (j = 0; j < ElementNumber; j++)
    {
        fscanf(input, "%d %d %d\n", &x_i, &y_i,&z_i);
        Element[3*j]=x_i;
        Element[3*j+1]=y_i;
        Element[3*j+2]=z_i;
    }
    fclose(input);
    return true;
}
void FEMModel::print(void)
{
    int i, j;
    printf("%d %d\n", NodeNumber,ElementNumber);

    for (i = 0; i < NodeNumber; i++)
    {
        printf("%d %d\n", Node[2*i],Node[2*i+1]);
    }
    for (j = 0; j < ElementNumber; j++)
    {
        printf("%d %d %d\n", Element[3*j],Element[3*j+1],Element[3*j+2]);
    }
}

void FEMModel::LUDecomposition(void)
{
    int n=2*NodeNumber-4;
    //mxn
    int i = 0, j = 0, k = 0;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (j < i)
                L_LU[j*n+i] = 0;
            else
            {
                L_LU[j*n+i] = KReduced[j*n+i];
                for (k = 0; k < i; k++)
                {
                    L_LU[j*n+i] = L_LU[j*n+i] - L_LU[j*n+k] * U_LU[k*n+i];
                }
            }
        }
        for (j = 0; j < n; j++)
        {
            if (j < i)
                U_LU[i*n+j] = 0;
            else if (j == i)
                U_LU[i*n+j] = 1;
            else
            {
                U_LU[i*n+j] = KReduced[i*n+j] / L_LU[i*n+i];
                for (k = 0; k < i; k++)
                {
                    U_LU[i*n+j] = U_LU[i*n+j] - ((L_LU[i*n+k] * U_LU[k*n+j]) / L_LU[i*n+i]);
                }
            }
        }
    }
}

// bool FEMModel::ReadKFiles(const char *inputName)
// {
//     KReduced=new double [9];
//     FILE *input;
//     int i, j;
//     double x, y;
//     int x_i, y_i,z_i;

//     input = NULL;
//     input = fopen(inputName, "r");
//     if(input == NULL)
//     {
//         return false;
//     }

//     for (i=0; i<9; ++i)
//     {
//         fscanf(input, "%lf", &x);
//         KReduced[i]=x;
//     }

//     fclose(input);
//     return true;

// }

// bool FEMModel::ReadFFiles(const char *inputName)
// {
//     FReduced=new double [9];
//     FILE *input;
//     int i, j;
//     double x, y;
//     int x_i, y_i,z_i;

//     input = NULL;
//     input = fopen(inputName, "r");
//     if(input == NULL)
//     {
//         return false;
//     }

//     for (i=0; i<3; ++i)
//     {
//         fscanf(input, "%lf", &x);
//         FReduced[i]=x;
//     }

//     fclose(input);
//     return true;

// }
// void FEMModel::Lprint(void)
// {
//     for (int i = 0; i < 9; i++)
//     {
//         printf("%lf ", L_LU[i]);
//     }
//     printf("\n");
// }
// void FEMModel::Uprint(void)
// {
//     for (int i = 0; i < 9; i++)
//     {
//         printf("%lf ", U_LU[i]);
//     }
//     printf("\n");
// }


void FEMModel::SolverU(void)
{
    int n=2*NodeNumber-4;
    double *y=new double[n];
    y[0] = FReduced[0]/L_LU[0];
    for(int i=1;i<n;i++)
    {
        double sum = 0.;
        for(int j=0;j<i;j++)
        {
            sum = sum + L_LU[i*n+j]*y[j];
        }
        y[i] = (FReduced[i]-sum)/L_LU[i*n+i];
    }



    UReduced[n-1] = y[n-1];
    for(int i=1;i<n;i++) {
        int j = n-i-1;
        double sum = 0;
        for(int k=j+1;k<n;k++)
        {
            sum = sum + U_LU[j*n+k]*UReduced[k];
        }
        UReduced[j] = y[j] - sum;
    }
    for (int i = 0; i < n; ++i) {
        printf("U Reduced: %lf\n", UReduced[i]);
    }
    delete [] y;

}

int FEMModel::Position(const int l, int i, int j)
{
    return l*j + i;
}

void FEMModel::Assemble(int CarWeight,double Carposition)// 这个地方需要改！！！！！
{
    const int n = NodeNumber, m = ElementNumber, N = 2, L=2;//我需要节点数n,单元数m,固定点数N,最好有固定点的数组N[]，以及桥长度L(有几个单元);
    //int Node[2 * n] = { 0,0,0,1,1,0 }, Element[3 * m] = { 0,1,1,1,2,1,0,2,1 }, ElementBridge[L];
    //需要上一步的Node 和 Element 数组；
    int ElementBridge[L];
    int x1, x2, y1, y2, i, j, k;
    double cs, c2, s2, l, r;
	//double dt=0.025;
    double Carpos = Carposition, Carwei = CarWeight, Rho = 1, EA = 1;//需要知道单元材料刚度EA[Element[3*i+2]],材料单位长度重量Rho[Element[3*i+2]],车重量Carwei,车位置Carpos.

    for (i = 0; i < 4 * n*n; i++)
    {
        K[i] = 0;
    }// Initialize K.

    for (i = 0; i < 2*n; i++)
    {
        F[i] = 0;
    }// Initialize F.

    for (i = 0; i < L; i++)
    {
        ElementBridge[i] = 0;
    }// Initialize ElementBridge.

    for (i = 0; i < 3; i++)
    {
        printf("Node %d: %d %d\n", i, Node[2 * i], Node[2 * i + 1]);
    }// Output Node data.

    for (i = 0; i < 3; i++)
    {
        printf("Elem %d: %d %d %d\n", i, Element[3 * i], Element[3 * i + 1], Element[3 * i + 2]);
    }// Output Element data.

    ///////////////////////////////////////////////////////////////////////////////////////
    for (i = 0; i < m; i++)
    {
        //printf("Element %d:\n", i);
        //printf("Node: %d %d\n", Element[3 * i], Element[3 * i + 1]);

        x1 = Node[2 * Element[3 * i]];
        y1 = Node[2 * Element[3 * i] + 1];
        x2 = Node[2 * Element[3 * i + 1]];
        y2 = Node[2 * Element[3 * i + 1] + 1];
        cs = (x2 - x1)*(y2 - y1);
        s2 = (y2 - y1)*(y2 - y1);
        c2 = (x2 - x1)*(x2 - x1);

        //printf("c2=%f s2=%f cs=%f\n", c2, s2, cs);

        l = sqrt(c2 + s2);
        cs /= l*l*l;
        s2 /= l*l*l;
        c2 /= l*l*l;
        // 我们应该加上材料参数。

        K[Position(2 * n, 2 * Element[3 * i], 2 * Element[3 * i])] += c2;
        K[Position(2 * n, 2 * Element[3 * i + 1], 2 * Element[3 * i + 1])] += c2;
        K[Position(2 * n, 2 * Element[3 * i] + 1, 2 * Element[3 * i] + 1)] += s2;
        K[Position(2 * n, 2 * Element[3 * i + 1] + 1, 2 * Element[3 * i + 1] + 1)] += s2;
        K[Position(2 * n, 2 * Element[3 * i], 2 * Element[3 * i] + 1)] += cs;
        K[Position(2 * n, 2 * Element[3 * i] + 1, 2 * Element[3 * i])] += cs;
        K[Position(2 * n, 2 * Element[3 * i], 2 * Element[3 * i + 1])] += -c2;
        K[Position(2 * n, 2 * Element[3 * i + 1], 2 * Element[3 * i])] += -c2;
        K[Position(2 * n, 2 * Element[3 * i], 2 * Element[3 * i + 1] + 1)] += -cs;
        K[Position(2 * n, 2 * Element[3 * i + 1] + 1, 2 * Element[3 * i])] += -cs;
        K[Position(2 * n, 2 * Element[3 * i + 1], 2 * Element[3 * i] + 1)] += -cs;
        K[Position(2 * n, 2 * Element[3 * i] + 1, 2 * Element[3 * i + 1])] += -cs;
        K[Position(2 * n, 2 * Element[3 * i] + 1, 2 * Element[3 * i + 1] + 1)] += -s2;
        K[Position(2 * n, 2 * Element[3 * i + 1] + 1, 2 * Element[3 * i] + 1)] += -s2;
        K[Position(2 * n, 2 * Element[3 * i + 1], 2 * Element[3 * i + 1] + 1)] += cs;
        K[Position(2 * n, 2 * Element[3 * i + 1] + 1, 2 * Element[3 * i + 1])] += cs;

        //printf("%d %d %d\n",2 * Element[3 * i], 2 * Element[3 * i], Position(2 * n, 2 * Element[3 * i], 2 * Element[3 * i]));

        //printf("c2=%f s2=%f cs=%f\n", c2, s2, cs);
        /*for (j = 0; j < 2 * n; j++)
          {
          for (k = 0; k < 2 * n; k++)
          {
          printf("%lf ", K[2 * n*j + k]);
          }
          printf("\n");
          }
          printf("\n");*/

        F[2 * Element[3 * i] + 1] += -Rho*l / 2;
        F[2 * Element[3 * i + 1] + 1] += -Rho*l / 2;

        if (y1 == 0 && y2 == 0)
        {
            if (x1 < x2)
            {
                ElementBridge[x1] = i;
            }
            if (x1 > x2)
            {
                ElementBridge[x2] = i;
            }
        }
    }

    k = floor(Carpos);
    r = Carpos - k;
    //printf("r= %f\n", r);
    F[Element[ElementBridge[k] * 3] * 2 + 1] += -r*Carwei;
    F[Element[ElementBridge[k] * 3 + 1] * 2 + 1] += -r*Carwei;

    printf("\n");
    printf("%d\n", ElementBridge[k]);
    //for(i=0;i)
    printf("\n");

    for (i = 0; i < 2 * (n - N); i++) //这里用的是N=2,如果要改的话有其他地方直接用了2；
    {
        FReduced[i] = F[i + 4];
    }//Assemble FReduced.

    ///////////////////////////////////////////////////////////////////////////////////
    for (i = 0; i < 2 * (n - N); i++)
    {
        for (j = 0; j < 2 * n - 2 * N; j++)
        {
            KReduced[Position(2 * n - 2 * N, i, j)] = K[Position(2 * n, i + 2 * N, j + 2 * N)];
        }
    }//Assemble KReduced

    printf("K=\n");
    for (j = 0; j < 2 * n; j++)
    {
        for (i = 0; i < 2 * n; i++)
        {
            //K[2 * n*j + i] = 2 * n*j + i;
            printf("%f ", K[2 * n*j + i]);
        }
        printf("\n");
    }
    printf("\n");// Output K

    printf("F=\n");
    for (i = 0; i < 2 * n; i++)
    {
        printf("%f\n", F[i]);
    }
    printf("\n");//Output F.

    printf("KReduced=\n");
    for (j = 0; j < 2 * n - 4; j++)
    {
        for (i = 0; i < 2 * n - 4; i++)
        {
            printf("%f ", KReduced[2 * (n - 2)*j + i]);
        }
        printf("\n");
    }
    printf("\n");// Output KReduced

    printf("FReduced=\n");
    for (i = 0; i < 2 * (n - N); i++)
    {
        printf("%f\n", FReduced[i]);
    }
    printf("\n");//Output FReduced.
    /////////////////////////////////////////////////////////////////////////////////////

}


void FEMModel::SolverStress(void)
{

    int eStart, eEnd, eMaterial,NStart_x,NStart_y,NEnd_x,NEnd_y;
    double UStart_x,UStart_y,UEnd_x,UEnd_y;
    U[0]=0;
    U[1]=0;
    U[2]=0;
    U[3]=0;
    for (int i = 0; i < 2*NodeNumber-4; ++i) {
        U[i+4]=UReduced[i];
    }

    Stress=new double [ElementNumber];

    for (int i = 0; i < ElementNumber; ++i) {
        eStart=Element[3*i];
        eEnd=Element[3*i+1];
        eMaterial=Element[3*i+2];
        NStart_x=Node[2*eStart];
        NStart_y=Node[2*eStart+1];
        NEnd_x=Node[2*eEnd];
        NEnd_y=Node[2*eEnd+1];
        UStart_x=U[2*eStart];
        UStart_y=U[2*eStart+1];
        UEnd_x=U[2*eEnd];
        UEnd_y=U[2*eEnd+1];
        double Xr=NEnd_x-NStart_x;
        double Yr=NEnd_y-NStart_y;
        double Ur=UEnd_x-UStart_x;
        double Vr=UEnd_y-UStart_y;
        Stress[i]=(Xr*Ur+Yr*Vr)/(Xr*Xr+Yr*Yr)*eMaterial;
    }
    for (int i = 0; i < ElementNumber ; i++)
    {
        printf("Stress: %lf\n", Stress[i]);
    }
}
