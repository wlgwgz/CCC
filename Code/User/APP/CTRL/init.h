#ifndef CONFIG_H
#define CONFIG_H    1
#ifdef CONFIG_GLOBAL
#define CONFIG_EXT
#else 
#define CONFIG_EXT extern
#endif

CONFIG_EXT void config(void);
CONFIG_EXT void config_uart1(void);
#endif

