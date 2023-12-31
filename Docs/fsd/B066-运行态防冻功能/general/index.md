<!-- 注意事项 -->
<!-- 起始分级标题：##(二级标题) -->

## 运行态防冻功能

- 当【**防冻功能设置**】设为 “禁用” 时，则不可执行 “**运行态防冻**” 逻辑
- 当【**防冻功能设置**】设为 “使用” 时，则允许执行 “**运行态防冻**” 逻辑

### 运行态防冻类型

“**运行态防冻**” 有 “**运行态空调水路防冻**” 、“**运行态热水水路防冻**” 两种类型：

- “**运行态空调水路防冻**”
  - 当满足以下任意条件时，则不可执行 “**运行态空调水路防冻**” 逻辑：
    - A区开启
    - B区开启
    - 【**模块个数**】= 1
    - 【**热水模块数**】≥【**模块个数**】
  - 当满足以下所有条件时，则允许执行 “**运行态空调水路防冻**” 逻辑：
    - 【**模块个数**】＞ 1
    - 【**热水模块数**】＜【**模块个数**】
    - A区关闭
    - B区关闭
    - 热水开启

- “**运行态热水水路防冻**”
  - 当满足以下任意条件时，则不可执行 “**运行态热水水路防冻**” 逻辑：
    - 热水开启
    - 【**模块个数**】= 1
    - 【**热水模块数**】≥【**模块个数**】
  - 当【**模块个数**】＞ 1，且【**热水模块数**】＜【**模块个数**】时，且满足以下任意条件时，则允许执行 “**运行态热水水路防冻**” 逻辑：
    - A区开启，B区关闭，热水关闭
    - A区关闭，B区开启，热水关闭
    - A区开启，B区开启，热水关闭
  - **注**：若进入 “**运行态热水水路防冻**” 时，0～m#机组处于制冷运行，则会先停机，然后再重新开启

### 运行态防冻温度

- “**运行态空调水路防冻**”
  - “**进运行态防冻温度**” 需从以下温度选择一个最低的温度：
    - MODMIN_NHW(**板换回水温度`TWin1`**)
    - MODMIN_NHW(**板换出水温度`TWout1`**)
    - MODMIN_NHW(**总出水温度`TWout2`**)
    - **系统总出水温度`TWout3`**
    - **缓冲水箱上部温度`TACt1`**
    - **缓冲水箱下部温度`TACt2`**
    - **地暖进水温度`TFLin`**

  - “**退运行态防冻温度** 需从以下温度选择一个最低的温度：
    - MODMIN_NHW(**板换回水温度`TWin1`**)
    - MODMIN_NHW(**板换出水温度`TWout1`**)
    - MODMIN_NHW(**总出水温度`TWout2`**)

- “**运行态热水水路防冻**”
  - “**进运行态防冻温度**” 需从以下温度选择一个最低的温度：
    - MODMIN_HW(**板换回水温度`TWin1`**)
    - MODMIN_HW(**板换出水温度`TWout1`**)
    - MODMIN_HW(**总出水温度`TWout2`**)
    - **热水水箱温度`THWt`**

  - “**退运行态防冻温度**” 需从以下温度选择一个最低的温度：
    - MODMIN_HW(**板换回水温度`TWin1`**)
    - MODMIN_HW(**板换出水温度`TWout1`**)
    - MODMIN_HW(**总出水温度`TWout2`**)

### 运行态防冻间隔

- 当 “**进运行态防冻温度**” ＜【**压机防冻温度**】时，“**运行态防冻间隔**” ＝ MIN(3分钟,【**防冻间隔2**】)(高优先级)
- 当 **环境温度`TA`** ≥【**防冻间隔环温**】时，“**运行态防冻间隔**” ＝【**防冻间隔1**】
- 当 **环境温度`TA`** <【**防冻间隔环温**】或 **环境温度`TA`** 故障时，“**运行态防冻间隔**” ＝【**防冻间隔2**】

### 进入运行态防冻

