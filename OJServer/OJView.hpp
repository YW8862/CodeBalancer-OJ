#pragma once
#include <vector>
#include <iostream>
#include "OJModel.hpp"
#include <ctemplate/template.h>
#include "../common/utils.hpp"

namespace ns_view
{
    using namespace ns_model;
    using namespace ns_utils;

    const std::string templatePath = "./templateHtml/";
    class View
    {
    public:
        View() {}
        ~View() {}

        void allExpandHtml(const std::vector<Question> questions, std::string *html)
        {
            // 题目编号   题目标题  题目难度
            // 使用表格展示
            //1.获取原始路径
            std::string srcHtml = templatePath + "allQuestion.html";
            //形成字典，
            ctemplate::TemplateDictionary root("allQuestions");
            for(auto &question:questions)
            {
                ctemplate::TemplateDictionary *sub = root.AddSectionDictionary("questionList");
                sub->SetValue("number",question.number);
                sub->SetValue("title",question.title);
                sub->SetValue("difficulity",question.difficulity);
            }
            //3.获取被渲染后的html文件
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(srcHtml,ctemplate::DO_NOT_STRIP);

            //4.执行渲染
            tpl->Expand(html,&root);
        }

        void oneExpandHtml(const Question &question,std::string *html)
        {
            //1.形成路径
            std::string srcHtml = templatePath + "oneQuestion.html";

            //2.形成数字典
            ctemplate::TemplateDictionary root("oneQuestion");
            root.SetValue("number",question.number);
            root.SetValue("title",question.title);
            root.SetValue("difficulity",question.difficulity);
            //std::cout<<question.difficulity<<std::endl;
            root.SetValue("desc",question.desc);
            std::cout<<question.desc<<std::endl;
            root.SetValue("preCode",question.header);

            //3.获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(srcHtml,ctemplate::DO_NOT_STRIP);

            //4.执行渲染
            tpl->Expand(html,&root);
        }
    };
};