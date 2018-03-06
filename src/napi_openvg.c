#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <vg/openvg.h>
#include <vg/vgu.h>

#define NAPI_DEFINE_CONSTANT(target, constant)                           \
  do {                                                                   \
    napi_value v = NULL;                                                 \
    napi_create_int32(env, constant, &v);                                \
    napi_set_named_property(env, target, #constant, v);                  \
  }                                                                      \
  while (0)


#define NAPI_SET_METHOD(target, name, callback)                         \
  do {                                                                  \
    napi_value fn = NULL;                                               \
    napi_create_function(env, NULL, 0, callback, NULL, &fn);            \
    napi_set_named_property(env, target, name, fn);                     \
  }                                                                     \
  while (0)

/*
typedef enum {
  napi_int8_array,
  napi_uint8_array,
  napi_uint8_clamped_array,
  napi_int16_array,
  napi_uint16_array,
  napi_int32_array,
  napi_uint32_array,
  napi_float32_array,
  napi_float64_array,
} napi_typedarray_type;
*/
napi_value vgTestCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  uint32_t len;
  napi_get_array_length(env, argv[0], &len);

  // typed array...
  napi_typedarray_type type;
  void* data;
  int byte_offset;
  napi_value out;
  napi_get_typedarray_info(env, argv[0], &type, &len, &data, &out, &byte_offset);

  fprintf(stderr, "arr info len %d type %d offset %d\n", len, type, byte_offset);
  {
    VGfloat* ff = (VGfloat *)((char *)data + byte_offset);
    for(int i = 0; i < len; i++) {
      fprintf(stderr, "float %d %f\n", i, ff[i]);
      ff[i] += 1.0;
    }
    fprintf(stderr, "float OK\n");
  }
/*
  for(int i = 0; i < 4; i++) {
    napi_value e;
    napi_get_element(env, argv[0], i, &e);
    
    double v;
    napi_get_value_double(env, e, &v);
    fprintf(stderr, "arr %d is %f\n", i, v);
    //int v;
    //napi_get_value_int32(env, e, &v);
    //fprintf(stderr, "arr %d is %d\n", i, v);
  }
  */
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgCreateContextSHCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  vgCreateContextSH((int)arg0, (int)arg1);

  // correct upside down
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, arg0, arg1, 0);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgResizeSurfaceSHCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  vgResizeSurfaceSH((int)arg0, (int)arg1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,arg0,arg1,0);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgDestroyContextSHCallback(napi_env env, napi_callback_info info) {

  vgDestroyContextSH();

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgCreateImageCallback(napi_env env, napi_callback_info info) {

  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1, arg2, arg3;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);
  napi_get_value_int32(env, argv[3], &arg3);

  VGImage imageObj = vgCreateImage((VGImageFormat)arg0, arg1, arg2, arg3);

  napi_value ret;
  napi_create_int64(env, (int64_t) imageObj, &ret);
  return ret;
}

