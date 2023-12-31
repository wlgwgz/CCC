<!-- 注意事项 -->
<!-- 起始分级标题：##（二级标题） -->

## 开机与停机

### 机组启停方式

有7种方式可以启动/停止机组：

1. **3.5线控器`UI35`** 上的ON/OFF键；
2. 线控开关：可通过设置 **【联动功能设置】**：
   1. 禁用、线控水箱电热、线控外部热源：禁用联动、线控开关机功能；
   2. 联动动作有效：（以联动开关为主）
      - 联动开关由断开变为闭合时：机组开机
      - 联动开关保持闭合时：机组不能通过线控器进行关机
      - 联动开关由闭合变为断开时：机组关机
      - 联动开关保持断开时：机组可通过线控器进行开关机操作

   3. 联动闭合有效：（联动开关闭合是开机前提条件）
      - 联动开关断开时：
        - 若线控器为开机，则机组为准备态，不开启空调泵及压机，等待联动开关闭合后再开机
        - 若线控器为关机，则机组为待机态
      - 联动开关闭合时：
        - 若线控器为开机，则机组正常启动
        - 若线控器为关机，则机组为待机态

   4. 线控开关机：
      - 线控开关机与线控器都可以独立进行开关机操作。线控开关机可通过【**线控开关类型**】设置触发方式：
        - 拨动开关：检测到 **线控开关信号`LDIrm`** 闭合时启动机组，断开时停止机组
        - 脉冲开关：检测到 **线控开关信号`LDIrm`** 由闭合->断开时有效(脉冲宽度>300ms)；如果处于停机则启动机组，如果处于运行则停止机组
3. 定时开关机：根据设定的时间开机或关机。
4. 温控器开关机，具体见温控器控制章节。
5. 手机APP掌上慧联开机或者关机。
6. 网页远程控制开机与关机。
7. BMS上位机控制开机与关机。
