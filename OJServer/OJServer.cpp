/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-28 09:25:55
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-30 08:47:55
 * @FilePath: /yw/projects/CodeBalancer-OJ/OJServer/OJControl.hpp
 * @Description: 为外界提供OJ服务
 */
#include <iostream>
#include <signal.h>
#include "OJControl.hpp"

#include "jsoncpp/json/json.h"
#include "../common/httplib.h"
#include "OJControl.hpp"
#include "OJView.hpp"
#include "OJModel.hpp"

using namespace httplib;
using namespace ns_control;

static Control *ctlptr = nullptr;

void recovery(int signal)
{
    ctlptr->recoveryMachine();
}

int main()
{
    signal(SIGQUIT,recovery);
    // 用户请求路由
    Server svr;
    Control control;
    ctlptr = &control;
    View view;
    svr.set_base_dir("./wwwroot");

    // 获取题目列表
    svr.Get("/allQuestions", [&control](const Request &request, Response &response)
            { 
        //返回一张包含所有题目的网页
        std::string html;
        control.allQuestions(&html);
        response.set_content(html, "text/html;charset=utf-8"); });
        
    // 根据题目编号获得题目内容
    // 如question100
    svr.Get(R"(/question/(\d+))", [&control](const Request &request, Response &response)
            {
        //一个题目
        std::string number = request.matches[1];
        std::string html;
        control.oneQuestion(number,&html);
        response.set_content(html,"text/html;charset=utf-8"); });

    // 用户提交代码使用判题功能 -->每题的测试用例   编译并运行
    svr.Post(R"(/judge/(\d+))", [&control](const Request &request, Response &response)
            {
        //获取题目编号
        std::string number = request.matches[1];
        std::string resultJson;
        control.judge(number,request.body,&resultJson);

        response.set_content(resultJson,"application/json;charset=utf-8"); });

    svr.listen("0.0.0.0", 8888);
    return 0;
}