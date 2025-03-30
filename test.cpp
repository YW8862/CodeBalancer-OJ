/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-25 22:13:02
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-30 08:39:48
 * @FilePath: /yw/projects/CodeBalancer-OJ/test.cpp
 * @Description: 该文件为测试代码，用于测试工具功能
 */

#include <iostream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ctemplate/template.h>
#include <boost/algorithm/string.hpp>
//#include <jsoncpp/json/json.h>

int main()
{
    // std::string str = "1 2 3 4 5 6 7 8 9";
    // std::vector<std::string> tokens;
    // boost::split(tokens, str, boost::is_any_of(" "), boost::token_compress_on);
    // for (const auto &token : tokens)
    // {
    //     std::cout << token << std::endl;
    // }

    std::string inhtml = "./test.html";
    std::string value = "测试代码";
    
    //形成数据字典
    ctemplate::TemplateDictionary root("test");
    root.SetValue("key",value);
    
    ctemplate::Template * tpl = ctemplate::Template::GetTemplate(inhtml,ctemplate::DO_NOT_STRIP);
    std::string outhtml;
    tpl->Expand(&outhtml,&root);

    std::cout<<outhtml<<std::endl;
    // Json::Value root;
    // root["code"] = "mycode";
    // root["user"] = "yangwei";
    // root["age"] = "19";

    // Json::StyledWriter writer;
    // std::string str = writer.write(root);
    // std::cout<<str<<std::endl;
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