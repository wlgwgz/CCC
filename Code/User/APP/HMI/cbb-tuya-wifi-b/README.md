# 涂鸦Wi-Fi功能模块（后板）

**涂鸦Wi-Fi功能模块（后板）** 为 **涂鸦Wi-Fi功能** 的核心逻辑部分，其主要功能包括：

+ 构建上报功能点数据

+ 解析下发功能点命令

+ 功能点的上报策略

## 目录

```
├── Tuya_B.h  // 涂鸦核心头文件
├── Tuya_B.c  // 涂鸦核心源文件
├── SLP_NT    // 暖通业务逻辑文件夹
│   ├── Tuya_B_SLP_HLxxx.h        // 暖通HL系列业务逻辑头文件
│   ├── Tuya_B_SLP_HLxxx.c        // 暖通HL系列业务逻辑源文件
│   ├── CHANGELOG.md              // 暖通业务逻辑更改记录
│   └── templates(Tuya_B_Config)  // 暖通涂鸦配置文件模板文件夹
│       └── ...                   // 暖通涂鸦配置文件模板
└── README.md  // 说明文档
```

## 使用指引

### 获取模块源码

1. 点击仓库右上方 **”克隆“** 按钮，复制 **”使用 HTTP 克隆“** 一栏中的仓库URL地址
2. 在本地项目文件夹中，在空白处单击 **鼠标右键** --> 选择 **"Git 克隆..."**，或使用命令行等方式，将模块源码拉取至本地项目文件夹中
3. 克隆完成后，请检查 `cbb-tuya-wifi-b` 文件夹是否成功拉取

### 创建配置文件

1. 打开本地项目文件夹，进入 `cbb-tuya-wifi-b` 文件夹，在 **涂鸦配置文件模板文件夹** 中，浏览&选择与最适用于当前项目的 **涂鸦配置文件模板**：
   + 涂鸦配置头文件模板：`Tuya_B_Config_XX.XXXXXX.XXX.XXX-X.h`
   + 涂鸦配置源文件模板：`Tuya_B_Config_XX.XXXXXX.XXX.XXX-X.c`
2. 将选好的 **涂鸦配置文件模板** 拷贝至 `cbb-tuya-wifi-b` 文件夹以外的相关位置，并重命名为 `Tuya_B_Config.h` 和 `Tuya_B_Config.c`
   + **注意**：请勿将 `Tuya_B_Config.h` 和 `Tuya_B_Config.c` 放入 `cbb-tuya-wifi-b` 文件夹路径下

### Keil工程配置

1. 打开Keil工程，点击 **Options for Target** --> **C/C++** --> **Include Paths** 中添加 `Tuya_B.c` 和 `Tuya_B_Config.c` 所在路径
2. 在 **Project** 一栏中添加源文件：`Tuya_B.c` 和 `Tuya_B_Config.c`

### 适配功能模块

1. `includes.h` 中添加涂鸦核心头文件 `Tuya_B.h`
2. 配置 `Tuya_B_Config.h` 文件，具体步骤请参考各业务下的相关文档
3. 配置 `Tuya_B_Config.c` 文件，具体步骤请参考各业务下的相关文档