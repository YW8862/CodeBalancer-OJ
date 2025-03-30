
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
