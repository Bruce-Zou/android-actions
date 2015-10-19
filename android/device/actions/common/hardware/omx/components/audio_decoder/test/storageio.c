/********************************************************************************
*                              5003
*                            Module: storage io
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*       kkli     2008-09-02 10:00     1.0             build this file
********************************************************************************/
/*!
* \file     storageio.c
* \brief    ʵ�ֶԴ洢�豸���в����ĺ���
* \author   kkli
* \par      GENERAL DESCRIPTION:
*               storage io���������ļ���������һ�����ص㣬��ÿ��ֻ�ܲ���һ���ļ�
* \par      EXTERNALIZED FUNCTIONS:
*               �ļ�ϵͳ
* \version 1.0
* \date  2008/09/02
*******************************************************************************/
#include "music_parser_lib.h"
#include "stdio.h"
#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define MMM_FSEEK   fseeko
#define MMM_FTELL   ftello
#else
#define MMM_FSEEK   fseek
#define MMM_FTELL   ftell
#endif

typedef struct
{
    /*! ���ĵ�ʱ�� */
    int elapsed_time;
} storage_status_t;

typedef struct
{
    /*! ʣ��ռ䣬��λMBytes */
    int free_space;
    /*! ֧��д������ļ���С����λMBytes */
    int max_file_size;
} storage_info_t;

typedef struct
{
    storage_io_t io;            /* �����ӿ� */

    storage_mode_t mode;        /* ��Դģʽ */
    storage_rwmode_t rw_mode;   /* ��дģʽ */
    char *buffer;               /* ���ݻ�����ʼ��ַ */
    int buffer_size;            /* ���ݻ������Ĵ�С */
    char *rw_ptr;               /* ��ǰ���Զ�д��ָ���ַ*/
    int bytes_left;             /* ����������/�ռ�ʣ�࣬�ֱ��Ӧ����д */
    int bytes_filled;           /* ��ȡ�ļ�ʱ�����ļ���ȡ���ֽ��� */
    mmm_off_t file_size;        /* ֻ���ļ���С */
    int seek_fail_flag;         /* ��ʶ�Ƿ�seek���� */
    int elapsed_time;           /* �ļ����������ĵ�ʱ�� */
    void *private_data;         /* ��� */
} storage_internal_t;

static void disk_init_read(storage_internal_t *storage)
{
    int read_bytes = -1;

    /* ���Ӷ�fseek���ظ�ֵ���ж� */
    if (storage->seek_fail_flag == 0)
    {
        read_bytes = fread(storage->buffer, sizeof(char), storage->buffer_size, storage->private_data);
    }
    
    storage->bytes_filled = (read_bytes <= 0) ?  0 : read_bytes;
    storage->rw_ptr = storage->buffer;
    storage->bytes_left = storage->bytes_filled;
}

static int disk_read(void *buf, int size, int count, storage_io_t *io)
{
    storage_internal_t * const storage = (storage_internal_t *)io;
    char *cur_buf = (char *)buf;
    int bytes_need = size * count;
    int ret = count;


    if (storage->seek_fail_flag == 1)
    {
        return 0;
    }
    
    if (bytes_need >= storage->bytes_left)
    {
        memcpy(cur_buf, storage->rw_ptr, storage->bytes_left);
        bytes_need -= storage->bytes_left;
        cur_buf += storage->bytes_left;

        /* 
         * ��ÿ�ζ�storage->buffer_size(���)��Ϊһ�ζ����storage->buffer_size
         * ��߶���Ч�ʣ���Ҫ�����ڴ����ʵ���Ƶ��
         */
        if (bytes_need >= storage->buffer_size)
        {
            int quotient = bytes_need / storage->buffer_size;
            int read_bytes = 0;
            
            read_bytes = fread(cur_buf, sizeof(char), quotient * storage->buffer_size, storage->private_data);
            if (read_bytes > 0)
            {
               bytes_need -= read_bytes;
               cur_buf += read_bytes;
            }
            else
            {
                /* �ļ����������� */
            }
        }

        disk_init_read(storage);
        /* �������Ҫ��byte������һ��DISK_PAGE_SIZE�������ֵ����ļ����� */
        if (storage->bytes_left < bytes_need)
        {
            ret = ((size * count) - (bytes_need - storage->bytes_left)) / size;
            bytes_need = storage->bytes_left;
        }
    }

    memcpy(cur_buf, storage->rw_ptr, bytes_need);
    storage->rw_ptr += bytes_need;
    storage->bytes_left -= bytes_need;

    storage->elapsed_time += 0;
    return ret;
}

