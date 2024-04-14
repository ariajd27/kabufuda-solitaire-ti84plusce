#ifndef KBFDGFX_appvar_include_file
#define KBFDGFX_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define sizeof_global_palette 16
#define global_palette (KBFDGFX_appvar[0])
#define sprites_palette_offset 0
#define card1_width 25
#define card1_height 41
#define KBFDGFX_sprites_card1_index 1
#define card1 ((gfx_sprite_t*)KBFDGFX_appvar[1])
#define card2_width 25
#define card2_height 41
#define KBFDGFX_sprites_card2_index 2
#define card2 ((gfx_sprite_t*)KBFDGFX_appvar[2])
#define card3_width 25
#define card3_height 41
#define KBFDGFX_sprites_card3_index 3
#define card3 ((gfx_sprite_t*)KBFDGFX_appvar[3])
#define card4_width 25
#define card4_height 41
#define KBFDGFX_sprites_card4_index 4
#define card4 ((gfx_sprite_t*)KBFDGFX_appvar[4])
#define card5_width 25
#define card5_height 41
#define KBFDGFX_sprites_card5_index 5
#define card5 ((gfx_sprite_t*)KBFDGFX_appvar[5])
#define card6_width 25
#define card6_height 41
#define KBFDGFX_sprites_card6_index 6
#define card6 ((gfx_sprite_t*)KBFDGFX_appvar[6])
#define card7_width 25
#define card7_height 41
#define KBFDGFX_sprites_card7_index 7
#define card7 ((gfx_sprite_t*)KBFDGFX_appvar[7])
#define card8_width 25
#define card8_height 41
#define KBFDGFX_sprites_card8_index 8
#define card8 ((gfx_sprite_t*)KBFDGFX_appvar[8])
#define card9_width 25
#define card9_height 41
#define KBFDGFX_sprites_card9_index 9
#define card9 ((gfx_sprite_t*)KBFDGFX_appvar[9])
#define card10_width 25
#define card10_height 41
#define KBFDGFX_sprites_card10_index 10
#define card10 ((gfx_sprite_t*)KBFDGFX_appvar[10])
#define cardstack_width 25
#define cardstack_height 45
#define KBFDGFX_sprites_cardstack_index 11
#define cardstack ((gfx_sprite_t*)KBFDGFX_appvar[11])
#define freespace_width 25
#define freespace_height 41
#define KBFDGFX_sprites_freespace_index 12
#define freespace ((gfx_sprite_t*)KBFDGFX_appvar[12])
#define lock_width 7
#define lock_height 8
#define KBFDGFX_sprites_lock_index 13
#define lock ((gfx_sprite_t*)KBFDGFX_appvar[13])
#define selcorner_width 29
#define selcorner_height 45
#define KBFDGFX_sprites_selcorner_index 14
#define selcorner ((gfx_sprite_t*)KBFDGFX_appvar[14])
#define drpcorner_width 29
#define drpcorner_height 45
#define KBFDGFX_sprites_drpcorner_index 15
#define drpcorner ((gfx_sprite_t*)KBFDGFX_appvar[15])
#define KBFDGFX_entries_num 16
extern unsigned char *KBFDGFX_appvar[16];
unsigned char KBFDGFX_init(void);

#ifdef __cplusplus
}
#endif

#endif
