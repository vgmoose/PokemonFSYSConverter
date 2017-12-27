
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include <math.h>

#include "bytetools.h"
#include "igtl_Directory.h"


void HSVtoRGB(double h, double s, double v, double* r, double* g, double* b){
  if ( s == 0 ){
     *r = v;
     *g = v;
     *b = v;
  }else{
     double var_h = h * 6;
     double var_i = int( var_h );
     double var_1 = v * ( 1 - s );
     double var_2 = v * ( 1 - s * ( var_h - var_i ) );
     double var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

     if      ( var_i == 0 ) { *r = v     ; *g = var_3 ; *b = var_1; }
     else if ( var_i == 1 ) { *r = var_2 ; *g = v     ; *b = var_1; }
     else if ( var_i == 2 ) { *r = var_1 ; *g = v     ; *b = var_3; }
     else if ( var_i == 3 ) { *r = var_1 ; *g = var_2 ; *b = v;     }
     else if ( var_i == 4 ) { *r = var_3 ; *g = var_1 ; *b = v;     }
     else                   { *r = v     ; *g = var_1 ; *b = var_2; }
  }
}



bool saveTGA(const char* fn, size_t w, size_t h, const uint32_t* rgba) {
  FILE* fp = fopen(fn, "wb");
  if (fp == NULL) return false; // failed open?
  // write tga header. Magic!
  fputc(0, fp); // id length = 0 bytes
  fputc(0, fp); // color map type = none
  fputc(2, fp); // uncompressed RGBA
  fputc(0, fp); fputc(0, fp); // color map origin = N/A
  fputc(0, fp); fputc(0, fp); // color map length = N/A
  fputc(0, fp); // color map depth = N/A
  fputc(0, fp); fputc(0, fp); // x origin (short16) = 0
  fputc(0, fp); fputc(0, fp); // y origin (short16) = 0
  fputc( (w & 0xff) , fp); fputc( (w & 0xff00) >> 8 , fp); // width (LE short16)
  fputc( (h & 0xff) , fp); fputc( (h & 0xff00) >> 8 , fp); // height (LE short16)
  fputc(32, fp); // 32-bit RGBA
  fputc(8, fp); // image descriptor byte -- set to 8 for tga32 RGBA data

  for (size_t p = 1; p <= h; ++p) {
    fwrite(rgba + (w * (h - p)), w, 4, fp);
  }

  fclose(fp);
  return true;
}

//void flipRGB( unsigned char* src, size_t size ){

	//Swap RGB to BGR =>?
//}

void img_I1(char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {

  /*
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  bool bit = true;
  uint8_t tex_value;
  for(size_t i = 0; i < width * height; i++) {

	if( p > p_final ){ break; }

    if (bit){ tex_value = *(p++); }
    int tex_real_value = ((bit ? (tex_value >> 0x04) : (tex_value)) & 15)<<4;
    bit = !bit;

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = (tex_real_value << 16) | (tex_real_value << 8) | (tex_real_value);

    // Clamp coordinates within the specified texture size, block encoding might end up with
    // out of bounds writes...
    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 7 ) {
        tile_x = 0;
        tile_y++;
    }
    if( tile_y > 7 ) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 8;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
        i--;
    }
    if (img_y > height){ break; }
  }
  */
}

void img_I4 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  bool bit = true;
  uint8_t tex_value;
  for(size_t i = 0; i < width * height; i++) {

	if( p > p_final ){ break; }

    if (bit){ tex_value = *(p++); }
    int tex_real_value = ((bit ? (tex_value >> 0x04) : (tex_value)) & 15)<<4;
    bit = !bit;

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = (tex_real_value << 16) | (tex_real_value << 8) | (tex_real_value);

    // Clamp coordinates within the specified texture size, block encoding might end up with
    // out of bounds writes...
    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 7 ) {
        tile_x = 0;
        tile_y++;
    }
    if( tile_y > 7 ) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 8;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
        i--;
    }
    if (img_y > height){ break; }
  }
}

void img_IA4 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for(size_t i = 0; i < width * height; i++) {

	if( p > p_final ){ break; }

    uint8_t tex_value = *(p++);

    int tex_alpha_value = (0xFF - (((( tex_value >> 4 ) & 0x0F) << 4) | (tex_value >> 1) & 0x07))>>1 & 0x7F;
    int tex_real_value = ((tex_value & 0x0F) << 4);

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = tex_alpha_value << 24 | tex_real_value << 16 | tex_real_value << 8 | tex_real_value;

    // Clamp coordinates within the specified texture size, block encoding might end up with
    // out of bounds writes...
    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 7 ) {
        tile_x = 0;
        tile_y++;
    }
    if( tile_y > 3 ) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width ) {
      img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
        i--;
    }
    if (img_y > height) break;
  }
}


void img_I8 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {

  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for(size_t i = 0; i < width * height; i++) {

  	if( p > p_final ){ break; }

    uint16_t tex_value = *p; p ++;

    uint8_t tex_alpha_value = 255;
    uint8_t tex_real_value = tex_value;

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = tex_alpha_value << 24 | tex_real_value << 16 | tex_real_value << 8 | tex_real_value;

    // Clamp coordinates within the specified texture size, block encoding might end up with
    // out of bounds writes...
    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 7 ) {
        tile_x = 0;
        tile_y++;
    }
    if( tile_y > 3 ) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
        i--;
    }
    if (img_y > height) break;
  }
}

void img_IA8 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for(size_t i = 0; i < width * height; i++) {

  	if( p > p_final ){ break; }

    uint16_t tex_value = UChar2ToUIntHL((uchar*)p); p += 2;

    uint8_t tex_real_value = tex_value & 0xFF; //((tex_value & 0xFF))>>1 & 0x7F;
    uint8_t tex_alpha_value = 0xFF - ((tex_value>>8) & 0xFF); //(0xFF - ((tex_value >> 8) & 0xFF));

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = tex_alpha_value << 24 | tex_real_value << 16 | tex_real_value << 8 | tex_real_value;

    // Clamp coordinates within the specified texture size, block encoding might end up with
    // out of bounds writes...
    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 3 ) {
        tile_x = 0;
        tile_y++;
    }
    if( tile_y > 3 ) {
      tile_y = 0;
      img_x += 4;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
        i--;
    }
    if (img_y > height) break;
  }
}

void img_RGB565 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for( size_t i = 0; i < width * height; i++) {

  	if( p > p_final ){ break; }

    uint16_t pal_value_full = UChar2ToUIntHL((uchar*)p); p += 2;
    uint8_t pal_value_red = ( ( pal_value_full >> 11) & 0x1F) << 3;
    uint8_t pal_value_green = ( ( pal_value_full >> 5) & 0x3F) << 2;
    uint8_t pal_value_blue = ( pal_value_full & 0x1F) << 3;

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = (pal_value_red << 16) | (pal_value_green << 8) | pal_value_blue;
    if ( (x < width) && (y < height) ) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if (tile_x > 3 ) {
        tile_x = 0;
        tile_y++;
    }
    if (tile_y > 3 ) {
        tile_y = 0;
        img_x += 4;
    }
    if (img_x >= width) {
        img_x = 0;
        img_y += 4;
    }
    if( (tile_y + img_y >= height) || (img_x + tile_x >= width) ) {
        i--;
    }
    if (img_y > height) break;
  }
}

void img_RBG565 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for( size_t i = 0; i < width * height; i++) {

  	if( p > p_final ){ break; }

    uint16_t pal_value_full = UChar2ToUIntHL((uchar*)p); p += 2;
    uint8_t pal_value_red = ( ( pal_value_full >> 11) & 0x1F) << 3;
    uint8_t pal_value_green = ( ( pal_value_full >> 5) & 0x3F) << 2;
    uint8_t pal_value_blue = ( pal_value_full & 0x1F) << 3;

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = (pal_value_red << 16) | (pal_value_green << 8) | pal_value_blue;
    if ( (x < width) && (y < height) ) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if (tile_x > 3 ) {
        tile_x = 0;
        tile_y++;
    }
    if (tile_y > 3 ) {
        tile_y = 0;
        img_x += 4;
    }
    if (img_x >= width) {
        img_x = 0;
        img_y += 4;
    }
    if( (tile_y + img_y >= height) || (img_x + tile_x >= width) ) {
        i--;
    }
    if (img_y > height){ break; }
  }
}


void img_BGR565 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for( size_t i = 0; i < width * height; i++) {

  	if( p > p_final ){ break; }

    uint16_t pal_value_full = UChar2ToUIntHL((uchar*)p); p += 2;
    uint8_t pal_value_blue = ( ( pal_value_full >> 11) & 0x1F) << 3;
    uint8_t pal_value_green = ( ( pal_value_full >> 5) & 0x3F) << 2;
    uint8_t pal_value_red = ( pal_value_full & 0x1F) << 3;

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = (pal_value_red << 16) | (pal_value_green << 8) | pal_value_blue;
    if ( (x < width) && (y < height) ) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if (tile_x > 3 ) {
        tile_x = 0;
        tile_y++;
    }
    if (tile_y > 3 ) {
        tile_y = 0;
        img_x += 4;
    }
    if (img_x >= width) {
        img_x = 0;
        img_y += 4;
    }
    if( (tile_y + img_y >= height) || (img_x + tile_x >= width) ) {
        i--;
    }
    if (img_y > height){ break; }
  }
}

