
/**FreeRTOS、CubeMX、LL库的F4模板**/
1，照着大疆的A板移植了读取MPU6500和地磁传感器IST8310，可基于此按个人想法是否继续选择滤波算法进行姿态数据优化。

2，二值信号量，任务通知（含任务通知值，推荐直接任务通知模拟二值信号量）举例。

3，基于模板和步兵程序，开始创建并实现各个任务，同时，多和机械沟通。

4，特别注意https://www.robomaster.com/zh-CN/resource/pages/announcement/1489，看看各自兵种需要实现啥。

ps:采用了LL库，如果学了HAL库也差不多的，标准库更方便转LL库。