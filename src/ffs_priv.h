#ifndef H_FFS_PRIV_
#define H_FFS_PRIV_

#include <inttypes.h>
#include "os/queue.h"
#include "os/os_mempool.h"
#include "ffs/ffs.h"

#define FFS_ID_NONE             0xffffffff

#define FFS_AREA_MAGIC0         0xb98a31e2
#define FFS_AREA_MAGIC1         0x7fb0428c
#define FFS_AREA_MAGIC2         0xace08253
#define FFS_AREA_MAGIC3         0xb185fc8e
#define FFS_BLOCK_MAGIC         0x53ba23b9
#define FFS_INODE_MAGIC         0x925f8bc0

#define FFS_AREA_ID_SCRATCH     0xffff
#define FFS_AREA_OFFSET_IS_SCRATCH   23

#define FFS_SHORT_FILENAME_LEN  16

#define FFS_BLOCK_SIZE          512
#define FFS_BLOCK_DATA_LEN      (FFS_BLOCK_SIZE -                   \
                                 sizeof (struct ffs_disk_block))

#define FFS_HASH_SIZE           256

#define FFS_MAX_AREAS           32 /* XXX: Temporary. */

#define FFS_INODE_F_DELETED     0x01
#define FFS_INODE_F_DUMMY       0x02
#define FFS_INODE_F_DIRECTORY   0x04
#define FFS_INODE_F_TEST        0x80

#define FFS_BLOCK_F_DELETED     0x01

#define FFS_BLOCK_MAX_DATA_SZ   2048 /* XXX: This may be calculated. */


struct ffs_disk_block {
    uint32_t fdb_magic;
    uint32_t fdb_id;
    uint32_t fdb_seq;
    uint32_t fdb_rank;
    uint32_t fdb_inode_id;
    uint16_t reserved16;
    uint16_t fdb_flags;
    uint16_t fdb_data_len;
    uint32_t fdb_ecc; /* Real size tbd. */
    /* Followed by 'length' bytes of data. */
};

struct ffs_disk_inode {
    uint32_t fdi_magic;
    uint32_t fdi_id;
    uint32_t fdi_seq;
    uint32_t fdi_parent_id;
    uint16_t fdi_flags;
    uint8_t fdi_filename_len;
    uint32_t fdi_ecc; /* Real size tbd. */
    /* Followed by filename. */
};

struct ffs_disk_area {
    uint32_t fds_magic[4];
    uint32_t fds_length;
    uint16_t reserved16;
    uint8_t fds_seq;
    uint8_t fds_is_scratch;
};

#define FFS_OBJECT_TYPE_INODE   1
#define FFS_OBJECT_TYPE_BLOCK   2

struct ffs_base {
    SLIST_ENTRY(ffs_base) fb_hash_next;
    uint32_t fb_id;
    uint32_t fb_seq;
    uint32_t fb_offset;
    uint16_t fb_area_id;
    uint8_t fb_type;
};

struct ffs_block {
    struct ffs_base fb_base;
    struct ffs_inode *fb_inode;
    SLIST_ENTRY(ffs_block) fb_next;
    uint32_t fb_rank;
    uint16_t fb_data_len;
    uint8_t fb_flags;
};

SLIST_HEAD(ffs_inode_list, ffs_inode);
struct ffs_inode {
    struct ffs_base fi_base;
    SLIST_ENTRY(ffs_inode) fi_sibling_next;
    union {
        SLIST_HEAD(, ffs_block) fi_block_list;  /* If file. */
        struct ffs_inode_list fi_child_list;  /* If directory. */
    };
    struct ffs_inode *fi_parent;
    uint32_t fi_data_len; /* If file. */
    uint8_t fi_filename_len;
    uint8_t fi_flags;
    uint8_t fi_refcnt;
    uint8_t fi_filename[FFS_SHORT_FILENAME_LEN];
};

struct ffs_file {
    struct ffs_inode *ff_inode;
    uint32_t ff_offset;
    uint8_t ff_access_flags;
};

struct ffs_area {
    uint32_t fs_offset;
    uint32_t fs_length;
    uint32_t fs_cur;
    uint8_t fs_seq;
};

struct ffs_disk_object {
    int fdo_type;
    uint16_t fdo_area_id;
    uint32_t fdo_offset;
    union {
        struct ffs_disk_inode fdo_disk_inode;
        struct ffs_disk_block fdo_disk_block;
    };
};

#define FFS_PATH_TOKEN_NONE     0
#define FFS_PATH_TOKEN_BRANCH   1
#define FFS_PATH_TOKEN_LEAF     2

struct ffs_path_parser {
    int fpp_token_type;
    const char *fpp_path;
    const char *fpp_token;
    int fpp_token_len;
    int fpp_off;
};

extern struct os_mempool ffs_file_pool;
extern struct os_mempool ffs_inode_pool;
extern struct os_mempool ffs_block_pool;
extern uint32_t ffs_next_id;
extern struct ffs_area *ffs_areas;
extern int ffs_num_areas;
extern uint16_t ffs_scratch_area_id;

SLIST_HEAD(ffs_base_list, ffs_base);
extern struct ffs_base_list ffs_hash[FFS_HASH_SIZE];
extern struct ffs_inode *ffs_root_dir;

struct ffs_area *ffs_flash_find_area(uint16_t logical_id);
int ffs_flash_read(uint16_t area_id, uint32_t offset,
                   void *data, uint32_t len);
int ffs_flash_write(uint16_t area_id, uint32_t offset,
                    const void *data, uint32_t len);
int ffs_flash_copy(uint16_t area_id_from, uint32_t offset_from,
                   uint16_t area_id_to, uint32_t offset_to,
                   uint32_t len);

