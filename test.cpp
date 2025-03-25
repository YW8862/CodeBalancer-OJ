/*
 * @Author: YW8862 2165628227@qq.com
 * @Date: 2025-03-25 22:13:02
 * @LastEditors: YW8862 2165628227@qq.com
 * @LastEditTime: 2025-03-25 22:29:08
 * @FilePath: /yw/projects/CodeBalancer-OJ/test.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

int main()
{
    struct rlimit r;
    r.rlim_cur = 1;
    r.rlim_max = RLIM_INFINITY;

    setrlimit(RLIMIT_CPU,&r);
    while(1);
    return 0;
}