<!-- 注意事项 -->
<!-- 起始分级标题：##（二级标题） -->

## 外部热源控制

若满足以下任意条件时，**外部热源`AHS`**始终保持关闭：

- 【**系统总出水温度探头**】设为 “禁用”
- 【**外部热源**】均为 “禁用”；
- 机组制热运行，且【**外部热源**】设为 “仅热水”
- 机组制热水运行，且【**外部热源**】设为 “仅制热”
- 【**模块个数**】= 1 且 机组制冷运行
- **系统总出水温度`TWout3`** 故障
- **系统总出水温度`TWout3`** ≥ 70℃
- **水箱电热`TBH`** 开启
- 全部可制热水模块水流不足故障
- 进入能力测试
- 进入地暖预热功能
- 进入地暖烘干功能
- 全部可制热水模块 **变频水泵`PUMPf`** 未运行

定义“外部热源控制温度”：

- “能调控制对象” 为出水时为 **系统总出水温度`TWout3`**
- “能调控制对象” 为回水时按以下条件选择：
  - **缓冲水箱下部温度`TACt2`** 均可用时：AVG(**缓冲水箱上部温度`TACt1`**, **缓冲水箱下部温度`TACt2`**)
  - 仅 **缓冲水箱上部温度`TACt1`** 可用时：**缓冲水箱上部温度`TACt1`**
  - 仅 **缓冲水箱下部温度`TACt2`** 可用时：**缓冲水箱下部温度`TACt2`**

### 机组制热运行时控制

若可制热水模块机组制热运行，有以下控制逻辑：

- 若存在可制热水模块的压机处于 “压机正常状态” 时，有以下控制逻辑：
  - 若满足以下所有条件时，开启 **外部热源`AHS`**：
    - **环境温度`TA`** ＜【**外部热源开环温**】- 2℃
    - “外部热源控制温度” ≤ 调频目标温度 -【**外部热源开温差**】
    - **外部热源`AHS`** 已开启，或 **外部热源`AHS`** 关闭时间 ≥【**外部热源开启延**】
  - 若满足以下任意条件时，关闭 **外部热源`AHS`**：
    - **环境温度`TA`** ≥【**外部热源开环温**】
    - “外部热源控制温度” ≥ 调频目标温度 +【**外部热源关温差**】
- 若全部可制热水模块的压机处于 “压机异常状态” 时，有以下控制逻辑：
  - 若满足以下所有条件时，开启**外部热源`AHS`**：
    - “外部热源控制温度” ≤ 调频目标温度 – 3℃
    - **外部热源`AHS`** 已开启，或 **外部热源`AHS`** 关闭时间 ≥ 5分钟
  - 关闭 **外部热源`AHS`**：“外部热源控制温度” ≥ MIN(调频目标温度 + 能调卸载偏差, 70℃)

### 机组制热水运行时控制

若可制热水模块机组制热水运行，有以下控制逻辑：

- 若存在可制热水模块的压机处于 “压机正常状态” 时，有以下控制逻辑：
  - 若满足以下所有条件时，开启 **外部热源`AHS`**：
    - **热水水箱温度`THWt`** ＜ 40℃
    - **系统总出水温度`TWout3`** ≤ 70℃ -【**外部热源开温差**】
    - **外部热源`AHS`** 已开启，或 **外部热源`AHS`** 关闭时间 ≥【**外部热源开启延**】
  - 若满足以下任意条件时，关闭 **外部热源`AHS`**：
    - **热水水箱温度`THWt`** >【**热水设定温度**】
    - **系统总出水温度`TWout3`** ≥ 70℃
- 若全部可制热水模块的压机处于 “压机异常状态” 时，有以下控制逻辑：
  - 若满足以下所有条件时，开启 **外部热源`AHS`**：
    - **热水水箱温度`THWt`** ＜【**热水设定温度**】-【**热水回差**】
    - **系统总出水温度`TWout3`** ≤ 70℃ -【**外部热源开温差**】
    - **外部热源`AHS`** 已开启，或 **外部热源`AHS`** 关闭时间 ≥ 5分钟
  - 若满足以下任意条件时，关闭 **外部热源`AHS`**：
    - **热水水箱温度`THWt`** >【**热水设定温度**】
    - **系统总出水温度`TWout3`** ≥ 70℃

### 除霜时控制

- 若满足以下所有条件时，开启 **外部热源`AHS`**：
  - 机组正在除霜
  - **系统总出水温度`TWout3`** ≤ 16℃
- 若满足以下任意条件时，关闭 **外部热源`AHS`**：
  - 机组退出除霜
  - **系统总出水温度`TWout3`** ≥ 20℃