napi_value vgImageSubDataCallback(napi_env env, napi_callback_info info) {

  size_t argc = 8;
  napi_value argv[8];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  uint8_t* data;
  size_t len;
  napi_get_buffer_info(env, argv[1], (void**) &data, &len);

  int arg2, arg3, arg4, arg5, arg6, arg7;
  napi_get_value_int32(env, argv[2], &arg2);
  napi_get_value_int32(env, argv[3], &arg3);
  napi_get_value_int32(env, argv[4], &arg4);
  napi_get_value_int32(env, argv[5], &arg5);
  napi_get_value_int32(env, argv[6], &arg6);
  napi_get_value_int32(env, argv[7], &arg7);

  vgImageSubData((VGImage) arg0, data, arg2, (VGImageFormat) arg3, arg4, arg5, arg6, arg7);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgCopyImageCallback(napi_env env, napi_callback_info info) {

  size_t argc = 9;
  napi_value argv[9];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
  int64_t arg3;
  napi_get_value_int64(env, argv[3], &arg3);

  int arg1, arg2, arg4, arg5, arg6, arg7, arg8;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);
  napi_get_value_int32(env, argv[4], &arg4);
  napi_get_value_int32(env, argv[5], &arg5);
  napi_get_value_int32(env, argv[6], &arg6);
  napi_get_value_int32(env, argv[7], &arg7);
  napi_get_value_int32(env, argv[8], &arg8);

  vgCopyImage((VGImage) arg0, arg1, arg2, (VGImage)arg3, arg4, arg5, arg6, arg7, arg8);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgDrawImageCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  vgDrawImage((VGImage) arg0);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgDestroyImageCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  vgDestroyImage((VGImage) arg0);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgLoadIdentityCallback(napi_env env, napi_callback_info info) {

  vgLoadIdentity();

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgLoadMatrixCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  uint32_t len;
  napi_get_array_length(env, argv[0], &len);
  //VGfloat* arr = (VGfloat *) malloc(sizeof(VGfloat) * len); 
  VGfloat arr[12];

  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[0], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr[i] = (VGfloat) v;
  }
  vgLoadMatrix(arr);
  //free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgMultMatrixCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  uint32_t len;
  napi_get_array_length(env, argv[0], &len);

  //VGfloat* arr = (VGfloat *) malloc(sizeof(VGfloat) * len); 
  VGfloat arr[12];

  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[0], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr[i] = (VGfloat) v;
  }
  vgMultMatrix(arr);
  //free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgGetMatrixCallback(napi_env env, napi_callback_info info) {
/*
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  uint32_t len;
  napi_get_array_length(env, argv[0], &len);

  //VGfloat* arr = (VGfloat *) malloc(sizeof(VGfloat) * len); 
  VGfloat arr[12];
  for(int i = 0; i < (int) len; i++) {
    napi_value e;
    napi_get_element(env, argv[0], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr[i] = (VGfloat) v;
  }
  vgGetMatrix(arr);
  //free(arr);
*/
  VGfloat arr[12];
  vgGetMatrix(arr);

  // return array....
  napi_value ret;
  napi_create_array_with_length(env, 12, &ret);

  for(int i = 0; i < 12; i++) {
    napi_value e;
    napi_create_double(env, arr[i], &e);
    napi_set_element(env, ret, i, e);

  }
  return ret;
}

napi_value vgTranslateCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  double arg0, arg1;
  napi_get_value_double(env, argv[0], &arg0);
  napi_get_value_double(env, argv[1], &arg1);

  vgTranslate((VGfloat)arg0, (VGfloat) arg1);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgScaleCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  double arg0, arg1;
  napi_get_value_double(env, argv[0], &arg0);
  napi_get_value_double(env, argv[1], &arg1);

  vgScale((VGfloat)arg0, (VGfloat) arg1);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgShearCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  double arg0, arg1;
  napi_get_value_double(env, argv[0], &arg0);
  napi_get_value_double(env, argv[1], &arg1);

  vgShear((VGfloat)arg0, (VGfloat) arg1);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgRotateCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  double arg0;
  napi_get_value_double(env, argv[0], &arg0);
 
  vgRotate((VGfloat)arg0);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgFlushCallback(napi_env env, napi_callback_info info) {

  vgFlush();
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgSetiCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  vgSeti((VGParamType)arg0, (VGint) arg1);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgSetfCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0;
  napi_get_value_int32(env, argv[0], &arg0);
  double arg1;
  napi_get_value_double(env, argv[1], &arg1);

  vgSetf((VGParamType)arg0, (float) arg1);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgSetfvCallback(napi_env env, napi_callback_info info) {

  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  uint32_t len;
  napi_get_array_length(env, argv[2], &len);
  VGfloat* arr = (VGfloat *) malloc(sizeof(VGfloat) * len); 

  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[2], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr[i] = (VGfloat) v;
  }
  vgSetfv((VGParamType)arg0, (VGint)arg1, (VGfloat *)arr);
  free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgSetivCallback(napi_env env, napi_callback_info info) {

  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  uint32_t len;
  napi_get_array_length(env, argv[2], &len);
  
  VGint* arr = (VGint *) malloc(sizeof(VGint) * len); 
  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[2], i, &e);
    int v;
    napi_get_value_int32(env, e, &v);
    arr[i] = (VGint) v;
  }
  vgSetiv((VGParamType)arg0, (VGint)arg1, (VGint *)arr);
  free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgGetiCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0;
  napi_get_value_int32(env, argv[0], &arg0);

  VGint v = vgGeti((VGParamType) arg0);
  
  napi_value ret;
  napi_create_int32(env, (int32_t) v, &ret);
  return ret;
}

