#ifndef ERR_MOD_H
#define ERR_MOD_H    1
#ifdef ERR_MOD_GLOBAL
#define ERR_MOD_EXT
#else 
#define ERR_MOD_EXT extern
#endif	

#define s_set_error(err_num)	SET_BIT(pCell->cell_error[(err_num) / ERR_BIT_NUM], (err_num) % ERR_BIT_NUM)
#define s_clr_error(err_num)	CLR_BIT(pCell->cell_error[(err_num) / ERR_BIT_NUM], (err_num) % ERR_BIT_NUM)
#define s_get_error(err_num)	GET_BIT(pCell->cell_error[(err_num) / ERR_BIT_NUM], (err_num) % ERR_BIT_NUM)


ERR_MOD_EXT U08 mod_err_comp[MAX_COMP];		/* 压机故障(制冷、制热) */
ERR_MOD_EXT U08 mod_err_comp_risk[MAX_COMP];/* 压机故障：风险大，直接停机，而非降频停机 */

ERR_MOD_EXT BOOL fg_out_in_diff_high;       /* 出回温差过大 */

ERR_MOD_EXT BOL fg_water_lack_err;

ERR_MOD_EXT void config(void);
ERR_MOD_EXT	void err_mod_time(void);
ERR_MOD_EXT void err_mod(void);
ERR_MOD_EXT void err_mod_init(void);
//ERR_MOD_EXT BOOL err_limit_auto_reset_deal(U08 x, U08 err_old, U08 err_new, BOOL fg_reset_mode);
ERR_MOD_EXT BOOL err_limit_auto_reset_deal(U08 x, U08 err_old, U08 err_new);

ERR_MOD_EXT void err_limit_auto_reset_init(void);
ERR_MOD_EXT void err_mod_invt_err_reset(INVT_ID_ENUM id);

#define INVT_ERR_ERROR      0				/* 故障 */
#define INVT_ERR_COMMU      1				/* 通讯故障 */
ERR_MOD_EXT U16 err_invt_id_to_err_id(INVT_ID_ENUM id, U08 type);

#endif
