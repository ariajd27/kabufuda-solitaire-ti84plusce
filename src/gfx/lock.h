#ifndef lock_include_file
#define lock_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define lock_width 7
#define lock_height 8
#define lock_size 58
#define lock ((gfx_sprite_t*)lock_data)
extern unsigned char lock_data[58];

#ifdef __cplusplus
}
#endif

#endif
