/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:05:41
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-25 21:56:01
 * @FilePath: /yw/projects/onlineJudge/compileServer/compileServer.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "compiler.hpp"
#include "runner.hpp"
using namespace ns_compile;
using namespace ns_runner;
int main()
{
    Compiler::Compile("test");
    Runner::run("test");
    return 0;
}