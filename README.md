CERL2.0 - Erlang Model for C++
==============
Please add english language also as this is very dificult to understand for a majority of people as we dont know Chinese language

# 重要声明

> 开放 CERL2.0 仅仅基于供学术研究的目的。不建议你基于 CERL 开发任何商业代码。目前来说，没有任何人来推进 CERL 的后续改进计划。也没有人为如何使用 CERL 提供基本的、必要的支持。

# 背景

## CERL 既然是2009年的产物，为何迟迟没有开源？

实际上，在 2011 年初我们很严肃地讨论过是否将 CERL 开源的事情。我至今仍然清楚地记得李道兵当时的建议：应该尽快开源 CERL，如果有一天 Go 流行起来了，开源 CERL 就完全没有意义了。我当时也口头和大年（盛大创新院院长陈大年）沟通了把 CERL 库开源的想法，大年表示支持，并说让我发信给他提交正式的申请。不过，我反复考虑了下我在这个事情上希望达到的目标是什么。结果我发现我并没有找到支持我去将 CERL 开源的动力。那时我虽然还没有决定将开发的工具链切换到 Go，但是已经意识到 C++ 作为服务端开发的成本是非常昂贵的。CERL 这个网络库一定程度上达到了它预期的目标 —— 给 C++ 服务端开发降低负担，但是 C++ 本身的包袱太重，以至于我并不认为我已经找到了一种服务器端编程的最佳实践。

所以，支持我不开源 CERL 的原因非常简单：我不想开放一个连我自己接下来都不打算去使用的东西给社区。因为我认为那是极不负责任的做法。

## 为何今天我重新考虑将 CERL 发布出来？

在 ECUG Con 2014 大会，我重新回顾了我整个服务端最佳实践的历程，这一次我没有像 2010 年杭州的 ECUG Con 大会那样含糊其词，而是非常鲜明地探讨了 Erlang 编程范式的自相矛盾之处 ( http://blog.qiniu.com/archives/1059 )。尽管我自认为这只是技术探讨，并非是对 Erlang 语言进行恶意攻击（我至今仍然极度推崇  http://open.qiniudn.com/[Joe-Armstrong]Making-reliable-distributed-systems-in-the-presence-of-software-errors.pdf  ，认为那是对我个人了解分布式编程理论的启蒙之作），这个演讲确实让 Erlang 社区同学们大为受伤。这其中又以某位号称 Erlang 大拿的专家为最：

![image](http://open.qiniudn.com/cerl/yufeng.png)

我最初知道这件事情是李道兵给我发了一个信息，说知乎上有人对我那篇演讲开骂，我回复说：你很难讨好所有人，所以这事不必理会。但真到我看到骂的内容时，还是让我大为震怒。上面贴的是经过此人多次修饰后的内容，借自嘲自己写写了不少狗屎代码来试图让内容显得理性一些，最初的帖子则纯属谩骂。

让我震怒的原因有两点，一个这样的谩骂来自于曾经以为的朋友。一个是说 CERL 是狗屎，骂的并不只是我自己，而是整个盛大云存储团队。因为我个人原因，导致整个团队被骂，这一点让我非常难堪。

我重新考虑关于 CERL 开源的决定。就我个人而言，我自认为 CERL 代表了我自己在 C++ 领域的巅峰。也是我告别 C++ 的作品。但是整个 CERL 库的开发小组而言，这也许只是他们的某个里程碑。我因为个人非常自私不想承担责任的原因，将 CERL 库雪藏，也许并不见得是非常正确的决定。

于是我重新微信联系到了大年，探讨 CERL 库开源的事项。大年说，没问题，如果你需要，你可以找我签个合同。我说年总一诺千金，这事只是个小事，CERL 库现在事实上也处于没有客户的状态（何刚接管盛大云存储后，用 Java 重写了整个存储核心），没必要大费周章。

于是，CERL 库今天终于重见天日。

为了保留尽可能多的研究价值，我特意连里面的 meeting minutes（会议纪要）也没有删除，方便大家从中看到 CERL 的演化历史。

最后，我也凭借记忆整理了 CERL 库的贡献者名单：

* 许式伟
* 于曦鹤
* 莫华枫
* 马飞涛
* 宋海涛
* 段雪涛
* 李杰
* 王欢
* 金克
* 李道兵
* 郭理靖

# 关于 CERL 库的性能

没有最新的对比数据，2009 年由莫华枫提供的测试数据大致是：

* 在没有设定超时的情况下，CERL 的 performance 大体和 boost asio 相同。
* 在支持 io timeout 的情况下，CERL 的 performance 远好于 boost asio。究其原因是因为 CERL 的定时器做了较大程度的优化，而 boost asio 的定时器写得只是中规中矩。

# CERL 库的基本结构

整个 REPO 包含 5 个子项目，从依赖关系来说，依次是：

* memory: Boost Memory 库。这个库因为曾经提交到 Boost Sandbox 中，所以放在 boost 目录下。如果我还继续用 C++，也许会尝试让它进 Boost 库。这个库早就开源，最初放在 google code 上，其最新地址在 https://github.com/xushiwei/memory 。
* stdext: C++ 扩展库。内容比较杂，整体可以认为是我在金山时期积累的 KFC 库的重新梳理版。这个库早就开源，最初放在 google code 上，其最新地址在 https://github.com/xushiwei/stdext 。
* tpl: 文本处理库。这个是因为 CERL 里面定义了一门叫 SDL（Server Description Language），需要实现一个编译器而依赖。这个库早就开源，最初放在 google code 上，其最新地址在 https://github.com/xushiwei/tpl 。
* async: CERL 库的核心。虽然名字叫 async，但是它的编程模型是基于 fiber（纤程）写同步的程序。整个编程理念反对任何地方出现异步代码。这个库经历了多次演化，从最初的纯 Erlang Model，到试图引入预编译器将同步代码翻译成异步代码，到最后决定基于 fiber（纤程）直接写同步代码（详细见 [meeting minutes](https://github.com/qiniu/cerl/blob/master/async/docs/meeting_minutes/%5B2009-10-16%5D%5Bcerl2-model%5D/2009_Oct-16.txt)），从而最终确定了整个编程哲学。
* venus: 基于 async 库写的一个 RPC 框架。推荐写服务器的标准方式是先写一个 SDL，然后基于这个 SDL 用 venusc 编译出 Server 端框架和 Client 端的 SDK。这样实现者只需要在 Server 端框架里面填写业务代码，Client 端直接基于生成的 SDK 去调用服务器的功能。关于 SDL 语言的细节，我曾经在 CSDN 博客做过详细介绍：http://blog.csdn.net/xushiweizh/article/details/4444942

更多细节，后面得空再补。也欢迎小伙伴们提 pull request 进行完善。

最后再次强调：

* 非常不推荐基于 CERL 库来写商业代码。但是学习 CERL 库（特别是 async 这个子库）也许非常有助于你理解 Golang 语言的背后机理。而了解 CERL 的整个历程，也可能有助于你理解为何七牛选择了 Golang。
