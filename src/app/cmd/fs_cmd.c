#include <app.h>
#include <ff.h>

void do_cat(char *argvs[], uint32_t argc)
{
    FIL file;
    FRESULT res;
    char *file_name = argvs[1];
    uint8_t buf[64];
    uint32_t len = 0;
    uint32_t i = 0;

    res = f_open(&file, file_name, FA_READ);
    if (res != FR_OK) {
        trace("open file failed\n");
        f_close(&file);
        return;
    }

    do {
        f_read(&file, buf, 64, &len);
        for (i = 0; i < len; i++) {
            trace("%c", buf[i]);
        }
    } while (len != 0);

    f_close(&file);
}

void do_ls(char *argvs[], uint32_t argc)
{
    DIR dir_info;
    FILINFO file_info;
    FRESULT res;
    char path[64];

    if (argc == 1) {
        f_getcwd(path, 64);
    } else {
        memcpy(path, argvs[1], 64);
    }

    res = f_opendir(&dir_info, path); //打开一个目录

    if (res == FR_OK) {
        while (1) {
            res = f_readdir(&dir_info, &file_info); //读取目录下的一个文件
            if (res != FR_OK || file_info.fname[0] == 0)
                break;
            trace("%s\n", file_info.fname);
        }
 
    } else {
        trace("%s\n", path);
    }
}
