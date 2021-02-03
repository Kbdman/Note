/*
组件最大/小堆，将堆顶移出对放入顺序队列，如此循环 O(nlogn)
*/
#include <iostream>
void swap(int* a,int*b)
{
    int tmp=*a;
    *a=*b;
    *b=tmp;
}
void heap(int* array,int idx,int size)
{
    if(idx*2+1>=size)
    {
        return;
    }
    int* mc=array+(idx*2+1);
    if(idx*2+1<size)
    {
        heap(array,idx*2+1,size);
    }
    if(idx*2+2<size)
    {
        heap(array,idx*2+2,size);
        if(*mc<array[idx*2+2])
            mc=array+(idx*2+2);
    }
    if(array[idx]<*mc)
        swap(mc,array+idx);
}
void HeapSort(int* array,int size)
{
   
    for( int i=size;i>1;i--)
    {
        heap(array,0,i);
        swap(array,array+i-1);
    }
}
int main()
{
    int ar[]={1,43,336,213,654,32,645,4321};
    HeapSort(ar,8);
    for(int i=0;i<8;i++)
    {
        std::cout<<ar[i]<<std::endl;
    }
}