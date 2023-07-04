#ifndef ON_OFF_H
#define ON_OFF_H 1
#ifdef ON_OFF_GLOBAL
#define ON_OFF_EXT
#else 
#define ON_OFF_EXT extern
#endif


//ON_OFF_EXT void auto_run_read_from_eeprom(void);
//ON_OFF_EXT void limit_time_read_from_eeprom(void);
ON_OFF_EXT void Logic_machine_start(void);
ON_OFF_EXT void Logic_machine_stop(void);
ON_OFF_EXT void on_off_init(void);
ON_OFF_EXT void on_off_limit(void);
ON_OFF_EXT void on_off_remote(void);
ON_OFF_EXT void on_off_union_act(void);

ON_OFF_EXT void on_off_timed_disinfect(void);
ON_OFF_EXT void on_off_force_hotw_mode(void);
ON_OFF_EXT void on_off_floor_preheat(void);
ON_OFF_EXT void on_off_floor_dryup(void);
ON_OFF_EXT void on_off_holiday_away(void);
ON_OFF_EXT void on_off_holiday_home(void);

#endif
