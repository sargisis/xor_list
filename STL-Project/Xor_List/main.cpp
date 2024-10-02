#include "xor_list.h"

using namespace my_std;

int main()
{
    xor_list<int> a = {1,2,34,5};
    for (auto& n : a)
    {
        std::cout << n << std::endl; 
    }

}