void img_RGB5A3 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  for( size_t i = 0; i < width * height; i++) {

	if( p > p_final ){ break; }

    int pal_value_test;
    int pal_value_alpha;
    int pal_value_red;
    int pal_value_green;
    int pal_value_blue;

    uint16_t pal_value_full = UChar2ToUIntHL((uchar*)p); p += 2;
    pal_value_test = pal_value_full & 0x8000;
    if( pal_value_test > 0) {
      pal_value_alpha = 0;
      pal_value_red = ( ( pal_value_full >> 10) & 0x1F) << 3;
      pal_value_green = ( ( pal_value_full >> 5) & 0x1F) << 3;
      pal_value_blue = ( pal_value_full & 0x1F) << 3;
    } else {
      pal_value_alpha = (255 - ((( pal_value_full >> 12) & 0x07) << 0x04)) >> 1 & 0x7F;
      pal_value_red = ( ( pal_value_full >> 8) & 0x0F) << 4;
      pal_value_green = ( ( pal_value_full >> 4) & 0x0F) << 4;
      pal_value_blue = ( pal_value_full & 0x0F) << 4;
    }

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = (pal_value_alpha << 24) | (pal_value_red << 16) | (pal_value_green << 8) | pal_value_blue;
    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if (tile_x > 3 ) {
        tile_x = 0;
        tile_y++;
    }
    if (tile_y > 3 ) {
        tile_y = 0;
        img_x += 4;
    }
    if (img_x >= width) {
        img_x = 0;
        img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
        i--;
    }
    if (img_y > height) break;
  }
}



void img_RGBA8 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;
  int bit = 1;
  int img_tile[16];
  char* p = img_base;
  char* p_final = img_base + img_length;
  for(size_t i = 0; i < width * height * 2; i++) {

	if( p > p_final ){ break; }

    size_t tile_pos = i % 16;
    if( tile_pos == 0 ) bit = 1 - bit;

    img_tile[ tile_pos ] = ( img_tile[ tile_pos ] << 16 ) | UChar2ToUIntHL((uchar*)p); p += 2;

    if( bit == 1 ) {
      uint32_t x = tile_x + img_x;
      uint32_t y = tile_y + img_y;
      uint32_t color_value = img_tile[ tile_pos ];
      color_value = ( 0x7F000000 - ( (color_value >> 1) & 0xFF000000) ) | ( color_value & 0xFFFFFF );
      if (x < width && y < height) {
        out_pixels[x + (y * width)] = color_value;
      }
      img_tile[ tile_pos ] = 0;
      tile_x++;
    }
    if( tile_x > 3 ) {
      tile_x = 0;
      tile_y++;
    }
    if( tile_y > 3 ) {
      tile_y = 0;
      img_x += 4;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
      i--;
    }
    if (img_y > height) break;
  }
}

void img_CI4 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* palette_data, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;
  char* p = img_base;
  char* p_final = img_base + img_length;
  bool bit = true;
  uint8_t last_p;

  for(size_t i = 0; i < width * height; i++) {

	if( p > p_final ){ break; }

    if (bit) last_p = *(p++);
    uint8_t palette_index = (bit ? (last_p > 4) : last_p) & 0xf;
    bit = !bit;
    uint32_t color_value = palette_data[palette_index];

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;

    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 7 ) {
      tile_x = 0;
      tile_y++;
    }
    if( tile_y > 3 ) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
      i--;
    }
    if( img_y > height ) break;
  }
}

void img_CI8 (char* img_base, size_t img_length, size_t width, size_t height, uint32_t* palette_data, uint32_t* out_pixels) {
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;
  char* p = img_base;
  char* p_final = img_base + img_length;
  for(size_t i = 0; i < width * height; i++) {

	if( p > p_final ){ break; }

    uint32_t x = tile_x + img_x;
    uint32_t y = tile_y + img_y;
    uint32_t color_value = palette_data[*(p++) & 0xff];

    if (x < width && y < height) {
      out_pixels[x + (y * width)] = color_value;
    }

    tile_x++;
    if( tile_x > 7 ) {
      tile_x = 0;
      tile_y++;
    }
    if( tile_y > 3 ) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width ) {
      img_x = 0;
      img_y += 4;
    }
    if( tile_y + img_y >= height || img_x + tile_x >= width ) {
      i--;
    }
    if( img_y > height ) break;
  }
}


void img_CMPRGB(char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  int bit = 0;
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;
  uint8_t tile = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  while (p < (img_base + img_length)) {

	if( p > p_final ){ break; }

    uint16_t color0 = UChar2ToUIntHL((uchar*)p); p += 2;
    uint16_t color1 = UChar2ToUIntHL((uchar*)p); p += 2;

    for (int j = 0; j < 16; ++j) {
      if (bit == 0) tile = *(p++);

      int tile_value = ( tile >> ( 0x02 * ( 3 - bit ) ) ) & 0x03;
      bit = ( bit + 1 ) % 4;
      if( color0 <= color1 ) tile_value |= 0x04;

      int colorx;
      switch( tile_value ) {
        case 0:
        case 4:
          colorx = color0;
          break;
        case 1:
        case 5:
          colorx = color1;
          break;
        case 2:
          //colorx = ( color0 * 2 + color1 ) / 3;
          colorx = ((((( color0 >> 11 ) & 0x1F ) * 2 + (( color1 >> 11) & 0x1F )) / 3 ) << 11 ) |
                   ((((( color0 >> 5 ) & 0x3F ) * 2 + (( color1 >> 5) & 0x3F )) / 3 ) << 5 ) |
                   ((( color0 & 0x1F ) * 2 + ( color1 & 0x1F )) / 3 );
          break;
        case 3:
          //colorx = ( color0 + color1 * 2 ) / 3;
          colorx = ((((( color1 >> 11 ) & 0x1F ) * 2 + (( color0 >> 11) & 0x1F )) / 3 ) << 11 ) |
                   ((((( color1 >> 5 ) & 0x3F ) * 2 + (( color0 >> 5) & 0x3F )) / 3 ) << 5 ) |
                   ((( color1 & 0x1F ) * 2 + ( color0 & 0x1F )) / 3 );
          break;
        case 6:
          //colorx = ( color0 + color1 ) / 2;
          colorx = ((((( color1 >> 11 ) & 0x1F ) + (( color0 >> 11) & 0x1F )) / 2 ) << 11 ) |
                   ((((( color1 >> 5 ) & 0x3F ) + (( color0 >> 5) & 0x3F )) / 2 ) << 5 ) |
                   ((( color1 & 0x1F ) + ( color0 & 0x1F )) / 2 );
          break;
        case 7:
          colorx = ( 0xFF << 16 ) |
                   ((((( color1 >> 11 ) & 0x1F ) * 2 + (( color0 >> 11) & 0x1F )) / 3 ) << 11 ) |
                   ((((( color1 >> 5 ) & 0x3F ) * 2 + (( color0 >> 5) & 0x3F )) / 3 ) << 5 ) |
                   ((( color1 & 0x1F ) * 2 + ( color0 & 0x1F )) / 3 );
          break;
      }

      uint32_t color_value = ( ( colorx << 8 ) & 0xFF000000 ) |
                             ( ( ( colorx >> 11) & 0x1F ) << 19) |
                             ( ( ( colorx >> 5) & 0x3F ) << 10) |
                             ( ( colorx & 0x1F ) << 3);

      uint32_t x = (j % 4) + img_x + tile_x;
      uint32_t y = (j / 4) + img_y + tile_y;
      if (x < width && y < height) {
        out_pixels[x + (y * width)] = color_value;
      }
    }
    tile_x += 4;
    if( tile_x > 7) {
      tile_x = 0;
      tile_y += 4;
    }
    if( tile_y > 7) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width) {
      img_x = 0;
      img_y += 8;
    }
    if (img_y > height) break;
  }
}