static int disk_write(void *buf, int size, int count, storage_io_t *io)
{
    storage_internal_t * const storage = (storage_internal_t *)io;
    char *cur_buf = (char *)buf;
    int space_need = size * count;
    int ret = count;


    if (space_need >= storage->bytes_left)
    {
        /* TODO: �Դ����Ƿ��Ѿ������ж� */
        memcpy(storage->rw_ptr, cur_buf, storage->bytes_left);
        fwrite(storage->buffer, sizeof(char), storage->buffer_size, storage->private_data);
        space_need -= storage->bytes_left;
        cur_buf += storage->bytes_left;

        while (space_need >= storage->buffer_size)
        {
            fwrite(cur_buf, sizeof(char), storage->buffer_size, storage->private_data);
            space_need -= storage->buffer_size;
            cur_buf += storage->buffer_size;
        }

        storage->bytes_left = storage->buffer_size;
        storage->rw_ptr = storage->buffer;
    }

    if (space_need > 0)
    {
        memcpy(storage->rw_ptr, cur_buf, space_need);
        storage->rw_ptr += space_need;
        storage->bytes_left -= space_need;
    }

    storage->elapsed_time += 0;
    return ret;
}

static int disk_seek(storage_io_t *io, mmm_off_t offset, int whence)
{
    storage_internal_t * const storage = (storage_internal_t *)io;
    mmm_off_t seek_bytes = 0;
    mmm_off_t cur_offset = offset;
   

    if (storage->rw_mode == RWMODE_WRITE)
    {
        /* �������еĵ�����д��ȥ */
        fwrite(storage->buffer, sizeof(char), storage->buffer_size - storage->bytes_left, storage->private_data);
        storage->rw_ptr = storage->buffer;
        storage->bytes_left = storage->buffer_size;
        if ((whence == SEEK_SET) && (offset == 0))
        {
            MMM_FSEEK(storage->private_data, offset, SEEK_SET);
            goto seek_exit0;
        }
        else
        {
            goto seek_exit1;
        }
    }

    switch (whence)
    {
    case SEEK_SET:
        /* �����ƫ����Ϊ����ʱ������������»�������ָ�볬����Ч��Χ */
        if (cur_offset < 0)
        {
            cur_offset = 0;
        }
        
        /* assure to seek to a buffer_size boudary */
        while (cur_offset >= storage->buffer_size)
        {
            cur_offset -= storage->buffer_size;
            seek_bytes += storage->buffer_size;
        }
        storage->seek_fail_flag = MMM_FSEEK(storage->private_data, seek_bytes, SEEK_SET);
        disk_init_read(storage);
        break;

    case SEEK_CUR:
        if (offset >= 0)
        {
            if (cur_offset >= storage->bytes_left)
            {
                cur_offset -= storage->bytes_left;
                while (cur_offset >= storage->buffer_size)
                {
                    cur_offset -= storage->buffer_size;
                    seek_bytes += storage->buffer_size;
                }

                storage->seek_fail_flag = MMM_FSEEK(storage->private_data, seek_bytes, SEEK_CUR);
                disk_init_read(storage);
            }
        }
        else
        {
            cur_offset = -cur_offset;
            /* �޸��о�������>= -> >��ʹ��seek����ǰ�������Ķ���ʱ����reload���� */
            if (cur_offset > (storage->bytes_filled - storage->bytes_left))
            {
                cur_offset -= (storage->bytes_filled - storage->bytes_left);
                while (cur_offset >= storage->buffer_size)
                {
                    cur_offset -= storage->buffer_size;
                    seek_bytes -= storage->buffer_size;
                }

                cur_offset = storage->buffer_size - cur_offset;
                seek_bytes -= storage->bytes_filled;
                seek_bytes -= storage->buffer_size;
                storage->seek_fail_flag = MMM_FSEEK(storage->private_data, seek_bytes, SEEK_CUR);
                disk_init_read(storage);
            }
            else
            {
                cur_offset = offset;
            }
        }
        break;

    case SEEK_END:
        if (offset <= 0)
        {
            mmm_off_t size_unaligned = storage->file_size & (storage->buffer_size - 1); // DISK_PAGE_MASK

            seek_bytes = storage->file_size - size_unaligned;
            cur_offset = -cur_offset;
            if (cur_offset > size_unaligned)
            {
                cur_offset -= size_unaligned;
                while (cur_offset >= storage->buffer_size)
                {
                    cur_offset -= storage->buffer_size;
                    seek_bytes -= storage->buffer_size;
                }

                cur_offset = storage->buffer_size - cur_offset;
                seek_bytes -= storage->buffer_size;

                storage->seek_fail_flag = MMM_FSEEK(storage->private_data, seek_bytes, SEEK_SET);
                disk_init_read(storage);
            }
            else
            {
                storage->seek_fail_flag = MMM_FSEEK(storage->private_data, storage->file_size - size_unaligned, SEEK_SET);
                disk_init_read(storage);

                storage->rw_ptr += size_unaligned;
                storage->bytes_left = 0;
                cur_offset = offset;
            }
        }
        break;

    default:
        break;
    }

    storage->elapsed_time += 0;

    storage->rw_ptr += cur_offset;
    storage->bytes_left -= cur_offset;
    if (storage->bytes_left < 0)
    {
        goto seek_exit1;
    }
    
seek_exit0:
    /* seek�ɹ�������fail flag��0 */
    storage->seek_fail_flag = 0;
    return 0;

seek_exit1:
    printf("%s: seek fail, whence = %d, offset = 0x%x%08x\n", __FILE__, whence, (int)(offset >> 32), (int)(offset & 0xffffffff));
    storage->bytes_left = 0;
    storage->seek_fail_flag = 1;
    return -1;
}

