#include <iostream>
#include <string>
#include <map>
#include <algorithm> 

using namespace std;

class Solution{
    public:
        bool isPalindrome(int x){
            //write your code here
            return true;
        }
};
#ifndef COMPILEONLINE
#include "header.cpp"
#endif

void Test1()
{
    bool ret = Solution().isPalindrome(-10);
    if(!ret)
    {
        std::cout<<"通过用例1，测试-10通过.....OK!"<<std::endl;
    }
    else
    {
        std::cout<<"未通过用例1，-10"<<std::endl;
    }
}

void Test2()
{
    bool ret = Solution().isPalindrome(121);
    if(ret)
    {
        std::cout<<"通过用例1，测试121通过.....OK!"<<std::endl;
    }
    else
    {
        std::cout<<"未通过用例1，121..."<<std::endl;
    }
}

int main()
{
    Test1();
    Test2();
    return 0;
}