void img_CMPBGR(char* img_base, size_t img_length, size_t width, size_t height, uint32_t* out_pixels) {
  int bit = 0;
  size_t tile_x = 0;
  size_t tile_y = 0;
  size_t img_x = 0;
  size_t img_y = 0;
  uint8_t tile = 0;

  char* p = img_base;
  char* p_final = img_base + img_length;
  while (p < p_final ) {
    uint16_t color0 = UChar2ToUIntHL((uchar*)p); p += 2;
    uint16_t color1 = UChar2ToUIntHL((uchar*)p); p += 2;

    for (int j = 0; j < 16; ++j) {
      if (bit == 0) tile = *(p++);

      int tile_value = ( tile >> ( 0x02 * ( 3 - bit ) ) ) & 0x03;
      bit = ( bit + 1 ) % 4;
      if( color0 <= color1 ) tile_value |= 0x04;

      int colorx;
      switch( tile_value ) {
        case 0:
        case 4:
          colorx = color0;
          break;
        case 1:
        case 5:
          colorx = color1;
          break;
        case 2:
          //colorx = ( color0 * 2 + color1 ) / 3;
          colorx = ((((( color0 >> 11 ) & 0x1F ) * 2 + (( color1 >> 11) & 0x1F )) / 3 ) << 11 ) |
                   ((((( color0 >> 5 ) & 0x3F ) * 2 + (( color1 >> 5) & 0x3F )) / 3 ) << 5 ) |
                   ((( color0 & 0x1F ) * 2 + ( color1 & 0x1F )) / 3 );
          break;
        case 3:
          //colorx = ( color0 + color1 * 2 ) / 3;
          colorx = ((((( color1 >> 11 ) & 0x1F ) * 2 + (( color0 >> 11) & 0x1F )) / 3 ) << 11 ) |
                   ((((( color1 >> 5 ) & 0x3F ) * 2 + (( color0 >> 5) & 0x3F )) / 3 ) << 5 ) |
                   ((( color1 & 0x1F ) * 2 + ( color0 & 0x1F )) / 3 );
          break;
        case 6:
          //colorx = ( color0 + color1 ) / 2;
          colorx = ((((( color1 >> 11 ) & 0x1F ) + (( color0 >> 11) & 0x1F )) / 2 ) << 11 ) |
                   ((((( color1 >> 5 ) & 0x3F ) + (( color0 >> 5) & 0x3F )) / 2 ) << 5 ) |
                   ((( color1 & 0x1F ) + ( color0 & 0x1F )) / 2 );
          break;
        case 7:
          colorx = ( 0xFF << 16 ) |
                   ((((( color1 >> 11 ) & 0x1F ) * 2 + (( color0 >> 11) & 0x1F )) / 3 ) << 11 ) |
                   ((((( color1 >> 5 ) & 0x3F ) * 2 + (( color0 >> 5) & 0x3F )) / 3 ) << 5 ) |
                   ((( color1 & 0x1F ) * 2 + ( color0 & 0x1F )) / 3 );
          break;
      }

      //uint32_t color_value = ( ( colorx << 8 ) & 0xFF000000 ) |
      //                       ( ( ( colorx >> 11) & 0x1F ) << 19) |
      //                       ( ( ( colorx >> 5) & 0x3F ) << 10) |
      //                       ( ( colorx & 0x1F ) << 3);

      uint32_t color_value = ( ( colorx << 8 ) & 0xFF000000 ) |
                             ( ( colorx & 0x1F ) << 19) |
                             ( ( ( colorx >> 5) & 0x3F ) << 10) |
                             ( ( ( colorx >> 11) & 0x1F ) << 3);

      uint32_t x = (j % 4) + img_x + tile_x;
      uint32_t y = (j / 4) + img_y + tile_y;
      if (x < width && y < height) {
        out_pixels[x + (y * width)] = color_value;
      }
    }
    tile_x += 4;
    if( tile_x > 7) {
      tile_x = 0;
      tile_y += 4;
    }
    if( tile_y > 7) {
      tile_y = 0;
      img_x += 8;
    }
    if( img_x >= width) {
      img_x = 0;
      img_y += 8;
    }
    if (img_y > height) break;
  }
}

void flip_alpha(char* img_data, size_t pixelcount) {
  for (size_t i = 0; i < pixelcount; ++i) {
    img_data += 3;
    *img_data = 0xff - (*img_data & 0xff);
    img_data += 1;
  }
}

int decode_textureblock( uchar * fbegin, uchar * bstart, std::ostream & fout ){

	//Do something

	unsigned char * b = bstart;

	unsigned int xsize = UChar2ToUIntHL( b + 0 );
	unsigned int ysize = UChar2ToUIntHL( b + 2 );
	unsigned int offset = UChar4ToUIntHL( b + 40 ) + 0x40;

	fout<<"\tTex: "<<xsize<<", "<<ysize<<"\t@"<< UInt4ToString(offset) <<std::endl;

	fout<<"\t\t"<<UCharsToString( bstart + 0, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x10, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x20, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x30, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x40, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x50, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x60, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x70, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x80, 16 ) <<std::endl;
	fout<<"\t\t"<<UCharsToString( bstart + 0x90, 16 ) <<std::endl;


	//
	//Textures in pokemon are 'packed', so that all textures in a single file
	//are block packed into a single texture image; meaning the output image is large.
	//It is an odd idea, but that does mean there is information here about WHERE in the large
	//page to put the texture, as well as compression method and size.
	//
	//Or, at least the model data looks like this in the UV coords.
	//

	/*
	//? Type? Hm.
	unsigned int imgtype = UChar2ToUIntHL( b + 4 );
	unsigned int imglen = 0;
	switch( imgtype ){

		imglength = (size_x*size_y)/2;	//Hurr.
	}

	if( imglength > 0 ){

		unsigned char * imgbase = fbegin + offset;
		unsigned char * decoded_data = new unsigned char [imglength];

		switch( imgtype ){	//Guessing on image type

			case 0x00:	img_I4( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
			case 0x01:	img_I8( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
			case 0x02:	img_IA4( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
			case 0x04:	img_RGB565( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
			case 0x05:	img_RGB5A3( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
			case 0x06:	img_RGBA8( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
			case 0x0e: img_CMP( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;

			case 0x03:	//#ERROR, this isn't right!
				img_IA8( (char*)imgbase, imglength, size_x, size_y, (uint32_t*)decoded_data);	break;
				fout<<"\t#ERROR Incorrect compression 0x03"<<std::endl;
			case 0x08:
				{
				//palette_t p = palettes[base_fn];
				//assert(p.data);
				//img_CI4(imgbase, imglength, w, h, p.data, decoded_data);
				}
				fout<<"\t#ERROR Palette image 0x08"<<std::endl;
				break;
			case 0x09:
				{
				//palette_t p = palettes[base_fn];
				//assert(p.data);
				//img_CI8(imgbase, imglength, w, h, p.data, decoded_data);
				}
				fout<<"\t#ERROR Palette image 0x09"<<std::endl;
				break;
			default:
				fout<<"\t#ERROR Unknown image type: "<<imgtype<<std::endl;
				break;
		  }

		  //
		  //Now save decoded data!
		  //

		  flip_alpha((char*)decoded_data, size_x * size_y);

		  char buffy[32];
		  sprintf( buffy, "_%d_%d\0", curr_image, texture_page_count );

		  std::string imgname =  (filename + (std::string)buffy + ".tga");

			texnames.push_back( imgname );

		  saveTGA( imgname.c_str(), size_x, size_y, (uint32_t*)decoded_data );

		  delete [] decoded_data;
	}
	*/

	/*

	uchar * p = fbegin;

	if( UCharCompare( p, "TEX0", 4 ) ){

		p += 4;	//Skip TEX0

		unsigned int length = UChar4ToUIntHL( p );
		p += 4;		//read length

		p += 12;	//Skip some more bytes...

		uchar * base_fn = fbegin + UChar4ToUIntHL(p);	//Base filename
		p += 4;		//read filename
		uchar out_fn[ strlen( (char*)base_fn ) + 4 ];	//Out filename

		sprintf( (char*)out_fn, "%s.tga", (char*)base_fn);

		p += 4;		//skip some zeroes

		uint imgw = UChar2ToUIntHL( p ); p += 2;
		uint imgh = UChar2ToUIntHL( p ); p += 2;
		uint imgtype = UChar4ToUIntHL( p ); p += 4;

		uchar * decoded_data = new uchar[ imgw * imgh * 4 ];

		fout<<"\tTex: "<<out_fn<<" w:"<<imgw<<" h:"<<imgh<<" "<<imgtype<<std::endl;

		uchar * imgbase = fbegin + 64;	//Start of image data (header = 64)

		if( length > 64 ){

			uint imglength = length - 64;

			switch( imgtype ){
				case 0x00:
					img_I4( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x01:
					img_I8( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x02:
					img_IA4( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x03:
					//#ERROR, this isn't right!
					img_IA8( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x04:
					img_RGB565( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x05:
					img_RGB5A3( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x06:
					img_RGBA8( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				case 0x08:
					{
					//palette_t p = palettes[base_fn];
					//assert(p.data);
					//img_CI4(imgbase, imglength, w, h, p.data, decoded_data);
					}
					fout<<"\t#ERROR Palette image 0x08"<<std::endl;
					break;
				case 0x09:
					{
					//palette_t p = palettes[base_fn];
					//assert(p.data);
					//img_CI8(imgbase, imglength, w, h, p.data, decoded_data);
					}
					fout<<"\t#ERROR Palette image 0x09"<<std::endl;
					break;
				case 0x0e:
					img_CMP( (char*)imgbase, imglength, imgw, imgh, (uint32_t*)decoded_data);
					break;
				default:
					fout<<"\t#ERROR Unknown image type: "<<imgtype<<std::endl;
					break;
			  }

			  //
			  //Now save decoded data!
			  //

			  flip_alpha((char*)decoded_data, imgw * imgh);

			  saveTGA( (char*)out_fn, imgw, imgh, (uint32_t*)decoded_data );

		}else{

			fout<<"\t#ERROR Invalid image!"<<std::endl;
		}

		delete [] decoded_data;
	}
	*/

	return 0;
}




inline void cutdown_size( unsigned int & V, const unsigned int & maxsize = 65536 ){
	if( V > maxsize ){ V &= 16777215; if( V > maxsize ){ V &= 65535; if( V > maxsize ){ V &= 255; } } }
}

