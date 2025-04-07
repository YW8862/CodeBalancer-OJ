
#pragma once
// 根据题目mysql数据
// model:用来和数据进行交互，对外提供数据的接口

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <unordered_map>
#include "include/mysql.h"
#include "../common/log.hpp"
#include "../common/utils.hpp"

namespace ns_model
{
    using namespace ns_utils;

    struct Question
    {
        std::string number;      // 题目编号
        std::string title;       // 题目标题
        std::string desc;        // 题目描述
        std::string difficulity; // 题目难度
        std::string header;      // 题目预设代码
        std::string tail;        // 题目测试用例

        int cpuLimit;           // 时间限制
        int memLimit;           // 空间限制
    };

    class Model
    {
    private:
        const std::string host = "127.0.0.1";
        const std::string user = "OJClient";
        const std::string password = "123456";
        const std::string database = "oj";
        const int port = 3306;
        const std::string tableName = "oj_questions"; 
    public:
        Model()
        {
            
        }

        bool queryMysql(const std::string &sql,std::vector<Question> *result)
        {
            //创建mysql句柄
            MYSQL *mysql = mysql_init(nullptr);
            if(nullptr == mysql_real_connect(mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),port,nullptr,0))
            {
                LOG(FATAL,"数据库连接失败!");
                return false;
            }
            LOG(INFO,"链接数据库成功");
            //执行sql语句
            if(0 != mysql_query(mysql,sql.c_str()))
            {
                LOG(WARNING,"执行sql:%s失败",sql.c_str());
            }
            //提取结果
            MYSQL_RES *res = mysql_store_result(mysql);

            //分析结果
            int rows = mysql_num_rows(res);
            int cols = mysql_num_fields(res);

            for(int i = 0;i<rows;i++)
            {
                MYSQL_ROW row = mysql_fetch_row(res);
                Question question;
                question.number = row[0];
                question.title = row[1];
                question.difficulity = row[2];
                question.desc = row[3];
                question.header = row[4];
                question.tail = row[5];
                question.cpuLimit = atoi(row[6]);
                question.memLimit = atoi(row[7]);
                result->emplace_back(std::move(question));
            }
            //关闭链接
            mysql_close(mysql);
        }

        /**
         * @brief 获取加载后的所有题目
         * @param allQuestions :获取到的所有题目列表
         */
        bool getAllQuestions(std::vector<Question> *questionsList)
        {
            const std::string sql = "select * from ";
            sql += tableName;
            return queryMysql(sql,questionsList);
        }

        /**
         * @brief 获取单个题目的信息
         * @param number :需要获取的题目编号
         * @param question :获取到的题目
         */
        bool getOneQuestion(const std::string &number, Question *question)
        {
            const std::string sql = "select * from ";
            sql += tableName;
            sql = sql + "where number = " + number;
            std::vector<Question> questions;
            queryMysql(sql,&questions);
            
            //如果查询失败，直接返回false
            if(questions.empty())
            {
                return false;
            }
            *question = questions[0];
            return true;
        }

        ~Model() {}
    };
};