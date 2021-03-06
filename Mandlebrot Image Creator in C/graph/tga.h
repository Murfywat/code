#ifndef __TGA_FILE_FORMAT__
#define __TGA_FILE_FORMAT__

#define WIDTH 1920
#define HEIGHT 1440

struct tga_header {
  uint8_t id_length;
  uint8_t cmap_type;
  uint8_t img_type;
  uint8_t cmap_spec[5];
  uint8_t img_spec[10];
};

bool write_file (char * name);

extern uint8_t IMAGE[HEIGHT][WIDTH][3];
extern bool pFlag;
extern bool oFlag;

#endif
