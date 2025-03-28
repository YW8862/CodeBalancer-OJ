/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-28 09:25:55
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-28 19:27:41
 * @FilePath: /yw/projects/CodeBalancer-OJ/OJServer/OJControl.hpp
 * @Description: 为外界提供OJ服务
 */
#include <iostream>
#include "OJControl.hpp"
#include "OJView.hpp"
#include "OJModel.hpp"
#include "jsoncpp/json/json.h"
#include "../common/httplib.h"

using namespace httplib;

int main()
{
    // 用户请求路由
    Server svr;
    svr.set_base_dir("./wwwroot");
    
    // 获取题目列表
    svr.Get("/allQuestions", [](const Request &request, Response &response)
            { response.set_content("这是所有题目列表", "text/plain;charset=utf-8"); });

    // 根据题目编号获得题目内容
    // 如question100
    svr.Get(R"(/question/(\d+))", [](const Request &request, Response &response)
            {
        //获取题目编号
        std::string number = request.matches[1];
        response.set_content("这是一个题目："+number,"text/plain;charset=utf-8"); });

    // 用户提交代码使用判题功能 -->每题的测试用例   编译并运行
    svr.Get(R"(/judge/(\d+))", [](const Request &request, Response &response)
            {
        //获取题目编号
        std::string number = request.matches[1];
        response.set_content("编译运行题目："+number,"text/plain;charset=utf-8"); });

    svr.listen("0.0.0.0", 8888);
    return 0;
}