/*
取一个数作为标定值，将小于它的值全部移动到他左边，将大于他的值全部移动到他的右边，再对他左边的部分和右边部分分别这么操作，如此递归
如果将小于它的值全部移动到他左边，将大于他的值全部移动到他的右边？
    取第一个值做标定值，设置一个由前向后的指针front，一个有后向前的指针back
        back向前移动，直到找到一个比标定值小的值，将他保存到front位置，back位置成为一个插槽
        front向前移动，直到找到一个比标定值大的值，将他保存到back位置，front位置成为一个插槽
    循环上面的步骤，直到front和back重叠，将标定值插入这个位置

*/
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