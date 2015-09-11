/*
 * tga.c
 *
 *  Created on: Sep 6, 2014
 *      Author: Murphy Cooke
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tga.h"
#include <sys/stat.h>

bool write_file(char * name)
{
	FILE * fp = NULL;
	struct tga_header * theHeader = malloc(sizeof(struct tga_header));
	theHeader->cmap_type = 0x00;
	theHeader->id_length = 0x00;
	theHeader->img_type = 0x02;

	theHeader->cmap_spec[0] = 0x00;
	theHeader->cmap_spec[1] = 0x00;
	theHeader->cmap_spec[2] = 0x00;
	theHeader->cmap_spec[3] = 0x00;
	theHeader->cmap_spec[4] = 0x00;

	theHeader->img_spec[0] = 0x00;
	theHeader->img_spec[1] = 0x00;
	theHeader->img_spec[2] = 0x00;
	theHeader->img_spec[3] = 0x00;
	theHeader->img_spec[4] = 0x80;
	theHeader->img_spec[5] = 0x07;
	theHeader->img_spec[6] = 0xA0;
	theHeader->img_spec[7] = 0x05;
	theHeader->img_spec[8] = 0x18;
	theHeader->img_spec[9] = 0x00;

	fp = fopen(name, "w+b");
	if(fp != NULL)
	{
		fwrite(theHeader, sizeof(struct tga_header), 1, fp);
		fwrite(*IMAGE, sizeof(IMAGE), 1, fp);
	}

	struct stat theStat;
	stat(name, &theStat);
	int size = theStat.st_size;
	printf("Wrote %u bytes into file %s\n", size, name);

	return 0;
}

