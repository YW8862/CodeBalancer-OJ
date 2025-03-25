核心功能分三个模块
    一、common：  公共模块

`log.hpp`：打印日志模块

`utils`：工具模块，实现文件路径的拼接、判断文件状态


    二、.compile： 编译模块
   ``./compiler``实现编译功能

创建子进程完成编译功能，并且将错误信息输出到.compilererr文件中

```bash
./compile
```

  `./runner`实现运行功能


    3.OJServer：oj模块