struct ffs_base *ffs_hash_find(uint32_t id);
int ffs_hash_find_inode(struct ffs_inode **out_inode, uint32_t id);
int ffs_hash_find_block(struct ffs_block **out_block, uint32_t id);
void ffs_hash_insert(struct ffs_base *base);
void ffs_hash_remove(struct ffs_base *base);
void ffs_hash_init(void);

int ffs_path_parse_next(struct ffs_path_parser *parser);
void ffs_path_parser_new(struct ffs_path_parser *parser, const char *path);
int ffs_path_find(struct ffs_path_parser *parser,
                  struct ffs_inode **out_inode,
                  struct ffs_inode **out_parent);
int ffs_path_find_inode(struct ffs_inode **out_inode, const char *filename);
int ffs_path_unlink(const char *filename);
int ffs_path_rename(const char *from, const char *to);
int ffs_path_new_dir(const char *path);

int ffs_restore_full(const struct ffs_area_desc *area_descs);

struct ffs_inode *ffs_inode_alloc(void);
void ffs_inode_free(struct ffs_inode *inode);
uint32_t ffs_inode_calc_data_length(const struct ffs_inode *inode);
uint32_t ffs_inode_parent_id(const struct ffs_inode *inode);
void ffs_inode_delete_from_ram(struct ffs_inode *inode);
int ffs_inode_delete_from_disk(const struct ffs_inode *inode);
int ffs_inode_from_disk(struct ffs_inode *out_inode,
                        const struct ffs_disk_inode *disk_inode,
                        uint16_t area_id, uint32_t offset);
int ffs_inode_rename(struct ffs_inode *inode, const char *filename);
void ffs_inode_insert_block(struct ffs_inode *inode, struct ffs_block *block);
int ffs_inode_read_disk(struct ffs_disk_inode *out_disk_inode,
                        char *out_filename, uint16_t area_id,
                        uint32_t offset);
int ffs_inode_write_disk(const struct ffs_disk_inode *disk_inode,
                         const char *filename, uint16_t area_id,
                         uint32_t offset);
int ffs_inode_seek(const struct ffs_inode *inode, uint32_t offset,
                   struct ffs_block **out_prev_block,
                   struct ffs_block **out_block, uint32_t *out_block_off);
void ffs_inode_dec_refcnt(struct ffs_inode *inode);
int ffs_inode_add_child(struct ffs_inode *parent, struct ffs_inode *child);
void ffs_inode_remove_child(struct ffs_inode *child);
void ffs_inode_delete_from_ram(struct ffs_inode *inode);
int ffs_inode_is_root(const struct ffs_disk_inode *disk_inode);
int ffs_inode_filename_cmp_ram(int *result, const struct ffs_inode *inode,
                               const char *name, int name_len);
int ffs_inode_filename_cmp_flash(int *result, const struct ffs_inode *inode1,
                                 const struct ffs_inode *inode2);
int ffs_inode_read(const struct ffs_inode *inode, uint32_t offset,
                   void *data, uint32_t *len);

struct ffs_block *ffs_block_alloc(void);
void ffs_block_free(struct ffs_block *block);
uint32_t ffs_block_disk_size(const struct ffs_block *block);
int ffs_block_read_disk(struct ffs_disk_block *out_disk_block,
                        uint16_t area_id, uint32_t offset);
int ffs_block_write_disk(uint16_t *out_area_id, uint32_t *out_offset,
                         const struct ffs_disk_block *disk_block,
                         const void *data);
void ffs_block_delete_from_ram(struct ffs_block *block);
void ffs_block_delete_list_from_ram(struct ffs_block *first,
                                    struct ffs_block *last);
int ffs_block_delete_from_disk(const struct ffs_block *block);
void ffs_block_delete_list_from_disk(const struct ffs_block *first,
                                     const struct ffs_block *last);
void ffs_block_from_disk(struct ffs_block *out_block,
                         const struct ffs_disk_block *disk_block,
                         uint16_t area_id, uint32_t offset);

int ffs_misc_reserve_space(uint16_t *out_area_id, uint32_t *out_offset,
                           uint16_t size);

int ffs_file_open(struct ffs_file **out_file, const char *filename,
                  uint8_t access_flags);
int ffs_file_seek(struct ffs_file *file, uint32_t offset);
int ffs_file_close(struct ffs_file *file);
int ffs_file_new(struct ffs_inode **out_inode, struct ffs_inode *parent,
                 const char *filename, uint8_t filename_len, int is_dir);
void ffs_format_ram(void);

int ffs_format_area(uint16_t area_id, int is_scratch);
int ffs_format_from_scratch_area(uint16_t area_id);
int ffs_format_full(const struct ffs_area_desc *area_descs);

int ffs_gc(uint16_t *out_area_id);
int ffs_gc_until(uint16_t *out_area_id, uint32_t space);

int ffs_area_desc_validate(const struct ffs_area_desc *area_desc);
void ffs_area_set_magic(struct ffs_disk_area *disk_area);
int ffs_area_magic_is_set(const struct ffs_disk_area *disk_area);
int ffs_area_is_scratch(const struct ffs_disk_area *disk_area);
void ffs_area_to_disk(struct ffs_disk_area *out_disk_area,
                        const struct ffs_area *area);
uint32_t ffs_area_free_space(const struct ffs_area *area);

int ffs_misc_validate_root(void);
int ffs_misc_validate_scratch(void);

int ffs_write_to_file(struct ffs_file *file, const void *data, int len);

#define FFS_HASH_FOREACH(base, i)                                       \
    for ((i) = 0; (i) < FFS_HASH_SIZE; (i)++)                           \
        SLIST_FOREACH((base), &ffs_hash[i], fb_hash_next)

#endif