int decompress_pokemon_LZSS( unsigned char * inbuf, const unsigned int & insize, unsigned char * outbuf, const unsigned int & outsize ){

	unsigned int total = 0;

	//memcpy( outbuf, inbuf, insize ); total = insize;

	//
	//This LZSS is super simple and totally logical.
	//

	//The LZ buffer (output is written to this as well.)
	unsigned int lzsize = 4096;	//Pick a buffer size (correct)
	unsigned char lzbuff[ lzsize ];
	memset( lzbuff, 0, lzsize );	//Initialize buffer (some use dictionaries)
	unsigned int lzpos = 4078;//4078 Is starting position right? (4096 - 18) This is critical since the data directly references the table
	lzpos  %= lzsize;

	unsigned char * r = inbuf;
	unsigned char * r_final = inbuf + insize;
	unsigned char * w = outbuf;
	unsigned char * w_final = outbuf + outsize;

	while( r < r_final ){

		unsigned int flags = *r; r++;	//Read LZSS flags

		unsigned int count = 0;
		while( count < 8 ){

			if( (flags & 0x01) != 0 ){	//If flag is correct type

				unsigned char wchar = *r; r++;

				lzbuff[ lzpos ] = wchar; lzpos++; lzpos %= lzsize;//Write to lz buffer before outputting

				*w = wchar; w++; total++;//Direct copy 1 byte
			}else{

				//LZSS RLE time

				unsigned int byte0 = *r; r++;	//Get the next two bytes
				unsigned int byte1 = *r; r++;

				unsigned int bcount = (byte1 & 15) + 3;	//Count bits (correct)
				unsigned int offset = ((byte0)&255) + ((byte1<<4)&3840);//Offset (correct)

				unsigned int bufpos = offset + 0;	// +?

				bufpos %= lzsize;

				while( bcount > 0 ){
					bcount--;

					unsigned char wchar = lzbuff[ bufpos ]; bufpos++; bufpos %= lzsize;

					lzbuff[ lzpos ] = wchar; lzpos++; lzpos %= lzsize;//Write to lz buffer

					*w = wchar; w++; total++;
				}

				if( w >= w_final ){ break; }
			}

			flags >>= 1;	//Direction of flag reading? #ERROR

			count++;
		}

		if( r >= r_final ){ break; }
		if( w >= w_final ){ break; }
	};

	return total;
}