static mmm_off_t disk_tell(storage_io_t *io)
{
    storage_internal_t * const storage = (storage_internal_t *)io;
    mmm_off_t cur_pos = 0;


    cur_pos = MMM_FTELL(storage->private_data) - storage->bytes_left;

    storage->elapsed_time += 0;
    return cur_pos;
}

/******************************************************************************/
/*!
* \par  Description:
*     ����һ���Դ洢�豸���з��ʵĵĶ˿�
* \param[in]    mode ���ֲ�ͬ�Ĵ洢�豸
* \param[out]   none
* \return       ����һ����豸���з��ʵĺ���ָ��
* \retval           others sucess
* \retval           NULL failed
* \ingroup      storageio
* \note
*******************************************************************************/
storage_io_t *create_storage_io(storage_mode_t mode, int size)
{
    storage_internal_t *storage;
    int phy_add;

    storage = malloc(sizeof(storage_internal_t));
    if (storage == NULL)
    {
        return NULL;
    }

    storage->private_data = NULL;
    storage->mode = mode;
    storage->buffer_size = size;
    switch (storage->mode)
    {
    case MODE_DISK:
        storage->io.read = disk_read;
        storage->io.write = disk_write;
        storage->io.seek = disk_seek;
        storage->io.tell = disk_tell;
        break;

    default:
        return NULL;
    }

    /* ��ַ������BUFFER������߶�д�ٶ� */
    storage->buffer = malloc(storage->buffer_size);
    if (storage->buffer == NULL)
    {
        free(storage);
        storage = NULL;
        return NULL;
    }

    storage->rw_ptr = storage->buffer;
    storage->bytes_left = 0;

    return &storage->io;
}

int fini_storage_io(storage_io_t *io)
{
    storage_internal_t * const storage = (storage_internal_t *)io;

    if (storage->private_data != NULL)
    {
        switch (storage->mode)
        {
        case MODE_DISK:
            if (storage->rw_mode == RWMODE_WRITE)
            {
                /* �������еĵ�����д��ȥ */
                fwrite(storage->buffer,
                        sizeof(char),
                        storage->buffer_size - storage->bytes_left,
                        storage->private_data);
            }

            fclose(storage->private_data);
            break;

        default:
            break;
        }

        storage->private_data = NULL;
    }

    storage->rw_ptr = storage->buffer;
    storage->bytes_left = 0;
    storage->file_size = 0;
    storage->seek_fail_flag = 0;
    return 0;
}

