# IO扩展功能模块

> 基于Modbus的IO扩展KLV协议，自定义功能码0x48

+ **IO扩展功能模块** 是基于《**IO扩展板KLV通信协议**》实现的、用于 **控制器** IO资源扩展的交互框架

+ 本模块定义了 **KLV数据类型**（`EXIO_KLV_DATA`），其数据结构如下：

  | 字段            | 类型 | 标识符     | 字节数 |
  | --------------- | ---- | ---------- | ------ |
  | **KLV通信阶段** | U08  | stage      | 1      |
  | **数据缓存**    | U08  | data_buf[] | 242    |

+ 本模块支持多行业下不同业务逻辑的自定义（即 **KLV通信阶段** & **数据缓存** 的自定义）

## 空间占用

  |                      | Code + RO Data | RW Data + ZI Data |
  | -------------------- | -------------- | ----------------- |
  | 暖通多联供业务 | ≈ 7.17kB       | ≈ 0.54kB          |

注：Keil优化等级为“default”

## 仓库目录

```
├── exio_klv.h  // IO扩展功能入口头文件
├── exio_klv.c  // IO扩展功能入口源文件
├── core        // KLV核心文件夹
│   └── 0       // KLV核心0文件夹
│       ├── exio_klv_core_0.h     // KLV核心0头文件
│       └── exio_klv_core_0.c     // KLV核心0源文件
├── slp         // KLV业务文件夹
│   └── nt_dlg  // 暖通多联供业务文件夹
│       ├── exio_klv_nt_dlg.h     // 暖通多联供业务入口头文件 
│       ├── exio_klv_nt_dlg.c     // 暖通多联供业务入口源文件
│       └── 0   // 暖通多联供业务0
│           ├── exio_klv_nt_dlg_slp_0.h  // 暖通多联供业务0头文件
│           └── exio_klv_nt_dlg_slp_0.c  // 暖通多联供业务0源文件
├── template    // 配置模板文件夹
└── README.md   // 模块说明文档
```

## 使用指引

### 步骤1：使用前事项

若满足但不局限于以下情况时，请 **新建议题** 反馈：

1. 无有现成可用的业务逻辑
2. 需要引用与当前行业不同的业务逻辑
3. 优化&改进建议

### 步骤2：获取模块源码

+ **在普通文件夹中添加**
  1. 点击仓库右上方 **”克隆“** 按钮，复制 **“使用 SSH 克隆”** / **“使用 HTTP 克隆”** 一栏中的仓库URL地址
  2. 在本地项目文件夹中，在空白处单击 **鼠标右键** --> 选择 **“Git 克隆...”**，或使用命令行等方式，将模块源码拉取至本地项目文件夹中
  3. 克隆完成后，请检查 `cbb-exio-klv` 文件夹是否成功拉取
+ **在Git仓库中添加子模块**
  1. 点击仓库右上方 **“克隆”** 按钮，复制 **“使用 SSH 克隆”** / **“使用 HTTP 克隆”** 一栏中的仓库URL地址
  2. 在Git仓库中，在空白处单击 **鼠标右键** -->找到选项 **“添加子模块...”**，或使用命令行等方式，将子模块添加至项目中
  3. 克隆完成后，请检查 `cbb-exio-klv` 文件夹是否成功拉取

### 步骤3：创建配置文件

1. 打开本地项目文件夹，进入 `cbb-exio-klv/template/` 目录，浏览&选择与最适用于当前项目的 **IO扩展功能配置模板文件**：
   + `exio_klv_config_template_x_x.h`
   + `exio_klv_config_template_x_x.c`
2. 将选好的 **IO扩展功能配置文件模板** 拷贝至 `cbb-exio-klv` 文件夹以外的相关位置（如`cbb_config` 文件夹中），并重命名为 `exio_klv_config.h` 和 `exio_klv_config.c`
   + **注意**：请勿将 `exio_klv_config.h` 和 `exio_klv_config.c` 放入 `cbb-exio-klv` 文件夹路径下，防止影响本模块后续正常更新

### 步骤4：源码调用模块

1. `includes.h` 中添加 **IO扩展功能核心头文件**：`exio_klv.h`
2. 配置 `exio_klv_config.h` 文件，具体步骤请参考各业务下的相关指引
3. 配置 `exio_klv_config.c` 文件，具体步骤请参考各业务下的相关指引
4. 本模块可大致分为 **入口**、**核心** 和 **业务** 三大部分：
    + **模块入口**：`exio_klv.h`、`exio_klv.c`
    + **核心**：`core/x/exio_klv_core_x.h`、`core/x/exio_klv_core_x.c`
    + **业务**：
      + `slp/yy/exio_klv_yy.h`、`slp/yy/exio_klv_yy.c`
      + `slp/yy/x/exio_klv_yy_slp_x.h`、`slp/yy/x/exio_klv_yy_slp_x.c`

### 步骤5：Keil工程配置

1. 打开Keil工程，点击 **Options for Target** --> **C/C++** --> **Include Paths** 中添加 `exio_klv.c` 和 `exio_klv_config.c` 所在路径
2. 在 **Project** 一栏中添加源文件：`exio_klv.c` 和 `exio_klv_config.c`
3. 全编译Keil工程，消除编译错误
