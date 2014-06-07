#include "rendera.h"
#include <jpeglib.h>
#include <setjmp.h>

void load(Fl_Widget *, void *)
{
  Fl_File_Chooser *fc = new Fl_File_Chooser(".", "JPEG Image (*.jpg)", Fl_File_Chooser::SINGLE, "Load Image");
  fc->show();
  while(fc->shown())
  {
    Fl::wait();
  }
  const char *fn = fc->value();
  puts(fn);
  load_jpg(fn);
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

void load_jpg(const char *fn)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  JSAMPARRAY linebuf;
  int row_stride;

  FILE *in = fopen(fn, "rb");
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
  (void)jpeg_read_header(&cinfo, TRUE);
  (void)jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  linebuf = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);

  int bytes = cinfo.out_color_components;

  int w = row_stride / bytes;
  int h = cinfo.output_height;

  int aw = w + 64;
  int ah = h + 64;

  delete Bitmap::main;

  Bitmap::main = new Bitmap(aw, ah);
  Bitmap::main->clear(makecol(0, 0, 0));
  Bitmap::main->set_clip(64, 64, aw - 64, ah - 64);

  int x;
  int *p = Bitmap::main->row[32] + 32;
  while(cinfo.output_scanline < cinfo.output_height)
  {
    (void)jpeg_read_scanlines(&cinfo, linebuf, 1);
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

  (void)jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(in);
}


