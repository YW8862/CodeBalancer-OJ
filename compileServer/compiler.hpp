/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:04:34
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-25 22:44:58
 * @FilePath: /yw/projects/onlineJudge/compileServer/conpiler.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../common/log.hpp"
#include "../common/utils.hpp"

using namespace ns_utils;

namespace ns_compile
{
    class Compiler
    {
    public:
        Compiler() {}
        ~Compiler() {}

        // 实现编译功能
        /**
         * @param fileName:生成的临时源代码文件
         * @return:编译成功，返回true，否则返回false
         */
        static bool Compile(const std::string &fileName)
        {
            pid_t id = fork();
            // id小于0，创建进程失败，直接返回
            if (id < 0)
            {
                LOG(ERROR, "内部错误，创建子进程失败");
                return false;
            }
            // 子进程，进行编译
            else if (id == 0)
            {
                umask(0);
                // 打开标准错误文件，将编译报错信息写入错误文件中
                int stderrfd = open(PathUtils::compilererr(fileName).c_str(), O_CREAT | O_WRONLY);
                // 打开失败，直接退出
                if (stderrfd < 0)
                {
                    LOG(ERROR, "create compilererr fail");
                    exit(1);
                }

                // 重定向到标准错误
                dup2(stderrfd, 2);

                // 添加资源约束，防止恶意程序破环操作系统

                // 程序替换，并不会影响文件描述符，最后必须使用nullptr结尾
                execlp("g++", "g++", "-o", PathUtils::exe(fileName).c_str(), PathUtils::src(fileName).c_str(),
                        "-static", "-D", "COMPILEONLINE", "-std=c++11", nullptr);
                exit(1);
            }
            else
            {
                waitpid(id, nullptr, 0);
                // 编译是否成功，如果一个可执行程序存在，说明编译成功，否则失败
                if (FileUtil::isFileExists(PathUtils::exe(fileName)))
                {
                    LOG(INFO, "文件: %s 编译成功", PathUtils::src(fileName).c_str());
                    return true;
                }
                LOG(ERROR, "编译失败，未形成可执行程序");
            }
            return false;
        }
    };

};