napi_value vgGetfCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0;
  napi_get_value_int32(env, argv[0], &arg0);

  VGfloat v = vgGetf((VGParamType) arg0);
  
  napi_value ret;
  napi_create_double(env, (double) v, &ret);
  return ret;
}

napi_value vgGetfvCallback(napi_env env, napi_callback_info info) {

  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  uint32_t len;
  napi_get_array_length(env, argv[2], &len);

  VGfloat* arr = (VGfloat *) malloc(sizeof(VGfloat) * len); 
  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[2], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr[i] = (VGfloat) v;
  }
  vgGetfv((VGParamType)arg0, (VGint)arg1, (VGfloat *)arr);
  free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgGetivCallback(napi_env env, napi_callback_info info) {

  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);

  uint32_t len;
  napi_get_array_length(env, argv[2], &len);
  VGint* arr = (VGint *) malloc(sizeof(VGint) * len); 

  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[2], i, &e);
    int v;
    napi_get_value_int32(env, e, &v);
    arr[i] = (VGint) v;
  }
  vgGetiv((VGParamType)arg0, (VGint)arg1, (VGint *)arr);
  free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgClearCallback(napi_env env, napi_callback_info info) {

  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1, arg2, arg3;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);
  napi_get_value_int32(env, argv[3], &arg3);

  vgClear(arg0, arg1, arg2, arg3);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgCreatePathCallback(napi_env env, napi_callback_info info) {

  size_t argc = 7;
  napi_value argv[7];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1, arg4, arg5, arg6;
  double arg2, arg3;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_double(env, argv[2], &arg2);
  napi_get_value_double(env, argv[3], &arg3);
  napi_get_value_int32(env, argv[4], &arg4);
  napi_get_value_int32(env, argv[5], &arg5);
  napi_get_value_int32(env, argv[6], &arg6);


  VGPath path = vgCreatePath((VGint)arg0, (VGPathDatatype)arg1, (VGfloat)arg2, (VGfloat)arg3, (VGint)arg4, (VGint)arg5, (VGbitfield)arg6);
 
  napi_value ret;
  napi_create_int64(env, (int64_t) path, &ret);
  return ret;
}

napi_value vgAppendPathDataCallback(napi_env env, napi_callback_info info) {

  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  uint32_t len;
  napi_get_array_length(env, argv[2], &len);
  VGubyte* arr = (VGubyte *) malloc(sizeof(VGubyte) * len); 
  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[2], i, &e);
    int v;
    napi_get_value_int32(env, e, &v);
    arr[i] = (VGubyte) v;
  }

  napi_get_array_length(env, argv[3], &len);

  VGfloat* arr2 = (VGfloat *) malloc(sizeof(VGfloat) * len); 
  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[3], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr2[i] = (VGfloat) v;
  }

  vgAppendPathData((VGPath)arg0, (VGint)arg1, (VGubyte *)arr, (void* )arr2);
  free(arr);
  free(arr2);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgDrawPathCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  vgDrawPath((VGPath) arg0, (VGbitfield)arg1);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgClearPathCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  vgClearPath((VGPath) arg0, (VGbitfield)arg1);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgPointAlongPathCallback(napi_env env, napi_callback_info info) {

  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1, arg2;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);

  double arg3;
  napi_get_value_double(env, argv[3], &arg3);

  VGfloat x, y, tanx, tany;

  vgPointAlongPath((VGPath) arg0, arg1, arg2, arg3, &x, &y, &tanx, &tany);
 
  // return array....
  napi_value ret;
  napi_create_array_with_length(env, 4, &ret);

  napi_value e;
  napi_create_double(env, x, &e);
  napi_set_element(env, ret, 0, e);

  napi_create_double(env, y, &e);
  napi_set_element(env, ret, 1, e);

  napi_create_double(env, tanx, &e);
  napi_set_element(env, ret, 2, e);
  
  napi_create_double(env, tany, &e);
  napi_set_element(env, ret, 3, e);

  return ret;
}

