/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:05:41
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-28 09:10:17
 * @FilePath: /yw/projects/onlineJudge/compileServer/compileServer.cpp
 * @Description: 提供编译并运行服务，需要确保文件唯一性
 */

#include "compilerRun.hpp"
#include "../common/httplib.h"

using namespace ns_compileAndRun;
using namespace httplib;

void Usage(char *argv[])
{
    std::cout << "Usage:\n      " << argv[0] << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv);
        return 1;
    }
    Server svr;
    // svr.set_base_dir("./wwwroot");
    svr.Get("/hello", [](const Request &request, Response &response)
            { response.set_content("hello world,测试代码", "text/plain;charset=utf-8"); });

    svr.Post("/compileAndRun", [](const Request &request, Response &response)
             {
        //用户请求的正文就是想要的json字符串
        std::string inputJson = request.body;
        std::string outputJson;
        if(!inputJson.empty())
        {
            compileAndRun::start(inputJson,&outputJson);
            response.set_content(outputJson,"application/json;charset=utf-8");
        } });
    svr.listen("0.0.0.0", atoi(argv[1]));

    return 0;
}

// int main()
// {
//     LOGTOSCREEN();
//     std::string inJson;
//     Json::Value inValue;
//     inValue["code"] = "#include<iostream>\nint main(){\nstd::cout<<\"hello world\"<<std::endl;int a = 1;a/=0;\nreturn 0;\n}";
//     inValue["input"] = "";
//     inValue["cpuLimit"] = 5;
//     inValue["memLimit"] = 1024*10;

//     Json::Value outValue;

//     Json::FastWriter writer;
//     inJson = writer.write(inValue);
//     std::cout<<inJson<<std::endl;

//     std::string outJson;
//     compileAndRun::start(inJson,&outJson);
//     std::cout<<outJson<<std::endl;
//     //compileAndRun::removeTempFile();
//     return 0;
// }