mmm_off_t get_filesize_storage_io(storage_io_t *io)
{
    storage_internal_t * const storage = (storage_internal_t *)io;

    if (storage->file_size == 0)
    {
        MMM_FSEEK(storage->private_data, 0, SEEK_END);
        storage->file_size = MMM_FTELL(storage->private_data);
    }

    return storage->file_size;
}
/* ע�⣺���øýӿڴ��ļ�֮���ļ�λ�ò�ȷ�������ܹ�ֱ�Ӷ�
 * ��һ����Ҫ��seek��0��Ч�ʿ���û��Ĭ��������
 * */
int init_storage_io(storage_io_t *io, const char *filename, storage_rwmode_t mode)
{
    storage_internal_t * const storage = (storage_internal_t *)io;
    fini_storage_io(io);

    storage->rw_mode = mode;
    storage->elapsed_time = 0;
    switch (storage->mode)
    {
    case MODE_DISK:
        if (storage->rw_mode == RWMODE_READ)
        {
#if defined(_OS_UX_)
#if 1
            /* ���ַ�ʽ��֧�ִ�2G�����ļ���ͨ������-lc��� */
            int fd = open(filename, O_FASTFREE | O_RDONLY);
            if (fd <= 0)
            {
                printf("%s: file %s open read err\n", __FILE__, filename);
                return -1;
            }

            storage->private_data = fdopen(fd, "r");
            if (storage->private_data == NULL)
            {
                return -1;
            }
#else
            storage->private_data = fopen(filename, "rb");
            if (storage->private_data == NULL)
            {
                printf("%s: file %s open read err\n", __FILE__, filename);
                return -1;
            }
#endif
#elif defined(_OS_UC_)
            storage->private_data = fopen(filename, "rb");
            if (storage->private_data == NULL)
            {
                printf("%s: file %s open read err\n", __FILE__, filename);
                return -1;
            }

            fcntl(fileno((FILE *)storage->private_data), F_BUILDINDEX, 0);
            fcntl(fileno((FILE *)storage->private_data), F_CLUSTALIGN, 0);
            setbuf((FILE *)storage->private_data, NULL);
#else
            storage->private_data = fopen(filename, "rb");
            if (storage->private_data == NULL)
            {
                printf("%s: file %s open read err\n", __FILE__, filename);
                return -1;
            }
#endif
            storage->file_size = get_filesize_storage_io(io);
            if (storage->file_size < 512)
            {
                printf("%s: file empty\n", __FILE__);
                fini_storage_io(io);
                return -1;
            }
        }
        else if (storage->rw_mode == RWMODE_WRITE)
        {
            storage->private_data = fopen(filename, "wb");
            if (storage->private_data == NULL)
            {
                printf("%s: file %s open write err\n", __FILE__, filename);
                return -1;
            }

#ifdef _OS_UC_
            fcntl(fileno((FILE *)storage->private_data), F_BUILDINDEX, 0);
            fcntl(fileno((FILE *)storage->private_data), F_CLUSTALIGN, 0);
            fcntl(fileno((FILE *)storage->private_data), F_AVOIDMERGE, 0);
            setbuf((FILE *)storage->private_data, NULL);
#endif
            storage->rw_ptr = storage->buffer;
            storage->bytes_left = storage->buffer_size;
        }
        break;

    default:
        storage->private_data = NULL;
        break;
    }

    if (storage->private_data == NULL)
    {
        return -1;
    }

    return 0;
}

int dispose_storage_io(storage_io_t *io)
{
    storage_internal_t *storage = (storage_internal_t *)io;

    if (storage != NULL)
    {
        fini_storage_io(io);

        if (storage->buffer != NULL)
        {
            free(storage->buffer);
            storage->buffer = NULL;
        }

        free(storage);
        storage = NULL;
    }

    return 0;
}

int check_storage_io(storage_io_t *io, storage_status_t *storage_status)
{
    storage_internal_t *storage = (storage_internal_t *)io;

    storage_status->elapsed_time = storage->elapsed_time;

    return 0;
}

int get_storage_info(const char *path, storage_info_t *storage_info)
{
    int ret = 0;
#ifdef _OS_UC_
    struct statfs disk_statfs;
    
    ret = statfs(path, &disk_statfs);
    if (ret == 0)
    {
        storage_info->free_space = disk_statfs.f_bsize * disk_statfs.f_bavail;
        storage_info->max_file_size = disk_statfs.f_filemaxsize;
    }
    else
    {
        printf("%s: statfs error\n", __FILE__);
    }    
#endif
    
    return ret;
}