int parse_pokemon( unsigned char * inbuf, const unsigned int & insize, const std::string & filename, const unsigned int & curr_entry_version, std::ostream & fout ){

	//? file directory?
	std::string dirpath = "";
	std::string filenamed = "";
	std::string::const_iterator is = filename.end();
	std::string::const_iterator is_begin = filename.begin();
	while( is != is_begin ){
		is--;

		if( *is == '/' ){

			dirpath.assign( filename.begin(), is+1 );
			filenamed.assign( is+1, filename.end() );
			break;
		}else{

		}
	}

	//Generate default palette? Hm.
	unsigned char palette[256*4];	//RGBA palette.

	unsigned int ic = 0;
	while( ic < 256 ){

		unsigned char * rgba = palette + (ic << 2);

		double h,s,v,r,g,b;
		s = (((float)((ic>>5)&3)) / 3.0f);//2 bits for saturation level
		v = 0.5 + 0.5*((ic>>7)&1);	//1 bit dark/light colors
		h = (((float)(ic&31)) / 31.0f);	//5 bits of color hues
		HSVtoRGB( h, s, v, &r, &g, &b );

		rgba[0] = 255*r;
		rgba[1] = 255*g;
		rgba[2] = 255*b;
		rgba[3] = ic;

		ic++;
	}

	//
	//Convert this file (unknown type) into a 3D model; include animations if possible.
	//

	unsigned int header_numtexblocks = UChar2ToUIntHL( inbuf + 26 );
	unsigned int header_starttextable = UChar4ToUIntHL( inbuf + 12 );
	unsigned int header_offsetnametable = UChar4ToUIntHL( inbuf + 20 );
	unsigned int header_materialcount = UChar2ToUIntHL( inbuf + 30 );
	unsigned int header_meshcount = 1;	//GAH

	if( header_offsetnametable > insize ){ return -1; }

	//
	//Get some quick stats about da file:
	//
	unsigned int offset_ptrto_nametable = UChar4ToUIntHL( inbuf + 8 );
	if( offset_ptrto_nametable > insize ){ return -2; }
	unsigned int offset_nametable = UChar4ToUIntHL( inbuf + offset_ptrto_nametable );
	if( offset_nametable > insize ){ return -3; }
	unsigned int offset_name = UChar4ToUIntHL( inbuf + offset_nametable );
	if( offset_name > insize ){ return -4; }

	unsigned char * rn = inbuf + offset_name;
	std::string pokemon_name = "";
	while( *rn != 0 ){ pokemon_name += *rn; rn++; }

	fout<<"Pokemon Name: "<< pokemon_name <<std::endl;

	//
	//Save all textures; But don;t repeatedly save them if you can avoid it.
	//
	std::map< unsigned int, std::string > texptr_to_filename;
	std::map< unsigned int, std::string > texoffset_to_actualname;
	unsigned char * b = inbuf + header_starttextable;
	unsigned int currtex = 0;
	unsigned int netoffset = 64;
	while( currtex < header_numtexblocks ){

		unsigned int texptr = UChar4ToUIntHL( b ); b += 4;

		unsigned int texture_w = UChar2ToUIntHL( inbuf + texptr + 0 );
		unsigned int texture_h = UChar2ToUIntHL( inbuf + texptr + 2 );
		unsigned int texture_offset = UChar4ToUIntHL( inbuf + texptr + 40 ) + netoffset;

		netoffset += 160;	//Fix the texture offsets. How strange!

		if( texoffset_to_actualname.find( texture_offset ) != texoffset_to_actualname.end() ){

			//Don;t care
		}else{

			unsigned int texture_compression = UChar2ToUIntHL( inbuf + texptr + 4 );

			unsigned int texture_format = UChar4ToUIntHL( inbuf + texptr + 8 );

			unsigned int texture_format_0 = UChar4ToUIntHL( inbuf + texptr + 16 );	//Format values
			unsigned int texture_format_1 = UChar4ToUIntHL( inbuf + texptr + 20 );
			unsigned int texture_format_2 = UChar4ToUIntHL( inbuf + texptr + 24 );
			unsigned int texture_format_3 = UChar4ToUIntHL( inbuf + texptr + 28 );
			unsigned int texture_compression_format = UChar4ToUIntHL( inbuf + texptr + 76 );

			unsigned int imagelength = 0;

			//Generate filenames
			char buffy[256];
			sprintf( (char*)buffy, "%d_%d", curr_entry_version, currtex);

			std::string texpath = (dirpath + (std::string)"obm/");
			std::string texname = ("tex/" + pokemon_name + (std::string)"_" + (std::string)buffy + (std::string)".tga");

			texoffset_to_actualname[ texture_offset ] = texname;

			fout<<"\tTex: "<<texture_w<<", "<<texture_h<<"\t@"<< UInt4ToString(texture_offset)<<"\t"<<UInt4ToString(texture_compression)<<"\t"<<UInt4ToString(texture_compression_format)<<"\t"<<texname <<std::endl;
			fout<<"\t\tFormat: "<< UInt4ToString(texture_format) <<" "<<texture_format_0 <<" "<<texture_format_1<<" "<<texture_format_2<<" "<<texture_format_3<<std::endl;
			//Determine data length
			switch( texture_format ){

				//case 0x30:
				//	imagelength =  (texture_w * texture_h)*2;//? compression
				//	break;

				case 0x41:
					imagelength =  (texture_w * texture_h)*2;//img_A1 compression
					break;

				case 0x40:
					imagelength =  (texture_w * texture_h)/2;//img_I4 compression
					break;
				case 0x42:
					imagelength =  (texture_w * texture_h);//img_I8 compression
					break;
				case 0x43:
					imagelength =  (texture_w * texture_h)*2;//img_IA8 compression
					break;
				case 0x44:
					imagelength =  (texture_w * texture_h)*2;//BGR565 compression?
					break;
				case 0x45:
					imagelength =  (texture_w * texture_h)*2;//RGB565 compression
					break;
				case 0x90:
					imagelength =  (texture_w * texture_h)*2;//BGR565 compression
					break;
				case 0xB0:
					imagelength =  (texture_w * texture_h)/2;//CMP compression
					break;

				default:
					fout<<"\tUnknown image compression: "<< UInt4ToString(texture_format)<< std::endl;

					unsigned char * tr = ( inbuf + texptr );
					fout<<"\t\t"<<UCharsToString( tr + 0, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x10, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x20, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x30, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x40, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x50, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x60, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x70, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x80, 16 ) <<std::endl;
					fout<<"\t\t"<<UCharsToString( tr + 0x90, 16 ) <<std::endl;

					break;
			}

			//Files with bad textures:
			//
			//barubeat
			//agehunt
			//illumise
			//manaphy
			//migawari
			//usohachi
			//
			//regigigas -> 0x30
			//... looks like quite a few are good! Huh.

			//Unknown image compressions:
			//0x45	pkx_203	pkx_153 pkx_228
			//0x41	pkx_324 pkx_402

			//Save image if valid
			if( imagelength > 0 ){

				unsigned char * dstimag = new unsigned char [ 4 * texture_w * texture_h + 256 ];	//Add in safety buffer, just in case.

				switch( texture_format ){

					//case 0x45: ??	pkx_490
					//case 0x30:
					//	img_IA8( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
					//	//Hm, not sure. #ERROR
					//	break;

					case 0x40:
						img_I4( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);//??
						break;
					case 0x41:
						img_I8( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);//??
						break;
					case 0x42:
						img_I8( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
						break;
					case 0x43:
						img_IA8( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
						break;
					case 0x44:
						img_RBG565( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
						//? colors are wrong! Sometimes!
						break;
					case 0x45:
						img_RGB565( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);//??
						break;
					case 0x90:
						img_RGB5A3( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
						break;
					case 0xB0:
						//img_CMPBGR( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
						img_CMPRGB( (char*)( inbuf + texture_offset), imagelength, texture_w, texture_h, (uint32_t*)dstimag);
						break;
					default:
						break;
				}

				flip_alpha((char*)dstimag, texture_w * texture_h);

				saveTGA( (texpath + texname).c_str(), texture_w, texture_h, (uint32_t*)dstimag );

				delete [] dstimag;
			}
		}

		texptr_to_filename[ texptr ] = texoffset_to_actualname[ texture_offset ];

		currtex++;
	}

	//header_meshcount! - Hm.
	/*
	b = 64 + inbuf + 160 * header_numtexblocks;
	unsigned int currmeshblock = 0;
	while( currmeshblock < header_meshcount ){

		fout<<"Mesh Block @"<<UInt4ToString(b - inbuf)<<std::endl;

		unsigned char * tr = b;
		fout<<"\t\t"<<UCharsToString( tr + 0, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x10, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x20, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x30, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x40, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x50, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x60, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x70, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x80, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0x90, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0xA0, 16 ) <<std::endl;
		fout<<"\t\t"<<UCharsToString( tr + 0xB0, 16 ) <<std::endl;

		b += 192;
		currmeshblock++;
	}
	*/

	b = inbuf;

	//
	//Read in action tables: (animations ?)
	//
	unsigned int action_count = UChar2ToUIntHL( inbuf + offset_nametable + 8 );
	unsigned int offset_actiontable = UChar4ToUIntHL( inbuf + offset_nametable + 12 );

	b = inbuf + offset_actiontable;
	while( action_count > 0 ){
		action_count--;

		unsigned int nameoffset = UChar4ToUIntHL( b );

		unsigned char * rn = inbuf + nameoffset;
		std::string action_name = "";
		while( *rn != 0 ){ action_name += *rn; rn++; }

		//b now is at the start of the 48 byte action header, full of data I have no idea what it is.

		fout<<"\tAction: "<<action_name<<std::endl;

		b += 48;
	}

	unsigned int file_table_offset = UChar4ToUIntHL( inbuf + 20 );

	//00 00 02 F0 00 00 03 C4 00 00 0C 84 00 00 0D 58 0A 00 04 9F 00 00 00 02 00 00 0E 60 00 00 7D 48 00 00 00 00 00 00 81 5C 00 00 AC 5C 00 01 10 1C 00 00 00 00
	//->00 00 00 02 00 00 0E 60
	//H: 00 01 00 04 00 01 00 04

	//00 00 01 88 00 00 02 4C 0A 00 02 4A 00 00 00 01 00 00 09 60 00 00 40 50 00 00 00 00 00 00 44 5C 00 00 5C 9C 00 00 88 3C 00 00 00 00
	//->00 00 00 01 00 00 09 60
	//H: 00 01 00 02 00 00 00 02

	//00 00 07 4C 00 00 08 08 00 00 08 C4 00 00 09 80 00 00 0A 3C 00 00 0A F8 00 00 0B B4 00 00 13 A0 00 00 14 5C 00 00 15 18 00 00 15 D4 0A 00 02 14 00 00 00 01 00 00 16 E0 00 00 48 C0 00 00 00 00 00 00 49 50 00 00 60 20 00 00 8F 1C 00 00 00 00
	//->00 00 00 01 00 00 16 E0
	//H: 00 01 00 0B 00 00 00 0B

	//00 00 07 F0 00 00 08 A8 00 00 0D F0 00 00 0E A8 00 00 0F 60 00 00 10 18 00 00 10 B0 00 00 11 68 00 00 12 20 00 00 12 D8 00 00 13 94 00 00 14 50 00 00 15 0C 0A 00 01 B0 00 00 00 01 00 00 16 20 00 00 3E A0 00 00 00 00 00 00 3F 68 00 00 53 28 00 00 7E DC 00 00 00 00
	//->00 00 00 01 00 00 16 20
	//H: 00 01 00 0C 00 00 00 0D

	//Somewhere in this file table is information about the VA present. Hm.
	//It always seems to have a 0xA0?????? BEFORE a number, then the pointer to the VA... Perhaps that is the # verts?

	unsigned int vertexarray_offset = 0;
	unsigned int vertexarray_verticies = 0;
	unsigned int vertexarray_format = 0;
	unsigned int vertexarray_formnumber = 0;

	unsigned int vertexodd_offset = 0;
	unsigned int vertexodd_unknown = 0;

	unsigned int vertexuv_offset = 0;
	unsigned int vertexuv_size = 0;

	unsigned int vertexconn_offset = 0;

	unsigned int vertexnext_offset = 0;

	//0A 00 02 4A -> size of verticies
	//00 00 00 01 -> format flag, I suppose
	//00 00 09 60 -> pointer to VA data
	//00 00 40 50 -> points to start of that weird data; unknown what this is.
	//00 00 00 00
	//00 00 44 5C -> points to location that stores pointer to UV array!, this + 4 => size of UV array! hooray!
	//00 00 5C 9C -> points to 36(?) bytes, the 20th(?) has the pointer to start of index connections(?)
	//00 00 88 3C -> ? points to the start of some sort of intense animation data and table (quah?)

	fout<<"\tMaterial Table: "<<UInt4ToString(file_table_offset)<<std::endl;

	b = inbuf + file_table_offset;

	unsigned int currmaterial = 0;
	while( currmaterial < header_materialcount ){

		unsigned int matoffset = UChar4ToUIntHL( b ); b += 4;
		unsigned int matnameoffset = UChar4ToUIntHL( inbuf + matoffset );

		unsigned char * rn = inbuf + matnameoffset;
		std::string material_name = "";
		while( *rn != 0 ){ material_name += *rn; rn++; }

		//Pointers to something important? huh.	#ERROR, probably material groups or IA's or somethin.
		//currvalue = 1;

		//This points to a location that possibly points to the name of a material;

		fout<<"\t\tMaterial: "<<material_name<<"\t@"<<UInt4ToString(matoffset)<<"\t"<<matoffset<<"\t"<<matnameoffset<<std::endl;

		unsigned char * rdz = inbuf + matoffset + 4;
		fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		//fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		//fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;
		//fout<<"\t\t\t"<<UCharsToString( rdz, 16 )<<std::endl; rdz += 16;

		//#ERROR, lots of data in these blocks; Loos like pointers and colors maybe? Or more parameters?

		currmaterial++;
	}

	//Hm.
	vertexnext_offset = file_table_offset + 4*header_materialcount;

	//
	//Temporary file output direct;
	//
	char buffy[256];
	sprintf( buffy, "_%d", curr_entry_version );
	std::string modelname = (dirpath + "obm/" + pokemon_name + (std::string)buffy + ".obm");
	fout<<"Model out: "<<modelname<<" :"<<std::endl;
	std::ofstream obmout( modelname.c_str() , std::ios::out | std::ios::binary );
	if( obmout.is_open() ){

		obmout<< "o "<< pokemon_name << std::endl;

		unsigned int vertex_total = 0;
		unsigned int normal_total = 0;
		unsigned int texcoord_total = 0;
		unsigned int weight_total = 0;
		unsigned int curr_group = 0;



		while( curr_group < header_meshcount ){

			unsigned int curr_vertex_total = vertex_total;
			unsigned int curr_normal_total = normal_total;
			unsigned int curr_texcoord_total = texcoord_total;
			unsigned int curr_weight_total = weight_total;

			//
			//Read vertex data parameters
			//
			b = inbuf + vertexnext_offset;

			//header_meshcount = 0; //Quick fix for safety (UUGGGHHUUGHGH #ERROR)

			//#ERROR WARNING; Next offset may be pointing to the start of the keyframe table!!!
			//#ERROR WARNING; Next offset may be pointing to garbage!

			//Files with multi-models:
			//383 groudon
			// foodin
			//160 ordile
			//208 haganeil

			//Read next header: (Hm.)


			//if next header b[0] == 0x0A :
			//	Mesh data (as normal)
			//else:
			//	Keyframe data ? (break)

			unsigned int vh_num_keyframes = UChar2ToUIntHL( b );
			unsigned int vh_num_unknown = UChar2ToUIntHL( b + 2 );
			unsigned int vh_typer = UChar4ToUIntHL( b + 4 );
			unsigned int vh_offset = UChar4ToUIntHL( b + 8 );
			//b + 12 => start of data?

			fout<<"Next Data Header @"<<UInt4ToString( vertexnext_offset )<<std::endl;
			fout<<"\tType:"<<UInt4ToString( vh_num_keyframes )<<std::endl;
			fout<<"\tUnknown:"<<UInt4ToString( vh_num_unknown )<<std::endl;
			fout<<"\tTyper:"<<UInt4ToString( vh_typer )<<std::endl;
			fout<<"\tOffset:"<<UInt4ToString( vh_offset )<<std::endl;
			fout<<"\tP24:"<<UCharsToString( b - 24, 24 )<<std::endl;

			/*
			//Vertex
			Next Data Header @00000C30
				Type:00000A00
				Unknown:0000016B
				Offset:00000001
				Zeroes:00000C60
			Next Data Header @00000C3C
				Type:00000A00
				Unknown:000001EF
				Offset:00000001
				Zeroes:00000C60

			//KEyframes
			Next Data Header @00006154
				Type:000000XX
				Unknown:00000000
				Offset:00006160
				Zeroes:00000000
			Next Data Header @00007394
				Type:000000XX
				Unknown:00000000
				Offset:000073A0
				Zeroes:00000000

			*/

			if( b[0] == 0x0A ){
				//This is good
			}else if( b[0] == 0x00 ){
				//This is keyframes? Or is this the next VA?
				//Don;t know how to tell yet. #ERROR
			}

			if( b[0] != 0x0A ){	//Hax for Kamex/009

				//Not consistent at all... damn.
				if( (UChar4ToUIntHL( b ) == 0) && (UChar4ToUIntHL( b + 4 ) == 0) ){

					unsigned char * nb = b + 8;
					unsigned char * nb_final = nb + 256;

					while( nb[0] != 0x0A ){

						nb += 4;
						if( nb > nb_final ){ break; }
					}
					if( nb > nb_final ){ break; }
					b = nb;
				}else{

					break;
				}
			}


			//First->
			//			[verts] [form?]    [offset va] [ ?       ] [?        ] [uv offset ][ia offset ] [next offset?]
			//009	0A 00 00 4A 00 00 00 01 00 00 0E 80 00 00 00 00 00 00 00 00 00 00 15 70 00 00 17 C8 00 00 1B DC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
			//006	0A 00 02 4A 00 00 00 01 00 00 09 60 00 00 40 50 00 00 00 00 00 00 44 5C 00 00 5C 9C 00 00 88 3C 00 00 00 00
			//001	0A 00 01 6B 00 00 00 01 00 00 0C 60 00 00 2E 68 00 00 00 00 00 00 31 FC 00 00 43 64 00 00 61 3C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

			//Next->
			//		[ header .............................................................] [ ?       ] [ani offst] ...
			//Keyframe tables:
			//001	C0 2B 28 05 BE 55 32 67 C0 94 39 DA 40 4C 01 84 40 F8 AB AC 40 97 9E EE 00 0B 00 00 00 00 61 60 00 00 00 00 00 30 1E 00 00 00 61 B8 00 5F 1E 00 00 00 66 38 00 5A 1E 00 00 00 6F 20 00 69 1E 00
			//006	C1 19 C1 3D 41 82 51 A8 C1 AC 7A 27 40 F9 51 16 42 29 E7 10 41 65 96 8F 00 0F 00 00 00 00 88 60 00 00 00 00 00 3B 1E 00 00 00 88 D8 00 59 1E 00 00 00 8E 60 00 45 1E 00 00 00 96 B8 00 54 1E 00

			//New mesh data table:
			//009	BF 30 43 F6 BF 2F D9 12 BF F0 6C 22 3F 37 1D 0D 3F 31 06 CD 3D 62 44 63 00 00 00 00 00 00 00 00 00 00 00 00 0A 00 00 4A 00 00 00 01 00 00 1C 40 00 00 00 00 00 00 00 00 00 00 23 30 00 00 25 88


			vertexarray_format = ( (UChar4ToUIntHL( b )>>16) & 65535 );
			vertexarray_verticies = (UChar4ToUIntHL( b ) & 65535); b += 4;
			vertexarray_formnumber = UChar4ToUIntHL( b ); b += 4;
			vertexarray_offset = UChar4ToUIntHL( b ); b += 4;

			//00 00 46 78  00 00 00 00  00 00 4B 54  00 00 54 DC  00 00 69 1C  ???

			vertexodd_offset = UChar4ToUIntHL( b ); b += 4;
			vertexodd_unknown = UChar4ToUIntHL( b ); b += 4;
			vertexuv_offset = UChar4ToUIntHL( b ); b += 4;
			vertexuv_size = UChar2ToUIntHL( inbuf + vertexuv_offset + 4 );	//Get uv array size
			vertexuv_offset = UChar4ToUIntHL( inbuf + vertexuv_offset );	//Get actual offset pointer

			vertexconn_offset = UChar4ToUIntHL( b ); b += 4;
			vertexnext_offset = UChar4ToUIntHL( b ) + 24;	//uhh?

			if( vertexarray_verticies > 0 ){

				fout<<"\tVertex Data Table "<<curr_group<<" :"<<std::endl;


				fout<<"\t\tvertexarray_format: "<<UInt4ToString(vertexarray_format)<<std::endl;
				fout<<"\t\tvertexarray_verticies: "<<UInt4ToString(vertexarray_verticies)<<std::endl;
				fout<<"\t\tvertexarray_formnumber: "<<UInt4ToString(vertexarray_formnumber)<<std::endl;
				fout<<"\t\tvertexodd_offset: "<<UInt4ToString(vertexodd_offset)<<std::endl;
				fout<<"\t\tvertexodd_unknown: "<<UInt4ToString(vertexodd_unknown)<<std::endl;
				fout<<"\t\tvertexuv_size: "<<UInt4ToString(vertexuv_size)<<std::endl;
				fout<<"\t\tvertexuv_offset: "<<UInt4ToString(vertexuv_offset)<<std::endl;
				fout<<"\t\tvertexconn_offset: "<<UInt4ToString(vertexconn_offset)<<std::endl;
				fout<<"\t\tvertexnext_offset: "<<UInt4ToString(vertexnext_offset)<<std::endl;


				fout<<"\t\tVertex Array @"<<UInt4ToString(vertexarray_offset)<<": "<<vertexarray_verticies<<std::endl;
				fout<<"\t\tTexcoord Array @"<<UInt4ToString(vertexuv_offset)<<": "<<vertexuv_size<<std::endl;
				fout<<"\tIndexes @"<<UInt4ToString(vertexconn_offset)<<std::endl;
				fout<<"\t\tIndex Format: \t"<<UInt4ToString(vertexarray_format)<<std::endl;
				fout<<"\t\tIndex Form Number: \t"<<UInt4ToString(vertexarray_formnumber)<<std::endl;

				//Write V and N
				unsigned char * vptr = inbuf + vertexarray_offset;
				unsigned int vindex = 0;
				unsigned int vindex_final = vertexarray_verticies;
				while( vindex < vindex_final ){

					float vx = UChar4ToFloatHL( vptr ); vptr+= 4;	//Scale verticies to match 1m = 1 unit convention! (0.2 atm, probably needs more)
					float vy = UChar4ToFloatHL( vptr ); vptr+= 4;
					float vz = UChar4ToFloatHL( vptr ); vptr+= 4;

					float nx = UChar4ToFloatHL( vptr ); vptr+= 4;
					float ny = UChar4ToFloatHL( vptr ); vptr+= 4;
					float nz = UChar4ToFloatHL( vptr ); vptr+= 4;

					if( (vertexarray_format & 255) != 0 ){	//Rayquaza hack #ERROR

						vptr+= 24;	//These two extra verticies might be tangent and binormal... how strange.
					}

					sprintf( buffy, "v %0.6f %0.6f %0.6f\r\nvn %0.6f %0.6f %0.6f\r\n", vx, vy, vz, nx, ny, nz );
					obmout<< buffy;

					vertex_total++;
					normal_total++;
					vindex++;
				}

				//Write T
				vptr = inbuf + vertexuv_offset;
				vindex = 0;
				vindex_final = vertexuv_size;
				while( vindex < vindex_final ){

					float vu = UChar4ToFloatHL( vptr ); vptr+= 4;
					float vv = UChar4ToFloatHL( vptr ); vptr+= 4;

					//! Flip vv component since we flip the texture
					vv = 1.0 - vv;

					sprintf( buffy, "vt %0.6f %0.6f\r\n", vu, vv );
					obmout<< buffy;

					texcoord_total++;

					vindex++;
				}

				//unsigned int weight_total = 0;

				//Write faces (hm)

				fout<<"FaceHeader: \t"<<UCharsToString( inbuf + vertexconn_offset, 32 )<<std::endl;

				unsigned int nextblokoffset = vertexconn_offset;
				unsigned int currblock = 0;
				while( nextblokoffset != 0 ){

					fout<<"\tStrip Block @"<<UInt4ToString(nextblokoffset)<<std::endl;

					unsigned int striphdr_zeroes = UChar4ToUIntHL( inbuf + nextblokoffset + 4 );
					unsigned int striphdr_material = UChar4ToUIntHL( inbuf + nextblokoffset + 8 );
					unsigned int striphdr_type = UChar4ToUIntHL( inbuf + nextblokoffset + 12 );	//This isn;t the type. Not coorelated.
					unsigned int striphdr_unknownptr = UChar4ToUIntHL( inbuf + nextblokoffset + 16 );
					unsigned int striphdr_stripdataptr = UChar4ToUIntHL( inbuf + nextblokoffset + 20 );
					unsigned int striphdr_unknown = UChar4ToUIntHL( inbuf + nextblokoffset + 24 );
					unsigned int striphdr_stripnextdataptr = UChar4ToUIntHL( inbuf + nextblokoffset + 28 );

					vptr = inbuf + striphdr_stripdataptr;	//Hack; Might not work all the time.
					nextblokoffset = striphdr_stripnextdataptr;	//Hack; Might not work all the time.

					//g name		Face Group
					//fc r g b a	Face group color
					//ft name		Face group texture filename

					//unsigned int matnameoffset = UChar4ToUIntHL( inbuf + striphdr_material );
					//unsigned char * rn = inbuf + matnameoffset;
					//std::string material_name = "";
					//while( *rn != 0 ){ material_name += *rn; rn++; }

					sprintf( buffy, "mat_%d", currblock );
					std::string material_name = buffy;

					obmout<<"g "<< material_name <<std::endl;

					//
					//You can color regions, or just use textures; WE got this working, so just use textures.
					//
					//double dr,dg,db;
					//HSVtoRGB( ((float)(currblock % 12))/12.0f, 1.0, 1.0, &dr, &dg, &db );
					//obmout<<"fc "<<(int)(255*dr)<<" "<<(int)(255*dg)<<" "<<(int)(255*db)<<" 255"<<std::endl;

					obmout<<"fc 255 255 255 255"<<std::endl;

					unsigned int texptrofset = UChar4ToUIntHL( inbuf + striphdr_material + 24 );

					if( texptr_to_filename.find( texptrofset ) != texptr_to_filename.end() ){

						std::string texname = texptr_to_filename[ texptrofset ];
						obmout<<"ft "<<texname<<std::endl;
						fout<<"\t\tStrip Texture: "<<texname<<std::endl;
					}else{
						fout<<"\t\tStrip Textureless: "<<UInt4ToString(texptrofset)<<std::endl;
					}

					//Face data format
					obmout<<"ff v/n/t/w"<<std::endl;

					fout<<"\t\tStrip Block Next @"<<UInt4ToString( nextblokoffset )<<std::endl;
					fout<<"\t\tStrip Block Zeros: "<<UInt4ToString( striphdr_zeroes )<<std::endl;
					fout<<"\t\tStrip Block Material: "<<UInt4ToString( striphdr_material )<<std::endl;
					fout<<"\t\tStrip Block Type: "<<UInt4ToString( striphdr_type )<<std::endl;
					fout<<"\t\tStrip Block Unknown Data Block Pointer: "<<UInt4ToString( striphdr_unknownptr )<<std::endl;
					fout<<"\t\tStrip Block Indicies @"<<UInt4ToString( UChar4ToUIntHL( inbuf + nextblokoffset + 20 ) )<<std::endl;
					fout<<"\t\tStrip Block Unknown @"<<UInt4ToString( striphdr_unknown )<<std::endl;

					//
					//Material dictates vertex format? Possible.
					//

					//Vertex data parameters... Hm. Interesting.
					unsigned int vskip_v = 0;
					unsigned int vskip_n = 0;
					unsigned int vskip_t = 0;
					unsigned int vskip_b = 0;

					unsigned int vsize_v = 0;
					unsigned int vsize_n = 0;
					unsigned int vsize_t = 0;
					unsigned int vsize_b = 0;

					unsigned int vcsize = 0;

					//Determine vertex counts
					switch( vertexarray_formnumber ){

						//ptera is 1, 0A00 and is NOT this kind of format! Why??

						case 1:				//Lizardon, Lugia, Bangiras
							vcsize = 6;
							vskip_v = 0;
							vskip_n = 0;
							vskip_t = 4;
							vsize_v = 2;
							vsize_n = 2;
							vsize_t = 2;
							break;
						case 2:				//Gyrarados,
							vcsize = 8;
							vskip_v = 0;
							vskip_n = 0;
							vskip_t = 4;
							vsize_v = 2;
							vsize_n = 2;
							vsize_t = 2;
							break;
						case 3:				//Rayquaza
							vcsize = 10;
							vskip_v = 0;
							vskip_n = 0;
							vskip_t = 4;
							vsize_v = 2;
							vsize_n = 2;
							vsize_t = 2;
							break;
						default:
							fout<<"Unknown vertex format: "<<vertexarray_formnumber<<std::endl;
							break;
					}


					enum{
						VCACHE_VERTEX = 0,
						VCACHE_NORMAL = 1,
						VCACHE_BINORMAL = 2,
						VCACHE_WEIGHT = 3,
						VCACHE_TEXCOORD = 4,
						VCACHE_STRIDE = 8,
					};
					unsigned int vcache[ VCACHE_STRIDE * 4 ];	//Index cache


					//Do the thing
					bool broken = false;
					if( vcsize < 1 ){
						broken = true;
						fout<<"Unknown block type! "<<std::endl;
					}
					while( !broken ){

						unsigned char type = *vptr; vptr++;
						unsigned int vcount = UChar2ToUIntHL( vptr ); vptr += 2;
						unsigned char * vptr_final = vptr + vcount * vcsize;

						unsigned int vcachecurr = 0;
						unsigned int vcachetoggle = 0;

						if( vcount > 0 ){

							switch( type ){

								case 0:	//Normal termination.
									broken = true;
									break;

								case 0x98://Triangle strip

									while( vcount != 0 ){
										vcount--;

										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_VERTEX ] = UCharNToUIntHL( vptr + vskip_v, vsize_v );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_NORMAL ] = UCharNToUIntHL( vptr + vskip_n, vsize_n );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_TEXCOORD ] = UCharNToUIntHL( vptr + vskip_t, vsize_t );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_BINORMAL ] = UCharNToUIntHL( vptr + vskip_b, vsize_b );

										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_VERTEX ] > vertexarray_verticies ){ fout<<"Bad Vertex Format for verticies!"<<std::endl; broken = true; break; }
										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_NORMAL ] > vertexarray_verticies ){ fout<<"Bad Vertex Format for normals!"<<std::endl; broken = true; break; }
										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_TEXCOORD ] > vertexuv_size ){ fout<<"Bad Vertex Format for texcoords!"<<std::endl; broken = true; break; }

										vcachecurr++;
										if( vcachecurr == 3 ){

											if( vcachetoggle == 0 ){

												//2,1,0
												obmout<<"f ";
												obmout<<curr_vertex_total + vcache[ 2*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
												obmout<<curr_normal_total + vcache[ 2*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
												obmout<<curr_texcoord_total + vcache[ 2*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
												obmout<<" ";
												obmout<<curr_vertex_total + vcache[ 1*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
												obmout<<curr_normal_total + vcache[ 1*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
												obmout<<curr_texcoord_total + vcache[ 1*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
												obmout<<" ";
												obmout<<curr_vertex_total + vcache[ 0*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
												obmout<<curr_normal_total + vcache[ 0*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
												obmout<<curr_texcoord_total + vcache[ 0*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
												obmout<<std::endl;

											}else{

												//1,2,0
												//obmout<<"f v/n/t/w v/n/t/w v/n/t/w"<<std::endl;
												obmout<<"f ";
												obmout<<curr_vertex_total + vcache[ 1*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
												obmout<<curr_normal_total + vcache[ 1*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
												obmout<<curr_texcoord_total + vcache[ 1*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
												obmout<<" ";
												obmout<<curr_vertex_total + vcache[ 2*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
												obmout<<curr_normal_total + vcache[ 2*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
												obmout<<curr_texcoord_total + vcache[ 2*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
												obmout<<" ";
												obmout<<curr_vertex_total + vcache[ 0*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
												obmout<<curr_normal_total + vcache[ 0*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
												obmout<<curr_texcoord_total + vcache[ 0*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
												obmout<<std::endl;
											}

											//Stack down
											vcache[ 0*VCACHE_STRIDE + VCACHE_VERTEX ] = vcache[ 1*VCACHE_STRIDE + VCACHE_VERTEX ];
											vcache[ 0*VCACHE_STRIDE + VCACHE_NORMAL ] = vcache[ 1*VCACHE_STRIDE + VCACHE_NORMAL ];
											vcache[ 0*VCACHE_STRIDE + VCACHE_TEXCOORD ] = vcache[ 1*VCACHE_STRIDE + VCACHE_TEXCOORD ];
											vcache[ 0*VCACHE_STRIDE + VCACHE_BINORMAL ] = vcache[ 1*VCACHE_STRIDE + VCACHE_BINORMAL ];

											vcache[ 1*VCACHE_STRIDE + VCACHE_VERTEX ] = vcache[ 2*VCACHE_STRIDE + VCACHE_VERTEX ];
											vcache[ 1*VCACHE_STRIDE + VCACHE_NORMAL ] = vcache[ 2*VCACHE_STRIDE + VCACHE_NORMAL ];
											vcache[ 1*VCACHE_STRIDE + VCACHE_TEXCOORD ] = vcache[ 2*VCACHE_STRIDE + VCACHE_TEXCOORD ];
											vcache[ 1*VCACHE_STRIDE + VCACHE_BINORMAL ] = vcache[ 2*VCACHE_STRIDE + VCACHE_BINORMAL ];

											vcachecurr = 2;
											vcachetoggle = !vcachetoggle;
										}

										vptr += vcsize;
									}
									break;

								case 0x90://Triangles

									while( vcount != 0 ){
										vcount--;

										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_VERTEX ] = UCharNToUIntHL( vptr + vskip_v, vsize_v );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_NORMAL ] = UCharNToUIntHL( vptr + vskip_n, vsize_n );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_TEXCOORD ] = UCharNToUIntHL( vptr + vskip_t, vsize_t );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_BINORMAL ] = UCharNToUIntHL( vptr + vskip_b, vsize_b );

										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_VERTEX ] > vertexarray_verticies ){ fout<<"Bad Vertex Format for verticies!"<<std::endl; broken = true; break; }
										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_NORMAL ] > vertexarray_verticies ){ fout<<"Bad Vertex Format for normals!"<<std::endl; broken = true; break; }
										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_TEXCOORD ] > vertexuv_size ){ fout<<"Bad Vertex Format for texcoords!"<<std::endl; broken = true; break; }

										vcachecurr++;
										if( vcachecurr == 3 ){

											obmout<<"f ";
											obmout<<curr_vertex_total + vcache[ 2*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 2*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 2*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<" ";
											obmout<<curr_vertex_total + vcache[ 1*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 1*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 1*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<" ";
											obmout<<curr_vertex_total + vcache[ 0*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 0*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 0*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<std::endl;

											vcachecurr = 0;
										}

										vptr += vcsize;
									}
									break;

								case 0x80://Single quads? Uhm.

									while( vcount != 0 ){
										vcount--;

										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_VERTEX ] = UCharNToUIntHL( vptr + vskip_v, vsize_v );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_NORMAL ] = UCharNToUIntHL( vptr + vskip_n, vsize_n );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_TEXCOORD ] = UCharNToUIntHL( vptr + vskip_t, vsize_t );
										vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_BINORMAL ] = UCharNToUIntHL( vptr + vskip_b, vsize_b );

										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_VERTEX ] > vertexarray_verticies ){ fout<<"Bad Vertex Format for verticies!"<<std::endl; broken = true; break; }
										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_NORMAL ] > vertexarray_verticies ){ fout<<"Bad Vertex Format for normals!"<<std::endl; broken = true; break; }
										if( vcache[ vcachecurr*VCACHE_STRIDE + VCACHE_TEXCOORD ] > vertexuv_size ){ fout<<"Bad Vertex Format for texcoords!"<<std::endl; broken = true; break; }

										vcachecurr++;
										if( vcachecurr == 4 ){

											obmout<<"f ";
											obmout<<curr_vertex_total + vcache[ 2*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 2*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 2*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<" ";
											obmout<<curr_vertex_total + vcache[ 1*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 1*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 1*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<" ";
											obmout<<curr_vertex_total + vcache[ 0*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 0*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 0*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<std::endl;

											obmout<<"f ";
											obmout<<curr_vertex_total + vcache[ 3*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 3*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 3*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<" ";
											obmout<<curr_vertex_total + vcache[ 2*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 2*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 2*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<" ";
											obmout<<curr_vertex_total + vcache[ 0*VCACHE_STRIDE + VCACHE_VERTEX ]<<"/";
											obmout<<curr_normal_total + vcache[ 0*VCACHE_STRIDE + VCACHE_NORMAL ]<<"/";
											obmout<<curr_texcoord_total + vcache[ 0*VCACHE_STRIDE + VCACHE_TEXCOORD ]<<"/0";
											obmout<<std::endl;

											vcachecurr = 0;
										}

										vptr += vcsize;
									}
									break;

								//98

								//case 0x20: //Weight declaration
								//case 0x28: //Weight declaration 2 ? (gkoopa?)
								//case 0x30: //Weight declaration 3 ? gkoopa
								//case 0xA0://Single quad
								default:
									fout<<"Unknown connection type: "<< UInt4ToString((unsigned int)type) <<" size:"<< vcount <<"\t@"<<UInt4ToString((vptr - inbuf) - 3)<<std::endl;
									broken = true;
									break;
							}
						}else{
							broken = true;
						}

						vptr = vptr_final;
					}

					currblock++;
				}

			}

			curr_group++;
		}


		//
		//Time for keyframes! ? Hm. Seems right;
		//
		b = inbuf + vertexnext_offset;

		fout<<"Keyframes @"<<UInt4ToString( b - inbuf )<<std::endl;

		//? Da hell is this mess? (Start of some connection data??)

		//Write mesh keyframes (hm)
		//vptr = inbuf + vertexnext_offset;

		//vptr + 24 => keyframes for this anim
		//vptr + 28 => pointer to start of keyframe data
		//	kptr -> some sort of flags
		//	kptr + 4 -> pointer to keyframe vertex data array, which is vx,vy,vz,nx,ny,nz for same size as mesh.
		//	...

		//OBM specs:
		//km t, name	Mesh keyframe time and name
		//kv i x y z	Mesh keyframe vertex
		//kn i x y z	Mesh keyframe normal

		//? Da hell is THIS mess? (Start of some anim table data??)



		obmout.close();

	}

	/*
	//std::ofstream obmout( (pokemon_name + ".obm").c_str() , std::ios::out | std::ios::binary );
	//if( obmout.is_open() ){

		//006 lizardon = 0x0960 0xC13244F6 14064
		//130 gyarados = 0x0E60 0xC0602071 28392
		//142 ptera2 = 0x1620 0x357FFFFA 10368
		//		0x475C0 0xC0220033 8808		Vertex keyframes! Neato. Includes vertex and normal data.
		//248 bangiras = 0x16E0 0x3FE63151 12768
		//384 rayquaza = 0x15C0 0x3FC2F35E 92352

		unsigned int vtx_offset = 0x475C0;
		unsigned int vtx_count = 8808/12;
		unsigned int key_check = 0xC0220033;

		if( (vtx_offset > insize) || ((vtx_offset + vtx_count*12) > insize) ){ return 0; }

		char buffy[128];

		b = inbuf + vtx_offset;
		if( UChar4ToUIntHL( b ) == key_check ){

			fout<< "o "<< pokemon_name << std::endl;
			fout<< "g default" << std::endl;
			while( vtx_count != 0 ){
				vtx_count--;

				float vx = UChar4ToFloatHL( b ); b+= 4;
				float vy = UChar4ToFloatHL( b ); b+= 4;
				float vz = UChar4ToFloatHL( b ); b+= 4;

				sprintf( buffy, "v %0.6f %0.6f %0.6f", vx, vy, vz );
				fout<< buffy << std::endl;
			}

			return 1;
		}

	//}
	//obmout.close();
	*/

	return 1;
}


int parse_pokemon_fsys_file( const std::string & filename, std::ostream & fout ){

	std::ifstream infile( filename.c_str(), std::ios::in | std::ios::binary );

	//? file directory?
	std::string dirpath = "";
	std::string filenamed = "";
	std::string::const_iterator is = filename.end();
	std::string::const_iterator is_begin = filename.begin();
	while( is != is_begin ){
		is--;

		if( *is == '/' ){

			dirpath.assign( filename.begin(), is+1 );
			filenamed.assign( is+1, filename.end() );
			break;
		}else{

		}
	}

	if( infile.is_open() ){

		infile.seekg( 0, std::ios::end );
		unsigned int file_size = (unsigned int)infile.tellg();
		infile.seekg( 0, std::ios::beg );

		unsigned char * fbuf = new unsigned char [ file_size ];	//Load file into memory

		infile.read( (char*)fbuf, file_size );

		infile.close();

		fout<<"File: "<<filename<<"\tBytes: "<<file_size<<std::endl;

		//
		//
		//FSYS  [?] [?] [number of entries]
		//[?] [?] [?] [total header size (includes this 32 byte block)]
		//[?] [?] [?] [?]
		//[?] [?] [?] [?]
		//[start of entry offsets] [start of names] [?] [?]

		//entry: [?] [offset to start of LZSS block]

		unsigned int num_entries = UChar4ToUIntHL( fbuf + 12 );
		unsigned int start_entry_offsets = UChar4ToUIntHL( fbuf + 64 );
		//unsigned int start_entry_names = UChar4ToUIntHL( fbuf + 68 );

		unsigned int currentry = 0;
		unsigned char * entry_ptrs = fbuf + start_entry_offsets;
		while( currentry < num_entries ){

			unsigned int entry_start = UChar4ToUIntHL( entry_ptrs ); entry_ptrs += 4;
			unsigned int lzss_start = UChar4ToUIntHL( fbuf + entry_start + 4 );

			unsigned char * data_start = fbuf + lzss_start;

			//"LZSS" = data_start
			unsigned int entry_final_size = UChar4ToUIntHL( data_start + 4 ) & 16777215;	//hm.
			unsigned int entry_size = UChar4ToUIntHL( data_start + 8 );
			//unsigned int entry_? = UChar4ToUIntHL( data_start + 12 );

			unsigned char * decomdata = new unsigned char [ entry_final_size + 32 ];	//Add some buffer space in case our pointer get's walked like a small dog.
			unsigned int total_out = decompress_pokemon_LZSS( data_start + 16, entry_size, decomdata, entry_final_size );

			fout<<"Wrote: "<<total_out<<"/"<<entry_final_size<<"\tFrom: "<<entry_size<<std::endl;

			//Parse the decompressed data?
			char cbuffy[16];
			sprintf( cbuffy, "%d", currentry );
			std::string outfilename = dirpath + "out/" + filenamed + "_" + (std::string)cbuffy + ".out";

			if( parse_pokemon( decomdata, total_out, filename, currentry, fout ) ){

			}else{

				std::cout<<"#ERROR PARSING: "<<outfilename<<std::endl;
			}

			//Hm.
			std::ofstream outfile( outfilename.c_str(), std::ios::out | std::ios::binary );
			if( outfile.is_open() ){

				outfile.write( (char*)decomdata, total_out );
			}
			outfile.close();

			std::cout<<"\tEntry: "<<outfilename<<std::endl;

			delete [] decomdata;

			currentry++;

			//return 1;	//Parse ONLY the nth entry???
		}

		delete [] fbuf;	//Remove file buffer
	}

	return 1;
}

int main()
{

	std::ofstream fout("outlog.txt", std::ios::out | std::ios::binary );

	IGTL::Dir dirc;

	std::string directory = "./files";

	dirc.Open( directory );	//Default

	dirc.First();
	while( dirc.Valid() ){

		if( !dirc.IsDir() ){

			std::string filename;
			dirc.Get( filename );

			std::cout<<"File: "<<filename<<std::endl;

			parse_pokemon_fsys_file( directory + "/" + filename, fout );
		}

		dirc.Next();
	}

	fout.close();

    return 0;
}
