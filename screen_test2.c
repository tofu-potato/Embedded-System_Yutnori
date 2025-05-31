#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include "screen2.h"
#include <string.h>

#define BIT_VALUE_24BIT 24
#define FBDEV_FILE "/dev/fb0"
int red_positions[4][2];
int blue_positions[4][2];
//int red_positions[4][2] = { {100,100}, {200,100}, {100,200}, {200,200} };  // test
//int blue_positions[4][2] = { {300,100}, {400,100}, {300,200}, {400,200} }; // test

void close_bmp(char **pDib)     // DIB(Device Independent Bitmap)
{
    free(*pDib);
}

void read_bmp(char *filename, char **pDib, char **data, int *cols, int *rows)
{
    BITMAPFILEHEADER    bmpHeader;
    BITMAPINFOHEADER    *bmpInfoHeader;
    unsigned int    size;
    unsigned char   magicNum[2];
    int     nread;
    FILE    *fp;

    fp  =  fopen(filename, "rb");
    if(fp == NULL) {
        printf("ERROR\n");
        return;
    }
    printf("%s\r\n",filename);
    // identify bmp file
    magicNum[0]   =   fgetc(fp);
    magicNum[1]   =   fgetc(fp);
    printf("magicNum : %c%c\n", magicNum[0], magicNum[1]);

    if(magicNum[0] != 'B' && magicNum[1] != 'M') {
        printf("It's not a bmp file!\n");
        fclose(fp);
        return;
    }

    nread   =   fread(&bmpHeader.bfSize, 1, sizeof(BITMAPFILEHEADER), fp);
    size    =   bmpHeader.bfSize - sizeof(BITMAPFILEHEADER);
    *pDib   =   (unsigned char *)malloc(size);      // DIB Header(Image Header)
    fread(*pDib, 1, size, fp);
    bmpInfoHeader   =   (BITMAPINFOHEADER *)*pDib;

    printf("nread : %d\n", nread);
    printf("size : %d\n", size);

    // check 24bit
    if(BIT_VALUE_24BIT != (bmpInfoHeader->biBitCount))     // bit value
    {
        printf("It supports only 24bit bmp!\n");
        fclose(fp);
        return;
    }

    *cols   =   bmpInfoHeader->biWidth;
    *rows   =   bmpInfoHeader->biHeight;
    *data   =   (char *)(*pDib + bmpHeader.bfOffBits - sizeof(bmpHeader) - 2);
    fclose(fp);
}

// offset doesn't change //////////////////////////////////////////////////////////

// 말 위치를 그리는 원
static void draw_filled_circle_r(uint32_t *fb_pixels, int fb_width, int fb_height,
                                int cx, int cy, int r) {
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy <= r * r) {
                int x = cx + dx;
                int y = cy + dy;
                if (x >= 0 && x < fb_width && y >= 0 && y < fb_height) {
                    fb_pixels[y * fb_width + x] = 0xFF0000;  // 빨간색 원
                }
            }
        }
    }
}

static void draw_filled_circle_b(uint32_t *fb_pixels, int fb_width, int fb_height,
                                int cx, int cy, int r) {
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy <= r * r) {
                int x = cx + dx;
                int y = cy + dy;
                if (x >= 0 && x < fb_width && y >= 0 && y < fb_height) {
                    fb_pixels[y * fb_width + x] = 0x0000FF;  // blue circle
                }
            }
        }
    }
}

