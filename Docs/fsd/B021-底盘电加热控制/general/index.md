<!-- 注意事项 -->
<!-- 起始分级标题：##（二级标题） -->

## 底盘电加热控制

- 当【**底盘电热模式**】设为 “除霜底盘热” 时：
  - 除霜时（非能力测试）：
    - 当 **环境温度`TA`** ≤【**底盘电热环温**】，开启 **底盘电加热`DPH`**
    - 当 **环境温度`TA`** ＞【**底盘电热环温**】+2，关闭 **底盘电加热`DPH`**
  - 退除霜时：等待360秒后，**底盘电加热`DPH`** 关闭
- 当【**底盘电热模式**】设为 “低环底盘热” 时：
  - **变频压机`COMPf`** 开启时（非能力测试）：
    - 当 **环境温度`TA`** ＜【**底盘电热环温**】时，开启 **底盘电加热`DPH`**
    - 当 **环境温度`TA`** ＞【**底盘电热环温**】+2时，关闭 **底盘电加热`DPH`**
  - **变频压机`COMPf`** 关闭时：等待360秒后，关闭 **底盘电加热`DPH`**
