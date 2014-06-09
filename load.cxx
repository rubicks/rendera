#include "rendera.h"
#include <jpeglib.h>
#include <setjmp.h>

int *preview_data = 0;

void load(Fl_Widget *, void *)
{
  Fl_Native_File_Chooser *fc = new Fl_Native_File_Chooser();
  fc->title("Load Image");
  fc->filter("JPEG Image\t*.{jpg,jpeg}");
  fc->options(Fl_Native_File_Chooser::PREVIEW);
  fc->type(Fl_Native_File_Chooser::BROWSE_FILE);
  fc->show();

  const char *fn = fc->filename();
  load_jpg(fn);

  delete Map::main;
  Map::main = new Map(Bitmap::main->w, Bitmap::main->h);

  delete fc;
}

struct my_error_mgr
{
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

static void jpg_exit(j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr)cinfo->err;
  (*cinfo->err->output_message)(cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

Fl_Image *preview_jpg(const char *fn, unsigned char *header, int len)
{
  // check header
  unsigned char jpeg_header[2] = { 0xff, 0xd8 };
  if(memcmp(header, jpeg_header, 2) != 0)
    return 0;

  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  JSAMPARRAY linebuf;
  int row_stride;

  FILE *in = fl_fopen(fn, "rb");
  if(!in)
    return 0;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jpg_exit;

  if(setjmp(jerr.setjmp_buffer))
  {
    jpeg_destroy_decompress(&cinfo);
    fclose(in);
    return 0;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, in);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  linebuf = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

  int bytes = cinfo.out_color_components;

  int w = row_stride / bytes;
  int h = cinfo.output_height;

  if(w < 1 || h < 1)
    return 0;

  delete[] preview_data;
  preview_data = new int[w * h];
  Fl_RGB_Image *image = new Fl_RGB_Image((unsigned char *)preview_data, w, h, 4, 0);

  int x;
  int i;
  int *p = &preview_data[0];
  while(cinfo.output_scanline < cinfo.output_height)
  {
    jpeg_read_scanlines(&cinfo, linebuf, 1);
    if(bytes == 3)
    {
      for(x = 0; x < row_stride; x += 3)
      {
        *p = makecol((linebuf[0][x] & 0xFF),
                     (linebuf[0][x + 1] & 0xFF), (linebuf[0][x + 2]) & 0xFF);
        p++;
      }
    }
    else
    {
      for(x = 0; x < row_stride; x += 1)
      {
        *p = makecol((linebuf[0][x] & 0xFF),
                     (linebuf[0][x] & 0xFF), (linebuf[0][x]) & 0xFF);
        p++;
      }
    }
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(in);

  return image;
}

void load_jpg(const char *fn)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  JSAMPARRAY linebuf;
  int row_stride;

  FILE *in = fl_fopen(fn, "rb");
  if(!in)
    return;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jpg_exit;

  if(setjmp(jerr.setjmp_buffer))
  {
    jpeg_destroy_decompress(&cinfo);
    fclose(in);
    return;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, in);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  linebuf = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

  int bytes = cinfo.out_color_components;

  int w = row_stride / bytes;
  int h = cinfo.output_height;

  int aw = w + 64;
  int ah = h + 64;

  delete Bitmap::main;

  Bitmap::main = new Bitmap(aw, ah);
  Bitmap::main->clear(makecol(0, 0, 0));
  Bitmap::main->set_clip(32, 32, aw - 32 - 1, ah - 32 - 1);

  int x;
  int *p = Bitmap::main->row[32] + 32;
  while(cinfo.output_scanline < cinfo.output_height)
  {
    jpeg_read_scanlines(&cinfo, linebuf, 1);
    if(bytes == 3)
    {
      for(x = 0; x < row_stride; x += 3)
      {
        *p = makecol((linebuf[0][x] & 0xFF),
                     (linebuf[0][x + 1] & 0xFF), (linebuf[0][x + 2]) & 0xFF);
        p++;
      }
    }
    else
    {
      for(x = 0; x < row_stride; x++)
      {
        *p = makecol((linebuf[0][x] & 0xFF),
                     (linebuf[0][x] & 0xFF), (linebuf[0][x]) & 0xFF);
        p++;
      }
    }
    p += 64;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(in);
}

