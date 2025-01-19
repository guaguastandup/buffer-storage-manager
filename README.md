# 《高级数据库系统》 课程实验

主讲教师：金培权老师 (highly recommend)

## 实验提交内容：

(1） 文档：提交一个说明文档，陈述你的设计和实现结果(包括设计结果、实现结果、运行结果等）。格式要求：word或pdf文档。

(2） 源码以及运行说明。

(3）提交截止时间：1月11日，11:59 PM （北京时间）

Lab1：根据下面的设计要求，实现一个Storage and Buffer Manager。

设计具体要求：adbs-lab.pdf

1. 按文档要求实现一个Storage and Butfer Manager， 要求至少实现LRU算法。
2. 底层文件默认采用目录式堆文件。
3. 建议先构建一个5万个page （页号从O到49999）的堆文件（使用自己实现的FixNewPage()接口），然后再运行trace文件：data-5W-50w-zipt.txt（包含了50万次满足Zipfan分布-0.8的页面请求，即80%的请求集中在前20%的页号上），根据设计文档要求统计磁盛10、Buffer命中率、运行时间等数据。
4. 下面的实验为选做内容，不做强制要求。如果完成了选做实验（一个或者多个），最后实验成绩会根据完成的情况适当加分：

（1) 实现CLOCK、LRU-2、LIRS等其它缓存置换算法（至少一种），并与LRU算法的性能进行对比；

（2） 加入缓存请求时的并发控制，通过内存锁(latch)解决缓存访问时的冲突；要求通过多线程方式执行trace并给出测试结果．


## run command
g++ test_main.cpp storage_manager.cpp buffer_manager.cpp  lru_replacer.cpp -o a && ./a

