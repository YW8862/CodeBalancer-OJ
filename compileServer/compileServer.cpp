/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:05:41
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-26 22:01:27
 * @FilePath: /yw/projects/onlineJudge/compileServer/compileServer.cpp
 * @Description: 提供编译并运行服务，需要确保文件唯一性
 */

#include "compilerRun.hpp"

using namespace ns_compileAndRun;

int main()
{
    std::string inJson;

    Json::Value inValue;
    inValue["code"] = "#include<iostream>\nint main(){\nstd::cout<<\"hello world\"<<std::endl;\nreturn 0;\n}";
    inValue["input"] = "";
    inValue["cpuLimit"] = 1;
    inValue["memLimit"] = 1024;

    Json::Value outValue;


    Json::FastWriter writer;
    inJson = writer.write(inValue);
    std::cout<<inJson<<std::endl;

    std::string outJson;
    compileAndRun::start(inJson,&outJson);
    return 0;
}