### 强开热水模式时控制

- 当满足以下所有条件时，开启 **外部热源`AHS`**：
  - 进入强开热水模式功能
  - **热水水箱温度`THWt`** ＜【**热水设定温度**】
- 当满足以下任意条件时，关闭 **外部热源`AHS`**：
  - **热水水箱温度`THWt`** ≥【**热水设定温度**】
  - 退出强开热水模式功能

### 强开外部热源

- 机组制热运行时：
  - 当满足以下所有条件时，开启 **外部热源`AHS`**：
    - 接收到强开外部热源命令
    - “外部热源控制温度” ＜ 调频目标温度 - 2℃
  - 当满足以下任意条件时，关闭**外部热源`AHS`**：
    - 接收到关机命令
    - “外部热源控制温度” ≥ 调频目标温度 + 能调卸载偏差

- 机组制热水运行时：
  - 当满足以下所有条件时，开启 **外部热源`AHS`**：
    - 接收到强开外部热源命令
    - **热水水箱温度`THWt`** ＜【**热水设定温度**】 - 1℃
  - 当满足以下任意条件时，关闭**外部热源`AHS`**：
    - 接收到关机命令
    - **热水水箱温度`THWt`** ≥【**热水设定温度**】

### 定时杀菌时控制

![外部热源控制-定时杀菌时控制](.img/外部热源控制-定时杀菌时控制.svg#large)

- 其中
  - **THWdi_stop**: MIN(70℃,【**杀菌温度**】+1℃)
  - **THWdi_start**:【**杀菌温度**】-2℃
  - **THWt_stop**：【**热水设定温度**】
  - **THWt_start**：【**热水设定温度**】-【**热水回差**】
  - **t_HWdi_high**：【**杀菌高温时间**】
  - **t_HWdi_max**：【**杀菌最大周期**】
  - **S1**：杀菌阶段1
  - **S2**：杀菌阶段2
  - **SK**：落入该区域前的杀菌阶段（**S1** 或 **S2**）

- 处于杀菌阶段1：机组制热水运行时，**外部热源`AHS`** 动作逻辑请参见 “机组制热水运行时控制”
- 处于杀菌阶段2：辅助热源杀菌时，**外部热源`AHS`** 动作逻辑如下：
  - 当 0 ＜ 杀菌时间 ＜【**杀菌最大周期**】，且 高温杀菌时间 ＜【**杀菌高温时间**】时，有以下控制逻辑：
    - 当满足以下所有条件时，开启 **外部热源`AHS`**：
      - 机组无制热水需求，**变频压机`COMPf`** 到温停机
      - **水箱电热`TBH`**、**辅助电热1`IBH1`**、**辅助电热2`IBH2`** 均未开启
      - **热水水箱温度`THWt`** ≤【**杀菌温度**】-2℃
      - **系统总出水温度`TWout3`** ≤ 70℃ -【**外部热源开温差**】
    - 当满足以下任意条件时，关闭 **外部热源`AHS`**：
      - **热水水箱温度`THWt`** ≥ MIN(70℃,【**杀菌温度**】+1℃)
      - **系统总出水温度`TWout3`** ≥ 70℃
  - 当 杀菌时间 ≥ 【**杀菌最大周期**】，或 高温杀菌时间 ≥【**杀菌高温时间**】时，关闭 **外部热源`AHS`**，并退出定时杀菌功能

### 线控外部热源控制

- 机组制热运行时：
  - 当满足以下所有条件时，开启 **外部电热`AHS`**：
    - 【**联动功能设置**】设为 “线控外部热源”
    - 检测到 **线控开关信号`LDIrm`** ，且持续3秒
    - **系统总出水温度`TWout3`** ＜ 调频目标温度 - 2℃
  - 当满足以下任意条件时，关闭 **外部热源`AHS`**：
    - 【**联动功能设置**】设为非 “线控外部热源”
    - 未检测到**线控开关信号`LDIrm`**
    - **系统总出水温度`TWout3`** ≥ 调频目标温度 + 能调卸载偏差

- 机组制热水运行时：
  - 当满足以下所有条件时，开启 **外部电热`AHS`**：
    - 【**联动功能设置**】设为 “线控外部热源”
    - 检测到**线控开关信号`LDIrm`** ，且持续3秒
    - **热水水箱温度`THWt`** ＜【**热水设定温度**】 - 1℃
  - 当满足以下任意条件时，关闭 **外部热源`AHS`**：
    - 【**联动功能设置**】设为非 “线控外部热源”
    - 未检测到 **线控开关信号`LDIrm`**
    - **热水水箱温度`THWt`** ≥【**热水设定温度**】
