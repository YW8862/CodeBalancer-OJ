/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-24 14:31:39
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-26 21:48:27
 * @FilePath: /yw/projects/onlineJudge/common/utils.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <iostream>
#include <fstream>
#include <atomic>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>

namespace ns_utils
{
    const std::string tempPath = "./temp/";
    class PathUtils
    {
    public:
        /**
         * @brief:拼接文件路径
         * @param fileName:需要拼接的文件名，不包括路径和后缀
         * @param suffix:文件后缀
         */
        static std::string addSuffix(const std::string &fileName, const std::string &suffix)
        {
            std::string stringPath = tempPath + fileName + suffix;
            return stringPath;
        }
        /**
         * @param fileName:文件名
         */

        //编译时需要的临时文件.cpp   .exe    .compilererr
        static std::string src(const std::string &fileName)
        {
            return addSuffix(fileName, ".cpp");
        }

        static std::string exe(const std::string &fileName)
        {
            return addSuffix(fileName, ".exe");
        }

        static std::string compilererr(const std::string &fileName)
        {
            return addSuffix(fileName,".compilererr");
        }

        //运行时需要的临时文件.stdin   .sstdout  .stderr
        static std::string stdin(const std::string &fileName)
        {
            return addSuffix(fileName, ".stdin");
        }

        static std::string stdout(const std::string &fileName)
        {
            return addSuffix(fileName, ".stdout");
        }

        static std::string stderr(const std::string &fileName)
        {
            return addSuffix(fileName, ".stderr");
        }
    };

    class TimeUtils
    {
    public:
        static std::string getTimeStamp()
        {
            struct timeval time;
            gettimeofday(&time, nullptr);
            return std::to_string(time.tv_sec);
        }

        static std::string getTimems()
        {
            //获得毫秒级时间戳
            struct timeval time;
            gettimeofday(&time,nullptr);
            return std::to_string(time.tv_sec*1000+time.tv_usec/1000);
        }
    };

    class FileUtil
    {
    public:
        static bool isFileExists(const std::string &fileName)
        {
            struct stat statbuf;
            return stat(fileName.c_str(), &statbuf) == 0;
            // return access(fileName.c_str(),F_OK) == 0;
        }

        //获得唯一文件名
        static std::string UniqFileName(const std::string &code)
        {
            static std::atomic_uint id(0);
            //毫秒级时间戳+原子级自增计数器
            std::string ms = TimeUtils::getTimems();
            //获得原子级计数器
            std::string uniqId = std::to_string(id);
            return ms+uniqId;
        }

        /**
         * @brief 将代码写入到临时源文件当中
         * @param fileName 需要写入的目标文件名
         * @param code 需要写的代码
         */
        static bool writeFile(const std::string &fileName,const std::string &code)
        {
            std::ofstream out(fileName);
            if(!out.is_open())
            {
                //写入失败
                return false;
            }

            out.write(code.c_str(),code.size());
            //out<<code;
            out.close();
            return true;
        }

        /**
         * @brief:读取文件内容
         * @param fileName:待读取文件名称
         * @param outString:输出字符串
         * @param keep:是否保留换行符标识
         * @returns 读取成功，返回true,否则返回false
         */
        static bool readFile(const std::string &fileName,std::string *outString,bool keep = false)
        {
            std::ifstream in(fileName,std::ios::binary);
            if(!in.is_open())
            {
                //读取失败
                return false;
            }
            std::string line;
            while(std::getline(in,line))
            {
                *outString += line;
                *outString += (keep?"\n":"");
            }
            in.close();
            return true;
        }
    };
};