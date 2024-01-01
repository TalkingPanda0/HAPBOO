#include <stdio.h>
#include <png.h>
#include <stdlib.h> 
void write_png_file(char *filename,png_bytep *row_pointers,int width,int height) {
  int y;

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!row_pointers) abort();

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}

int main(int argc, char** argv)
{
	char *file_name = argv[1];
	FILE *fp = fopen(file_name, "rb");
	if(!fp)
		return 2;
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png)
		return 3;
	png_infop info = png_create_info_struct(png);
	if(!info)
		return 4;
	
	if(setjmp(png_jmpbuf(png)))
		return 5;
	
	png_init_io(png, fp);
	
	png_read_info(png, info);
	int width = png_get_image_width(png,info);
	int height = png_get_image_height(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);
	png_bytep *row_pointers = NULL;
	if(bit_depth == 16)
		png_set_strip_16(png);
	png_byte color_type = png_get_color_type(png, info);
	if(color_type == PNG_COLOR_TYPE_PALETTE)
	  png_set_palette_to_rgb(png);
	
	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	  png_set_expand_gray_1_2_4_to_8(png);
	
	if(png_get_valid(png, info, PNG_INFO_tRNS))
	  png_set_tRNS_to_alpha(png);
	
	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB ||
	   color_type == PNG_COLOR_TYPE_GRAY ||
	   color_type == PNG_COLOR_TYPE_PALETTE)
	  png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	
	if(color_type == PNG_COLOR_TYPE_GRAY ||
	   color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	  png_set_gray_to_rgb(png);
	
	png_read_update_info(png, info);
	
	if (row_pointers)
		return 7;
	
	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for(int y = 0; y < height; y++) {
	  row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}
	
	png_read_image(png, row_pointers);
	
	fclose(fp);
	
	for(int y = 0; y < height; y++) {
    		png_bytep row = row_pointers[y];
    		for(int x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			if(px[3] != 0)
				px[3] = 1;
		}
	}
	printf("pixel frame%c[] = {",argv[1][0]);
	typedef struct pixel {
		int alpha;
		int red;
		int green;
		int blue;
		int islast;
	} pixel;
	pixel pixels[] = {{0,256,255,255}};
	for(int y = 0; y < height; y++) {
    		png_bytep row = row_pointers[y];
    		for(int x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			if(y==(height-1) && (x==(width-1)))
			{
				printf("{%d,%d,%d,%d,%d}",px[3],px[0],px[1],px[2],0);
				continue;

			}
			printf("{%d,%d,%d,%d,%d},",px[3],px[0],px[1],px[2],x==(width-1));
		}
	}

	printf("};\n");
	png_destroy_read_struct(&png, &info, NULL);

}
