#include <iostream>
void QuickSort(int* array,int size)
{
    if(size<=1)
        return;
    int tmp=array[0];
    int* back=&(array[size-1]);
    int* front=array;
    while(front!=back)
    {
        while(*back>=tmp&&back>front)
            --back;
        *front=*back;
        while(*front<=tmp&&back>front)
            ++front;
        *back=*front;
    }
    *front=tmp;
    int idx=front-array;
    QuickSort(array,idx);
    QuickSort(array+idx+1,size-idx-1);
}
int main()
{
    int ar[]={1,43,336,213,654,32,645,4321};
    QuickSort(ar,8);
    for(int i=0;i<8;i++)
    {
        std::cout<<ar[i]<<std::endl;
    }
}