napi_value vgTransformPathCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int64_t arg1;
  napi_get_value_int64(env, argv[1], &arg1);

  vgTransformPath((VGPath) arg0, (VGPath)arg1);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgDestroyPathCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  vgDestroyPath((VGPath)arg0);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgCreatePaintCallback(napi_env env, napi_callback_info info) {

  VGPath paint = vgCreatePaint();

  napi_value ret;
  napi_create_int64(env, (int64_t) paint, &ret);
  return ret;
}


napi_value vgSetPaintCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  vgSetPaint((VGPath) arg0, (VGbitfield)arg1);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgDestroyPaintCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  vgDestroyPaint((VGPath)arg0);
 
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgSetParameteriCallback(napi_env env, napi_callback_info info) {

  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1, arg2;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);

  vgSetParameteri((VGHandle) arg0, (VGint)arg1, (VGint) arg2);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgSetParameterfvCallback(napi_env env, napi_callback_info info) {

  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1, arg2;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);

  uint32_t len;
  napi_get_array_length(env, argv[3], &len);
  VGfloat* arr = (VGfloat *) malloc(sizeof(VGfloat) * len); 

  for(int i = 0; i < (int)len; i++) {
    napi_value e;
    napi_get_element(env, argv[3], i, &e);
    double v;
    napi_get_value_double(env, e, &v);
    arr[i] = (VGfloat) v;
  }
  vgSetParameterfv((VGHandle) arg0, (VGint)arg1, (VGint) arg2, (VGfloat *)arr);
  free(arr);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

////
// mask
napi_value vgCreateMaskLayerCallback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int arg0, arg1;
  napi_get_value_int32(env, argv[0], &arg0);
  napi_get_value_int32(env, argv[1], &arg1);


  //VGMaskLayer layer = vgCreateMaskLayer(arg0, arg1);
  int64_t layer = 0;
  
  napi_value ret;
  napi_create_int64(env, (int64_t) layer, &ret);
  return ret;
}

napi_value vgDestroyMaskLayerCallback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  //vgDestroyMaskLayer((VGMaskLayer)arg0);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgMaskCallback(napi_env env, napi_callback_info info) {

  size_t argc = 6;
  napi_value argv[6];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1, arg2, arg3, arg4, arg5;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);
  napi_get_value_int32(env, argv[3], &arg3);
  napi_get_value_int32(env, argv[4], &arg4);
  napi_get_value_int32(env, argv[5], &arg5);

  //vgMask((VGMaskLayer)arg0, (VGMaskOperation) arg1, arg2, arg3, arg4, arg5);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value vgCopyMaskCallback(napi_env env, napi_callback_info info) {

  size_t argc = 8;
  napi_value argv[8];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1, arg2, arg3, arg4, arg5, arg6;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);
  napi_get_value_int32(env, argv[3], &arg3);
  napi_get_value_int32(env, argv[4], &arg4);
  napi_get_value_int32(env, argv[5], &arg5);
  napi_get_value_int32(env, argv[6], &arg6);

  double arg7;
  napi_get_value_double(env, argv[7], &arg7);

  //vgCopyMask((VGMaskLayer)arg0, arg1, arg2, arg3, arg4, arg5, arg6, (VGfloat) arg7);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value vgRenderToMaskCallback(napi_env env, napi_callback_info info) {

  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1, arg2;
  napi_get_value_int32(env, argv[1], &arg1);
  napi_get_value_int32(env, argv[2], &arg2);

  //vgRenderToMask((VGPath)arg0, arg1, arg2);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