//int bmp_read(char *fn, int piece_x, int piece_y)
int bmp_read(char *fn, int red_position[4][2], int blue_position[4][2])
{
    int fb_fd = open(FBDEV_FILE, O_RDWR);
    if (fb_fd < 0) {
        perror("fb open error");
        return -1;
    }

    struct fb_var_screeninfo fbvar;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvar)) {
        perror("fb ioctl error");
        close(fb_fd);
        return -1;
    }

    int fb_width = fbvar.xres;
    int fb_height = fbvar.yres;
    int fb_size = fb_width * fb_height * 4;

    printf("Framebuffer size: %dx%d\n", fb_width, fb_height);

    uint32_t *fb_pixels = mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if ((int)fb_pixels == -1) {
        perror("mmap error");
        close(fb_fd);
        return -1;
    }

    char *pData = NULL, *data = NULL;
    int cols, rows;

    read_bmp(fn, &pData, &data, &cols, &rows);
    printf("BMP size: %dx%d\n", cols, rows);

    int draw_cols = cols;
    int draw_rows = rows;

    if (draw_cols > fb_width)
        draw_cols = fb_width;

    if (draw_rows > fb_height)
        draw_rows = fb_height;

    for (int y = 0; y < draw_rows; y++) {
        for (int x = 0; x < draw_cols; x++) {
            int b = data[y * cols * 3 + x * 3 + 0];
            int g = data[y * cols * 3 + x * 3 + 1];
            int r = data[y * cols * 3 + x * 3 + 2];

            int draw_x = x;
            int draw_y = y;

            if (draw_x < 0 || draw_x >= fb_width || draw_y < 0 || draw_y >= fb_height)
                continue;

            fb_pixels[draw_y * fb_width + draw_x] = (r << 16) | (g << 8) | b;
        }
    }

    // 빨간색 원은 입력된 위치에 그린다
    // draw_filled_circle(fb_pixels, fb_width, fb_height, piece_x, piece_y, 8);
    
    // Multiple Circles
    draw_filled_circle_r(fb_pixels, fb_width, fb_height, red_positions[0][0], red_positions[0][1], 10);
    draw_filled_circle_r(fb_pixels, fb_width, fb_height, red_positions[1][0], red_positions[1][1], 10);
    draw_filled_circle_r(fb_pixels, fb_width, fb_height, red_positions[2][0], red_positions[2][1], 10);
    draw_filled_circle_r(fb_pixels, fb_width, fb_height, red_positions[3][0], red_positions[3][1], 10);


    draw_filled_circle_b(fb_pixels, fb_width, fb_height, blue_positions[0][0], blue_positions[0][1], 10);
    draw_filled_circle_b(fb_pixels, fb_width, fb_height, blue_positions[1][0], blue_positions[1][1], 10);
    draw_filled_circle_b(fb_pixels, fb_width, fb_height, blue_positions[2][0], blue_positions[2][1], 10);
    draw_filled_circle_b(fb_pixels, fb_width, fb_height, blue_positions[3][0], blue_positions[3][1], 10);

    close_bmp(&pData);
    munmap(fb_pixels, fb_size);
    close(fb_fd);
    return 0;
}

// 8 circles ///////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <bmp_file> <x1,y1/x2,y2/.../x8,y8>\n", argv[0]);
        return -1;
    }

    char *bmp_file = argv[1];
    char *coord_str = argv[2];

    //int red_position[4][2];
    //int blue_position[4][2];

    char *token = strtok(coord_str, "/");
    int i = 0;
    while (1) {
        FILE *fp = fopen("coords.txt", "r");
        if (!fp) {
            perror("coords.txt open error");
            return -1;
        }

        char coord_str[512];
        if (!fgets(coord_str, sizeof(coord_str), fp)) {
            fclose(fp);
            continue;
        }
        fclose(fp);

        // 공통 파싱 루틴 시작
        //int red_position[4][2];
        //int blue_position[4][2];
        char *token = strtok(coord_str, "/");
        int i = 0;
        while (token != NULL && i < 8) {
            int x, y;
            sscanf(token, "%d,%d", &x, &y);
            if (i < 4) {
                red_position[i][0] = x;
                red_position[i][1] = y;
            }
            else {
                blue_position[i - 4][0] = x;
                blue_position[i - 4][1] = y;
            }
            i++;
            token = strtok(NULL, "/");
        }
        if (i == 8) {
            bmp_read(bmp_file, red_position, blue_position);
        }
        sleep(0.5);  // 100ms 주기
}

