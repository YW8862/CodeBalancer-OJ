#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../common/log.hpp"
#include "../common/utils.hpp"

namespace ns_runner
{
    using namespace ns_utils;

    class Runner
    {
    public:
        Runner() {}
        ~Runner() {}

    public:
        
        //提供设置进程占用资源大小的接口
        static void SetProcLimit(int cpuLimit, int memLimit)
        {
            // 设置CPU时长
            struct rlimit cpuRlimit;
            cpuRlimit.rlim_max = RLIM_INFINITY;
            cpuRlimit.rlim_cur = cpuLimit;
            setrlimit(RLIMIT_CPU, &cpuRlimit);

            // 设置内存大小
            struct rlimit memRlimit;
            memRlimit.rlim_max = RLIM_INFINITY;
            memRlimit.rlim_cur = memLimit * 1024; //转化成为KB
            setrlimit(RLIMIT_AS, &memRlimit);
        }


        // 指明文件名即可，不需要代理路径，不需要带后缀
        /*******************************************
         * 返回值 > 0: 程序异常了，退出时收到了信号，返回值就是对应的信号编号
         * 返回值 == 0: 正常运行完毕的，结果保存到了对应的临时文件中
         * 返回值 < 0: 内部错误
         * 
         * cpu_limit: 该程序运行的时候，可以使用的最大cpu资源上限
         * mem_limit: 改程序运行的时候，可以使用的最大的内存大小(KB)
         * *****************************************/
        /**
         * @brief:运行可执行程序
         * @param fileName:需要运行的文件名
         * @param cpuLimit:cpu限制时间,单位:s
         * @param memLimit:虚拟内存限制大仙，单位kB
         */
        static int Run(const std::string &fileName, int cpuLimit, int memLimit)
        {
            /*********************************************
             * 程序运行：
             * 1. 代码跑完，结果正确
             * 2. 代码跑完，结果不正确
             * 3. 代码没跑完，异常了
             * 该模块只考虑程序是否正确运行完毕
             * 标准输入: 不处理
             * 标准输出: 程序运行完成，输出结果是什么
             * 标准错误: 运行时错误信息
             * *******************************************/
            std::string execute = PathUtil::exe(fileName);
            std::string _stdin   = PathUtil::stdin(fileName);
            std::string _stdout  = PathUtil::stdout(fileName);
            std::string _stderr  = PathUtil::stderr(fileName);

            umask(0);
            int stdinFD = open(_stdin.c_str(), O_CREAT|O_RDONLY, 0644);
            int stdoutFD = open(_stdout.c_str(), O_CREAT|O_WRONLY, 0644);
            int stderrFD = open(_stderr.c_str(), O_CREAT|O_WRONLY, 0644);

            if(stdinFD < 0 || stdoutFD < 0 || stderrFD < 0){
                LOG(ERROR) << "运行时打开标准文件失败" << "\n";
                return -1; //代表打开文件失败
            }            

            pid_t pid = fork();
            //子进程创建失败
            if (pid < 0)
            {
                LOG(ERROR,“运行子进程创建失败”);
                close(stdinFD);
                close(stdoutFD);
                close(stderrFD);
                return -2; 
            }
            else if (pid == 0)
            {
                dup2(stdinFD, 0);
                dup2(stdoutFD, 1);
                dup2(stderrFD, 2);

                SetProcLimit(cpuLimit, memLimit);
                
                execl(execute.c_str(), _execute.c_str(), nullptr);
                exit(1);
            }
            else
            {
                close(stdinFD);
                close(stdoutFD);
                close(stderrFD);

                //保存子进程退出信息
                int status = 0;
                waitpid(pid, &status, 0);
                
                LOG(INFO,"运行完毕, info:%d",status & 0x7F); 
                return status & 0x7F;
            }
        }
    };
}