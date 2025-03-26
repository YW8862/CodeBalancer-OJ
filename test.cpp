/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-25 22:13:02
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-26 15:34:08
 * @FilePath: /yw/projects/CodeBalancer-OJ/test.cpp
 * @Description: 该文件为测试代码，用于测试工具功能
 */

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>

int main()
{
    Json::Value root;
    root["code"] = "mycode";
    root["user"] = "yangwei";
    root["age"] = "19";

    Json::StyledWriter writer;
    std::string str = writer.write(root);
    std::cout<<str<<std::endl;
    return 0;
}

// int main()
// {
//     struct rlimit r;
//     r.rlim_cur = 1;
//     r.rlim_max = RLIM_INFINITY;

//     setrlimit(RLIMIT_CPU,&r);
//     while(1);
//     return 0;
// }