- **环境温度`TA`** ≤【**进防冻环温**】且 **变频水泵`PUMPf`** 停机时间达到 “**运行态防冻间隔**” 后，启动 **变频水泵`PUMPf`**。**变频水泵`PUMPf`** 运转180秒后检测 “**退运行态防冻温度**”，“**退运行态防冻温度**”根据4个温度区域执行不同动作，温度区域划分下图所示：

  ![运行态防冻功能-进入运行态防冻](.img/运行态防冻功能-进入运行态防冻.svg#large)

  - 其中
    - **A**：A区
    - **B**：B区
    - **C**：C区
    - **D**：D区
    - **TA**：【**水泵防冻温度**】
    - **TE**：【**电热防冻温度**】
    - **TC**：【**压机防冻温度**】

  - A、B、C、D四个温度区域描述
    - **A区**：进入 “**压机防冻**”。
      - 启动一台 **变频压机`COMPf`**，以后每8分钟若 “**退运行态防冻温度**” 的温升小于1℃，加载1台 **变频压机`COMPf`** 投入运行。(注：**变频压机`COMPf`** 开启前，应满足以下条件：**变频压机`COMPf`** 停机时间 ＞【**防频繁启动**】、**变频水泵`PUMPf`**、**变频风机`FANf`** 都已开启，并且水流已检测完毕)
      - “**压机防冻**” 最长时间为60分钟
    - **B区**：进入 “**电热防冻**”
      - **辅助电热1`IBH1`**(和 **辅助电热2`IBH2`**)保持开启
      - “**电热防冻**” 最长时间为60分钟
    - **C区**：进入 “**水泵防冻**”。
      - 启动 **变频水泵`PUMPf`** 并保持运行
      - “**水泵防冻**” 最长时间为15分钟
    - **D区**：见 “退出运行态防冻”

### 运行态防冻动作

进入运行态防冻时各水泵、电磁三通阀动作描述如下：

- “运行态空调水路防冻”
  - **变频水泵`PUMPf`**：打开
  - **外循环泵`PUMPo`**：打开
  - **混合水泵`PUMPm`**：打开
  - **下回水泵`PUMPret`**：按正常逻辑动作
  - **太阳能水泵`PUMPsl`**：按正常逻辑动作
  - **电磁三通阀1`S3V1`**：按正常逻辑动作
  - **电磁三通阀2`S3V2`**：进入防冻后，先保持OFF输出，再切换为ON输出，切换周期为90秒
  - **电磁三通阀3`S3V3`**：进入防冻后，先保持ON输出，再切换为OFF输出，切换周期为90秒（需将【地暖进水温度探头】  设为“使用”）

- “运行态热水水路防冻”
  - **变频水泵`PUMPf`**：打开
  - **外循环泵`PUMPo`**：按正常逻辑动作
  - **混合水泵`PUMPm`**：按正常逻辑动作
  - **下回水泵`PUMPret`**：打开（需将【下回水泵】和【热水功能】均设为“使用”）
  - **太阳能水泵`PUMPsl`**：打开（需将【太阳能】和【热水功能】均设为“使用”）
  - **电磁三通阀1`S3V1`**：保持OFF输出
  - **电磁三通阀2`S3V2`**：按正常逻辑动作
  - **电磁三通阀3`S3V3`**：按正常逻辑动作

### 退出运行态防冻

- 当 **环境温度`TA`** ＞【**进防冻环温**】＋【**退防冻环温差**】时，不检测 “**退运行态防冻温度**”，直接退出 “**运行态防冻**”。
- 当 **环境温度`TA`** ≤【**进防冻环温**】＋【**退防冻环温差**】或 **环境温度`TA`** 故障时，按以下规则退出 “**运行态防冻**”：
  1. 退出 “**水泵防冻**”，需满足以下所有条件：
     - 只有 **变频水泵`PUMPf`** 运行，**变频压机`COMPf`** 和 **辅助电热`IBH1`**(和 **辅助电热`IBH2`**)均未进入防冻运行
     - “**退运行态防冻温度**” ＞【**水泵防冻温度**】时，退出 “**水泵防冻**”。此时，**变频水泵`PUMPf`** 停止运行
  2. 退出 “**电热防冻**”，需满足以下条件：
     - 当 “**退运行态防冻温度**” ＞【**电热退防冻温度**】时，退出 “**电热防冻**”。此时，**辅助电热1`IBH1`**(和 **辅助电热`IBH2`**)停止运行
  3. 退出 “**压机防冻**”，需满足以下条件：
     - 当 “**退运行态防冻温度**” ＞【**压机退防冻温度**】时，退出 “**压机防冻**”。此时，**变频压机`COMPf`** 和 **辅助电热1`IBH1`**(和 **辅助电热`IBH2`**)均停止运行，**变频水泵`PUMPf`** 延时关闭。

<!-- - 注：“**运行态防冻**” 退出时，**变频水泵`PUMPf`** 在所有 **变频压机`COMPf`** / **辅助电热1`IBH1`**(和 **辅助电热`IBH2`**)停止运行60秒后停止 -->

### 异常情况防冻

- **环境温度`TA`** 故障时，无环境温度限制条件，只要 **变频水泵`PUMPf`** 停机时间 ≥ “**运行态防冻间隔**”，且满足 “**进运行态防冻温度**” ≤【**水泵防冻温度**】时，就启动 **变频水泵`PUMPf`** 运行180秒，然后根据 “**退运行态防冻温度**” 进行防冻
- “**退运行态防冻温度**” 故障，且 **环境温度`TA`** 正常时，此时根据 **环境温度`TA`** 防冻，此时只会开 **变频水泵`PUMPf`**，无 **变频压机`COMPf`** 和 **辅助电热1`IBH1`**(和 **辅助电热`IBH2`**)动作
- “**退运行态防冻温度**” 故障，且 **环境温度`TA`** 故障时，只要 **变频水泵`PUMPf`** 停机时间 ≥ “**运行态防冻间隔**”，**变频水泵`PUMPf`** 一直运行
- 当检测到水流不足时，若【**水流不足开泵**】为 “使用” 且 **环境温度`TA`** ＜ 2℃，则保持 **变频水泵`PUMPf`** 继续运行

### 防冻过程中除霜

若在进入 “**压机防冻**” 后，在 “**压机防冻**” 过程中满足除霜条件，则优先进行除霜，除霜完成后继续防冻直至退出防冻。
