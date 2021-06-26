#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <mcheck.h>
#include <errno.h>
#define block_size 1024

typedef struct superblock {
	unsigned long int key;
	char datablocks[block_size*100];		//numero total de bloques
	char data_bitmap[105];      			//array de numero de bloques disponibles
	char inode_bitmap[105];   				//array de numero de inodos disponibles
} superblock;


typedef struct filetype {
	int valid;
	char path[100];
	char name[100];
	struct filetype ** children;
	int num_children;
	int num_links;
	struct filetype * parent;
	char type[20];                  //==file extension
	mode_t permissions;		        // Permisos
	uid_t user_id;		            // userid
	gid_t group_id;		            // groupid
	time_t a_time;                  // tiempo de acceso
	time_t m_time;                  // tiempo de modificacion
	time_t c_time;                  // Status change time
	time_t b_time;                  // tiempo de cracion
	off_t size;                     // tamaño

	int datablocks[16];
	int number;
	int blocks;

} filetype;


int save_contents();
int find_free_inode();
int find_free_db();
int inode_check();
int myreaddir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi );
int myrmdir(const char * path);
int myunlink(const char * path);
int mycreate(const char * path, mode_t mode, struct fuse_file_info *fi);
int myopen(const char *path, struct fuse_file_info *fi);
int myopendir(const char* path, struct fuse_file_info* fi);
int myread(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi);
int myaccess(const char * path, int mask);
int myrename(const char* from, const char* to);
int mywrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int myfsync(const char* path, int isdatasync, struct fuse_file_info* fi);
int mystatfs(const char* path, struct statvfs* stbuf);
void encode_datablock();
void decode_datablock();
void add_child(filetype * parent, filetype * child);
void initialize_root_directory();
void initialize_superblock();
void tree_to_array(filetype * queue, int * front, int * rear, int * index);
filetype * filetype_from_path(char * path);
static int mygetattr(const char *path, struct stat *statit);
static int mymkdir(const char *path, mode_t mode);
static int myflush(struct superblock * block, int offset, int len);


superblock spblock;
filetype * root;
filetype file_array[50];

static struct fuse_operations operations = {
	.mkdir=mymkdir,
	.getattr=mygetattr,
	.readdir=myreaddir,
	.rmdir=myrmdir,
	.open=myopen,
	.read=myread,
	.write=mywrite,
	.create=mycreate,
	.rename=myrename,
	.unlink=myunlink,
	.fsync=myfsync,
	.access=myaccess,
	.opendir=myopendir,
	.statfs=mystatfs,
};
