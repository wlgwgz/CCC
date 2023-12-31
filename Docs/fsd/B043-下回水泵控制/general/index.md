<!-- 注意事项 -->
<!-- 起始分级标题：##（二级标题） -->

## 下回水泵控制

若满足以下任意条件时，**下回水泵`PUMPret`** 始终保持关闭：

- 【**下回水泵**】设为 “禁用”
- 【**热水功能**】设为 “禁用”

否则，**下回水泵`PUMPret`** 按照如下方式进行控制：

- 若【**下回水泵定时**】设为 “使用” 时，有以下逻辑：
  - 当时间到达用户设置的“下回水泵开启时间”时（如17:00），开启 **下回水泵`PUMPret`**
  - 当 **下回水泵`PUMPret`** 运行时间 ≥【**下回水泵运行**】时，关闭 **下回水泵`PUMPret`**

- 若【**下回水泵杀菌**】设为“使用”时，有以下逻辑：
  - 当满足以下全部条件时，**下回水泵`PUMPret`** 开启：
    - 进入定时杀菌功能（详见定时杀菌功能）
    - **热水水箱温度`THWt`** ≥ 【**杀菌温度**】-2℃；（【**水箱电热**】设定为 “禁用” 时，此条件为 **热水水箱温度`THWt`** ≥ 58℃）
  - 当满足以下任意条件时，****下回水泵`PUMPret`**** 关闭：
    - 退出定时杀菌功能（详见定时杀菌功能）
    - **下回水泵`PUMPret`** 已运行超过【**下回水泵运行**】+5分钟

**下回水泵`PUMPret`** 的其它动作逻辑请参见章节：

- 待机态防冻
- 运行态热水水路防冻
- 热水定时防冻
- 空气排空试运行功能
- 水泵试运行功能