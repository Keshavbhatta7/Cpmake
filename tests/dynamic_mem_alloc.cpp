#include <iostream>
#define MG_NUM 20

int main(void)
{
    char* str = new char[MG_NUM];

    for (int i = 0; i < MG_NUM; i++)
        *(str+i) = '_';

    delete[] str;
    std::cout << str << std::endl;
}
