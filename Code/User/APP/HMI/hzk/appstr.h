#ifndef _APPSTR_H_
#define _APPSTR_H_
extern U08 const str_bin[];
extern U32 const str_base[];
extern U08 const chinese_bytes[];
extern U32 get_chinese_idx_ofs(U16 i);
#define MAX_STRING_BYTES (170 + 2) // 最长字符串长度(包括结束符,冗余2字节)
extern U08 uncompress_buf[MAX_STRING_BYTES];
extern const U08 * chinese_uncompress_text(U16 idx);
#define CHINESE_END_CHAR 0xFFFF 

extern U08 const english_bytes[];
extern U32 get_english_idx_ofs(U16 i);
extern const U08 * english_uncompress_text(U16 idx);
#define ENGLISH_END_CHAR 0xFFFF 

extern U08 const spanish_bytes[];
extern U32 get_spanish_idx_ofs(U16 i);
extern const U08 * spanish_uncompress_text(U16 idx);
#define SPANISH_END_CHAR 0xFFFF 

extern U08 const greek_bytes[];
extern U32 get_greek_idx_ofs(U16 i);
extern const U08 * greek_uncompress_text(U16 idx);
#define GREEK_END_CHAR 0xFFFF 

extern U08 const french_bytes[];
extern U32 get_french_idx_ofs(U16 i);
extern const U08 * french_uncompress_text(U16 idx);
#define FRENCH_END_CHAR 0xFFFF 

extern U08 const italian_bytes[];
extern U32 get_italian_idx_ofs(U16 i);
extern const U08 * italian_uncompress_text(U16 idx);
#define ITALIAN_END_CHAR 0xFFFF 

extern U08 const polish_bytes[];
extern U32 get_polish_idx_ofs(U16 i);
extern const U08 * polish_uncompress_text(U16 idx);
#define POLISH_END_CHAR 0xFFFF 

extern U08 const hungarian_bytes[];
extern U32 get_hungarian_idx_ofs(U16 i);
extern const U08 * hungarian_uncompress_text(U16 idx);
#define HUNGARIAN_END_CHAR 0xFFFF 

extern U08 const german_bytes[];
extern U32 get_german_idx_ofs(U16 i);
extern const U08 * german_uncompress_text(U16 idx);
#define GERMAN_END_CHAR 0xFFFF 

#endif