/*
    while (token != NULL && i < 8) {
        int x, y;
        sscanf(token, "%d,%d", &x, &y);
        if (i < 4) {
            red_position[i][0] = x;
            red_position[i][1] = y;
        } else {
            blue_position[i - 4][0] = x;
            blue_position[i - 4][1] = y;
        }
        i++;
        token = strtok(NULL, "/");
    }

    if (i != 8) {
        printf("Error: Expected 8 coordinates.\n");
        return -1;
    }

    bmp_read(bmp_file, red_position, blue_position);
    return 0;
*/ // single implementation -test
}

// 8 circles //////////////////////////////////////////////////////////////////////

// single circle ////////////////////////////////////////////////////////////////////
/*
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <bmp_file> <circle_x> <circle_y>\n", argv[0]);
        return -1;
    }

    char *bmp_file = argv[1];
    int circle_x = atoi(argv[2]);
    int circle_y = atoi(argv[3]);

    while (1) {
	//8 circles
	

	// only one circle
        //bmp_read(bmp_file, circle_x, circle_y);

	// multiple circles
	bmp_read(bmp_file, red_position, blue_position);
        sleep(0.5);  // 1초마다 반복
    }

    return 0;
}
*/
// single circle /////////////////////////////////////////////////////////////////


// offset doesn't change //////////////////////////////////////////////////////////




// offset changes//////////////////////////////////////////////////////////////////
// 말 위치를 그리는 원
/*
static void draw_filled_circle(uint32_t *fb_pixels, int fb_width, int fb_height,
                                int cx, int cy, int r) {
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy <= r * r) {
                int x = cx + dx;
                int y = cy + dy;
                if (x >= 0 && x < fb_width && y >= 0 && y < fb_height) {
                    fb_pixels[y * fb_width + x] = 0xFF0000;  // 빨간색 원
                }
            }
        }
    }
}

int bmp_read(char *fn, int piece_x, int piece_y)
{
    int fb_fd = open(FBDEV_FILE, O_RDWR);
    if (fb_fd < 0) {
        perror("fb open error");
        return -1;
    }

    struct fb_var_screeninfo fbvar;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvar)) {
        perror("fb ioctl error");
        close(fb_fd);
        return -1;
    }

    int fb_width = fbvar.xres;
    int fb_height = fbvar.yres;
    int fb_size = fb_width * fb_height * 4;

    printf("Framebuffer size: %dx%d\n", fb_width, fb_height);

    uint32_t *fb_pixels = mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if ((int)fb_pixels == -1) {
        perror("mmap error");
        close(fb_fd);
        return -1;
    }

    char *pData = NULL, *data = NULL;
    int cols, rows;

    read_bmp(fn, &pData, &data, &cols, &rows);
    printf("BMP size: %dx%d\n", cols, rows);

    if (piece_x + cols > fb_width || piece_y + rows > fb_height) {
        printf("\u26a0\ufe0f BMP (%dx%d) is too large for framebuffer (%dx%d) at offset (%d,%d)\n",
               cols, rows, fb_width, fb_height, piece_x, piece_y);
        close_bmp(&pData);
        munmap(fb_pixels, fb_size);
        close(fb_fd);
        return -1;
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int b = data[y * cols * 3 + x * 3 + 0];
            int g = data[y * cols * 3 + x * 3 + 1];
            int r = data[y * cols * 3 + x * 3 + 2];

            int draw_x = piece_x + x;
            int draw_y = piece_y + y;

            if (draw_x < 0 || draw_x >= fb_width || draw_y < 0 || draw_y >= fb_height)
                continue;

            fb_pixels[draw_y * fb_width + draw_x] = (r << 16) | (g << 8) | b;
        }
    }

    draw_filled_circle(fb_pixels, fb_width, fb_height, piece_x + 10, piece_y + 10, 8);

    close_bmp(&pData);
    munmap(fb_pixels, fb_size);
    close(fb_fd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <bmp_file> <x> <y>\n", argv[0]);
        return -1;
    }

    char *bmp_file = argv[1];
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);

    while (1) {
        bmp_read(bmp_file, x, y);
        sleep(0.05);  // 1초마다 반복
    }

    return 0;
}
*/
// offset changes ///////////////////////////////////////////////////////////////////////////