// VGU

napi_value vguRectCallback(napi_env env, napi_callback_info info) {

  size_t argc = 5;
  napi_value argv[5];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  double arg1, arg2, arg3, arg4;
  napi_get_value_double(env, argv[1], &arg1);
  napi_get_value_double(env, argv[2], &arg2);
  napi_get_value_double(env, argv[3], &arg3);
  napi_get_value_double(env, argv[4], &arg4);

  vguRect((VGPath) arg0, arg1, arg2, arg3, arg4);
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

/////

napi_value Init(napi_env env, napi_value exports) {

  NAPI_DEFINE_CONSTANT(exports, VG_MATRIX_MODE);
  NAPI_DEFINE_CONSTANT(exports, VG_MATRIX_PATH_USER_TO_SURFACE);
  NAPI_DEFINE_CONSTANT(exports, VG_CLEAR_COLOR);
  
  NAPI_DEFINE_CONSTANT(exports, VG_RENDERING_QUALITY);
  NAPI_DEFINE_CONSTANT(exports, VG_RENDERING_QUALITY_BETTER);
  NAPI_DEFINE_CONSTANT(exports, VG_RENDERING_QUALITY_FASTER);




  // path & stroke
  NAPI_DEFINE_CONSTANT(exports, VG_PATH_FORMAT_STANDARD);
  NAPI_DEFINE_CONSTANT(exports, VG_PATH_NUM_SEGMENTS);
  NAPI_DEFINE_CONSTANT(exports, VG_PATH_DATATYPE_F);
  NAPI_DEFINE_CONSTANT(exports, VG_PATH_CAPABILITY_ALL);
  NAPI_DEFINE_CONSTANT(exports, VG_PATH_CAPABILITY_POINT_ALONG_PATH);  
  NAPI_DEFINE_CONSTANT(exports, VG_MOVE_TO);
  NAPI_DEFINE_CONSTANT(exports, VG_MOVE_TO_ABS);
  NAPI_DEFINE_CONSTANT(exports, VG_LINE_TO);
  NAPI_DEFINE_CONSTANT(exports, VG_LINE_TO_ABS);
  NAPI_DEFINE_CONSTANT(exports, VG_QUAD_TO);
  NAPI_DEFINE_CONSTANT(exports, VG_QUAD_TO_ABS);
  NAPI_DEFINE_CONSTANT(exports, VG_CUBIC_TO);
  NAPI_DEFINE_CONSTANT(exports, VG_CUBIC_TO_ABS);
  NAPI_DEFINE_CONSTANT(exports, VG_SCWARC_TO);
  NAPI_DEFINE_CONSTANT(exports, VG_SCWARC_TO_ABS);
  NAPI_DEFINE_CONSTANT(exports, VG_SCCWARC_TO);
  NAPI_DEFINE_CONSTANT(exports, VG_SCCWARC_TO_ABS);
  NAPI_DEFINE_CONSTANT(exports, VG_HLINE_TO_REL);
  NAPI_DEFINE_CONSTANT(exports, VG_VLINE_TO_REL);
  NAPI_DEFINE_CONSTANT(exports, VG_CLOSE_PATH);
  NAPI_DEFINE_CONSTANT(exports, VG_STROKE_PATH);
  NAPI_DEFINE_CONSTANT(exports, VG_FILL_PATH);
  NAPI_DEFINE_CONSTANT(exports, VG_STROKE_LINE_WIDTH);

  NAPI_DEFINE_CONSTANT(exports, VG_STROKE_MITER_LIMIT);
  NAPI_DEFINE_CONSTANT(exports, VG_STROKE_DASH_PATTERN);


  NAPI_DEFINE_CONSTANT(exports, VG_STROKE_CAP_STYLE);
  NAPI_DEFINE_CONSTANT(exports, VG_CAP_BUTT);
  NAPI_DEFINE_CONSTANT(exports, VG_CAP_ROUND);
  NAPI_DEFINE_CONSTANT(exports, VG_CAP_SQUARE);


  NAPI_DEFINE_CONSTANT(exports, VG_STROKE_JOIN_STYLE);
  NAPI_DEFINE_CONSTANT(exports, VG_JOIN_ROUND);
  NAPI_DEFINE_CONSTANT(exports, VG_JOIN_BEVEL);
  NAPI_DEFINE_CONSTANT(exports, VG_JOIN_MITER);


  NAPI_DEFINE_CONSTANT(exports, VG_MASKING);
  NAPI_DEFINE_CONSTANT(exports, VG_SET_MASK);
  NAPI_DEFINE_CONSTANT(exports, VG_INTERSECT_MASK);



  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_TYPE);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_TYPE_COLOR);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_COLOR);
  
  // gradient
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_TYPE_LINEAR_GRADIENT);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_LINEAR_GRADIENT);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_TYPE_RADIAL_GRADIENT);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_RADIAL_GRADIENT);

  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_COLOR_RAMP_SPREAD_MODE);
  NAPI_DEFINE_CONSTANT(exports, VG_COLOR_RAMP_SPREAD_PAD);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_COLOR_RAMP_PREMULTIPLIED);
  NAPI_DEFINE_CONSTANT(exports, VG_PAINT_COLOR_RAMP_STOPS);

  // image
  NAPI_DEFINE_CONSTANT(exports, VG_IMAGE_MODE);
  NAPI_DEFINE_CONSTANT(exports, VG_DRAW_IMAGE_NORMAL);
  NAPI_DEFINE_CONSTANT(exports, VG_DRAW_IMAGE_MULTIPLY);

  NAPI_DEFINE_CONSTANT(exports, VG_IMAGE_QUALITY_BETTER);
  NAPI_DEFINE_CONSTANT(exports, VG_IMAGE_QUALITY_NONANTIALIASED);
  NAPI_DEFINE_CONSTANT(exports, VG_MATRIX_IMAGE_USER_TO_SURFACE);

  NAPI_DEFINE_CONSTANT(exports, VG_lBGRA_8888);
  NAPI_DEFINE_CONSTANT(exports, VG_lRGBA_8888);
  NAPI_DEFINE_CONSTANT(exports, VG_lARGB_8888);
  NAPI_DEFINE_CONSTANT(exports, VG_lABGR_8888);

  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_MODE);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_SRC);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_SRC_IN);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_SRC_OVER);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_DST_IN);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_DST_OVER);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_LIGHTEN);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_MULTIPLY);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_SCREEN);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_DARKEN);
  NAPI_DEFINE_CONSTANT(exports, VG_BLEND_ADDITIVE);

  NAPI_DEFINE_CONSTANT(exports, VG_TILE_REPEAT);
  NAPI_DEFINE_CONSTANT(exports, VG_TILE_FILL);
  NAPI_DEFINE_CONSTANT(exports, VG_TILE_PAD);

  NAPI_SET_METHOD(exports, "vgTest", vgTestCallback);
  

  NAPI_SET_METHOD(exports, "vgCreateContextSH", vgCreateContextSHCallback);
  NAPI_SET_METHOD(exports, "vgResizeSurfaceSH", vgResizeSurfaceSHCallback);
  NAPI_SET_METHOD(exports, "vgDestroyContextSH", vgDestroyContextSHCallback);
  
  NAPI_SET_METHOD(exports, "vgFlush", vgFlushCallback);
  NAPI_SET_METHOD(exports, "vgClear", vgClearCallback);

  NAPI_SET_METHOD(exports, "vgCreateMaskLayer", vgCreateMaskLayerCallback);
  NAPI_SET_METHOD(exports, "vgDestroyMaskLayer", vgDestroyMaskLayerCallback);
  NAPI_SET_METHOD(exports, "vgMask", vgMaskCallback);
  NAPI_SET_METHOD(exports, "vgCopyMask", vgCopyMaskCallback);
  NAPI_SET_METHOD(exports, "vgRenderToMask", vgRenderToMaskCallback);

  NAPI_SET_METHOD(exports, "vgSeti", vgSetiCallback);
  NAPI_SET_METHOD(exports, "vgSetf", vgSetfCallback);
  NAPI_SET_METHOD(exports, "vgSetfv", vgSetfvCallback);
  NAPI_SET_METHOD(exports, "vgSetiv", vgSetivCallback);

  NAPI_SET_METHOD(exports, "vgGeti", vgGetiCallback);
  NAPI_SET_METHOD(exports, "vgGetf", vgGetfCallback);
  NAPI_SET_METHOD(exports, "vgGetfv", vgGetfvCallback);
  NAPI_SET_METHOD(exports, "vgGetiv", vgGetivCallback);

  NAPI_SET_METHOD(exports, "vgSetParameteri", vgSetParameteriCallback);
  NAPI_SET_METHOD(exports, "vgSetParameterfv", vgSetParameterfvCallback);

  // path
  NAPI_SET_METHOD(exports, "vgCreatePath", vgCreatePathCallback);
  NAPI_SET_METHOD(exports, "vgAppendPathData", vgAppendPathDataCallback);
  NAPI_SET_METHOD(exports, "vgDrawPath", vgDrawPathCallback);
  NAPI_SET_METHOD(exports, "vgClearPath", vgClearPathCallback);
  NAPI_SET_METHOD(exports, "vgDestroyPath", vgDestroyPathCallback);
  NAPI_SET_METHOD(exports, "vgPointAlongPath", vgPointAlongPathCallback);
  NAPI_SET_METHOD(exports, "vgTransformPath", vgTransformPathCallback);

  // paint
  NAPI_SET_METHOD(exports, "vgCreatePaint", vgCreatePaintCallback);
  NAPI_SET_METHOD(exports, "vgSetPaint", vgSetPaintCallback);
  NAPI_SET_METHOD(exports, "vgDestroyPaint", vgDestroyPaintCallback);

  // image
  NAPI_SET_METHOD(exports, "vgCreateImage", vgCreateImageCallback);
  NAPI_SET_METHOD(exports, "vgImageSubData", vgImageSubDataCallback);
  NAPI_SET_METHOD(exports, "vgDestroyImage", vgDestroyImageCallback);
  NAPI_SET_METHOD(exports, "vgDrawImage", vgDrawImageCallback);
  NAPI_SET_METHOD(exports, "vgCopyImage", vgCopyImageCallback);

  // vgu
  NAPI_SET_METHOD(exports, "vguRect", vguRectCallback);

  // matrix function
  NAPI_SET_METHOD(exports, "vgLoadMatrix", vgLoadMatrixCallback);
  NAPI_SET_METHOD(exports, "vgMultMatrix", vgMultMatrixCallback);
  NAPI_SET_METHOD(exports, "vgGetMatrix", vgGetMatrixCallback);

  NAPI_SET_METHOD(exports, "vgLoadIdentity", vgLoadIdentityCallback);
  NAPI_SET_METHOD(exports, "vgTranslate", vgTranslateCallback);
  NAPI_SET_METHOD(exports, "vgScale", vgScaleCallback);
  NAPI_SET_METHOD(exports, "vgShear", vgShearCallback);
  NAPI_SET_METHOD(exports, "vgRotate", vgRotateCallback);
 
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)