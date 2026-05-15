#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __has_builtin
#if __has_builtin(__builtin_pow)
#define _zc_pow __builtin_pow
#endif
#endif
#ifndef _zc_pow
extern double pow(double, double);
#define _zc_pow pow
#endif
#include <unistd.h>
#include <fcntl.h>
#define ZC_SIMD(T, N) T __attribute__((vector_size(N * sizeof(T))))
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L
#define ZC_AUTO auto
#define ZC_AUTO_INIT(var, init) auto var = (init)
#else
#define ZC_AUTO __auto_type
#define ZC_AUTO_INIT(var, init) __auto_type var = (init)
#endif
#define ZC_CAST(T, x) ((T)(x))
#ifdef __TINYC__
#undef ZC_AUTO
#define ZC_AUTO __auto_type
#undef ZC_AUTO_INIT
#define ZC_AUTO_INIT(var, init) __typeof__((init)) var = (init)

#ifndef __builtin_expect
#define __builtin_expect(x, v) (x)
#endif

#ifndef __builtin_unreachable
#define __builtin_unreachable()
#endif
#else
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L
#define ZC_AUTO_INIT(var, init) auto var = (init)
#else
#define ZC_AUTO_INIT(var, init) __auto_type var = (init)
#endif
#endif
static inline const char* _z_bool_str(_Bool b) { return b ? "true" : "false"; }
#ifdef __SIZEOF_INT128__
static inline const char *_z_u128_str(unsigned __int128 val) {
    static _Thread_local char buf[40];
    if (val == 0) return "0";
    int i = 38;
    buf[39] = 0;
    while (val > 0) { buf[i--] = (char)((val % 10) + '0'); val /= 10; }
    return &buf[i + 1];
}
static inline const char *_z_i128_str(__int128 val) {
    static _Thread_local char buf[41];
    if (val == 0) return "0";
    int neg = val < 0;
    unsigned __int128 uval = neg ? -val : val;
    int i = 39;
    buf[40] = 0;
    while (uval > 0) { buf[i--] = (char)((uval % 10) + '0'); uval /= 10; }
    if (neg) buf[i--] = '-';
    return &buf[i + 1];
}
#define _z_128_map ,__int128: "%s", unsigned __int128: "%s"
#else
#define _z_128_map
#endif
#ifdef __OBJC__
#define _z_objc_map ,id: "%s", Class: "%s", SEL: "%s"
#define _z_objc_arg_map(x) ,id: [(id)(x) description].UTF8String, Class: class_getName((Class)(x)), SEL: sel_getName((SEL)(x))
#else
#define _z_objc_map
#define _z_objc_arg_map(x)
#endif

#define _z_str(x) _Generic((x), _Bool: "%s", char: "%c", signed char: "%c", unsigned char: "%u", short: "%d", unsigned short: "%u", int: "%d", unsigned int: "%u", long: "%ld", unsigned long: "%lu", long long: "%lld", unsigned long long: "%llu", float: "%f", double: "%f", char*: "%s", const char*: "%s", void*: "%p" _z_128_map _z_objc_map)
#ifdef __SIZEOF_INT128__
#define _z_safe_i128(x) _Generic((x), __int128: (x), default: (__int128)0)
#define _z_safe_u128(x) _Generic((x), unsigned __int128: (x), default: (unsigned __int128)0)
#define _z_128_arg_map(x) ,__int128: _z_i128_str(_z_safe_i128(x)), unsigned __int128: _z_u128_str(_z_safe_u128(x))
#else
#define _z_128_arg_map(x)
#endif
#define _z_safe_bool(x) _Generic((x), _Bool: (x), default: (_Bool)0)
#define _z_arg(x) _Generic((x), _Bool: _z_bool_str(_z_safe_bool(x)) _z_128_arg_map(x) _z_objc_arg_map(x), default: (x))
typedef size_t usize;
typedef char* string;
typedef struct { void *func; void *ctx; void (*drop)(void*); } z_closure_T;
static void *_z_closure_ctx_stash[256];
typedef void U0;
typedef int8_t I8;
typedef uint8_t U8;
typedef int16_t I16;
typedef uint16_t U16;
typedef int32_t I32;
typedef uint32_t U32;
typedef int64_t I64;
typedef uint64_t U64;
#define F32 float
#define F64 double
#define z_malloc malloc
#define z_realloc realloc
#define z_free free
#define z_print printf
void __zenc_panic(const char* msg) { fprintf(stderr, "Panic: %s\n", msg); exit(1); }
#if defined(__APPLE__)
#define _ZC_SEC __attribute__((used,section("__DATA,__zarch")))
#elif defined(_WIN32)
#define _ZC_SEC __attribute__((used))
#else
#define _ZC_SEC __attribute__((used,section(".note.zarch")))
#endif
static const unsigned char _zc_abi_v1[] _ZC_SEC = {0x07,0xd5,0x59,0x30,0x7c,0x7f,0x66,0x75,0x30,0x69,0x7f,0x65,0x3c,0x30,0x59,0x7c,0x79,0x7e,0x73,0x71};
void _z_autofree_impl(void *p) { void **pp = (void**)p; if(*pp) { z_free(*pp); *pp = NULL; } }
#define __zenc_assert(cond, ...) if (!(cond)) { fprintf(stderr, "\"Assertion failed: \" " __VA_ARGS__); exit(1); }
string _z_readln_raw() { size_t cap = 64; size_t len = 0; char *line = z_malloc(cap); if(!line) return NULL; int c; while((c = fgetc(stdin)) != EOF) { if(c == '\n') break; if(len + 1 >= cap) { cap *= 2; char *n = z_realloc(line, cap); if(!n) { z_free(line); return NULL; } line = n; } line[len++] = c; } if(len == 0 && c == EOF) { z_free(line); return NULL; } line[len] = 0; return line; }
int _z_scan_helper(const char *fmt, ...) { char *l = _z_readln_raw(); if(!l) return 0; va_list ap; va_start(ap, fmt); int r = vsscanf(l, fmt, ap); va_end(ap); z_free(l); return r; }
int _z_orig_stdout = -1;
void _z_suppress_stdout() {
    fflush(stdout);
    if (_z_orig_stdout == -1) _z_orig_stdout = dup(STDOUT_FILENO);
    int nullfd = open("/dev/null", O_WRONLY);
    dup2(nullfd, STDOUT_FILENO);
    close(nullfd);
}
void _z_restore_stdout() {
    fflush(stdout);
    if (_z_orig_stdout != -1) {
        dup2(_z_orig_stdout, STDOUT_FILENO);
        close(_z_orig_stdout);
        _z_orig_stdout = -1;
    }
}
#ifndef ZC_CFG_windows
#define ZC_CFG_windows 1
#endif
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cglm/cglm.h"
#include "stb_image.h"
#include "cglm/affine.h"
#include "math.h"
typedef char* string;
typedef struct { void **data; int len; int cap; } Vec;
#define Vec_new() (Vec){.data=0, .len=0, .cap=0}
void _z_vec_push(Vec *v, void *item) { if(v->len >= v->cap) { v->cap = v->cap?v->cap*2:8; v->data = z_realloc(v->data, v->cap * sizeof(void*)); } v->data[v->len++] = item; }
static inline Vec _z_make_vec(int count, ...) { Vec v = {0}; v.cap = count > 8 ? count : 8; v.data = z_malloc(v.cap * sizeof(void*)); v.len = 0; va_list args; va_start(args, count); for(int i=0; i<count; i++) { v.data[v.len++] = va_arg(args, void*); } va_end(args); return v; }
#define Vec_push(v, i) _z_vec_push(&(v), (void*)(uintptr_t)(i))
static inline long _z_check_bounds(long index, long limit) { if(index < 0 || index >= limit) { fprintf(stderr, "Index out of bounds: %ld (limit %ld)\n", index, limit); exit(1); } return index; }

typedef struct Result_string Result_string;
typedef struct Result_uint32_t Result_uint32_t;
typedef struct VecIterResult_DirEntryPtr VecIterResult_DirEntryPtr;
typedef struct VecIterRef_DirEntryPtr VecIterRef_DirEntryPtr;
typedef struct Option_DirEntryPtr Option_DirEntryPtr;
typedef struct VecIter_DirEntryPtr VecIter_DirEntryPtr;
typedef struct Vec_DirEntryPtr Vec_DirEntryPtr;
typedef struct VecIterResult_DirEntry VecIterResult_DirEntry;
typedef struct VecIterRef_DirEntry VecIterRef_DirEntry;
typedef struct VecIter_DirEntry VecIter_DirEntry;
typedef struct Vec_DirEntry Vec_DirEntry;
typedef struct VecIterResult_StringPtr VecIterResult_StringPtr;
typedef struct VecIterRef_StringPtr VecIterRef_StringPtr;
typedef struct Option_StringPtr Option_StringPtr;
typedef struct VecIter_StringPtr VecIter_StringPtr;
typedef struct Vec_StringPtr Vec_StringPtr;
typedef struct Result_bool Result_bool;
typedef struct VecIterResult_String VecIterResult_String;
typedef struct VecIterRef_String VecIterRef_String;
typedef struct VecIter_String VecIter_String;
typedef struct Vec_String Vec_String;
typedef struct VecIterResult_int32_t VecIterResult_int32_t;
typedef struct VecIterRef_int32_t VecIterRef_int32_t;
typedef struct VecIter_int32_t VecIter_int32_t;
typedef struct Vec_int32_t Vec_int32_t;
typedef struct VecIterResult_size_t VecIterResult_size_t;
typedef struct VecIterRef_size_t VecIterRef_size_t;
typedef struct VecIter_size_t VecIter_size_t;
typedef struct Vec_size_t Vec_size_t;
typedef struct Option_size_t Option_size_t;
typedef struct Option_int32_t Option_int32_t;
typedef struct VecIterResult_char VecIterResult_char;
typedef struct VecIterRef_char VecIterRef_char;
typedef struct Option_char Option_char;
typedef struct VecIter_char VecIter_char;
typedef struct Vec_char Vec_char;
typedef struct Math Math;
typedef struct ShaderProgram ShaderProgram;
typedef struct Metadata Metadata;
typedef struct File File;
typedef struct StringCharsIter StringCharsIter;
typedef struct Sort Sort;
typedef struct Result_ShaderProgram Result_ShaderProgram;
typedef struct Result_Vec_DirEntry Result_Vec_DirEntry;
typedef struct Result_Metadata Result_Metadata;
typedef struct Result_Vec_String Result_Vec_String;
typedef struct Result_File Result_File;
typedef struct String String;
typedef struct Result_String Result_String;
typedef struct Option_String Option_String;
typedef struct DirEntry DirEntry;
typedef struct Option_DirEntry Option_DirEntry;
typedef struct Clone_VTable {
    void* (*clone)(void* self);
} Clone_VTable;
typedef struct Clone { void *self; Clone_VTable *vtable; } Clone;

typedef struct Copy_VTable {
} Copy_VTable;
typedef struct Copy { void *self; Copy_VTable *vtable; } Copy;

typedef struct Drop_VTable {
    void (*drop)(void* self);
} Drop_VTable;
typedef struct Drop { void *self; Drop_VTable *vtable; } Drop;


    #define ZC_IMPL_SORT(T) \
    static int _z_partition_##T(T* arr, int low, int high) { \
        T pivot = arr[high]; \
        int i = low - 1; \
        for (int j = low; j < high; j++) { \
            if (arr[j] < pivot) { \
                i++; \
                T temp = arr[i]; \
                arr[i] = arr[j]; \
                arr[j] = temp; \
            } \
        } \
        T temp2 = arr[i + 1]; \
        arr[i + 1] = arr[high]; \
        arr[high] = temp2; \
        return i + 1; \
    } \
    static void _z_quick_sort_##T(T* arr, int low, int high) { \
        if (low < high) { \
            int pivot_idx = _z_partition_##T(arr, low, high); \
            _z_quick_sort_##T(arr, low, pivot_idx - 1); \
            _z_quick_sort_##T(arr, pivot_idx + 1, high); \
        } \
    } \
    void sort_##T(T* arr, size_t len) { \
        if (len > 1) { \
            _z_quick_sort_##T(arr, 0, (int)len - 1); \
        } \
    }

    // Pre-declare standard library types
    ZC_IMPL_SORT(int)
    ZC_IMPL_SORT(long)
    ZC_IMPL_SORT(float)
    ZC_IMPL_SORT(double)

struct Result_string {
    bool is_ok;
    string val;
    char* err;
};

struct Result_uint32_t {
    bool is_ok;
    uint32_t val;
    char* err;
};

struct VecIterResult_DirEntryPtr {
    struct DirEntry** ptr;
};

struct VecIterRef_DirEntryPtr {
    struct DirEntry** data;
    size_t count;
    size_t idx;
};

struct Option_DirEntryPtr {
    bool is_some;
    struct DirEntry* val;
};

struct VecIter_DirEntryPtr {
    struct DirEntry** data;
    size_t count;
    size_t idx;
};

struct Vec_DirEntryPtr {
    DirEntry** data;
    size_t len;
    size_t cap;
};

struct VecIterResult_DirEntry {
    DirEntry* ptr;
};

struct VecIterRef_DirEntry {
    DirEntry* data;
    size_t count;
    size_t idx;
};

struct VecIter_DirEntry {
    DirEntry* data;
    size_t count;
    size_t idx;
};

struct Vec_DirEntry {
    DirEntry* data;
    size_t len;
    size_t cap;
};

struct VecIterResult_StringPtr {
    struct String** ptr;
};

struct VecIterRef_StringPtr {
    struct String** data;
    size_t count;
    size_t idx;
};

struct Option_StringPtr {
    bool is_some;
    struct String* val;
};

struct VecIter_StringPtr {
    struct String** data;
    size_t count;
    size_t idx;
};

struct Vec_StringPtr {
    String** data;
    size_t len;
    size_t cap;
};

struct Result_bool {
    bool is_ok;
    bool val;
    char* err;
};

struct VecIterResult_String {
    String* ptr;
};

struct VecIterRef_String {
    String* data;
    size_t count;
    size_t idx;
};

struct VecIter_String {
    String* data;
    size_t count;
    size_t idx;
};

struct Vec_String {
    String* data;
    size_t len;
    size_t cap;
};

struct VecIterResult_int32_t {
    int32_t* ptr;
};

struct VecIterRef_int32_t {
    int32_t* data;
    size_t count;
    size_t idx;
};

struct VecIter_int32_t {
    int32_t* data;
    size_t count;
    size_t idx;
};

struct Vec_int32_t {
    int32_t* data;
    size_t len;
    size_t cap;
};

struct VecIterResult_size_t {
    size_t* ptr;
};

struct VecIterRef_size_t {
    size_t* data;
    size_t count;
    size_t idx;
};

struct VecIter_size_t {
    size_t* data;
    size_t count;
    size_t idx;
};

struct Vec_size_t {
    size_t* data;
    size_t len;
    size_t cap;
};

struct Option_size_t {
    bool is_some;
    size_t val;
};

struct Option_int32_t {
    bool is_some;
    int32_t val;
};

struct VecIterResult_char {
    char* ptr;
};

struct VecIterRef_char {
    char* data;
    size_t count;
    size_t idx;
};

struct Option_char {
    bool is_some;
    char val;
};

struct VecIter_char {
    char* data;
    size_t count;
    size_t idx;
};

struct Vec_char {
    char* data;
    size_t len;
    size_t cap;
};

struct Math {
    char _placeholder;
};

struct ShaderProgram {
    uint32_t ID;
};

struct Metadata {
    uint64_t size;
    bool is_dir;
    bool is_file;
};

struct File {
    void* handle;
};

struct StringCharsIter {
    char* data;
    size_t len;
    size_t pos;
};

struct Sort {
    char _placeholder;
};

struct Result_ShaderProgram {
    bool is_ok;
    ShaderProgram val;
    char* err;
};

struct Result_Vec_DirEntry {
    bool is_ok;
    Vec_DirEntry val;
    char* err;
};

struct Result_Metadata {
    bool is_ok;
    Metadata val;
    char* err;
};

struct Result_Vec_String {
    bool is_ok;
    Vec_String val;
    char* err;
};

struct Result_File {
    bool is_ok;
    File val;
    char* err;
};

struct String {
    Vec_char vec;
};

struct Result_String {
    bool is_ok;
    String val;
    char* err;
};

struct Option_String {
    bool is_some;
    String val;
};

struct DirEntry {
    String name;
    bool is_dir;
};

struct Option_DirEntry {
    bool is_some;
    DirEntry val;
};


    typedef struct DirEntry* DirEntryPtr;
    
    // DIR* wrappers - opendir/closedir/readdir use DIR* which conflicts with void*
    void* _z_fs_opendir(const char* name) {
        return opendir(name);
    }

    int _z_fs_closedir(void* dir) {
        return closedir((DIR*)dir);
    }

    // struct stat access - cannot define matching Zen-C struct for stat
    int _z_fs_get_metadata(const char* path, uint64_t* size, int* is_dir, int* is_file) {
        struct stat st;
        if (stat(path, &st) != 0) return -1;
        *size = st.st_size;
        *is_dir = S_ISDIR(st.st_mode);
        *is_file = S_ISREG(st.st_mode);
        return 0;
    }
    
    // struct dirent access - readdir returns struct dirent*
    int _z_fs_read_entry(void* dir, char* out_name, int buf_size, int* is_dir) {
        struct dirent* ent = readdir((DIR*)dir);
        if (!ent) return 0;
        strncpy(out_name, ent->d_name, buf_size - 1);
        out_name[buf_size - 1] = 0;
#if defined(_DIRENT_HAVE_D_TYPE) || defined(DT_DIR)
        *is_dir = (ent->d_type == DT_DIR);
#else
        *is_dir = 0; // Fallback for Windows/MinGW if d_type missing
#endif
        return 1;
    }
 
    // mkdir has different signatures on Windows vs POSIX
    int _z_fs_mkdir(const char* path) {
        #ifdef _WIN32
            return mkdir(path);
        #else
            return mkdir(path, 0777);
        #endif
    }

const int32_t Z_SEEK_SET = 0;
const int32_t Z_SEEK_END = 2;
const int32_t Z_F_OK = 0;
const uint32_t SCREEN_WIDTH = 800;
const uint32_t SCREEN_HEIGHT = 600;
Clone Clone__clone(Clone* self) {
    void* ret = self->vtable->clone(self->self);
    return (Clone){.self = ret, .vtable = self->vtable};
}



void Drop__drop(Drop* self) {
    return self->vtable->drop(self->self);
}


Result_string Result_string__Ok(string v);
Result_string Result_string__Err(char* e);
bool Result_string__is_ok(Result_string* self);
bool Result_string__is_err(Result_string* self);
void Result_string__forget(Result_string* self);
string Result_string__unwrap(Result_string* self);
string* Result_string__unwrap_ref(Result_string* self);
string Result_string__expect(Result_string* self, char* msg);
Result_uint32_t Result_uint32_t__Ok(uint32_t v);
Result_uint32_t Result_uint32_t__Err(char* e);
bool Result_uint32_t__is_ok(Result_uint32_t* self);
bool Result_uint32_t__is_err(Result_uint32_t* self);
void Result_uint32_t__forget(Result_uint32_t* self);
uint32_t Result_uint32_t__unwrap(Result_uint32_t* self);
uint32_t* Result_uint32_t__unwrap_ref(Result_uint32_t* self);
uint32_t Result_uint32_t__expect(Result_uint32_t* self, char* msg);
Result_ShaderProgram Result_ShaderProgram__Ok(ShaderProgram v);
Result_ShaderProgram Result_ShaderProgram__Err(char* e);
bool Result_ShaderProgram__is_ok(Result_ShaderProgram* self);
bool Result_ShaderProgram__is_err(Result_ShaderProgram* self);
void Result_ShaderProgram__forget(Result_ShaderProgram* self);
ShaderProgram Result_ShaderProgram__unwrap(Result_ShaderProgram* self);
ShaderProgram* Result_ShaderProgram__unwrap_ref(Result_ShaderProgram* self);
ShaderProgram Result_ShaderProgram__expect(Result_ShaderProgram* self, char* msg);
Result_Vec_DirEntry Result_Vec_DirEntry__Ok(Vec_DirEntry v);
Result_Vec_DirEntry Result_Vec_DirEntry__Err(char* e);
bool Result_Vec_DirEntry__is_ok(Result_Vec_DirEntry* self);
bool Result_Vec_DirEntry__is_err(Result_Vec_DirEntry* self);
void Result_Vec_DirEntry__forget(Result_Vec_DirEntry* self);
Vec_DirEntry Result_Vec_DirEntry__unwrap(Result_Vec_DirEntry* self);
Vec_DirEntry* Result_Vec_DirEntry__unwrap_ref(Result_Vec_DirEntry* self);
Vec_DirEntry Result_Vec_DirEntry__expect(Result_Vec_DirEntry* self, char* msg);
Vec_DirEntryPtr Vec_DirEntryPtr__new(void);
Vec_DirEntryPtr Vec_DirEntryPtr__with_capacity(size_t cap);
void Vec_DirEntryPtr__grow(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__grow_to_fit(Vec_DirEntryPtr* self, size_t new_len);
VecIter_DirEntryPtr Vec_DirEntryPtr__iterator(Vec_DirEntryPtr* self);
VecIterRef_DirEntryPtr Vec_DirEntryPtr__iter_ref(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__push(Vec_DirEntryPtr* self, DirEntry* item);
void Vec_DirEntryPtr__insert(Vec_DirEntryPtr* self, size_t idx, DirEntry* item);
DirEntry* Vec_DirEntryPtr__pop(Vec_DirEntryPtr* self);
Option_DirEntryPtr Vec_DirEntryPtr__pop_opt(Vec_DirEntryPtr* self);
DirEntry* Vec_DirEntryPtr__remove(Vec_DirEntryPtr* self, size_t idx);
void Vec_DirEntryPtr__append(Vec_DirEntryPtr* self, Vec_DirEntryPtr other);
DirEntry* Vec_DirEntryPtr__get(Vec_DirEntryPtr* self, size_t idx);
DirEntry* Vec_DirEntryPtr__index(Vec_DirEntryPtr* self, size_t idx);
DirEntry** Vec_DirEntryPtr__get_ref(Vec_DirEntryPtr* self, size_t idx);
DirEntry* Vec_DirEntryPtr__last(Vec_DirEntryPtr* self);
size_t Vec_DirEntryPtr__length(Vec_DirEntryPtr* self);
bool Vec_DirEntryPtr__contains(Vec_DirEntryPtr* self, DirEntry* item);
bool Vec_DirEntryPtr__is_empty(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__clear(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__free(Vec_DirEntryPtr* self);
DirEntry* Vec_DirEntryPtr__first(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__set(Vec_DirEntryPtr* self, size_t idx, DirEntry* item);
void Vec_DirEntryPtr__reverse(Vec_DirEntryPtr* self);
bool Vec_DirEntryPtr__eq(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other);
void Vec_DirEntryPtr__forget(Vec_DirEntryPtr* self);
Vec_DirEntryPtr Vec_DirEntryPtr__add(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other);
void Vec_DirEntryPtr__add_assign(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other);
bool Vec_DirEntryPtr__neq(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other);
void Vec_DirEntryPtr__shl(Vec_DirEntryPtr* self, DirEntry* item);
void Vec_DirEntryPtr__shr(Vec_DirEntryPtr* self, DirEntry** out_item);
Vec_DirEntryPtr Vec_DirEntryPtr__mul(Vec_DirEntryPtr* self, size_t count);
void Vec_DirEntryPtr__mul_assign(Vec_DirEntryPtr* self, size_t count);
Vec_DirEntryPtr Vec_DirEntryPtr__clone(Vec_DirEntryPtr* self);
VecIterResult_DirEntryPtr VecIterRef_DirEntryPtr__next(VecIterRef_DirEntryPtr* self);
VecIterRef_DirEntryPtr VecIterRef_DirEntryPtr__iterator(VecIterRef_DirEntryPtr* self);
bool VecIterResult_DirEntryPtr__is_none(VecIterResult_DirEntryPtr* self);
struct DirEntry** VecIterResult_DirEntryPtr__unwrap(VecIterResult_DirEntryPtr* self);
Option_DirEntryPtr VecIter_DirEntryPtr__next(VecIter_DirEntryPtr* self);
VecIter_DirEntryPtr VecIter_DirEntryPtr__iterator(VecIter_DirEntryPtr* self);
Option_DirEntryPtr Option_DirEntryPtr__Some(struct DirEntry* v);
Option_DirEntryPtr Option_DirEntryPtr__None(void);
bool Option_DirEntryPtr__is_some(Option_DirEntryPtr* self);
bool Option_DirEntryPtr__is_none(Option_DirEntryPtr* self);
void Option_DirEntryPtr__forget(Option_DirEntryPtr* self);
struct DirEntry* Option_DirEntryPtr__unwrap(Option_DirEntryPtr* self);
struct DirEntry** Option_DirEntryPtr__unwrap_ref(Option_DirEntryPtr* self);
struct DirEntry* Option_DirEntryPtr__unwrap_or(Option_DirEntryPtr* self, struct DirEntry* def_val);
struct DirEntry* Option_DirEntryPtr__expect(Option_DirEntryPtr* self, char* msg);
Option_DirEntryPtr Option_DirEntryPtr__or_else(Option_DirEntryPtr* self, Option_DirEntryPtr other);
void Vec_DirEntryPtr__Drop_drop(Vec_DirEntryPtr* self);
Vec_DirEntry Vec_DirEntry__new(void);
Vec_DirEntry Vec_DirEntry__with_capacity(size_t cap);
void Vec_DirEntry__grow(Vec_DirEntry* self);
void Vec_DirEntry__grow_to_fit(Vec_DirEntry* self, size_t new_len);
VecIter_DirEntry Vec_DirEntry__iterator(Vec_DirEntry* self);
VecIterRef_DirEntry Vec_DirEntry__iter_ref(Vec_DirEntry* self);
void Vec_DirEntry__push(Vec_DirEntry* self, DirEntry item);
void Vec_DirEntry__insert(Vec_DirEntry* self, size_t idx, DirEntry item);
DirEntry Vec_DirEntry__pop(Vec_DirEntry* self);
Option_DirEntry Vec_DirEntry__pop_opt(Vec_DirEntry* self);
DirEntry Vec_DirEntry__remove(Vec_DirEntry* self, size_t idx);
void Vec_DirEntry__append(Vec_DirEntry* self, Vec_DirEntry other);
DirEntry Vec_DirEntry__get(Vec_DirEntry* self, size_t idx);
DirEntry Vec_DirEntry__index(Vec_DirEntry* self, size_t idx);
DirEntry* Vec_DirEntry__get_ref(Vec_DirEntry* self, size_t idx);
DirEntry Vec_DirEntry__last(Vec_DirEntry* self);
size_t Vec_DirEntry__length(Vec_DirEntry* self);
bool Vec_DirEntry__contains(Vec_DirEntry* self, DirEntry item);
bool Vec_DirEntry__is_empty(Vec_DirEntry* self);
void Vec_DirEntry__clear(Vec_DirEntry* self);
void Vec_DirEntry__free(Vec_DirEntry* self);
DirEntry Vec_DirEntry__first(Vec_DirEntry* self);
void Vec_DirEntry__set(Vec_DirEntry* self, size_t idx, DirEntry item);
void Vec_DirEntry__reverse(Vec_DirEntry* self);
bool Vec_DirEntry__eq(Vec_DirEntry* self, Vec_DirEntry* other);
void Vec_DirEntry__forget(Vec_DirEntry* self);
Vec_DirEntry Vec_DirEntry__add(Vec_DirEntry* self, Vec_DirEntry* other);
void Vec_DirEntry__add_assign(Vec_DirEntry* self, Vec_DirEntry* other);
bool Vec_DirEntry__neq(Vec_DirEntry* self, Vec_DirEntry* other);
void Vec_DirEntry__shl(Vec_DirEntry* self, DirEntry item);
void Vec_DirEntry__shr(Vec_DirEntry* self, DirEntry* out_item);
Vec_DirEntry Vec_DirEntry__mul(Vec_DirEntry* self, size_t count);
void Vec_DirEntry__mul_assign(Vec_DirEntry* self, size_t count);
Vec_DirEntry Vec_DirEntry__clone(Vec_DirEntry* self);
VecIterResult_DirEntry VecIterRef_DirEntry__next(VecIterRef_DirEntry* self);
VecIterRef_DirEntry VecIterRef_DirEntry__iterator(VecIterRef_DirEntry* self);
bool VecIterResult_DirEntry__is_none(VecIterResult_DirEntry* self);
DirEntry* VecIterResult_DirEntry__unwrap(VecIterResult_DirEntry* self);
Option_DirEntry VecIter_DirEntry__next(VecIter_DirEntry* self);
VecIter_DirEntry VecIter_DirEntry__iterator(VecIter_DirEntry* self);
Option_DirEntry Option_DirEntry__Some(DirEntry v);
Option_DirEntry Option_DirEntry__None(void);
bool Option_DirEntry__is_some(Option_DirEntry* self);
bool Option_DirEntry__is_none(Option_DirEntry* self);
void Option_DirEntry__forget(Option_DirEntry* self);
DirEntry Option_DirEntry__unwrap(Option_DirEntry* self);
DirEntry* Option_DirEntry__unwrap_ref(Option_DirEntry* self);
DirEntry Option_DirEntry__unwrap_or(Option_DirEntry* self, DirEntry def_val);
DirEntry Option_DirEntry__expect(Option_DirEntry* self, char* msg);
Option_DirEntry Option_DirEntry__or_else(Option_DirEntry* self, Option_DirEntry other);
void Vec_DirEntry__Drop_drop(Vec_DirEntry* self);
Result_Metadata Result_Metadata__Ok(Metadata v);
Result_Metadata Result_Metadata__Err(char* e);
bool Result_Metadata__is_ok(Result_Metadata* self);
bool Result_Metadata__is_err(Result_Metadata* self);
void Result_Metadata__forget(Result_Metadata* self);
Metadata Result_Metadata__unwrap(Result_Metadata* self);
Metadata* Result_Metadata__unwrap_ref(Result_Metadata* self);
Metadata Result_Metadata__expect(Result_Metadata* self, char* msg);
Result_Vec_String Result_Vec_String__Ok(Vec_String v);
Result_Vec_String Result_Vec_String__Err(char* e);
bool Result_Vec_String__is_ok(Result_Vec_String* self);
bool Result_Vec_String__is_err(Result_Vec_String* self);
void Result_Vec_String__forget(Result_Vec_String* self);
Vec_String Result_Vec_String__unwrap(Result_Vec_String* self);
Vec_String* Result_Vec_String__unwrap_ref(Result_Vec_String* self);
Vec_String Result_Vec_String__expect(Result_Vec_String* self, char* msg);
Vec_StringPtr Vec_StringPtr__new(void);
Vec_StringPtr Vec_StringPtr__with_capacity(size_t cap);
void Vec_StringPtr__grow(Vec_StringPtr* self);
void Vec_StringPtr__grow_to_fit(Vec_StringPtr* self, size_t new_len);
VecIter_StringPtr Vec_StringPtr__iterator(Vec_StringPtr* self);
VecIterRef_StringPtr Vec_StringPtr__iter_ref(Vec_StringPtr* self);
void Vec_StringPtr__push(Vec_StringPtr* self, String* item);
void Vec_StringPtr__insert(Vec_StringPtr* self, size_t idx, String* item);
String* Vec_StringPtr__pop(Vec_StringPtr* self);
Option_StringPtr Vec_StringPtr__pop_opt(Vec_StringPtr* self);
String* Vec_StringPtr__remove(Vec_StringPtr* self, size_t idx);
void Vec_StringPtr__append(Vec_StringPtr* self, Vec_StringPtr other);
String* Vec_StringPtr__get(Vec_StringPtr* self, size_t idx);
String* Vec_StringPtr__index(Vec_StringPtr* self, size_t idx);
String** Vec_StringPtr__get_ref(Vec_StringPtr* self, size_t idx);
String* Vec_StringPtr__last(Vec_StringPtr* self);
size_t Vec_StringPtr__length(Vec_StringPtr* self);
bool Vec_StringPtr__contains(Vec_StringPtr* self, String* item);
bool Vec_StringPtr__is_empty(Vec_StringPtr* self);
void Vec_StringPtr__clear(Vec_StringPtr* self);
void Vec_StringPtr__free(Vec_StringPtr* self);
String* Vec_StringPtr__first(Vec_StringPtr* self);
void Vec_StringPtr__set(Vec_StringPtr* self, size_t idx, String* item);
void Vec_StringPtr__reverse(Vec_StringPtr* self);
bool Vec_StringPtr__eq(Vec_StringPtr* self, Vec_StringPtr* other);
void Vec_StringPtr__forget(Vec_StringPtr* self);
Vec_StringPtr Vec_StringPtr__add(Vec_StringPtr* self, Vec_StringPtr* other);
void Vec_StringPtr__add_assign(Vec_StringPtr* self, Vec_StringPtr* other);
bool Vec_StringPtr__neq(Vec_StringPtr* self, Vec_StringPtr* other);
void Vec_StringPtr__shl(Vec_StringPtr* self, String* item);
void Vec_StringPtr__shr(Vec_StringPtr* self, String** out_item);
Vec_StringPtr Vec_StringPtr__mul(Vec_StringPtr* self, size_t count);
void Vec_StringPtr__mul_assign(Vec_StringPtr* self, size_t count);
Vec_StringPtr Vec_StringPtr__clone(Vec_StringPtr* self);
VecIterResult_StringPtr VecIterRef_StringPtr__next(VecIterRef_StringPtr* self);
VecIterRef_StringPtr VecIterRef_StringPtr__iterator(VecIterRef_StringPtr* self);
bool VecIterResult_StringPtr__is_none(VecIterResult_StringPtr* self);
struct String** VecIterResult_StringPtr__unwrap(VecIterResult_StringPtr* self);
Option_StringPtr VecIter_StringPtr__next(VecIter_StringPtr* self);
VecIter_StringPtr VecIter_StringPtr__iterator(VecIter_StringPtr* self);
Option_StringPtr Option_StringPtr__Some(struct String* v);
Option_StringPtr Option_StringPtr__None(void);
bool Option_StringPtr__is_some(Option_StringPtr* self);
bool Option_StringPtr__is_none(Option_StringPtr* self);
void Option_StringPtr__forget(Option_StringPtr* self);
struct String* Option_StringPtr__unwrap(Option_StringPtr* self);
struct String** Option_StringPtr__unwrap_ref(Option_StringPtr* self);
struct String* Option_StringPtr__unwrap_or(Option_StringPtr* self, struct String* def_val);
struct String* Option_StringPtr__expect(Option_StringPtr* self, char* msg);
Option_StringPtr Option_StringPtr__or_else(Option_StringPtr* self, Option_StringPtr other);
void Vec_StringPtr__Drop_drop(Vec_StringPtr* self);
Result_bool Result_bool__Ok(bool v);
Result_bool Result_bool__Err(char* e);
bool Result_bool__is_ok(Result_bool* self);
bool Result_bool__is_err(Result_bool* self);
void Result_bool__forget(Result_bool* self);
bool Result_bool__unwrap(Result_bool* self);
bool* Result_bool__unwrap_ref(Result_bool* self);
bool Result_bool__expect(Result_bool* self, char* msg);
Result_String Result_String__Ok(String v);
Result_String Result_String__Err(char* e);
bool Result_String__is_ok(Result_String* self);
bool Result_String__is_err(Result_String* self);
void Result_String__forget(Result_String* self);
String Result_String__unwrap(Result_String* self);
String* Result_String__unwrap_ref(Result_String* self);
String Result_String__expect(Result_String* self, char* msg);
Result_File Result_File__Ok(File v);
Result_File Result_File__Err(char* e);
bool Result_File__is_ok(Result_File* self);
bool Result_File__is_err(Result_File* self);
void Result_File__forget(Result_File* self);
File Result_File__unwrap(Result_File* self);
File* Result_File__unwrap_ref(Result_File* self);
File Result_File__expect(Result_File* self, char* msg);
Vec_String Vec_String__new(void);
Vec_String Vec_String__with_capacity(size_t cap);
void Vec_String__grow(Vec_String* self);
void Vec_String__grow_to_fit(Vec_String* self, size_t new_len);
VecIter_String Vec_String__iterator(Vec_String* self);
VecIterRef_String Vec_String__iter_ref(Vec_String* self);
void Vec_String__push(Vec_String* self, String item);
void Vec_String__insert(Vec_String* self, size_t idx, String item);
String Vec_String__pop(Vec_String* self);
Option_String Vec_String__pop_opt(Vec_String* self);
String Vec_String__remove(Vec_String* self, size_t idx);
void Vec_String__append(Vec_String* self, Vec_String other);
String Vec_String__get(Vec_String* self, size_t idx);
String Vec_String__index(Vec_String* self, size_t idx);
String* Vec_String__get_ref(Vec_String* self, size_t idx);
String Vec_String__last(Vec_String* self);
size_t Vec_String__length(Vec_String* self);
bool Vec_String__contains(Vec_String* self, String item);
bool Vec_String__is_empty(Vec_String* self);
void Vec_String__clear(Vec_String* self);
void Vec_String__free(Vec_String* self);
String Vec_String__first(Vec_String* self);
void Vec_String__set(Vec_String* self, size_t idx, String item);
void Vec_String__reverse(Vec_String* self);
bool Vec_String__eq(Vec_String* self, Vec_String* other);
void Vec_String__forget(Vec_String* self);
Vec_String Vec_String__add(Vec_String* self, Vec_String* other);
void Vec_String__add_assign(Vec_String* self, Vec_String* other);
bool Vec_String__neq(Vec_String* self, Vec_String* other);
void Vec_String__shl(Vec_String* self, String item);
void Vec_String__shr(Vec_String* self, String* out_item);
Vec_String Vec_String__mul(Vec_String* self, size_t count);
void Vec_String__mul_assign(Vec_String* self, size_t count);
Vec_String Vec_String__clone(Vec_String* self);
VecIterResult_String VecIterRef_String__next(VecIterRef_String* self);
VecIterRef_String VecIterRef_String__iterator(VecIterRef_String* self);
bool VecIterResult_String__is_none(VecIterResult_String* self);
String* VecIterResult_String__unwrap(VecIterResult_String* self);
Option_String VecIter_String__next(VecIter_String* self);
VecIter_String VecIter_String__iterator(VecIter_String* self);
Option_String Option_String__Some(String v);
Option_String Option_String__None(void);
bool Option_String__is_some(Option_String* self);
bool Option_String__is_none(Option_String* self);
void Option_String__forget(Option_String* self);
String Option_String__unwrap(Option_String* self);
String* Option_String__unwrap_ref(Option_String* self);
String Option_String__unwrap_or(Option_String* self, String def_val);
String Option_String__expect(Option_String* self, char* msg);
Option_String Option_String__or_else(Option_String* self, Option_String other);
void Vec_String__Drop_drop(Vec_String* self);
Vec_int32_t Vec_int32_t__new(void);
Vec_int32_t Vec_int32_t__with_capacity(size_t cap);
void Vec_int32_t__grow(Vec_int32_t* self);
void Vec_int32_t__grow_to_fit(Vec_int32_t* self, size_t new_len);
VecIter_int32_t Vec_int32_t__iterator(Vec_int32_t* self);
VecIterRef_int32_t Vec_int32_t__iter_ref(Vec_int32_t* self);
void Vec_int32_t__push(Vec_int32_t* self, int32_t item);
void Vec_int32_t__insert(Vec_int32_t* self, size_t idx, int32_t item);
int32_t Vec_int32_t__pop(Vec_int32_t* self);
Option_int32_t Vec_int32_t__pop_opt(Vec_int32_t* self);
int32_t Vec_int32_t__remove(Vec_int32_t* self, size_t idx);
void Vec_int32_t__append(Vec_int32_t* self, Vec_int32_t other);
int32_t Vec_int32_t__get(Vec_int32_t* self, size_t idx);
int32_t Vec_int32_t__index(Vec_int32_t* self, size_t idx);
int32_t* Vec_int32_t__get_ref(Vec_int32_t* self, size_t idx);
int32_t Vec_int32_t__last(Vec_int32_t* self);
size_t Vec_int32_t__length(Vec_int32_t* self);
bool Vec_int32_t__contains(Vec_int32_t* self, int32_t item);
bool Vec_int32_t__is_empty(Vec_int32_t* self);
void Vec_int32_t__clear(Vec_int32_t* self);
void Vec_int32_t__free(Vec_int32_t* self);
int32_t Vec_int32_t__first(Vec_int32_t* self);
void Vec_int32_t__set(Vec_int32_t* self, size_t idx, int32_t item);
void Vec_int32_t__reverse(Vec_int32_t* self);
bool Vec_int32_t__eq(Vec_int32_t* self, Vec_int32_t* other);
void Vec_int32_t__forget(Vec_int32_t* self);
Vec_int32_t Vec_int32_t__add(Vec_int32_t* self, Vec_int32_t* other);
void Vec_int32_t__add_assign(Vec_int32_t* self, Vec_int32_t* other);
bool Vec_int32_t__neq(Vec_int32_t* self, Vec_int32_t* other);
void Vec_int32_t__shl(Vec_int32_t* self, int32_t item);
void Vec_int32_t__shr(Vec_int32_t* self, int32_t* out_item);
Vec_int32_t Vec_int32_t__mul(Vec_int32_t* self, size_t count);
void Vec_int32_t__mul_assign(Vec_int32_t* self, size_t count);
Vec_int32_t Vec_int32_t__clone(Vec_int32_t* self);
VecIterResult_int32_t VecIterRef_int32_t__next(VecIterRef_int32_t* self);
VecIterRef_int32_t VecIterRef_int32_t__iterator(VecIterRef_int32_t* self);
bool VecIterResult_int32_t__is_none(VecIterResult_int32_t* self);
int32_t* VecIterResult_int32_t__unwrap(VecIterResult_int32_t* self);
Option_int32_t VecIter_int32_t__next(VecIter_int32_t* self);
VecIter_int32_t VecIter_int32_t__iterator(VecIter_int32_t* self);
void Vec_int32_t__Drop_drop(Vec_int32_t* self);
Vec_size_t Vec_size_t__new(void);
Vec_size_t Vec_size_t__with_capacity(size_t cap);
void Vec_size_t__grow(Vec_size_t* self);
void Vec_size_t__grow_to_fit(Vec_size_t* self, size_t new_len);
VecIter_size_t Vec_size_t__iterator(Vec_size_t* self);
VecIterRef_size_t Vec_size_t__iter_ref(Vec_size_t* self);
void Vec_size_t__push(Vec_size_t* self, size_t item);
void Vec_size_t__insert(Vec_size_t* self, size_t idx, size_t item);
size_t Vec_size_t__pop(Vec_size_t* self);
Option_size_t Vec_size_t__pop_opt(Vec_size_t* self);
size_t Vec_size_t__remove(Vec_size_t* self, size_t idx);
void Vec_size_t__append(Vec_size_t* self, Vec_size_t other);
size_t Vec_size_t__get(Vec_size_t* self, size_t idx);
size_t Vec_size_t__index(Vec_size_t* self, size_t idx);
size_t* Vec_size_t__get_ref(Vec_size_t* self, size_t idx);
size_t Vec_size_t__last(Vec_size_t* self);
size_t Vec_size_t__length(Vec_size_t* self);
bool Vec_size_t__contains(Vec_size_t* self, size_t item);
bool Vec_size_t__is_empty(Vec_size_t* self);
void Vec_size_t__clear(Vec_size_t* self);
void Vec_size_t__free(Vec_size_t* self);
size_t Vec_size_t__first(Vec_size_t* self);
void Vec_size_t__set(Vec_size_t* self, size_t idx, size_t item);
void Vec_size_t__reverse(Vec_size_t* self);
bool Vec_size_t__eq(Vec_size_t* self, Vec_size_t* other);
void Vec_size_t__forget(Vec_size_t* self);
Vec_size_t Vec_size_t__add(Vec_size_t* self, Vec_size_t* other);
void Vec_size_t__add_assign(Vec_size_t* self, Vec_size_t* other);
bool Vec_size_t__neq(Vec_size_t* self, Vec_size_t* other);
void Vec_size_t__shl(Vec_size_t* self, size_t item);
void Vec_size_t__shr(Vec_size_t* self, size_t* out_item);
Vec_size_t Vec_size_t__mul(Vec_size_t* self, size_t count);
void Vec_size_t__mul_assign(Vec_size_t* self, size_t count);
Vec_size_t Vec_size_t__clone(Vec_size_t* self);
VecIterResult_size_t VecIterRef_size_t__next(VecIterRef_size_t* self);
VecIterRef_size_t VecIterRef_size_t__iterator(VecIterRef_size_t* self);
bool VecIterResult_size_t__is_none(VecIterResult_size_t* self);
size_t* VecIterResult_size_t__unwrap(VecIterResult_size_t* self);
Option_size_t VecIter_size_t__next(VecIter_size_t* self);
VecIter_size_t VecIter_size_t__iterator(VecIter_size_t* self);
void Vec_size_t__Drop_drop(Vec_size_t* self);
Option_size_t Option_size_t__Some(size_t v);
Option_size_t Option_size_t__None(void);
bool Option_size_t__is_some(Option_size_t* self);
bool Option_size_t__is_none(Option_size_t* self);
void Option_size_t__forget(Option_size_t* self);
size_t Option_size_t__unwrap(Option_size_t* self);
size_t* Option_size_t__unwrap_ref(Option_size_t* self);
size_t Option_size_t__unwrap_or(Option_size_t* self, size_t def_val);
size_t Option_size_t__expect(Option_size_t* self, char* msg);
Option_size_t Option_size_t__or_else(Option_size_t* self, Option_size_t other);
Option_int32_t Option_int32_t__Some(int32_t v);
Option_int32_t Option_int32_t__None(void);
bool Option_int32_t__is_some(Option_int32_t* self);
bool Option_int32_t__is_none(Option_int32_t* self);
void Option_int32_t__forget(Option_int32_t* self);
int32_t Option_int32_t__unwrap(Option_int32_t* self);
int32_t* Option_int32_t__unwrap_ref(Option_int32_t* self);
int32_t Option_int32_t__unwrap_or(Option_int32_t* self, int32_t def_val);
int32_t Option_int32_t__expect(Option_int32_t* self, char* msg);
Option_int32_t Option_int32_t__or_else(Option_int32_t* self, Option_int32_t other);
Vec_char Vec_char__new(void);
Vec_char Vec_char__with_capacity(size_t cap);
void Vec_char__grow(Vec_char* self);
void Vec_char__grow_to_fit(Vec_char* self, size_t new_len);
VecIter_char Vec_char__iterator(Vec_char* self);
VecIterRef_char Vec_char__iter_ref(Vec_char* self);
void Vec_char__push(Vec_char* self, char item);
void Vec_char__insert(Vec_char* self, size_t idx, char item);
char Vec_char__pop(Vec_char* self);
Option_char Vec_char__pop_opt(Vec_char* self);
char Vec_char__remove(Vec_char* self, size_t idx);
void Vec_char__append(Vec_char* self, Vec_char other);
char Vec_char__get(Vec_char* self, size_t idx);
char Vec_char__index(Vec_char* self, size_t idx);
char* Vec_char__get_ref(Vec_char* self, size_t idx);
char Vec_char__last(Vec_char* self);
size_t Vec_char__length(Vec_char* self);
bool Vec_char__contains(Vec_char* self, char item);
bool Vec_char__is_empty(Vec_char* self);
void Vec_char__clear(Vec_char* self);
void Vec_char__free(Vec_char* self);
char Vec_char__first(Vec_char* self);
void Vec_char__set(Vec_char* self, size_t idx, char item);
void Vec_char__reverse(Vec_char* self);
bool Vec_char__eq(Vec_char* self, Vec_char* other);
void Vec_char__forget(Vec_char* self);
Vec_char Vec_char__add(Vec_char* self, Vec_char* other);
void Vec_char__add_assign(Vec_char* self, Vec_char* other);
bool Vec_char__neq(Vec_char* self, Vec_char* other);
void Vec_char__shl(Vec_char* self, char item);
void Vec_char__shr(Vec_char* self, char* out_item);
Vec_char Vec_char__mul(Vec_char* self, size_t count);
void Vec_char__mul_assign(Vec_char* self, size_t count);
Vec_char Vec_char__clone(Vec_char* self);
VecIterResult_char VecIterRef_char__next(VecIterRef_char* self);
VecIterRef_char VecIterRef_char__iterator(VecIterRef_char* self);
bool VecIterResult_char__is_none(VecIterResult_char* self);
char* VecIterResult_char__unwrap(VecIterResult_char* self);
Option_char VecIter_char__next(VecIter_char* self);
VecIter_char VecIter_char__iterator(VecIter_char* self);
Option_char Option_char__Some(char v);
Option_char Option_char__None(void);
bool Option_char__is_some(Option_char* self);
bool Option_char__is_none(Option_char* self);
void Option_char__forget(Option_char* self);
char Option_char__unwrap(Option_char* self);
char* Option_char__unwrap_ref(Option_char* self);
char Option_char__unwrap_or(Option_char* self, char def_val);
char Option_char__expect(Option_char* self, char* msg);
Option_char Option_char__or_else(Option_char* self, Option_char other);
void Vec_char__Drop_drop(Vec_char* self);
int main(void);
void framebuffer_size_callback(GLFWwindow* _window, int32_t width, int32_t height);
void process_input(GLFWwindow* window);
int32_t _main(void);
void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height);
int _z_fs_closedir(void* dir);
void* _z_fs_opendir(const char* name);
int _z_fs_read_entry(void* dir, char* out_name, int buf_size, int* is_dir);
int _z_fs_get_metadata(const char* path, uint64_t* size, int* is_dir, int* is_file);
int _z_fs_mkdir(const char* path);
int32_t strcasecmp(const char* s1, const char* s2);
void sort_double(double* arr, size_t len);
void sort_float(float* arr, size_t len);
void sort_long(long* arr, size_t len);
void sort_int(int32_t* arr, size_t len);
void __zenc_todo_impl(const char* file, int32_t line, const char* func, const char* msg);
void __zenc_panic_impl(const char* file, int32_t line, const char* func, const char* msg);
double Math__PI(void);
double Math__E(void);
double Math__abs(double x);
double Math__sin(double x);
double Math__cos(double x);
double Math__tan(double x);
double Math__asin(double x);
double Math__acos(double x);
double Math__atan(double x);
double Math__atan2(double y, double x);
double Math__sqrt(double x);
double Math__pow(double base, double exp);
double Math__exp(double x);
double Math__log(double x);
double Math__log10(double x);
double Math__ceil(double x);
double Math__floor(double x);
double Math__round(double x);
double Math__mod(double x, double y);
double Math__max(double a, double b);
double Math__min(double a, double b);
void ShaderProgram__Drop_drop(ShaderProgram* self);
Result_ShaderProgram ShaderProgram__new(char* vertexPath, char* fragmentPath);
Result_uint32_t ShaderProgram__compile_shader(const char* source, uint32_t glType, char* typeStr);
Result_string ShaderProgram__check_compile_errors(uint32_t object, char* typeStr);
ShaderProgram ShaderProgram__newId(uint32_t id);
void ShaderProgram__useProgram(ShaderProgram* self);
void ShaderProgram__setBool(ShaderProgram* self, char* name, bool value);
void ShaderProgram__setInt(ShaderProgram* self, char* name, int32_t value);
void ShaderProgram__setFloat(ShaderProgram* self, char* name, float value);
void ShaderProgram__setMat4(ShaderProgram* self, char* name, mat4 value);
void File__Drop_drop(File* self);
Result_File File__open(char* path, char* mode);
void File__close(File* self);
Result_String File__read_to_string(File* self);
Result_String File__read_all(char* path);
Result_bool File__write_string(File* self, char* content);
Result_Vec_String File__read_lines(char* path);
Result_bool File__write_lines(char* path, Vec_String* lines);
bool File__exists(char* path);
Result_Metadata File__metadata(char* path);
Result_bool File__create_dir(char* path);
Result_bool File__remove_file(char* path);
Result_bool File__remove_dir(char* path);
Result_Vec_DirEntry File__read_dir(char* path);
Result_String File__current_dir(void);
String String__new(char* s);
String String__from(char* s);
String String__from_rune(int32_t r);
String String__from_runes(int32_t* runes, size_t count);
char* String__c_str(String* self);
char* String__to_string(String* self);
void String__destroy(String* self);
void String__forget(String* self);
void String__append(String* self, String* other);
void String__append_c(String* self, char* s);
void String__push_rune(String* self, int32_t r);
void String__append_c_ptr(String* ptr, char* s);
String String__add(String* self, String* other);
void String__add_assign(String* self, String* other);
bool String__eq(String* self, String* other);
bool String__neq(String* self, String* other);
int32_t String__compare(String* self, String* other);
bool String__lt(String* self, String* other);
bool String__gt(String* self, String* other);
bool String__le(String* self, String* other);
bool String__ge(String* self, String* other);
int32_t String__compare_ignore_case(String* self, String* other);
bool String__eq_ignore_case(String* self, String* other);
bool String__eq_str(String* self, char* s);
size_t String__length(String* self);
String String__substring(String* self, size_t start, size_t len);
bool String__contains_str(String* self, char* target);
Option_size_t String__find_str(String* self, char* target);
Vec_size_t String__find_all_str(String* self, char* target);
String String__to_lowercase(String* self);
String String__pad_right(String* self, size_t target_len, char pad_char);
String String__pad_left(String* self, size_t target_len, char pad_char);
String String__to_uppercase(String* self);
Option_size_t String__find(String* self, char target);
void String__print(String* self);
void String__println(String* self);
bool String__is_empty(String* self);
bool String__contains(String* self, char target);
bool String__starts_with(String* self, char* prefix);
bool String__ends_with(String* self, char* suffix);
void String__reserve(String* self, size_t cap);
void String__free(String* self);
size_t String___utf8_seq_len(char first_byte);
size_t String__utf8_len(String* self);
String String__utf8_at(String* self, size_t idx);
int32_t String__utf8_get(String* self, size_t idx);
Vec_int32_t String__runes(String* self);
StringCharsIter String__iterator(String* self);
StringCharsIter String__chars(String* self);
String String__from_runes_vec(Vec_int32_t runes);
void String__insert_rune(String* self, size_t idx, int32_t r);
int32_t String__remove_rune_at(String* self, size_t idx);
String String__utf8_substr(String* self, size_t start_idx, size_t num_chars);
Vec_String String__split(String* self, char delim);
String String__trim(String* self);
String String__replace(String* self, char* target, char* replacement);
Option_int32_t StringCharsIter__next(StringCharsIter* self);
StringCharsIter StringCharsIter__iterator(StringCharsIter* self);
// Auto-Generated RAII Glue for Vec_DirEntryPtr
void Vec_DirEntryPtr__Drop_glue(Vec_DirEntryPtr *self) {
}

// Auto-Generated RAII Glue for Vec_DirEntry
void Vec_DirEntry__Drop_glue(Vec_DirEntry *self) {
}

// Auto-Generated RAII Glue for Vec_StringPtr
void Vec_StringPtr__Drop_glue(Vec_StringPtr *self) {
}

// Auto-Generated RAII Glue for Vec_String
void Vec_String__Drop_glue(Vec_String *self) {
}

// Auto-Generated RAII Glue for Vec_int32_t
void Vec_int32_t__Drop_glue(Vec_int32_t *self) {
}

// Auto-Generated RAII Glue for Vec_size_t
void Vec_size_t__Drop_glue(Vec_size_t *self) {
}

// Auto-Generated RAII Glue for Vec_char
void Vec_char__Drop_glue(Vec_char *self) {
}

// Auto-Generated RAII Glue for ShaderProgram
void ShaderProgram__Drop_glue(ShaderProgram *self) {
    ShaderProgram__Drop_drop(self);
}

// Auto-Generated RAII Glue for File
void File__Drop_glue(File *self) {
    File__Drop_drop(self);
}

// Auto-Generated RAII Glue for Result_ShaderProgram
void Result_ShaderProgram__Drop_glue(Result_ShaderProgram *self) {
    ShaderProgram__Drop_glue(&self->val);
}

// Auto-Generated RAII Glue for Result_Vec_DirEntry
void Result_Vec_DirEntry__Drop_glue(Result_Vec_DirEntry *self) {
    Vec_DirEntry__Drop_glue(&self->val);
}

// Auto-Generated RAII Glue for Result_Vec_String
void Result_Vec_String__Drop_glue(Result_Vec_String *self) {
    Vec_String__Drop_glue(&self->val);
}

// Auto-Generated RAII Glue for Result_File
void Result_File__Drop_glue(Result_File *self) {
    File__Drop_glue(&self->val);
}

// Auto-Generated RAII Glue for String
void String__Drop_glue(String *self) {
    Vec_char__Drop_glue(&self->vec);
}

// Auto-Generated RAII Glue for Result_String
void Result_String__Drop_glue(Result_String *self) {
    String__Drop_glue(&self->val);
}

// Auto-Generated RAII Glue for Option_String
void Option_String__Drop_glue(Option_String *self) {
    String__Drop_glue(&self->val);
}

// Auto-Generated RAII Glue for DirEntry
void DirEntry__Drop_glue(DirEntry *self) {
    String__Drop_glue(&self->name);
}

// Auto-Generated RAII Glue for Option_DirEntry
void Option_DirEntry__Drop_glue(Option_DirEntry *self) {
    DirEntry__Drop_glue(&self->val);
}


#line 10 "D:/zenc/std/result.zc"

Result_string Result_string__Ok(string v)
{
    {

#line 11 "D:/zenc/std/result.zc"
    return (Result_string){.is_ok = true, .val = v};
    }
}

#line 14 "D:/zenc/std/result.zc"

Result_string Result_string__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_string){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_string__is_ok(Result_string* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_string__is_err(Result_string* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_string__forget(Result_string* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(char*));
    }
}

#line 30 "D:/zenc/std/result.zc"

string Result_string__unwrap(Result_string* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    string v = self->val;

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(char*));

#line 37 "D:/zenc/std/result.zc"
    return v;
    }
}

#line 40 "D:/zenc/std/result.zc"

string* Result_string__unwrap_ref(Result_string* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

string Result_string__expect(Result_string* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_uint32_t Result_uint32_t__Ok(uint32_t v)
{
    {

#line 11 "D:/zenc/std/result.zc"
    return (Result_uint32_t){.is_ok = true, .val = v};
    }
}

#line 14 "D:/zenc/std/result.zc"

Result_uint32_t Result_uint32_t__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_uint32_t){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_uint32_t__is_ok(Result_uint32_t* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_uint32_t__is_err(Result_uint32_t* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_uint32_t__forget(Result_uint32_t* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(uint32_t));
    }
}

#line 30 "D:/zenc/std/result.zc"

uint32_t Result_uint32_t__unwrap(Result_uint32_t* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    uint32_t v = self->val;

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(uint32_t));

#line 37 "D:/zenc/std/result.zc"
    return v;
    }
}

#line 40 "D:/zenc/std/result.zc"

uint32_t* Result_uint32_t__unwrap_ref(Result_uint32_t* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

uint32_t Result_uint32_t__expect(Result_uint32_t* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_ShaderProgram Result_ShaderProgram__Ok(ShaderProgram v)
{

#line 10 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/result.zc"
    { Result_ShaderProgram _z_ret = (Result_ShaderProgram){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) ShaderProgram__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) ShaderProgram__Drop_glue(&v);
}

#line 14 "D:/zenc/std/result.zc"

Result_ShaderProgram Result_ShaderProgram__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_ShaderProgram){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_ShaderProgram__is_ok(Result_ShaderProgram* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_ShaderProgram__is_err(Result_ShaderProgram* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_ShaderProgram__forget(Result_ShaderProgram* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(ShaderProgram));
    }
}

#line 30 "D:/zenc/std/result.zc"

ShaderProgram Result_ShaderProgram__unwrap(Result_ShaderProgram* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1; ShaderProgram v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(ShaderProgram));

#line 37 "D:/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) ShaderProgram__Drop_glue(&v);
_z_ret_mv; });

#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) ShaderProgram__Drop_glue(&v);
    }
}

#line 40 "D:/zenc/std/result.zc"

ShaderProgram* Result_ShaderProgram__unwrap_ref(Result_ShaderProgram* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

ShaderProgram Result_ShaderProgram__expect(Result_ShaderProgram* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_Vec_DirEntry Result_Vec_DirEntry__Ok(Vec_DirEntry v)
{

#line 10 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/result.zc"
    { Result_Vec_DirEntry _z_ret = (Result_Vec_DirEntry){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
}

#line 14 "D:/zenc/std/result.zc"

Result_Vec_DirEntry Result_Vec_DirEntry__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_Vec_DirEntry){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_Vec_DirEntry__is_ok(Result_Vec_DirEntry* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_Vec_DirEntry__is_err(Result_Vec_DirEntry* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_Vec_DirEntry__forget(Result_Vec_DirEntry* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Vec_DirEntry));
    }
}

#line 30 "D:/zenc/std/result.zc"

Vec_DirEntry Result_Vec_DirEntry__unwrap(Result_Vec_DirEntry* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1; Vec_DirEntry v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Vec_DirEntry));

#line 37 "D:/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
_z_ret_mv; });

#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
    }
}

#line 40 "D:/zenc/std/result.zc"

Vec_DirEntry* Result_Vec_DirEntry__unwrap_ref(Result_Vec_DirEntry* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

Vec_DirEntry Result_Vec_DirEntry__expect(Result_Vec_DirEntry* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_DirEntryPtr Vec_DirEntryPtr__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_DirEntryPtr){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_DirEntryPtr Vec_DirEntryPtr__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_DirEntryPtr){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_DirEntryPtr){.data = ((DirEntry**)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(DirEntry*))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__grow(Vec_DirEntryPtr* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((DirEntry**)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry*))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__grow_to_fit(Vec_DirEntryPtr* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((DirEntry**)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry*))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_DirEntryPtr Vec_DirEntryPtr__iterator(Vec_DirEntryPtr* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_DirEntryPtr){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_DirEntryPtr Vec_DirEntryPtr__iter_ref(Vec_DirEntryPtr* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_DirEntryPtr){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__push(Vec_DirEntryPtr* self, DirEntry* item)
{
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__insert(Vec_DirEntryPtr* self, size_t idx, DirEntry* item)
{
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(DirEntry*)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntryPtr__pop(Vec_DirEntryPtr* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_DirEntryPtr Vec_DirEntryPtr__pop_opt(Vec_DirEntryPtr* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_DirEntryPtr__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_DirEntryPtr__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntryPtr__remove(Vec_DirEntryPtr* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    DirEntry* item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(DirEntry*)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__append(Vec_DirEntryPtr* self, Vec_DirEntryPtr other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(DirEntry*)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_DirEntryPtr__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntryPtr__get(Vec_DirEntryPtr* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntryPtr__index(Vec_DirEntryPtr* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

DirEntry** Vec_DirEntryPtr__get_ref(Vec_DirEntryPtr* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntryPtr__last(Vec_DirEntryPtr* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_DirEntryPtr__length(Vec_DirEntryPtr* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_DirEntryPtr__contains(Vec_DirEntryPtr* self, DirEntry* item)
{
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(DirEntry*)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_DirEntryPtr__is_empty(Vec_DirEntryPtr* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__clear(Vec_DirEntryPtr* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__free(Vec_DirEntryPtr* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntryPtr__first(Vec_DirEntryPtr* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__set(Vec_DirEntryPtr* self, size_t idx, DirEntry* item)
{
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__reverse(Vec_DirEntryPtr* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    DirEntry* tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_DirEntryPtr__eq(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(DirEntry*)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__forget(Vec_DirEntryPtr* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_DirEntryPtr Vec_DirEntryPtr__add(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_DirEntryPtr result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__append(&result, Vec_DirEntryPtr__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntryPtr__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntryPtr__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__add_assign(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__append(self, Vec_DirEntryPtr__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_DirEntryPtr__neq(Vec_DirEntryPtr* self, Vec_DirEntryPtr* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__shl(Vec_DirEntryPtr* self, DirEntry* item)
{
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__push(self, item);
    }
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__shr(Vec_DirEntryPtr* self, DirEntry** out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_DirEntryPtr Vec_DirEntryPtr__mul(Vec_DirEntryPtr* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_DirEntryPtr result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__append(&result, Vec_DirEntryPtr__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntryPtr__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntryPtr__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__mul_assign(Vec_DirEntryPtr* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(DirEntry*)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_DirEntryPtr Vec_DirEntryPtr__clone(Vec_DirEntryPtr* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_DirEntryPtr){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    DirEntry** new_data = ((DirEntry**)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(DirEntry*)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_DirEntryPtr){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_DirEntryPtr VecIterRef_DirEntryPtr__next(VecIterRef_DirEntryPtr* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    struct DirEntry** item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_DirEntryPtr){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_DirEntryPtr){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_DirEntryPtr VecIterRef_DirEntryPtr__iterator(VecIterRef_DirEntryPtr* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_DirEntryPtr__is_none(VecIterResult_DirEntryPtr* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

struct DirEntry** VecIterResult_DirEntryPtr__unwrap(VecIterResult_DirEntryPtr* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_DirEntryPtr VecIter_DirEntryPtr__next(VecIter_DirEntryPtr* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    struct DirEntry* item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    return 
#line 47 "D:/zenc/std/vec.zc"
Option_DirEntryPtr__Some(item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_DirEntryPtr__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_DirEntryPtr VecIter_DirEntryPtr__iterator(VecIter_DirEntryPtr* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_DirEntryPtr Option_DirEntryPtr__Some(struct DirEntry* v)
{
    {

#line 11 "D:/zenc/std/option.zc"
    return (Option_DirEntryPtr){.is_some = true, .val = v};
    }
}

#line 14 "D:/zenc/std/option.zc"

Option_DirEntryPtr Option_DirEntryPtr__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    Option_DirEntryPtr opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(struct DirEntry*));

#line 18 "D:/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_DirEntryPtr__is_some(Option_DirEntryPtr* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_DirEntryPtr__is_none(Option_DirEntryPtr* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_DirEntryPtr__forget(Option_DirEntryPtr* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct DirEntry*));
    }
}

#line 33 "D:/zenc/std/option.zc"

struct DirEntry* Option_DirEntryPtr__unwrap(Option_DirEntryPtr* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    struct DirEntry* v = self->val;

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct DirEntry*));

#line 40 "D:/zenc/std/option.zc"
    return v;
    }
}

#line 43 "D:/zenc/std/option.zc"

struct DirEntry** Option_DirEntryPtr__unwrap_ref(Option_DirEntryPtr* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

struct DirEntry* Option_DirEntryPtr__unwrap_or(Option_DirEntryPtr* self, struct DirEntry* def_val)
{
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    return self->val;
    }

#line 55 "D:/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "D:/zenc/std/option.zc"

struct DirEntry* Option_DirEntryPtr__expect(Option_DirEntryPtr* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_DirEntryPtr Option_DirEntryPtr__or_else(Option_DirEntryPtr* self, Option_DirEntryPtr other)
{
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    return *self;
    }

#line 68 "D:/zenc/std/option.zc"
    return other;
    }
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_DirEntryPtr__Drop_drop(Vec_DirEntryPtr* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_DirEntryPtr__free(self);
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_DirEntry Vec_DirEntry__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_DirEntry){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_DirEntry Vec_DirEntry__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_DirEntry){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_DirEntry){.data = ((DirEntry*)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(DirEntry))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_DirEntry__grow(Vec_DirEntry* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((DirEntry*)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_DirEntry__grow_to_fit(Vec_DirEntry* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((DirEntry*)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_DirEntry Vec_DirEntry__iterator(Vec_DirEntry* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_DirEntry){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_DirEntry Vec_DirEntry__iter_ref(Vec_DirEntry* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_DirEntry){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_DirEntry__push(Vec_DirEntry* self, DirEntry item)
{

#line 123 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_DirEntry__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = ({ __z_drop_flag_item = 0; item; }));

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 123 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_DirEntry__insert(Vec_DirEntry* self, size_t idx, DirEntry item)
{

#line 131 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_DirEntry__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(DirEntry)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 131 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

#line 147 "D:/zenc/std/vec.zc"

DirEntry Vec_DirEntry__pop(Vec_DirEntry* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_DirEntry Vec_DirEntry__pop_opt(Vec_DirEntry* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_DirEntry__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_DirEntry__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

DirEntry Vec_DirEntry__remove(Vec_DirEntry* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; DirEntry item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(DirEntry)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = item; memset(&item, 0, sizeof(_z_ret_mv)); __z_drop_flag_item = 0; 
#line 169 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
_z_ret_mv; });

#line 169 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_DirEntry__append(Vec_DirEntry* self, Vec_DirEntry other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_DirEntry__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(DirEntry)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_DirEntry__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_DirEntry__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

DirEntry Vec_DirEntry__get(Vec_DirEntry* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

DirEntry Vec_DirEntry__index(Vec_DirEntry* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_DirEntry__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

DirEntry* Vec_DirEntry__get_ref(Vec_DirEntry* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

DirEntry Vec_DirEntry__last(Vec_DirEntry* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_DirEntry__length(Vec_DirEntry* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_DirEntry__contains(Vec_DirEntry* self, DirEntry item)
{

#line 221 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(DirEntry)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    { bool _z_ret = true; 
#line 221 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
return _z_ret; }
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    { bool _z_ret = false; 
#line 221 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
return _z_ret; }
    }

#line 221 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_DirEntry__is_empty(Vec_DirEntry* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_DirEntry__clear(Vec_DirEntry* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_DirEntry__free(Vec_DirEntry* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

DirEntry Vec_DirEntry__first(Vec_DirEntry* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_DirEntry__set(Vec_DirEntry* self, size_t idx, DirEntry item)
{

#line 253 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));
    }

#line 253 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_DirEntry__reverse(Vec_DirEntry* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    int __z_drop_flag_tmp = 1; DirEntry tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = ({ __z_drop_flag_tmp = 0; tmp; }));
(i++);
(j--);

#line 265 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_tmp) DirEntry__Drop_glue(&tmp);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_DirEntry__eq(Vec_DirEntry* self, Vec_DirEntry* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(DirEntry)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_DirEntry__forget(Vec_DirEntry* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_DirEntry Vec_DirEntry__add(Vec_DirEntry* self, Vec_DirEntry* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_DirEntry result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_DirEntry__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_DirEntry__append(&result, Vec_DirEntry__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntry__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntry__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_DirEntry__add_assign(Vec_DirEntry* self, Vec_DirEntry* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_DirEntry__append(self, Vec_DirEntry__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_DirEntry__neq(Vec_DirEntry* self, Vec_DirEntry* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_DirEntry__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_DirEntry__shl(Vec_DirEntry* self, DirEntry item)
{

#line 311 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_DirEntry__push(self, ({ __z_drop_flag_item = 0; item; }));
    }

#line 311 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_DirEntry__shr(Vec_DirEntry* self, DirEntry* out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_DirEntry__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_DirEntry__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_DirEntry Vec_DirEntry__mul(Vec_DirEntry* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_DirEntry result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_DirEntry__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_DirEntry__append(&result, Vec_DirEntry__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntry__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_DirEntry__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_DirEntry__mul_assign(Vec_DirEntry* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_DirEntry__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_DirEntry__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(DirEntry)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_DirEntry Vec_DirEntry__clone(Vec_DirEntry* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_DirEntry){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    DirEntry* new_data = ((DirEntry*)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(DirEntry)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_DirEntry){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_DirEntry VecIterRef_DirEntry__next(VecIterRef_DirEntry* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    DirEntry* item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_DirEntry){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_DirEntry){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_DirEntry VecIterRef_DirEntry__iterator(VecIterRef_DirEntry* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_DirEntry__is_none(VecIterResult_DirEntry* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

DirEntry* VecIterResult_DirEntry__unwrap(VecIterResult_DirEntry* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_DirEntry VecIter_DirEntry__next(VecIter_DirEntry* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; DirEntry item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    { Option_DirEntry _z_ret = 
#line 47 "D:/zenc/std/vec.zc"
Option_DirEntry__Some(({ __z_drop_flag_item = 0; item; })); 
#line 45 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
return _z_ret; }

#line 45 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_DirEntry__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_DirEntry VecIter_DirEntry__iterator(VecIter_DirEntry* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_DirEntry Option_DirEntry__Some(DirEntry v)
{

#line 10 "D:/zenc/std/option.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/option.zc"
    { Option_DirEntry _z_ret = (Option_DirEntry){.is_some = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
}

#line 14 "D:/zenc/std/option.zc"

Option_DirEntry Option_DirEntry__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    int __z_drop_flag_opt = 1; Option_DirEntry opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(DirEntry));

#line 18 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = opt; memset(&opt, 0, sizeof(_z_ret_mv)); __z_drop_flag_opt = 0; 
#line 15 "D:/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option_DirEntry__Drop_glue(&opt);
_z_ret_mv; });

#line 15 "D:/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option_DirEntry__Drop_glue(&opt);
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_DirEntry__is_some(Option_DirEntry* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_DirEntry__is_none(Option_DirEntry* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_DirEntry__forget(Option_DirEntry* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(DirEntry));
    }
}

#line 33 "D:/zenc/std/option.zc"

DirEntry Option_DirEntry__unwrap(Option_DirEntry* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    int __z_drop_flag_v = 1; DirEntry v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(DirEntry));

#line 40 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 38 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
_z_ret_mv; });

#line 38 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
    }
}

#line 43 "D:/zenc/std/option.zc"

DirEntry* Option_DirEntry__unwrap_ref(Option_DirEntry* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

DirEntry Option_DirEntry__unwrap_or(Option_DirEntry* self, DirEntry def_val)
{

#line 51 "D:/zenc/std/option.zc"
    int __z_drop_flag_def_val = 1;
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    { DirEntry _z_ret = self->val; 
#line 51 "D:/zenc/std/option.zc"
    if (__z_drop_flag_def_val) DirEntry__Drop_glue(&def_val);
return _z_ret; }
    }

#line 55 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv)); __z_drop_flag_def_val = 0; 
#line 51 "D:/zenc/std/option.zc"
    if (__z_drop_flag_def_val) DirEntry__Drop_glue(&def_val);
_z_ret_mv; });
    }

#line 51 "D:/zenc/std/option.zc"
    if (__z_drop_flag_def_val) DirEntry__Drop_glue(&def_val);
}

#line 58 "D:/zenc/std/option.zc"

DirEntry Option_DirEntry__expect(Option_DirEntry* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_DirEntry Option_DirEntry__or_else(Option_DirEntry* self, Option_DirEntry other)
{

#line 66 "D:/zenc/std/option.zc"
    int __z_drop_flag_other = 1;
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    { Option_DirEntry _z_ret = *self; 
#line 66 "D:/zenc/std/option.zc"
    if (__z_drop_flag_other) Option_DirEntry__Drop_glue(&other);
return _z_ret; }
    }

#line 68 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = other; memset(&other, 0, sizeof(_z_ret_mv)); __z_drop_flag_other = 0; 
#line 66 "D:/zenc/std/option.zc"
    if (__z_drop_flag_other) Option_DirEntry__Drop_glue(&other);
_z_ret_mv; });
    }

#line 66 "D:/zenc/std/option.zc"
    if (__z_drop_flag_other) Option_DirEntry__Drop_glue(&other);
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_DirEntry__Drop_drop(Vec_DirEntry* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_DirEntry__free(self);
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_Metadata Result_Metadata__Ok(Metadata v)
{
    {

#line 11 "D:/zenc/std/result.zc"
    return (Result_Metadata){.is_ok = true, .val = v};
    }
}

#line 14 "D:/zenc/std/result.zc"

Result_Metadata Result_Metadata__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_Metadata){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_Metadata__is_ok(Result_Metadata* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_Metadata__is_err(Result_Metadata* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_Metadata__forget(Result_Metadata* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Metadata));
    }
}

#line 30 "D:/zenc/std/result.zc"

Metadata Result_Metadata__unwrap(Result_Metadata* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    Metadata v = self->val;

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Metadata));

#line 37 "D:/zenc/std/result.zc"
    return v;
    }
}

#line 40 "D:/zenc/std/result.zc"

Metadata* Result_Metadata__unwrap_ref(Result_Metadata* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

Metadata Result_Metadata__expect(Result_Metadata* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_Vec_String Result_Vec_String__Ok(Vec_String v)
{

#line 10 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/result.zc"
    { Result_Vec_String _z_ret = (Result_Vec_String){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_String__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_String__Drop_glue(&v);
}

#line 14 "D:/zenc/std/result.zc"

Result_Vec_String Result_Vec_String__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_Vec_String){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_Vec_String__is_ok(Result_Vec_String* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_Vec_String__is_err(Result_Vec_String* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_Vec_String__forget(Result_Vec_String* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Vec_String));
    }
}

#line 30 "D:/zenc/std/result.zc"

Vec_String Result_Vec_String__unwrap(Result_Vec_String* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1; Vec_String v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Vec_String));

#line 37 "D:/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_String__Drop_glue(&v);
_z_ret_mv; });

#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec_String__Drop_glue(&v);
    }
}

#line 40 "D:/zenc/std/result.zc"

Vec_String* Result_Vec_String__unwrap_ref(Result_Vec_String* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

Vec_String Result_Vec_String__expect(Result_Vec_String* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_StringPtr Vec_StringPtr__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_StringPtr){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_StringPtr Vec_StringPtr__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_StringPtr){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_StringPtr){.data = ((String**)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(String*))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_StringPtr__grow(Vec_StringPtr* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((String**)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String*))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_StringPtr__grow_to_fit(Vec_StringPtr* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((String**)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String*))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_StringPtr Vec_StringPtr__iterator(Vec_StringPtr* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_StringPtr){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_StringPtr Vec_StringPtr__iter_ref(Vec_StringPtr* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_StringPtr){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_StringPtr__push(Vec_StringPtr* self, String* item)
{
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_StringPtr__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_StringPtr__insert(Vec_StringPtr* self, size_t idx, String* item)
{
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_StringPtr__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(String*)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "D:/zenc/std/vec.zc"

String* Vec_StringPtr__pop(Vec_StringPtr* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_StringPtr Vec_StringPtr__pop_opt(Vec_StringPtr* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_StringPtr__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_StringPtr__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

String* Vec_StringPtr__remove(Vec_StringPtr* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    String* item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(String*)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_StringPtr__append(Vec_StringPtr* self, Vec_StringPtr other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_StringPtr__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(String*)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_StringPtr__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_StringPtr__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

String* Vec_StringPtr__get(Vec_StringPtr* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

String* Vec_StringPtr__index(Vec_StringPtr* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_StringPtr__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

String** Vec_StringPtr__get_ref(Vec_StringPtr* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

String* Vec_StringPtr__last(Vec_StringPtr* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_StringPtr__length(Vec_StringPtr* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_StringPtr__contains(Vec_StringPtr* self, String* item)
{
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(String*)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_StringPtr__is_empty(Vec_StringPtr* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_StringPtr__clear(Vec_StringPtr* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_StringPtr__free(Vec_StringPtr* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

String* Vec_StringPtr__first(Vec_StringPtr* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_StringPtr__set(Vec_StringPtr* self, size_t idx, String* item)
{
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_StringPtr__reverse(Vec_StringPtr* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    String* tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_StringPtr__eq(Vec_StringPtr* self, Vec_StringPtr* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(String*)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_StringPtr__forget(Vec_StringPtr* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_StringPtr Vec_StringPtr__add(Vec_StringPtr* self, Vec_StringPtr* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_StringPtr result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_StringPtr__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_StringPtr__append(&result, Vec_StringPtr__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_StringPtr__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_StringPtr__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_StringPtr__add_assign(Vec_StringPtr* self, Vec_StringPtr* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_StringPtr__append(self, Vec_StringPtr__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_StringPtr__neq(Vec_StringPtr* self, Vec_StringPtr* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_StringPtr__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_StringPtr__shl(Vec_StringPtr* self, String* item)
{
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_StringPtr__push(self, item);
    }
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_StringPtr__shr(Vec_StringPtr* self, String** out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_StringPtr__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_StringPtr__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_StringPtr Vec_StringPtr__mul(Vec_StringPtr* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_StringPtr result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_StringPtr__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_StringPtr__append(&result, Vec_StringPtr__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_StringPtr__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_StringPtr__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_StringPtr__mul_assign(Vec_StringPtr* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_StringPtr__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_StringPtr__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(String*)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_StringPtr Vec_StringPtr__clone(Vec_StringPtr* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_StringPtr){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    String** new_data = ((String**)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(String*)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_StringPtr){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_StringPtr VecIterRef_StringPtr__next(VecIterRef_StringPtr* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    struct String** item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_StringPtr){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_StringPtr){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_StringPtr VecIterRef_StringPtr__iterator(VecIterRef_StringPtr* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_StringPtr__is_none(VecIterResult_StringPtr* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

struct String** VecIterResult_StringPtr__unwrap(VecIterResult_StringPtr* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_StringPtr VecIter_StringPtr__next(VecIter_StringPtr* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    struct String* item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    return 
#line 47 "D:/zenc/std/vec.zc"
Option_StringPtr__Some(item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_StringPtr__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_StringPtr VecIter_StringPtr__iterator(VecIter_StringPtr* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_StringPtr Option_StringPtr__Some(struct String* v)
{
    {

#line 11 "D:/zenc/std/option.zc"
    return (Option_StringPtr){.is_some = true, .val = v};
    }
}

#line 14 "D:/zenc/std/option.zc"

Option_StringPtr Option_StringPtr__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    Option_StringPtr opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(struct String*));

#line 18 "D:/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_StringPtr__is_some(Option_StringPtr* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_StringPtr__is_none(Option_StringPtr* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_StringPtr__forget(Option_StringPtr* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct String*));
    }
}

#line 33 "D:/zenc/std/option.zc"

struct String* Option_StringPtr__unwrap(Option_StringPtr* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    struct String* v = self->val;

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct String*));

#line 40 "D:/zenc/std/option.zc"
    return v;
    }
}

#line 43 "D:/zenc/std/option.zc"

struct String** Option_StringPtr__unwrap_ref(Option_StringPtr* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

struct String* Option_StringPtr__unwrap_or(Option_StringPtr* self, struct String* def_val)
{
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    return self->val;
    }

#line 55 "D:/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "D:/zenc/std/option.zc"

struct String* Option_StringPtr__expect(Option_StringPtr* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_StringPtr Option_StringPtr__or_else(Option_StringPtr* self, Option_StringPtr other)
{
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    return *self;
    }

#line 68 "D:/zenc/std/option.zc"
    return other;
    }
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_StringPtr__Drop_drop(Vec_StringPtr* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_StringPtr__free(self);
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_bool Result_bool__Ok(bool v)
{
    {

#line 11 "D:/zenc/std/result.zc"
    return (Result_bool){.is_ok = true, .val = v};
    }
}

#line 14 "D:/zenc/std/result.zc"

Result_bool Result_bool__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_bool){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_bool__is_ok(Result_bool* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_bool__is_err(Result_bool* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_bool__forget(Result_bool* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(bool));
    }
}

#line 30 "D:/zenc/std/result.zc"

bool Result_bool__unwrap(Result_bool* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    bool v = self->val;

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(bool));

#line 37 "D:/zenc/std/result.zc"
    return v;
    }
}

#line 40 "D:/zenc/std/result.zc"

bool* Result_bool__unwrap_ref(Result_bool* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

bool Result_bool__expect(Result_bool* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_String Result_String__Ok(String v)
{

#line 10 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/result.zc"
    { Result_String _z_ret = (Result_String){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
}

#line 14 "D:/zenc/std/result.zc"

Result_String Result_String__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_String){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_String__is_ok(Result_String* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_String__is_err(Result_String* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_String__forget(Result_String* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(String));
    }
}

#line 30 "D:/zenc/std/result.zc"

String Result_String__unwrap(Result_String* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1; String v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(String));

#line 37 "D:/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
_z_ret_mv; });

#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
    }
}

#line 40 "D:/zenc/std/result.zc"

String* Result_String__unwrap_ref(Result_String* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

String Result_String__expect(Result_String* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "D:/zenc/std/result.zc"

Result_File Result_File__Ok(File v)
{

#line 10 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/result.zc"
    { Result_File _z_ret = (Result_File){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop_glue(&v);
}

#line 14 "D:/zenc/std/result.zc"

Result_File Result_File__Err(char* e)
{
    {

#line 15 "D:/zenc/std/result.zc"
    return (Result_File){.is_ok = false, .err = e};
    }
}

#line 18 "D:/zenc/std/result.zc"

bool Result_File__is_ok(Result_File* self)
{
    {

#line 19 "D:/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 22 "D:/zenc/std/result.zc"

bool Result_File__is_err(Result_File* self)
{
    {

#line 23 "D:/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 26 "D:/zenc/std/result.zc"

void Result_File__forget(Result_File* self)
{
    {

#line 27 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(File));
    }
}

#line 30 "D:/zenc/std/result.zc"

File Result_File__unwrap(Result_File* self)
{
    {

#line 31 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 32 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 33 "D:/zenc/std/result.zc"
exit(1);
    }

#line 35 "D:/zenc/std/result.zc"
    int __z_drop_flag_v = 1; File v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 36 "D:/zenc/std/result.zc"
memset((&self->val), 0, sizeof(File));

#line 37 "D:/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop_glue(&v);
_z_ret_mv; });

#line 35 "D:/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop_glue(&v);
    }
}

#line 40 "D:/zenc/std/result.zc"

File* Result_File__unwrap_ref(Result_File* self)
{
    {

#line 41 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 42 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 43 "D:/zenc/std/result.zc"
exit(1);
    }

#line 45 "D:/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 48 "D:/zenc/std/result.zc"

File Result_File__expect(Result_File* self, char* msg)
{
    {

#line 49 "D:/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 50 "D:/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 51 "D:/zenc/std/result.zc"
exit(1);
    }

#line 53 "D:/zenc/std/result.zc"
    return self->val;
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_String Vec_String__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_String){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_String Vec_String__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_String){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_String){.data = ((String*)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(String))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_String__grow(Vec_String* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((String*)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_String__grow_to_fit(Vec_String* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((String*)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_String Vec_String__iterator(Vec_String* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_String){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_String Vec_String__iter_ref(Vec_String* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_String){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_String__push(Vec_String* self, String item)
{

#line 123 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_String__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = ({ __z_drop_flag_item = 0; item; }));

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 123 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_String__insert(Vec_String* self, size_t idx, String item)
{

#line 131 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_String__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(String)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 131 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

#line 147 "D:/zenc/std/vec.zc"

String Vec_String__pop(Vec_String* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_String Vec_String__pop_opt(Vec_String* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_String__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_String__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

String Vec_String__remove(Vec_String* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; String item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(String)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = item; memset(&item, 0, sizeof(_z_ret_mv)); __z_drop_flag_item = 0; 
#line 169 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
_z_ret_mv; });

#line 169 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_String__append(Vec_String* self, Vec_String other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_String__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(String)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_String__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_String__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

String Vec_String__get(Vec_String* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

String Vec_String__index(Vec_String* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_String__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

String* Vec_String__get_ref(Vec_String* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

String Vec_String__last(Vec_String* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_String__length(Vec_String* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_String__contains(Vec_String* self, String item)
{

#line 221 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(String)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    { bool _z_ret = true; 
#line 221 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
return _z_ret; }
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    { bool _z_ret = false; 
#line 221 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
return _z_ret; }
    }

#line 221 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_String__is_empty(Vec_String* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_String__clear(Vec_String* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_String__free(Vec_String* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

String Vec_String__first(Vec_String* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_String__set(Vec_String* self, size_t idx, String item)
{

#line 253 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));
    }

#line 253 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_String__reverse(Vec_String* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    int __z_drop_flag_tmp = 1; String tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = ({ __z_drop_flag_tmp = 0; tmp; }));
(i++);
(j--);

#line 265 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_tmp) String__Drop_glue(&tmp);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_String__eq(Vec_String* self, Vec_String* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(String)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_String__forget(Vec_String* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_String Vec_String__add(Vec_String* self, Vec_String* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_String result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_String__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_String__append(&result, Vec_String__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_String__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_String__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_String__add_assign(Vec_String* self, Vec_String* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_String__append(self, Vec_String__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_String__neq(Vec_String* self, Vec_String* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_String__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_String__shl(Vec_String* self, String item)
{

#line 311 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_String__push(self, ({ __z_drop_flag_item = 0; item; }));
    }

#line 311 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_String__shr(Vec_String* self, String* out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_String__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_String__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_String Vec_String__mul(Vec_String* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_String result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_String__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_String__append(&result, Vec_String__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_String__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_String__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_String__mul_assign(Vec_String* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_String__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_String__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(String)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_String Vec_String__clone(Vec_String* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_String){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    String* new_data = ((String*)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(String)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_String){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_String VecIterRef_String__next(VecIterRef_String* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    String* item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_String){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_String){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_String VecIterRef_String__iterator(VecIterRef_String* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_String__is_none(VecIterResult_String* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

String* VecIterResult_String__unwrap(VecIterResult_String* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_String VecIter_String__next(VecIter_String* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; String item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    { Option_String _z_ret = 
#line 47 "D:/zenc/std/vec.zc"
Option_String__Some(({ __z_drop_flag_item = 0; item; })); 
#line 45 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
return _z_ret; }

#line 45 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop_glue(&item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_String__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_String VecIter_String__iterator(VecIter_String* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_String Option_String__Some(String v)
{

#line 10 "D:/zenc/std/option.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "D:/zenc/std/option.zc"
    { Option_String _z_ret = (Option_String){.is_some = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
return _z_ret; }
    }

#line 10 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
}

#line 14 "D:/zenc/std/option.zc"

Option_String Option_String__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    int __z_drop_flag_opt = 1; Option_String opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(String));

#line 18 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = opt; memset(&opt, 0, sizeof(_z_ret_mv)); __z_drop_flag_opt = 0; 
#line 15 "D:/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option_String__Drop_glue(&opt);
_z_ret_mv; });

#line 15 "D:/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option_String__Drop_glue(&opt);
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_String__is_some(Option_String* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_String__is_none(Option_String* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_String__forget(Option_String* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(String));
    }
}

#line 33 "D:/zenc/std/option.zc"

String Option_String__unwrap(Option_String* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    int __z_drop_flag_v = 1; String v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(String));

#line 40 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 38 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
_z_ret_mv; });

#line 38 "D:/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop_glue(&v);
    }
}

#line 43 "D:/zenc/std/option.zc"

String* Option_String__unwrap_ref(Option_String* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

String Option_String__unwrap_or(Option_String* self, String def_val)
{

#line 51 "D:/zenc/std/option.zc"
    int __z_drop_flag_def_val = 1;
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    { String _z_ret = self->val; 
#line 51 "D:/zenc/std/option.zc"
    if (__z_drop_flag_def_val) String__Drop_glue(&def_val);
return _z_ret; }
    }

#line 55 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv)); __z_drop_flag_def_val = 0; 
#line 51 "D:/zenc/std/option.zc"
    if (__z_drop_flag_def_val) String__Drop_glue(&def_val);
_z_ret_mv; });
    }

#line 51 "D:/zenc/std/option.zc"
    if (__z_drop_flag_def_val) String__Drop_glue(&def_val);
}

#line 58 "D:/zenc/std/option.zc"

String Option_String__expect(Option_String* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_String Option_String__or_else(Option_String* self, Option_String other)
{

#line 66 "D:/zenc/std/option.zc"
    int __z_drop_flag_other = 1;
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    { Option_String _z_ret = *self; 
#line 66 "D:/zenc/std/option.zc"
    if (__z_drop_flag_other) Option_String__Drop_glue(&other);
return _z_ret; }
    }

#line 68 "D:/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = other; memset(&other, 0, sizeof(_z_ret_mv)); __z_drop_flag_other = 0; 
#line 66 "D:/zenc/std/option.zc"
    if (__z_drop_flag_other) Option_String__Drop_glue(&other);
_z_ret_mv; });
    }

#line 66 "D:/zenc/std/option.zc"
    if (__z_drop_flag_other) Option_String__Drop_glue(&other);
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_String__Drop_drop(Vec_String* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_String__free(self);
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_int32_t Vec_int32_t__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_int32_t){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_int32_t Vec_int32_t__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_int32_t){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_int32_t){.data = ((int32_t*)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(int32_t))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_int32_t__grow(Vec_int32_t* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((int32_t*)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(int32_t))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_int32_t__grow_to_fit(Vec_int32_t* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((int32_t*)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(int32_t))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_int32_t Vec_int32_t__iterator(Vec_int32_t* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_int32_t){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_int32_t Vec_int32_t__iter_ref(Vec_int32_t* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_int32_t){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_int32_t__push(Vec_int32_t* self, int32_t item)
{
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_int32_t__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_int32_t__insert(Vec_int32_t* self, size_t idx, int32_t item)
{
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_int32_t__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(int32_t)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "D:/zenc/std/vec.zc"

int32_t Vec_int32_t__pop(Vec_int32_t* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_int32_t Vec_int32_t__pop_opt(Vec_int32_t* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_int32_t__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_int32_t__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

int32_t Vec_int32_t__remove(Vec_int32_t* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    int32_t item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(int32_t)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_int32_t__append(Vec_int32_t* self, Vec_int32_t other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_int32_t__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(int32_t)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_int32_t__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_int32_t__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

int32_t Vec_int32_t__get(Vec_int32_t* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

int32_t Vec_int32_t__index(Vec_int32_t* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_int32_t__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

int32_t* Vec_int32_t__get_ref(Vec_int32_t* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

int32_t Vec_int32_t__last(Vec_int32_t* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_int32_t__length(Vec_int32_t* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_int32_t__contains(Vec_int32_t* self, int32_t item)
{
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(int32_t)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_int32_t__is_empty(Vec_int32_t* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_int32_t__clear(Vec_int32_t* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_int32_t__free(Vec_int32_t* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

int32_t Vec_int32_t__first(Vec_int32_t* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_int32_t__set(Vec_int32_t* self, size_t idx, int32_t item)
{
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_int32_t__reverse(Vec_int32_t* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    int32_t tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_int32_t__eq(Vec_int32_t* self, Vec_int32_t* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(int32_t)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_int32_t__forget(Vec_int32_t* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_int32_t Vec_int32_t__add(Vec_int32_t* self, Vec_int32_t* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_int32_t result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_int32_t__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_int32_t__append(&result, Vec_int32_t__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_int32_t__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_int32_t__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_int32_t__add_assign(Vec_int32_t* self, Vec_int32_t* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_int32_t__append(self, Vec_int32_t__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_int32_t__neq(Vec_int32_t* self, Vec_int32_t* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_int32_t__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_int32_t__shl(Vec_int32_t* self, int32_t item)
{
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_int32_t__push(self, item);
    }
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_int32_t__shr(Vec_int32_t* self, int32_t* out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_int32_t__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_int32_t__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_int32_t Vec_int32_t__mul(Vec_int32_t* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_int32_t result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_int32_t__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_int32_t__append(&result, Vec_int32_t__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_int32_t__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_int32_t__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_int32_t__mul_assign(Vec_int32_t* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_int32_t__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_int32_t__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(int32_t)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_int32_t Vec_int32_t__clone(Vec_int32_t* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_int32_t){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    int32_t* new_data = ((int32_t*)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(int32_t)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_int32_t){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_int32_t VecIterRef_int32_t__next(VecIterRef_int32_t* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    int32_t* item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_int32_t){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_int32_t){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_int32_t VecIterRef_int32_t__iterator(VecIterRef_int32_t* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_int32_t__is_none(VecIterResult_int32_t* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

int32_t* VecIterResult_int32_t__unwrap(VecIterResult_int32_t* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_int32_t VecIter_int32_t__next(VecIter_int32_t* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    int32_t item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    return 
#line 47 "D:/zenc/std/vec.zc"
Option_int32_t__Some(item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_int32_t__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_int32_t VecIter_int32_t__iterator(VecIter_int32_t* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_int32_t__Drop_drop(Vec_int32_t* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_int32_t__free(self);
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_size_t Vec_size_t__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_size_t){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_size_t Vec_size_t__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_size_t){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_size_t){.data = ((size_t*)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(size_t))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_size_t__grow(Vec_size_t* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((size_t*)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(size_t))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_size_t__grow_to_fit(Vec_size_t* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((size_t*)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(size_t))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_size_t Vec_size_t__iterator(Vec_size_t* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_size_t){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_size_t Vec_size_t__iter_ref(Vec_size_t* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_size_t){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_size_t__push(Vec_size_t* self, size_t item)
{
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_size_t__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_size_t__insert(Vec_size_t* self, size_t idx, size_t item)
{
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_size_t__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(size_t)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "D:/zenc/std/vec.zc"

size_t Vec_size_t__pop(Vec_size_t* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_size_t Vec_size_t__pop_opt(Vec_size_t* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_size_t__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_size_t__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

size_t Vec_size_t__remove(Vec_size_t* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    size_t item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(size_t)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_size_t__append(Vec_size_t* self, Vec_size_t other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_size_t__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(size_t)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_size_t__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_size_t__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

size_t Vec_size_t__get(Vec_size_t* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

size_t Vec_size_t__index(Vec_size_t* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_size_t__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

size_t* Vec_size_t__get_ref(Vec_size_t* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

size_t Vec_size_t__last(Vec_size_t* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_size_t__length(Vec_size_t* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_size_t__contains(Vec_size_t* self, size_t item)
{
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(size_t)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_size_t__is_empty(Vec_size_t* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_size_t__clear(Vec_size_t* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_size_t__free(Vec_size_t* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

size_t Vec_size_t__first(Vec_size_t* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_size_t__set(Vec_size_t* self, size_t idx, size_t item)
{
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_size_t__reverse(Vec_size_t* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    size_t tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_size_t__eq(Vec_size_t* self, Vec_size_t* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(size_t)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_size_t__forget(Vec_size_t* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_size_t Vec_size_t__add(Vec_size_t* self, Vec_size_t* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_size_t result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_size_t__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_size_t__append(&result, Vec_size_t__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_size_t__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_size_t__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_size_t__add_assign(Vec_size_t* self, Vec_size_t* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_size_t__append(self, Vec_size_t__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_size_t__neq(Vec_size_t* self, Vec_size_t* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_size_t__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_size_t__shl(Vec_size_t* self, size_t item)
{
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_size_t__push(self, item);
    }
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_size_t__shr(Vec_size_t* self, size_t* out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_size_t__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_size_t__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_size_t Vec_size_t__mul(Vec_size_t* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_size_t result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_size_t__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_size_t__append(&result, Vec_size_t__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_size_t__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_size_t__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_size_t__mul_assign(Vec_size_t* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_size_t__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_size_t__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(size_t)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_size_t Vec_size_t__clone(Vec_size_t* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_size_t){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    size_t* new_data = ((size_t*)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(size_t)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_size_t){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_size_t VecIterRef_size_t__next(VecIterRef_size_t* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    size_t* item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_size_t){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_size_t){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_size_t VecIterRef_size_t__iterator(VecIterRef_size_t* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_size_t__is_none(VecIterResult_size_t* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

size_t* VecIterResult_size_t__unwrap(VecIterResult_size_t* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_size_t VecIter_size_t__next(VecIter_size_t* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    size_t item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    return 
#line 47 "D:/zenc/std/vec.zc"
Option_size_t__Some(item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_size_t__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_size_t VecIter_size_t__iterator(VecIter_size_t* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_size_t__Drop_drop(Vec_size_t* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_size_t__free(self);
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_size_t Option_size_t__Some(size_t v)
{
    {

#line 11 "D:/zenc/std/option.zc"
    return (Option_size_t){.is_some = true, .val = v};
    }
}

#line 14 "D:/zenc/std/option.zc"

Option_size_t Option_size_t__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    Option_size_t opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(size_t));

#line 18 "D:/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_size_t__is_some(Option_size_t* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_size_t__is_none(Option_size_t* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_size_t__forget(Option_size_t* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(size_t));
    }
}

#line 33 "D:/zenc/std/option.zc"

size_t Option_size_t__unwrap(Option_size_t* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    size_t v = self->val;

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(size_t));

#line 40 "D:/zenc/std/option.zc"
    return v;
    }
}

#line 43 "D:/zenc/std/option.zc"

size_t* Option_size_t__unwrap_ref(Option_size_t* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

size_t Option_size_t__unwrap_or(Option_size_t* self, size_t def_val)
{
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    return self->val;
    }

#line 55 "D:/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "D:/zenc/std/option.zc"

size_t Option_size_t__expect(Option_size_t* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_size_t Option_size_t__or_else(Option_size_t* self, Option_size_t other)
{
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    return *self;
    }

#line 68 "D:/zenc/std/option.zc"
    return other;
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_int32_t Option_int32_t__Some(int32_t v)
{
    {

#line 11 "D:/zenc/std/option.zc"
    return (Option_int32_t){.is_some = true, .val = v};
    }
}

#line 14 "D:/zenc/std/option.zc"

Option_int32_t Option_int32_t__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    Option_int32_t opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(int32_t));

#line 18 "D:/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_int32_t__is_some(Option_int32_t* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_int32_t__is_none(Option_int32_t* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_int32_t__forget(Option_int32_t* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(int32_t));
    }
}

#line 33 "D:/zenc/std/option.zc"

int32_t Option_int32_t__unwrap(Option_int32_t* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    int32_t v = self->val;

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(int32_t));

#line 40 "D:/zenc/std/option.zc"
    return v;
    }
}

#line 43 "D:/zenc/std/option.zc"

int32_t* Option_int32_t__unwrap_ref(Option_int32_t* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

int32_t Option_int32_t__unwrap_or(Option_int32_t* self, int32_t def_val)
{
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    return self->val;
    }

#line 55 "D:/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "D:/zenc/std/option.zc"

int32_t Option_int32_t__expect(Option_int32_t* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_int32_t Option_int32_t__or_else(Option_int32_t* self, Option_int32_t other)
{
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    return *self;
    }

#line 68 "D:/zenc/std/option.zc"
    return other;
    }
}

#line 73 "D:/zenc/std/vec.zc"

Vec_char Vec_char__new(void)
{
    {

#line 74 "D:/zenc/std/vec.zc"
    return (Vec_char){.data = NULL};
    }
}

#line 77 "D:/zenc/std/vec.zc"

Vec_char Vec_char__with_capacity(size_t cap)
{
    {

#line 78 "D:/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "D:/zenc/std/vec.zc"
    return (Vec_char){.data = NULL};
    }

#line 81 "D:/zenc/std/vec.zc"
    return (Vec_char){.data = ((char*)(
#line 82 "D:/zenc/std/vec.zc"
malloc((cap * sizeof(char))))), .cap = cap};
    }
}

#line 88 "D:/zenc/std/vec.zc"

void Vec_char__grow(Vec_char* self)
{
    {

#line 89 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "D:/zenc/std/vec.zc"
(self->data = ((char*)(
#line 91 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(char))))));
    }
}

#line 94 "D:/zenc/std/vec.zc"

void Vec_char__grow_to_fit(Vec_char* self, size_t new_len)
{
    {

#line 95 "D:/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "D:/zenc/std/vec.zc"
    return ;
    }

#line 99 "D:/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "D:/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "D:/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "D:/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "D:/zenc/std/vec.zc"
(self->data = ((char*)(
#line 104 "D:/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(char))))));
    }
}

#line 107 "D:/zenc/std/vec.zc"

VecIter_char Vec_char__iterator(Vec_char* self)
{
    {

#line 108 "D:/zenc/std/vec.zc"
    return (VecIter_char){.data = self->data, .count = self->len};
    }
}

#line 115 "D:/zenc/std/vec.zc"

VecIterRef_char Vec_char__iter_ref(Vec_char* self)
{
    {

#line 116 "D:/zenc/std/vec.zc"
    return (VecIterRef_char){.data = self->data, .count = self->len};
    }
}

#line 123 "D:/zenc/std/vec.zc"

void Vec_char__push(Vec_char* self, char item)
{
    {

#line 124 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "D:/zenc/std/vec.zc"
Vec_char__grow(self);
    }

#line 127 "D:/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "D:/zenc/std/vec.zc"

void Vec_char__insert(Vec_char* self, size_t idx, char item)
{
    {

#line 132 "D:/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 136 "D:/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "D:/zenc/std/vec.zc"
Vec_char__grow(self);
    }

#line 140 "D:/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "D:/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(char)));
    }

#line 143 "D:/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "D:/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "D:/zenc/std/vec.zc"

char Vec_char__pop(Vec_char* self)
{
    {

#line 148 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 152 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "D:/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "D:/zenc/std/vec.zc"

Option_char Vec_char__pop_opt(Vec_char* self)
{
    {

#line 157 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "D:/zenc/std/vec.zc"
    return 
#line 158 "D:/zenc/std/vec.zc"
Option_char__None();
    }

#line 160 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "D:/zenc/std/vec.zc"
    return 
#line 161 "D:/zenc/std/vec.zc"
Option_char__Some(self->data[self->len]);
    }
}

#line 164 "D:/zenc/std/vec.zc"

char Vec_char__remove(Vec_char* self, size_t idx)
{
    {

#line 165 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 169 "D:/zenc/std/vec.zc"
    char item = self->data[idx];

#line 171 "D:/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "D:/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(char)));
    }

#line 174 "D:/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "D:/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "D:/zenc/std/vec.zc"

void Vec_char__append(Vec_char* self, Vec_char other)
{

#line 180 "D:/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "D:/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "D:/zenc/std/vec.zc"
Vec_char__grow_to_fit(self, new_len);

#line 184 "D:/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(char)));

#line 185 "D:/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "D:/zenc/std/vec.zc"
Vec_char__forget(&other);
    }

#line 180 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec_char__Drop_glue(&other);
}

#line 189 "D:/zenc/std/vec.zc"

char Vec_char__get(Vec_char* self, size_t idx)
{
    {

#line 190 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 194 "D:/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "D:/zenc/std/vec.zc"

char Vec_char__index(Vec_char* self, size_t idx)
{
    {

#line 198 "D:/zenc/std/vec.zc"
    return 
#line 198 "D:/zenc/std/vec.zc"
Vec_char__get(self, idx);
    }
}

#line 201 "D:/zenc/std/vec.zc"

char* Vec_char__get_ref(Vec_char* self, size_t idx)
{
    {

#line 202 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 206 "D:/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "D:/zenc/std/vec.zc"

char Vec_char__last(Vec_char* self)
{
    {

#line 210 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 214 "D:/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "D:/zenc/std/vec.zc"

size_t Vec_char__length(Vec_char* self)
{
    {

#line 218 "D:/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "D:/zenc/std/vec.zc"

bool Vec_char__contains(Vec_char* self, char item)
{
    {

#line 222 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "D:/zenc/std/vec.zc"
if ((
#line 224 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(char)) == 0))     {

#line 224 "D:/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "D:/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "D:/zenc/std/vec.zc"

bool Vec_char__is_empty(Vec_char* self)
{
    {

#line 231 "D:/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "D:/zenc/std/vec.zc"

void Vec_char__clear(Vec_char* self)
{
    {

#line 235 "D:/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "D:/zenc/std/vec.zc"

void Vec_char__free(Vec_char* self)
{
    {

#line 239 "D:/zenc/std/vec.zc"
if (self->data)     {

#line 239 "D:/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 242 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "D:/zenc/std/vec.zc"

char Vec_char__first(Vec_char* self)
{
    {

#line 246 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 250 "D:/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "D:/zenc/std/vec.zc"

void Vec_char__set(Vec_char* self, size_t idx, char item)
{
    {

#line 254 "D:/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 258 "D:/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "D:/zenc/std/vec.zc"

void Vec_char__reverse(Vec_char* self)
{
    {

#line 262 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "D:/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "D:/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "D:/zenc/std/vec.zc"
    char tmp = self->data[i];

#line 266 "D:/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "D:/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "D:/zenc/std/vec.zc"

bool Vec_char__eq(Vec_char* self, Vec_char* other)
{
    {

#line 276 "D:/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "D:/zenc/std/vec.zc"
    return false;
    }

#line 277 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "D:/zenc/std/vec.zc"
if ((
#line 279 "D:/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(char)) != 0))     {

#line 279 "D:/zenc/std/vec.zc"
    return false;
    }

#line 280 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "D:/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "D:/zenc/std/vec.zc"

void Vec_char__forget(Vec_char* self)
{
    {

#line 287 "D:/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "D:/zenc/std/vec.zc"
(self->len = 0);

#line 289 "D:/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "D:/zenc/std/vec.zc"

Vec_char Vec_char__add(Vec_char* self, Vec_char* other)
{
    {

#line 296 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_char result = 
#line 296 "D:/zenc/std/vec.zc"
Vec_char__clone(self);

#line 297 "D:/zenc/std/vec.zc"
Vec_char__append(&result, Vec_char__clone(other));

#line 298 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_char__Drop_glue(&result);
_z_ret_mv; });

#line 296 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_char__Drop_glue(&result);
    }
}

#line 302 "D:/zenc/std/vec.zc"

void Vec_char__add_assign(Vec_char* self, Vec_char* other)
{
    {

#line 303 "D:/zenc/std/vec.zc"
Vec_char__append(self, Vec_char__clone(other));
    }
}

#line 306 "D:/zenc/std/vec.zc"

bool Vec_char__neq(Vec_char* self, Vec_char* other)
{
    {

#line 307 "D:/zenc/std/vec.zc"
    return (!
#line 307 "D:/zenc/std/vec.zc"
Vec_char__eq(self, other));
    }
}

#line 311 "D:/zenc/std/vec.zc"

void Vec_char__shl(Vec_char* self, char item)
{
    {

#line 312 "D:/zenc/std/vec.zc"
Vec_char__push(self, item);
    }
}

#line 316 "D:/zenc/std/vec.zc"

void Vec_char__shr(Vec_char* self, char* out_item)
{
    {

#line 317 "D:/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "D:/zenc/std/vec.zc"
((*out_item) = 
#line 318 "D:/zenc/std/vec.zc"
Vec_char__pop(self));
    }
 else     {

#line 320 "D:/zenc/std/vec.zc"
Vec_char__pop(self);
    }
    }
}

#line 327 "D:/zenc/std/vec.zc"

Vec_char Vec_char__mul(Vec_char* self, size_t count)
{
    {

#line 328 "D:/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec_char result = 
#line 328 "D:/zenc/std/vec.zc"
Vec_char__with_capacity((self->len * count));

#line 329 "D:/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "D:/zenc/std/vec.zc"
Vec_char__append(&result, Vec_char__clone(self));

#line 332 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "D:/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_char__Drop_glue(&result);
_z_ret_mv; });

#line 328 "D:/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec_char__Drop_glue(&result);
    }
}

#line 340 "D:/zenc/std/vec.zc"

void Vec_char__mul_assign(Vec_char* self, size_t count)
{
    {

#line 341 "D:/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "D:/zenc/std/vec.zc"
Vec_char__clear(self);

#line 343 "D:/zenc/std/vec.zc"
    return ;
    }

#line 345 "D:/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "D:/zenc/std/vec.zc"
    return ;
    }

#line 348 "D:/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "D:/zenc/std/vec.zc"
Vec_char__grow_to_fit(self, (self->len * count));

#line 350 "D:/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "D:/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "D:/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(char)));

#line 353 "D:/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "D:/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "D:/zenc/std/vec.zc"

Vec_char Vec_char__clone(Vec_char* self)
{
    {

#line 360 "D:/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "D:/zenc/std/vec.zc"
    return (Vec_char){.data = NULL};
    }

#line 363 "D:/zenc/std/vec.zc"
    char* new_data = ((char*)(
#line 363 "D:/zenc/std/vec.zc"
malloc((self->len * sizeof(char)))));

#line 364 "D:/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "D:/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "D:/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "D:/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "D:/zenc/std/vec.zc"
    return (Vec_char){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "D:/zenc/std/vec.zc"

VecIterResult_char VecIterRef_char__next(VecIterRef_char* self)
{
    {

#line 59 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "D:/zenc/std/vec.zc"
    char* item = (&self->data[self->idx]);

#line 61 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "D:/zenc/std/vec.zc"
    return (VecIterResult_char){.ptr = item};
    }

#line 64 "D:/zenc/std/vec.zc"
    return (VecIterResult_char){.ptr = NULL};
    }
}

#line 67 "D:/zenc/std/vec.zc"

VecIterRef_char VecIterRef_char__iterator(VecIterRef_char* self)
{
    {

#line 68 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "D:/zenc/std/vec.zc"

bool VecIterResult_char__is_none(VecIterResult_char* self)
{
    {

#line 24 "D:/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "D:/zenc/std/vec.zc"

char* VecIterResult_char__unwrap(VecIterResult_char* self)
{
    {

#line 28 "D:/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "D:/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "D:/zenc/std/vec.zc"
exit(1);
    }

#line 32 "D:/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "D:/zenc/std/vec.zc"

Option_char VecIter_char__next(VecIter_char* self)
{
    {

#line 44 "D:/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "D:/zenc/std/vec.zc"
    char item = self->data[self->idx];

#line 46 "D:/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "D:/zenc/std/vec.zc"
    return 
#line 47 "D:/zenc/std/vec.zc"
Option_char__Some(item);
    }

#line 49 "D:/zenc/std/vec.zc"
    return 
#line 49 "D:/zenc/std/vec.zc"
Option_char__None();
    }
}

#line 52 "D:/zenc/std/vec.zc"

VecIter_char VecIter_char__iterator(VecIter_char* self)
{
    {

#line 53 "D:/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "D:/zenc/std/option.zc"

Option_char Option_char__Some(char v)
{
    {

#line 11 "D:/zenc/std/option.zc"
    return (Option_char){.is_some = true, .val = v};
    }
}

#line 14 "D:/zenc/std/option.zc"

Option_char Option_char__None(void)
{
    {

#line 15 "D:/zenc/std/option.zc"
    Option_char opt = {0};

#line 16 "D:/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "D:/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(char));

#line 18 "D:/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "D:/zenc/std/option.zc"

bool Option_char__is_some(Option_char* self)
{
    {

#line 22 "D:/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "D:/zenc/std/option.zc"

bool Option_char__is_none(Option_char* self)
{
    {

#line 26 "D:/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "D:/zenc/std/option.zc"

void Option_char__forget(Option_char* self)
{
    {

#line 30 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(char));
    }
}

#line 33 "D:/zenc/std/option.zc"

char Option_char__unwrap(Option_char* self)
{
    {

#line 34 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "D:/zenc/std/option.zc"
exit(1);
    }

#line 38 "D:/zenc/std/option.zc"
    char v = self->val;

#line 39 "D:/zenc/std/option.zc"
memset((&self->val), 0, sizeof(char));

#line 40 "D:/zenc/std/option.zc"
    return v;
    }
}

#line 43 "D:/zenc/std/option.zc"

char* Option_char__unwrap_ref(Option_char* self)
{
    {

#line 44 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "D:/zenc/std/option.zc"
exit(1);
    }

#line 48 "D:/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "D:/zenc/std/option.zc"

char Option_char__unwrap_or(Option_char* self, char def_val)
{
    {

#line 52 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "D:/zenc/std/option.zc"
    return self->val;
    }

#line 55 "D:/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "D:/zenc/std/option.zc"

char Option_char__expect(Option_char* self, char* msg)
{
    {

#line 59 "D:/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "D:/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "D:/zenc/std/option.zc"
exit(1);
    }

#line 63 "D:/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "D:/zenc/std/option.zc"

Option_char Option_char__or_else(Option_char* self, Option_char other)
{
    {

#line 67 "D:/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "D:/zenc/std/option.zc"
    return *self;
    }

#line 68 "D:/zenc/std/option.zc"
    return other;
    }
}

#line 378 "D:/zenc/std/vec.zc"

void Vec_char__Drop_drop(Vec_char* self)
{
    {

#line 379 "D:/zenc/std/vec.zc"
Vec_char__free(self);
    }
}

#line 29 "./src/main.zc"

int main(void)
{
    {

#line 30 "./src/main.zc"
_main();
    }
}

#line 356 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"

void framebuffer_size_callback(GLFWwindow* _window, int32_t width, int32_t height)
{
    {

#line 357 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[CALLBACK] Viewport updated"); fprintf(stdout, "\n"); 0; });

#line 359 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glViewport(0, 0, width, height);
    }
}

#line 348 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"

void process_input(GLFWwindow* window)
{
    {

#line 349 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
if ((
#line 349 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))     {

#line 350 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[INPUT] ESC pressed → closing window"); fprintf(stdout, "\n"); 0; });

#line 351 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwSetWindowShouldClose(window, true);
    }
    }
}

#line 17 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"

int32_t _main(void)
{
    {

#line 18 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[1] CPU: Program start"); fprintf(stdout, "\n"); 0; });

#line 26 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[2] CPU: Initializing GLFW"); fprintf(stdout, "\n"); 0; });

#line 28 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwInit();

#line 29 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"

#line 31 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[3] CPU: Configuring OpenGL context (3.3 core)"); fprintf(stdout, "\n"); 0; });

#line 33 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

#line 34 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#line 35 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#line 37 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[4] CPU: Creating window + OpenGL context"); fprintf(stdout, "\n"); 0; });

#line 39 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ZC_AUTO window = 
#line 39 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

#line 47 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
if ((window == NULL))     {

#line 48 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stderr, "%s", "Failed to create GLFW window"); fprintf(stderr, "\n"); 0; });

#line 49 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    { int32_t _z_ret = (-1); 
#line 29 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwTerminate();
return _z_ret; }
    }

#line 52 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[5] CPU: Making context current"); fprintf(stdout, "\n"); 0; });

#line 54 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwMakeContextCurrent(window);

#line 57 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#line 59 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[6] CPU → DRIVER: Loading OpenGL functions (GLAD)"); fprintf(stdout, "\n"); 0; });

#line 61 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
if ((
#line 61 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
gladLoadGLLoader(((GLADloadproc)(glfwGetProcAddress))) == 0))     {

#line 62 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stderr, "%s", "Failed to initialize GLAD"); fprintf(stderr, "\n"); 0; });

#line 63 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    { int32_t _z_ret = (-1); 
#line 29 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwTerminate();
return _z_ret; }
    }

#line 66 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glEnable(GL_DEPTH_TEST);

#line 68 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int __z_drop_flag_our_shader = 1; ShaderProgram our_shader = 
#line 68 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
Result_ShaderProgram__unwrap((__typeof__((ShaderProgram__new("./shaders/6.2.coordinate_systems_depth/6.2.shader.vs", "./shaders/6.2.coordinate_systems_depth/6.2.shader.fs")))[]){ShaderProgram__new("./shaders/6.2.coordinate_systems_depth/6.2.shader.vs", "./shaders/6.2.coordinate_systems_depth/6.2.shader.fs")});

#line 75 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[17] CPU: Defining vertex + index data"); fprintf(stdout, "\n"); 0; });

#line 78 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    const float vertices[180] = {(-0.5), (-0.5), (-0.5), 0.0, 0.0, 0.5, (-0.5), (-0.5), 1.0, 0.0, 0.5, 0.5, (-0.5), 1.0, 1.0, 0.5, 0.5, (-0.5), 1.0, 1.0, (-0.5), 0.5, (-0.5), 0.0, 1.0, (-0.5), (-0.5), (-0.5), 0.0, 0.0, (-0.5), (-0.5), 0.5, 0.0, 0.0, 0.5, (-0.5), 0.5, 1.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, 0.5, 1.0, 1.0, (-0.5), 0.5, 0.5, 0.0, 1.0, (-0.5), (-0.5), 0.5, 0.0, 0.0, (-0.5), 0.5, 0.5, 1.0, 0.0, (-0.5), 0.5, (-0.5), 1.0, 1.0, (-0.5), (-0.5), (-0.5), 0.0, 1.0, (-0.5), (-0.5), (-0.5), 0.0, 1.0, (-0.5), (-0.5), 0.5, 0.0, 0.0, (-0.5), 0.5, 0.5, 1.0, 0.0, 0.5, 0.5, 0.5, 1.0, 0.0, 0.5, 0.5, (-0.5), 1.0, 1.0, 0.5, (-0.5), (-0.5), 0.0, 1.0, 0.5, (-0.5), (-0.5), 0.0, 1.0, 0.5, (-0.5), 0.5, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 0.0, (-0.5), (-0.5), (-0.5), 0.0, 1.0, 0.5, (-0.5), (-0.5), 1.0, 1.0, 0.5, (-0.5), 0.5, 1.0, 0.0, 0.5, (-0.5), 0.5, 1.0, 0.0, (-0.5), (-0.5), 0.5, 0.0, 0.0, (-0.5), (-0.5), (-0.5), 0.0, 1.0, (-0.5), 0.5, (-0.5), 0.0, 1.0, 0.5, 0.5, (-0.5), 1.0, 1.0, 0.5, 0.5, 0.5, 1.0, 0.0, 0.5, 0.5, 0.5, 1.0, 0.0, (-0.5), 0.5, 0.5, 0.0, 0.0, (-0.5), 0.5, (-0.5), 0.0, 1.0};

#line 122 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    uint32_t VBO = 0;

#line 123 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"

#line 124 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    uint32_t VAO = 0;

#line 125 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"

#line 127 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[18] CPU → GPU: Creating buffers (VAO, VBO)"); fprintf(stdout, "\n"); 0; });

#line 129 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGenVertexArrays(1, (&VAO));

#line 130 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGenBuffers(1, (&VBO));

#line 132 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[19] INPUT ASSEMBLER: Binding VAO (captures state)"); fprintf(stdout, "\n"); 0; });

#line 134 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindVertexArray(VAO);

#line 136 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[20] INPUT ASSEMBLER: Uploading vertex buffer (VBO)"); fprintf(stdout, "\n"); 0; });

#line 138 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindBuffer(GL_ARRAY_BUFFER, VBO);

#line 139 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#line 141 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[22] INPUT ASSEMBLER: Defining vertex layout (attribute 0)"); fprintf(stdout, "\n"); 0; });

#line 144 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), ((void*)(0)));

#line 145 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glEnableVertexAttribArray(0);

#line 149 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), ((void*)((3 * sizeof(float)))));

#line 150 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glEnableVertexAttribArray(1);

#line 154 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    uint32_t texture01 = 0;

#line 155 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGenTextures(1, (&texture01));

#line 156 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindTexture(GL_TEXTURE_2D, texture01);

#line 157 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

#line 158 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#line 159 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

#line 160 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#line 162 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int32_t texture_width01 = 0;

#line 163 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int32_t texture_height01 = 0;

#line 164 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int32_t texture_nr_channels01 = 0;

#line 166 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    unsigned char* texture_data01 = 
#line 166 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
stbi_load("textures/container.jpg", (&texture_width01), (&texture_height01), (&texture_nr_channels01), 0);

#line 168 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
if ((texture_data01 != NULL))     {

#line 169 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width01, texture_height01, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data01);

#line 180 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGenerateMipmap(GL_TEXTURE_2D);
    }
 else     {

#line 182 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stderr, "%s", "Failed to load texture 01"); fprintf(stderr, "\n"); 0; });
    }

#line 185 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
stbi_image_free(texture_data01);

#line 189 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    uint32_t texture02 = 0;

#line 190 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGenTextures(1, (&texture02));

#line 191 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindTexture(GL_TEXTURE_2D, texture02);

#line 192 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

#line 193 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#line 194 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

#line 195 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#line 197 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int32_t texture_width02 = 0;

#line 198 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int32_t texture_height02 = 0;

#line 199 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    int32_t texture_nr_channels02 = 0;

#line 201 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
stbi_set_flip_vertically_on_load(true);

#line 202 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    unsigned char* texture_data02 = 
#line 202 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
stbi_load("textures/awesomeface.png", (&texture_width02), (&texture_height02), (&texture_nr_channels02), 0);

#line 204 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
if ((texture_data02 != NULL))     {

#line 205 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width02, texture_height02, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data02);

#line 216 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGenerateMipmap(GL_TEXTURE_2D);
    }
 else     {

#line 218 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stderr, "%s", "Failed to load texture 02"); fprintf(stderr, "\n"); 0; });
    }

#line 221 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
stbi_image_free(texture_data02);

#line 223 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
ShaderProgram__useProgram((&our_shader));

#line 224 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glUniform1i(glGetUniformLocation(our_shader.ID, "texture01"), 0);

#line 225 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
ShaderProgram__setInt((&our_shader), "texture02", 1);

#line 227 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[23] CPU: Unbinding VBO"); fprintf(stdout, "\n"); 0; });

#line 229 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindBuffer(GL_ARRAY_BUFFER, 0);

#line 231 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[24] CPU: Unbinding VAO"); fprintf(stdout, "\n"); 0; });

#line 232 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindVertexArray(0);

#line 234 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[25] RASTERIZER CONFIG: Wireframe mode enabled"); fprintf(stdout, "\n"); 0; });

#line 236 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[26] Setup complete → entering render loop"); fprintf(stdout, "\n"); 0; });

#line 240 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
while ((
#line 240 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwWindowShouldClose(window) == 0))     {

#line 241 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
process_input(window);

#line 243 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glClearColor(0.20000000000000001, 0.29999999999999999, 0.29999999999999999, 1.0);

#line 244 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glClear((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

#line 250 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glActiveTexture(GL_TEXTURE0);

#line 251 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindTexture(GL_TEXTURE_2D, texture01);

#line 252 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glActiveTexture(GL_TEXTURE1);

#line 253 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindTexture(GL_TEXTURE_2D, texture02);

#line 256 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
ShaderProgram__useProgram((&our_shader));

#line 260 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    mat4 model = GLM_MAT4_IDENTITY_INIT;

#line 261 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    mat4 view = GLM_MAT4_IDENTITY_INIT;

#line 262 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    mat4 projection = GLM_MAT4_IDENTITY_INIT;

#line 266 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    vec3 axis = {0.5, 1.0, 0.0};

#line 268 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glm_rotate(model, ((float)(
#line 270 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwGetTime())), axis);

#line 276 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    vec3 translation = {0.0, 0.0, (-3.0)};

#line 278 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glm_translate(view, translation);

#line 285 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glm_perspective(
#line 286 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glm_rad(45.0), (((float)(800)) / ((float)(600))), 0.10000000000000001, 100.0, projection);

#line 295 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ZC_AUTO modelLoc = 
#line 295 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGetUniformLocation(our_shader.ID, "model");

#line 300 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ZC_AUTO viewLoc = 
#line 300 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glGetUniformLocation(our_shader.ID, "view");

#line 307 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (&model[0][0]));

#line 314 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (&view[0][0]));

#line 323 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
ShaderProgram__setMat4((&our_shader), "projection", projection);

#line 329 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glBindVertexArray(VAO);

#line 337 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glDrawArrays(GL_TRIANGLES, 0, 36);

#line 339 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwSwapBuffers(window);

#line 340 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwPollEvents();
    }

#line 343 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    ({ fprintf(stdout, "%s", "[30] CPU: Program terminated"); fprintf(stdout, "\n"); 0; });

#line 344 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    { int32_t _z_ret = 0; 
#line 125 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glDeleteVertexArrays(1, (&VAO));

#line 123 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glDeleteBuffers(1, (&VBO));

#line 68 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    if (__z_drop_flag_our_shader) ShaderProgram__Drop_glue(&our_shader);

#line 29 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwTerminate();
return _z_ret; }

#line 125 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glDeleteVertexArrays(1, (&VAO));

#line 123 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glDeleteBuffers(1, (&VBO));

#line 68 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
    if (__z_drop_flag_our_shader) ShaderProgram__Drop_glue(&our_shader);

#line 29 "D:/zenc-projects/zenc-learnopengl/src/1-getting-started/6.2.coordinate_systems_depth/coordinate_systems_depth.zc"
glfwTerminate();
    }
}

#line 13 "D:/zenc/std/core.zc"

void __zenc_todo_impl(const char* file, int32_t line, const char* func, const char* msg)
{
    {

#line 14 "D:/zenc/std/core.zc"
fprintf(stderr, "todo: %s\n  at %s:%d in %s()\n", msg, file, line, func);

#line 15 "D:/zenc/std/core.zc"
exit(1);
    }
}

#line 8 "D:/zenc/std/core.zc"

void __zenc_panic_impl(const char* file, int32_t line, const char* func, const char* msg)
{
    {

#line 9 "D:/zenc/std/core.zc"
fprintf(stderr, "panic: %s\n  at %s:%d in %s()\n", msg, file, line, func);

#line 10 "D:/zenc/std/core.zc"
exit(1);
    }
}

#line 9 "D:/zenc/std/math.zc"

double Math__PI(void)
{
    {

#line 9 "D:/zenc/std/math.zc"
    return 3.1415926535897931;
    }
}

#line 10 "D:/zenc/std/math.zc"

double Math__E(void)
{
    {

#line 10 "D:/zenc/std/math.zc"
    return 2.7182818284590451;
    }
}

#line 13 "D:/zenc/std/math.zc"

double Math__abs(double x)
{
    {

#line 14 "D:/zenc/std/math.zc"
    return 
#line 14 "D:/zenc/std/math.zc"
fabs(x);
    }
}

#line 18 "D:/zenc/std/math.zc"

double Math__sin(double x)
{
    {

#line 19 "D:/zenc/std/math.zc"
    return 
#line 19 "D:/zenc/std/math.zc"
sin(x);
    }
}

#line 22 "D:/zenc/std/math.zc"

double Math__cos(double x)
{
    {

#line 23 "D:/zenc/std/math.zc"
    return 
#line 23 "D:/zenc/std/math.zc"
cos(x);
    }
}

#line 26 "D:/zenc/std/math.zc"

double Math__tan(double x)
{
    {

#line 27 "D:/zenc/std/math.zc"
    return 
#line 27 "D:/zenc/std/math.zc"
tan(x);
    }
}

#line 30 "D:/zenc/std/math.zc"

double Math__asin(double x)
{
    {

#line 31 "D:/zenc/std/math.zc"
    return 
#line 31 "D:/zenc/std/math.zc"
asin(x);
    }
}

#line 34 "D:/zenc/std/math.zc"

double Math__acos(double x)
{
    {

#line 35 "D:/zenc/std/math.zc"
    return 
#line 35 "D:/zenc/std/math.zc"
acos(x);
    }
}

#line 38 "D:/zenc/std/math.zc"

double Math__atan(double x)
{
    {

#line 39 "D:/zenc/std/math.zc"
    return 
#line 39 "D:/zenc/std/math.zc"
atan(x);
    }
}

#line 42 "D:/zenc/std/math.zc"

double Math__atan2(double y, double x)
{
    {

#line 43 "D:/zenc/std/math.zc"
    return 
#line 43 "D:/zenc/std/math.zc"
atan2(y, x);
    }
}

#line 47 "D:/zenc/std/math.zc"

double Math__sqrt(double x)
{
    {

#line 48 "D:/zenc/std/math.zc"
    return 
#line 48 "D:/zenc/std/math.zc"
sqrt(x);
    }
}

#line 51 "D:/zenc/std/math.zc"

double Math__pow(double base, double exp)
{
    {

#line 52 "D:/zenc/std/math.zc"
    return 
#line 52 "D:/zenc/std/math.zc"
pow(base, exp);
    }
}

#line 55 "D:/zenc/std/math.zc"

double Math__exp(double x)
{
    {

#line 56 "D:/zenc/std/math.zc"
    return 
#line 56 "D:/zenc/std/math.zc"
exp(x);
    }
}

#line 59 "D:/zenc/std/math.zc"

double Math__log(double x)
{
    {

#line 60 "D:/zenc/std/math.zc"
    return 
#line 60 "D:/zenc/std/math.zc"
log(x);
    }
}

#line 63 "D:/zenc/std/math.zc"

double Math__log10(double x)
{
    {

#line 64 "D:/zenc/std/math.zc"
    return 
#line 64 "D:/zenc/std/math.zc"
log10(x);
    }
}

#line 68 "D:/zenc/std/math.zc"

double Math__ceil(double x)
{
    {

#line 69 "D:/zenc/std/math.zc"
    return 
#line 69 "D:/zenc/std/math.zc"
ceil(x);
    }
}

#line 72 "D:/zenc/std/math.zc"

double Math__floor(double x)
{
    {

#line 73 "D:/zenc/std/math.zc"
    return 
#line 73 "D:/zenc/std/math.zc"
floor(x);
    }
}

#line 76 "D:/zenc/std/math.zc"

double Math__round(double x)
{
    {

#line 77 "D:/zenc/std/math.zc"
    return 
#line 77 "D:/zenc/std/math.zc"
round(x);
    }
}

#line 80 "D:/zenc/std/math.zc"

double Math__mod(double x, double y)
{
    {

#line 81 "D:/zenc/std/math.zc"
    return 
#line 81 "D:/zenc/std/math.zc"
fmod(x, y);
    }
}

#line 85 "D:/zenc/std/math.zc"

double Math__max(double a, double b)
{
    {

#line 86 "D:/zenc/std/math.zc"
    return 
#line 86 "D:/zenc/std/math.zc"
fmax(a, b);
    }
}

#line 89 "D:/zenc/std/math.zc"

double Math__min(double a, double b)
{
    {

#line 90 "D:/zenc/std/math.zc"
    return 
#line 90 "D:/zenc/std/math.zc"
fmin(a, b);
    }
}

#line 146 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

void ShaderProgram__Drop_drop(ShaderProgram* self)
{
    {

#line 147 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if ((self->ID != 0))     {

#line 148 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glDeleteProgram(self->ID);
    }
    }
}

#line 13 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

Result_ShaderProgram ShaderProgram__new(char* vertexPath, char* fragmentPath)
{
    {

#line 14 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    ({ fprintf(stdout, "%s", "Vertex path: "); fprintf(stdout, "%s", vertexPath); fprintf(stdout, "\n"); 0; });

#line 15 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    ({ fprintf(stdout, "%s", "Fragment path: "); fprintf(stdout, "%s", fragmentPath); fprintf(stdout, "\n"); 0; });

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    int __z_drop_flag_vertexCode = 1; String vertexCode = {0};

#line 18 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
{ Result_String _m_0 = 
#line 18 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
File__read_all(vertexPath); 
#line 19 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (_m_0.is_ok) { ZC_AUTO_INIT(code, _m_0.val); (vertexCode = code);
 } else 
#line 20 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (!_m_0.is_ok) { ZC_AUTO_INIT(msg, _m_0.err);     {

#line 21 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    ({ fprintf(stdout, "%s", "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: "); fprintf(stdout, _z_str(msg), _z_arg(msg)); fprintf(stdout, "\n"); 0; });

#line 22 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    { Result_ShaderProgram _z_ret = 
#line 22 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_ShaderProgram__Err("Failed to read vertex shader"); 
#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
return _z_ret; }
    }
 } };
    {
    }

#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    int __z_drop_flag_fragmentCode = 1; String fragmentCode = {0};

#line 27 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
{ Result_String _m_1 = 
#line 27 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
File__read_all(fragmentPath); 
#line 28 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (_m_1.is_ok) { ZC_AUTO_INIT(code, _m_1.val); (fragmentCode = code);
 } else 
#line 29 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (!_m_1.is_ok) { ZC_AUTO_INIT(msg, _m_1.err);     {

#line 30 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    ({ fprintf(stdout, "%s", "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: "); fprintf(stdout, _z_str(msg), _z_arg(msg)); fprintf(stdout, "\n"); 0; });

#line 31 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    { Result_ShaderProgram _z_ret = 
#line 31 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_ShaderProgram__Err("Failed to read fragment shader"); 
#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_fragmentCode) String__Drop_glue(&fragmentCode);

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
return _z_ret; }
    }
 } };
    {
    }

#line 35 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    uint32_t vertex = 0;

#line 36 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
{ Result_uint32_t _m_2 = 
#line 36 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
ShaderProgram__compile_shader(String__c_str((&vertexCode)), GL_VERTEX_SHADER, "VERTEX"); 
#line 37 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (_m_2.is_ok) { ZC_AUTO_INIT(v, _m_2.val); (vertex = v);
 } else 
#line 38 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (!_m_2.is_ok) { ZC_AUTO_INIT(msg, _m_2.err);     { Result_ShaderProgram _z_ret = 
#line 38 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_ShaderProgram__Err(msg); 
#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_fragmentCode) String__Drop_glue(&fragmentCode);

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
return _z_ret; }
 } };
    {
    }

#line 41 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    uint32_t fragment = 0;

#line 42 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
{ Result_uint32_t _m_3 = 
#line 42 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
ShaderProgram__compile_shader(String__c_str((&fragmentCode)), GL_FRAGMENT_SHADER, "FRAGMENT"); 
#line 43 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (_m_3.is_ok) { ZC_AUTO_INIT(f, _m_3.val); (fragment = f);
 } else 
#line 44 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (!_m_3.is_ok) { ZC_AUTO_INIT(msg, _m_3.err);     { Result_ShaderProgram _z_ret = 
#line 44 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_ShaderProgram__Err(msg); 
#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_fragmentCode) String__Drop_glue(&fragmentCode);

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
return _z_ret; }
 } };
    {
    }

#line 47 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    ZC_AUTO program = 
#line 47 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glCreateProgram();

#line 48 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glAttachShader(program, vertex);

#line 49 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glAttachShader(program, fragment);

#line 50 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glLinkProgram(program);

#line 52 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
{ Result_string _m_4 = 
#line 52 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
ShaderProgram__check_compile_errors(program, "PROGRAM"); 
#line 53 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (!_m_4.is_ok) { ZC_AUTO_INIT(msg, _m_4.err);     {

#line 54 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glDeleteShader(vertex);

#line 55 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glDeleteShader(fragment);

#line 56 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    { Result_ShaderProgram _z_ret = 
#line 56 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_ShaderProgram__Err(msg); 
#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_fragmentCode) String__Drop_glue(&fragmentCode);

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
return _z_ret; }
    }
 } else 
#line 58 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (1) {     {
    }
 } };
    {
    }

#line 61 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glDeleteShader(vertex);

#line 62 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glDeleteShader(fragment);

#line 64 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    { Result_ShaderProgram _z_ret = 
#line 64 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_ShaderProgram__Ok(ShaderProgram__newId(program)); 
#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_fragmentCode) String__Drop_glue(&fragmentCode);

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
return _z_ret; }

#line 26 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_fragmentCode) String__Drop_glue(&fragmentCode);

#line 17 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_vertexCode) String__Drop_glue(&vertexCode);
    }
}

#line 67 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

Result_uint32_t ShaderProgram__compile_shader(const char* source, uint32_t glType, char* typeStr)
{
    {

#line 68 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    uint32_t shader = 
#line 68 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glCreateShader(glType);

#line 70 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glShaderSource(shader, 1, (&source), NULL);

#line 71 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glCompileShader(shader);

#line 73 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
{ Result_string _m_5 = 
#line 73 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
ShaderProgram__check_compile_errors(shader, typeStr); 
#line 74 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (!_m_5.is_ok) { ZC_AUTO_INIT(msg, _m_5.err);     {

#line 75 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    ({ fprintf(stdout, _z_str(msg), _z_arg(msg)); fprintf(stdout, "\n"); 0; });

#line 76 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glDeleteShader(shader);

#line 77 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    return 
#line 77 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_uint32_t__Err(msg);
    }
 } else 
#line 79 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if (1) {     {
    }
 } };
    {
    }

#line 82 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    return 
#line 82 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_uint32_t__Ok(shader);
    }
}

#line 85 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

Result_string ShaderProgram__check_compile_errors(uint32_t object, char* typeStr)
{
    {

#line 86 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    int32_t success = 0;

#line 87 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    char infoLog[1024] = {0};

#line 89 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if ((
#line 89 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
strcmp(typeStr, "PROGRAM") != 0))     {

#line 90 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glGetShaderiv(object, GL_COMPILE_STATUS, (&success));

#line 91 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if ((success == 0))     {

#line 92 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glGetShaderInfoLog(object, 1024, NULL, infoLog);

#line 94 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    int __z_drop_flag_msg = 1; String msg = 
#line 94 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__from("ERROR::SHADER_COMPILATION_ERROR of type: ");

#line 95 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__append_c((&msg), typeStr);

#line 96 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__append_c((&msg), "\n");

#line 97 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__append_c((&msg), infoLog);

#line 98 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    { Result_string _z_ret = 
#line 98 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_string__Err(String__c_str((&msg))); 
#line 94 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_msg) String__Drop_glue(&msg);
return _z_ret; }

#line 94 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_msg) String__Drop_glue(&msg);
    }
    }
 else     {

#line 101 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glGetProgramiv(object, GL_LINK_STATUS, (&success));

#line 102 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
if ((success == 0))     {

#line 103 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glGetProgramInfoLog(object, 1024, NULL, infoLog);

#line 105 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    int __z_drop_flag_msg = 1; String msg = 
#line 105 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__from("ERROR::PROGRAM_LINKING_ERROR of type: ");

#line 106 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__append_c((&msg), typeStr);

#line 107 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__append_c((&msg), "\n");

#line 108 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
String__append_c((&msg), infoLog);

#line 109 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    { Result_string _z_ret = 
#line 109 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_string__Err(String__c_str((&msg))); 
#line 105 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_msg) String__Drop_glue(&msg);
return _z_ret; }

#line 105 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    if (__z_drop_flag_msg) String__Drop_glue(&msg);
    }
    }

#line 112 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    return 
#line 112 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
Result_string__Ok("ok");
    }
}

#line 115 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

ShaderProgram ShaderProgram__newId(uint32_t id)
{
    {

#line 116 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
    return (struct ShaderProgram){.ID = id};
    }
}

#line 119 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

void ShaderProgram__useProgram(ShaderProgram* self)
{
    {

#line 120 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glUseProgram(self->ID);
    }
}

#line 123 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

void ShaderProgram__setBool(ShaderProgram* self, char* name, bool value)
{
    {

#line 124 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glUniform1i(glGetUniformLocation(self->ID, name), value);
    }
}

#line 127 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

void ShaderProgram__setInt(ShaderProgram* self, char* name, int32_t value)
{
    {

#line 128 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glUniform1i(glGetUniformLocation(self->ID, name), value);
    }
}

#line 131 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

void ShaderProgram__setFloat(ShaderProgram* self, char* name, float value)
{
    {

#line 132 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glUniform1f(glGetUniformLocation(self->ID, name), value);
    }
}

#line 135 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"

void ShaderProgram__setMat4(ShaderProgram* self, char* name, mat4 value)
{
    {

#line 136 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glUniformMatrix4fv(
#line 137 "D:/zenc-projects/zenc-learnopengl/src/utils/shader_program.zc"
glGetUniformLocation(self->ID, name), 1, GL_FALSE, ((float*)(value)));
    }
}

#line 280 "D:/zenc/std/fs.zc"

void File__Drop_drop(File* self)
{
    {

#line 281 "D:/zenc/std/fs.zc"
File__close(self);
    }
}

#line 36 "D:/zenc/std/fs.zc"

Result_File File__open(char* path, char* mode)
{
    {

#line 37 "D:/zenc/std/fs.zc"
    void* h = 
#line 37 "D:/zenc/std/fs.zc"
fopen(path, mode);

#line 38 "D:/zenc/std/fs.zc"
if ((h == NULL))     {

#line 39 "D:/zenc/std/fs.zc"
    return 
#line 39 "D:/zenc/std/fs.zc"
Result_File__Err("Failed to open file");
    }

#line 41 "D:/zenc/std/fs.zc"
    return 
#line 41 "D:/zenc/std/fs.zc"
Result_File__Ok((struct File){.handle = h});
    }
}

#line 44 "D:/zenc/std/fs.zc"

void File__close(File* self)
{
    {

#line 45 "D:/zenc/std/fs.zc"
if (self->handle)     {

#line 46 "D:/zenc/std/fs.zc"
fclose(self->handle);

#line 47 "D:/zenc/std/fs.zc"
(self->handle = NULL);
    }
    }
}

#line 51 "D:/zenc/std/fs.zc"

Result_String File__read_to_string(File* self)
{
    {

#line 52 "D:/zenc/std/fs.zc"
if ((self->handle == NULL))     {

#line 53 "D:/zenc/std/fs.zc"
    return 
#line 53 "D:/zenc/std/fs.zc"
Result_String__Err("File not open");
    }

#line 56 "D:/zenc/std/fs.zc"
fseek(self->handle, 0, 2);

#line 57 "D:/zenc/std/fs.zc"
    ZC_AUTO size = 
#line 57 "D:/zenc/std/fs.zc"
ftell(self->handle);

#line 58 "D:/zenc/std/fs.zc"
fseek(self->handle, 0, 0);

#line 60 "D:/zenc/std/fs.zc"
    char* buffer = 
#line 60 "D:/zenc/std/fs.zc"
malloc((((size_t)(size)) + 1));

#line 61 "D:/zenc/std/fs.zc"
if ((buffer == NULL))     {

#line 62 "D:/zenc/std/fs.zc"
    return 
#line 62 "D:/zenc/std/fs.zc"
Result_String__Err("Out of memory");
    }

#line 65 "D:/zenc/std/fs.zc"
    ZC_AUTO read = 
#line 65 "D:/zenc/std/fs.zc"
fread(buffer, 1, size, self->handle);

#line 66 "D:/zenc/std/fs.zc"
(buffer[read] = 0);

#line 68 "D:/zenc/std/fs.zc"
    int __z_drop_flag_s = 1; String s = 
#line 68 "D:/zenc/std/fs.zc"
String__new(buffer);

#line 69 "D:/zenc/std/fs.zc"
free(buffer);

#line 71 "D:/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result_String res = 
#line 71 "D:/zenc/std/fs.zc"
Result_String__Ok(({ __z_drop_flag_s = 0; s; }));

#line 72 "D:/zenc/std/fs.zc"
String__forget((&s));

#line 74 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result_String ret = res;
__z_drop_flag_res = 0;

#line 75 "D:/zenc/std/fs.zc"
Result_String__forget((&res));

#line 76 "D:/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 74 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_String__Drop_glue(&ret);

#line 71 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);

#line 68 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
_z_ret_mv; });

#line 74 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_String__Drop_glue(&ret);

#line 71 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);

#line 68 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
}

#line 79 "D:/zenc/std/fs.zc"

Result_String File__read_all(char* path)
{
    {

#line 80 "D:/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result_File res = 
#line 80 "D:/zenc/std/fs.zc"
File__open(path, "rb");

#line 81 "D:/zenc/std/fs.zc"
if (
#line 81 "D:/zenc/std/fs.zc"
Result_File__is_err((&res)))     {

#line 82 "D:/zenc/std/fs.zc"
    { Result_String _z_ret = 
#line 82 "D:/zenc/std/fs.zc"
Result_String__Err(res.err); 
#line 80 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_File__Drop_glue(&res);
return _z_ret; }
    }

#line 85 "D:/zenc/std/fs.zc"
    int __z_drop_flag_f = 1; File f = 
#line 85 "D:/zenc/std/fs.zc"
Result_File__unwrap((&res));

#line 86 "D:/zenc/std/fs.zc"
    int __z_drop_flag_s_res = 1; Result_String s_res = 
#line 86 "D:/zenc/std/fs.zc"
File__read_to_string((&f));

#line 87 "D:/zenc/std/fs.zc"
File__close((&f));

#line 89 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result_String ret = s_res;
__z_drop_flag_s_res = 0;

#line 90 "D:/zenc/std/fs.zc"
Result_String__forget((&s_res));

#line 91 "D:/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 89 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_String__Drop_glue(&ret);

#line 86 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s_res) Result_String__Drop_glue(&s_res);

#line 85 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop_glue(&f);

#line 80 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_File__Drop_glue(&res);
_z_ret_mv; });

#line 89 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_String__Drop_glue(&ret);

#line 86 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s_res) Result_String__Drop_glue(&s_res);

#line 85 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop_glue(&f);

#line 80 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_File__Drop_glue(&res);
    }
}

#line 94 "D:/zenc/std/fs.zc"

Result_bool File__write_string(File* self, char* content)
{
    {

#line 95 "D:/zenc/std/fs.zc"
if ((self->handle == NULL))     {

#line 96 "D:/zenc/std/fs.zc"
    return 
#line 96 "D:/zenc/std/fs.zc"
Result_bool__Err("File not open");
    }

#line 99 "D:/zenc/std/fs.zc"
    ZC_AUTO len = 
#line 99 "D:/zenc/std/fs.zc"
strlen(content);

#line 100 "D:/zenc/std/fs.zc"
    ZC_AUTO written = 
#line 100 "D:/zenc/std/fs.zc"
fwrite(content, 1, len, self->handle);

#line 101 "D:/zenc/std/fs.zc"
if ((written != len))     {

#line 102 "D:/zenc/std/fs.zc"
    return 
#line 102 "D:/zenc/std/fs.zc"
Result_bool__Err("Write incomplete");
    }

#line 104 "D:/zenc/std/fs.zc"
    return 
#line 104 "D:/zenc/std/fs.zc"
Result_bool__Ok(true);
    }
}

#line 107 "D:/zenc/std/fs.zc"

Result_Vec_String File__read_lines(char* path)
{
    {

#line 108 "D:/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result_String res = 
#line 108 "D:/zenc/std/fs.zc"
File__read_all(path);

#line 109 "D:/zenc/std/fs.zc"
if (
#line 109 "D:/zenc/std/fs.zc"
Result_String__is_err((&res)))     {

#line 110 "D:/zenc/std/fs.zc"
    { Result_Vec_String _z_ret = 
#line 110 "D:/zenc/std/fs.zc"
Result_Vec_String__Err(res.err); 
#line 108 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);
return _z_ret; }
    }

#line 112 "D:/zenc/std/fs.zc"
    int __z_drop_flag_content = 1; String content = 
#line 112 "D:/zenc/std/fs.zc"
Result_String__unwrap((&res));

#line 114 "D:/zenc/std/fs.zc"
    int __z_drop_flag_lines = 1; Vec_String lines = 
#line 114 "D:/zenc/std/fs.zc"
String__split((&content), '\n');

#line 117 "D:/zenc/std/fs.zc"
for (size_t i = (size_t)(0); i < lines.len; i = (i + 1))     {

#line 118 "D:/zenc/std/fs.zc"
    String* line = 
#line 118 "D:/zenc/std/fs.zc"
Vec_String__get_ref((&lines), i);

#line 119 "D:/zenc/std/fs.zc"
    size_t len = 
#line 119 "D:/zenc/std/fs.zc"
String__length(line);

#line 120 "D:/zenc/std/fs.zc"
if ((len > 0))     {

#line 121 "D:/zenc/std/fs.zc"
    char last_char = 
#line 121 "D:/zenc/std/fs.zc"
Vec_char__get((&line->vec), (len - 1));

#line 122 "D:/zenc/std/fs.zc"
if ((last_char == '\r'))     {

#line 123 "D:/zenc/std/fs.zc"
    int __z_drop_flag_trimmed = 1; String trimmed = 
#line 123 "D:/zenc/std/fs.zc"
String__substring(line, 0, (len - 1));

#line 124 "D:/zenc/std/fs.zc"
String__destroy(line);

#line 125 "D:/zenc/std/fs.zc"
Vec_String__set((&lines), i, ({ __z_drop_flag_trimmed = 0; trimmed; }));

#line 123 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_trimmed) String__Drop_glue(&trimmed);
    }
    }
    }

#line 131 "D:/zenc/std/fs.zc"
if ((lines.len > 0))     {

#line 132 "D:/zenc/std/fs.zc"
    size_t last_idx = (lines.len - 1);

#line 133 "D:/zenc/std/fs.zc"
    String* last_line = 
#line 133 "D:/zenc/std/fs.zc"
Vec_String__get_ref((&lines), last_idx);

#line 134 "D:/zenc/std/fs.zc"
if ((
#line 134 "D:/zenc/std/fs.zc"
String__length(last_line) == 0))     {

#line 135 "D:/zenc/std/fs.zc"
    int __z_drop_flag_empty = 1; String empty = 
#line 135 "D:/zenc/std/fs.zc"
Vec_String__pop((&lines));

#line 136 "D:/zenc/std/fs.zc"
String__destroy((&empty));

#line 135 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_empty) String__Drop_glue(&empty);
    }
    }

#line 140 "D:/zenc/std/fs.zc"
String__destroy((&content));

#line 142 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ret_res = 1; Result_Vec_String ret_res = 
#line 142 "D:/zenc/std/fs.zc"
Result_Vec_String__Ok(({ __z_drop_flag_lines = 0; lines; }));

#line 143 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result_Vec_String ret = ret_res;
__z_drop_flag_ret_res = 0;

#line 144 "D:/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 143 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_Vec_String__Drop_glue(&ret);

#line 142 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret_res) Result_Vec_String__Drop_glue(&ret_res);

#line 114 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_lines) Vec_String__Drop_glue(&lines);

#line 112 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_content) String__Drop_glue(&content);

#line 108 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);
_z_ret_mv; });

#line 143 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_Vec_String__Drop_glue(&ret);

#line 142 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret_res) Result_Vec_String__Drop_glue(&ret_res);

#line 114 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_lines) Vec_String__Drop_glue(&lines);

#line 112 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_content) String__Drop_glue(&content);

#line 108 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);
    }
}

#line 147 "D:/zenc/std/fs.zc"

Result_bool File__write_lines(char* path, Vec_String* lines)
{
    {

#line 148 "D:/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result_File res = 
#line 148 "D:/zenc/std/fs.zc"
File__open(path, "wb");

#line 149 "D:/zenc/std/fs.zc"
if (
#line 149 "D:/zenc/std/fs.zc"
Result_File__is_err((&res)))     {

#line 150 "D:/zenc/std/fs.zc"
    { Result_bool _z_ret = 
#line 150 "D:/zenc/std/fs.zc"
Result_bool__Err(res.err); 
#line 148 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_File__Drop_glue(&res);
return _z_ret; }
    }

#line 152 "D:/zenc/std/fs.zc"
    int __z_drop_flag_f = 1; File f = 
#line 152 "D:/zenc/std/fs.zc"
Result_File__unwrap((&res));

#line 154 "D:/zenc/std/fs.zc"
for (size_t i = (size_t)(0); i < lines->len; i = (i + 1))     {

#line 155 "D:/zenc/std/fs.zc"
    String* line = 
#line 155 "D:/zenc/std/fs.zc"
Vec_String__get_ref(lines, i);

#line 156 "D:/zenc/std/fs.zc"
File__write_string((&f), String__c_str(line));

#line 157 "D:/zenc/std/fs.zc"
File__write_string((&f), "\n");
    }

#line 160 "D:/zenc/std/fs.zc"
File__close((&f));

#line 161 "D:/zenc/std/fs.zc"
    { Result_bool _z_ret = 
#line 161 "D:/zenc/std/fs.zc"
Result_bool__Ok(true); 
#line 152 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop_glue(&f);

#line 148 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_File__Drop_glue(&res);
return _z_ret; }

#line 152 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop_glue(&f);

#line 148 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_File__Drop_glue(&res);
    }
}

#line 164 "D:/zenc/std/fs.zc"

bool File__exists(char* path)
{
    {

#line 165 "D:/zenc/std/fs.zc"
    int zero = 0;

#line 166 "D:/zenc/std/fs.zc"
    return (
#line 166 "D:/zenc/std/fs.zc"
access(path, 0) == zero);
    }
}

#line 169 "D:/zenc/std/fs.zc"

Result_Metadata File__metadata(char* path)
{
    {

#line 170 "D:/zenc/std/fs.zc"
    uint64_t size = {0};

#line 171 "D:/zenc/std/fs.zc"
    int is_d = 0;

#line 172 "D:/zenc/std/fs.zc"
    int is_f = 0;

#line 174 "D:/zenc/std/fs.zc"
    int res = 
#line 174 "D:/zenc/std/fs.zc"
_z_fs_get_metadata(path, (&size), (&is_d), (&is_f));

#line 175 "D:/zenc/std/fs.zc"
    int non_zero = 0;

#line 176 "D:/zenc/std/fs.zc"
if ((res != non_zero))     {

#line 177 "D:/zenc/std/fs.zc"
    return 
#line 177 "D:/zenc/std/fs.zc"
Result_Metadata__Err("Failed to get metadata");
    }

#line 180 "D:/zenc/std/fs.zc"
    return 
#line 180 "D:/zenc/std/fs.zc"
Result_Metadata__Ok((struct Metadata){.size = ((uint64_t)(size)), .is_dir = (is_d != non_zero), .is_file = (is_f != non_zero)});
    }
}

#line 187 "D:/zenc/std/fs.zc"

Result_bool File__create_dir(char* path)
{
    {

#line 188 "D:/zenc/std/fs.zc"
    int status = 
#line 188 "D:/zenc/std/fs.zc"
_z_fs_mkdir(path);

#line 189 "D:/zenc/std/fs.zc"
    int zero = 0;

#line 190 "D:/zenc/std/fs.zc"
if ((status != zero))     {

#line 191 "D:/zenc/std/fs.zc"
    return 
#line 191 "D:/zenc/std/fs.zc"
Result_bool__Err("Failed to create directory");
    }

#line 193 "D:/zenc/std/fs.zc"
    return 
#line 193 "D:/zenc/std/fs.zc"
Result_bool__Ok(true);
    }
}

#line 196 "D:/zenc/std/fs.zc"

Result_bool File__remove_file(char* path)
{
    {

#line 197 "D:/zenc/std/fs.zc"
    ZC_AUTO status = 
#line 197 "D:/zenc/std/fs.zc"
unlink(path);

#line 198 "D:/zenc/std/fs.zc"
    int zero = 0;

#line 199 "D:/zenc/std/fs.zc"
if ((status != zero))     {

#line 200 "D:/zenc/std/fs.zc"
    return 
#line 200 "D:/zenc/std/fs.zc"
Result_bool__Err("Failed to remove file");
    }

#line 202 "D:/zenc/std/fs.zc"
    return 
#line 202 "D:/zenc/std/fs.zc"
Result_bool__Ok(true);
    }
}

#line 205 "D:/zenc/std/fs.zc"

Result_bool File__remove_dir(char* path)
{
    {

#line 206 "D:/zenc/std/fs.zc"
    ZC_AUTO status = 
#line 206 "D:/zenc/std/fs.zc"
rmdir(path);

#line 207 "D:/zenc/std/fs.zc"
    int zero = 0;

#line 208 "D:/zenc/std/fs.zc"
if ((status != zero))     {

#line 209 "D:/zenc/std/fs.zc"
    return 
#line 209 "D:/zenc/std/fs.zc"
Result_bool__Err("Failed to remove directory");
    }

#line 211 "D:/zenc/std/fs.zc"
    return 
#line 211 "D:/zenc/std/fs.zc"
Result_bool__Ok(true);
    }
}

#line 214 "D:/zenc/std/fs.zc"

Result_Vec_DirEntry File__read_dir(char* path)
{
    {

#line 215 "D:/zenc/std/fs.zc"
    void* dir = 
#line 215 "D:/zenc/std/fs.zc"
_z_fs_opendir(path);

#line 216 "D:/zenc/std/fs.zc"
if ((dir == NULL))     {

#line 217 "D:/zenc/std/fs.zc"
    return 
#line 217 "D:/zenc/std/fs.zc"
Result_Vec_DirEntry__Err("Failed to open directory");
    }

#line 220 "D:/zenc/std/fs.zc"
    int __z_drop_flag_entries = 1; Vec_DirEntry entries = 
#line 220 "D:/zenc/std/fs.zc"
Vec_DirEntry__new();

#line 221 "D:/zenc/std/fs.zc"
    char* name_buf = 
#line 221 "D:/zenc/std/fs.zc"
malloc(256);

#line 223 "D:/zenc/std/fs.zc"
if ((name_buf == NULL))     {

#line 224 "D:/zenc/std/fs.zc"
_z_fs_closedir(dir);

#line 225 "D:/zenc/std/fs.zc"
    { Result_Vec_DirEntry _z_ret = 
#line 225 "D:/zenc/std/fs.zc"
Result_Vec_DirEntry__Err("Out of memory"); 
#line 220 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_entries) Vec_DirEntry__Drop_glue(&entries);
return _z_ret; }
    }

#line 228 "D:/zenc/std/fs.zc"
    int is_d = 0;

#line 230 "D:/zenc/std/fs.zc"
    int is_d_zero = 0;

#line 231 "D:/zenc/std/fs.zc"
while (
#line 231 "D:/zenc/std/fs.zc"
_z_fs_read_entry(dir, name_buf, 256, (&is_d)))     {

#line 232 "D:/zenc/std/fs.zc"
    int zero_cmp = 0;

#line 233 "D:/zenc/std/fs.zc"
if (((
#line 233 "D:/zenc/std/fs.zc"
strcmp(name_buf, ".") == zero_cmp) || (strcmp(name_buf, "..") == zero_cmp)))     {

#line 234 "D:/zenc/std/fs.zc"
continue;
    }

#line 237 "D:/zenc/std/fs.zc"
    int __z_drop_flag_s = 1; String s = 
#line 237 "D:/zenc/std/fs.zc"
String__new(name_buf);

#line 238 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ent = 1; DirEntry ent = (struct DirEntry){.name = ({ __z_drop_flag_s = 0; s; }), .is_dir = (is_d != is_d_zero)};

#line 244 "D:/zenc/std/fs.zc"
String__forget((&s));

#line 246 "D:/zenc/std/fs.zc"
Vec_DirEntry__push((&entries), ({ __z_drop_flag_ent = 0; ent; }));

#line 249 "D:/zenc/std/fs.zc"
String__forget((&ent.name));

#line 238 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ent) DirEntry__Drop_glue(&ent);

#line 237 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }

#line 252 "D:/zenc/std/fs.zc"
free(name_buf);

#line 253 "D:/zenc/std/fs.zc"
_z_fs_closedir(dir);

#line 255 "D:/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result_Vec_DirEntry res = 
#line 255 "D:/zenc/std/fs.zc"
Result_Vec_DirEntry__Ok(({ __z_drop_flag_entries = 0; entries; }));

#line 256 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result_Vec_DirEntry ret = res;
__z_drop_flag_res = 0;

#line 257 "D:/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 256 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_Vec_DirEntry__Drop_glue(&ret);

#line 255 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_Vec_DirEntry__Drop_glue(&res);

#line 220 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_entries) Vec_DirEntry__Drop_glue(&entries);
_z_ret_mv; });

#line 256 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_Vec_DirEntry__Drop_glue(&ret);

#line 255 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_Vec_DirEntry__Drop_glue(&res);

#line 220 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_entries) Vec_DirEntry__Drop_glue(&entries);
    }
}

#line 260 "D:/zenc/std/fs.zc"

Result_String File__current_dir(void)
{
    {

#line 261 "D:/zenc/std/fs.zc"
    char* buf = 
#line 261 "D:/zenc/std/fs.zc"
malloc(1024);

#line 262 "D:/zenc/std/fs.zc"
if ((buf == NULL))     {

#line 262 "D:/zenc/std/fs.zc"
    return 
#line 262 "D:/zenc/std/fs.zc"
Result_String__Err("Out of memory");
    }

#line 264 "D:/zenc/std/fs.zc"
if ((
#line 264 "D:/zenc/std/fs.zc"
getcwd(buf, 1024) == NULL))     {

#line 265 "D:/zenc/std/fs.zc"
free(buf);

#line 266 "D:/zenc/std/fs.zc"
    return 
#line 266 "D:/zenc/std/fs.zc"
Result_String__Err("Failed to get current directory");
    }

#line 269 "D:/zenc/std/fs.zc"
    int __z_drop_flag_s = 1; String s = 
#line 269 "D:/zenc/std/fs.zc"
String__new(buf);

#line 270 "D:/zenc/std/fs.zc"
free(buf);

#line 271 "D:/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result_String res = 
#line 271 "D:/zenc/std/fs.zc"
Result_String__Ok(({ __z_drop_flag_s = 0; s; }));

#line 272 "D:/zenc/std/fs.zc"
String__forget((&s));

#line 273 "D:/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result_String ret = res;
__z_drop_flag_res = 0;

#line 274 "D:/zenc/std/fs.zc"
Result_String__forget((&res));

#line 275 "D:/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 273 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_String__Drop_glue(&ret);

#line 271 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);

#line 269 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
_z_ret_mv; });

#line 273 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result_String__Drop_glue(&ret);

#line 271 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result_String__Drop_glue(&res);

#line 269 "D:/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
}

#line 50 "D:/zenc/std/string.zc"

String String__new(char* s)
{
    {

#line 51 "D:/zenc/std/string.zc"
    ZC_AUTO len = 
#line 51 "D:/zenc/std/string.zc"
strlen(s);

#line 52 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 52 "D:/zenc/std/string.zc"
Vec_char__new();

#line 54 "D:/zenc/std/string.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 55 "D:/zenc/std/string.zc"
Vec_char__push((&v), s[i]);
    }

#line 57 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 60 "D:/zenc/std/string.zc"
    char* d = v.data;

#line 61 "D:/zenc/std/string.zc"
    size_t l = v.len;

#line 62 "D:/zenc/std/string.zc"
    size_t c = v.cap;

#line 65 "D:/zenc/std/string.zc"
Vec_char__forget((&v));

#line 67 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c}}; 
#line 52 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }

#line 52 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

#line 70 "D:/zenc/std/string.zc"

String String__from(char* s)
{
    {

#line 71 "D:/zenc/std/string.zc"
    return 
#line 71 "D:/zenc/std/string.zc"
String__new(s);
    }
}

#line 74 "D:/zenc/std/string.zc"

String String__from_rune(int32_t r)
{
    {

#line 75 "D:/zenc/std/string.zc"
    int __z_drop_flag_s = 1; String s = 
#line 75 "D:/zenc/std/string.zc"
String__new("");

#line 76 "D:/zenc/std/string.zc"
String__push_rune(&s, r);

#line 77 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 75 "D:/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
_z_ret_mv; });

#line 75 "D:/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
}

#line 80 "D:/zenc/std/string.zc"

String String__from_runes(int32_t* runes, size_t count)
{
    {

#line 81 "D:/zenc/std/string.zc"
    int __z_drop_flag_s = 1; String s = 
#line 81 "D:/zenc/std/string.zc"
String__new("");

#line 82 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < count; i = (i + 1))     {

#line 83 "D:/zenc/std/string.zc"
String__push_rune(&s, runes[i]);
    }

#line 85 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 81 "D:/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
_z_ret_mv; });

#line 81 "D:/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
}

#line 88 "D:/zenc/std/string.zc"

char* String__c_str(String* self)
{
    {

#line 89 "D:/zenc/std/string.zc"
    return self->vec.data;
    }
}

#line 92 "D:/zenc/std/string.zc"

char* String__to_string(String* self)
{
    {

#line 93 "D:/zenc/std/string.zc"
    return 
#line 93 "D:/zenc/std/string.zc"
String__c_str(self);
    }
}

#line 96 "D:/zenc/std/string.zc"

void String__destroy(String* self)
{
    {

#line 97 "D:/zenc/std/string.zc"
Vec_char__free((&self->vec));
    }
}

#line 100 "D:/zenc/std/string.zc"

void String__forget(String* self)
{
    {

#line 101 "D:/zenc/std/string.zc"
Vec_char__forget((&self->vec));
    }
}

#line 104 "D:/zenc/std/string.zc"

void String__append(String* self, String* other)
{
    {

#line 106 "D:/zenc/std/string.zc"
if ((self->vec.len > 0))     {

#line 107 "D:/zenc/std/string.zc"
(self->vec.len = (self->vec.len - 1));
    }

#line 110 "D:/zenc/std/string.zc"
    size_t other_len = (*other).vec.len;

#line 111 "D:/zenc/std/string.zc"
for (int i = (int)(0); i < other_len; i = (i + 1))     {

#line 112 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), Vec_char__get((&(*other).vec), i));
    }
    }
}

#line 116 "D:/zenc/std/string.zc"

void String__append_c(String* self, char* s)
{
    {

#line 117 "D:/zenc/std/string.zc"
if ((self->vec.len > 0))     {

#line 118 "D:/zenc/std/string.zc"
(self->vec.len = (self->vec.len - 1));
    }

#line 120 "D:/zenc/std/string.zc"
    ZC_AUTO len = 
#line 120 "D:/zenc/std/string.zc"
strlen(s);

#line 121 "D:/zenc/std/string.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 122 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), s[i]);
    }

#line 124 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), 0);
    }
}

#line 127 "D:/zenc/std/string.zc"

void String__push_rune(String* self, int32_t r)
{
    {

#line 128 "D:/zenc/std/string.zc"
if ((self->vec.len > 0))     {

#line 129 "D:/zenc/std/string.zc"
(self->vec.len = (self->vec.len - 1));
    }

#line 132 "D:/zenc/std/string.zc"
    uint32_t val = ((uint32_t)(r));

#line 133 "D:/zenc/std/string.zc"
if ((val < 128))     {

#line 134 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)(val)));
    }

#line 135 "D:/zenc/std/string.zc"
 else if ((val < 2048))     {

#line 136 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((192 | (val >> 6)))));

#line 137 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((128 | (val & 63)))));
    }

#line 138 "D:/zenc/std/string.zc"
 else if ((val < 65536))     {

#line 139 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((224 | (val >> 12)))));

#line 140 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((128 | ((val >> 6) & 63)))));

#line 141 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((128 | (val & 63)))));
    }
 else     {

#line 143 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((240 | (val >> 18)))));

#line 144 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((128 | ((val >> 12) & 63)))));

#line 145 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((128 | ((val >> 6) & 63)))));

#line 146 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), ((char)((128 | (val & 63)))));
    }

#line 148 "D:/zenc/std/string.zc"
Vec_char__push((&self->vec), 0);
    }
}

#line 151 "D:/zenc/std/string.zc"

void String__append_c_ptr(String* ptr, char* s)
{
    {

#line 152 "D:/zenc/std/string.zc"
if ((ptr->vec.len > 0))     {

#line 153 "D:/zenc/std/string.zc"
(ptr->vec.len = (ptr->vec.len - 1));
    }

#line 155 "D:/zenc/std/string.zc"
    ZC_AUTO len = 
#line 155 "D:/zenc/std/string.zc"
strlen(s);

#line 156 "D:/zenc/std/string.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 157 "D:/zenc/std/string.zc"
Vec_char__push((&ptr->vec), s[i]);
    }

#line 159 "D:/zenc/std/string.zc"
Vec_char__push((&ptr->vec), 0);
    }
}

#line 162 "D:/zenc/std/string.zc"

String String__add(String* self, String* other)
{
    {

#line 163 "D:/zenc/std/string.zc"
    int __z_drop_flag_new_s = 1; String new_s = 
#line 163 "D:/zenc/std/string.zc"
String__from(String__c_str(self));

#line 164 "D:/zenc/std/string.zc"
String__append((&new_s), other);

#line 166 "D:/zenc/std/string.zc"
    char* d = new_s.vec.data;

#line 167 "D:/zenc/std/string.zc"
    size_t l = new_s.vec.len;

#line 168 "D:/zenc/std/string.zc"
    size_t c = new_s.vec.cap;

#line 169 "D:/zenc/std/string.zc"
String__forget((&new_s));

#line 171 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c}}; 
#line 163 "D:/zenc/std/string.zc"
    if (__z_drop_flag_new_s) String__Drop_glue(&new_s);
return _z_ret; }

#line 163 "D:/zenc/std/string.zc"
    if (__z_drop_flag_new_s) String__Drop_glue(&new_s);
    }
}

#line 174 "D:/zenc/std/string.zc"

void String__add_assign(String* self, String* other)
{
    {

#line 175 "D:/zenc/std/string.zc"
String__append(self, other);
    }
}

#line 178 "D:/zenc/std/string.zc"

bool String__eq(String* self, String* other)
{
    {

#line 179 "D:/zenc/std/string.zc"
    int zero = 0;

#line 180 "D:/zenc/std/string.zc"
    return (
#line 180 "D:/zenc/std/string.zc"
strcmp(String__c_str(self), String__c_str((&(*other)))) == zero);
    }
}

#line 183 "D:/zenc/std/string.zc"

bool String__neq(String* self, String* other)
{
    {

#line 184 "D:/zenc/std/string.zc"
    return (!
#line 184 "D:/zenc/std/string.zc"
String__eq(self, other));
    }
}

#line 187 "D:/zenc/std/string.zc"

int32_t String__compare(String* self, String* other)
{
    {

#line 188 "D:/zenc/std/string.zc"
    return 
#line 188 "D:/zenc/std/string.zc"
strcmp(String__c_str(self), String__c_str((&(*other))));
    }
}

#line 191 "D:/zenc/std/string.zc"

bool String__lt(String* self, String* other)
{
    {

#line 192 "D:/zenc/std/string.zc"
    return (
#line 192 "D:/zenc/std/string.zc"
String__compare(self, other) < 0);
    }
}

#line 195 "D:/zenc/std/string.zc"

bool String__gt(String* self, String* other)
{
    {

#line 196 "D:/zenc/std/string.zc"
    return (
#line 196 "D:/zenc/std/string.zc"
String__compare(self, other) > 0);
    }
}

#line 199 "D:/zenc/std/string.zc"

bool String__le(String* self, String* other)
{
    {

#line 200 "D:/zenc/std/string.zc"
    return (
#line 200 "D:/zenc/std/string.zc"
String__compare(self, other) <= 0);
    }
}

#line 203 "D:/zenc/std/string.zc"

bool String__ge(String* self, String* other)
{
    {

#line 204 "D:/zenc/std/string.zc"
    return (
#line 204 "D:/zenc/std/string.zc"
String__compare(self, other) >= 0);
    }
}

#line 207 "D:/zenc/std/string.zc"

int32_t String__compare_ignore_case(String* self, String* other)
{
    {

#line 208 "D:/zenc/std/string.zc"
    return 
#line 208 "D:/zenc/std/string.zc"
strcasecmp(String__c_str(self), String__c_str((&(*other))));
    }
}

#line 211 "D:/zenc/std/string.zc"

bool String__eq_ignore_case(String* self, String* other)
{
    {

#line 212 "D:/zenc/std/string.zc"
    int32_t zero = 0;

#line 213 "D:/zenc/std/string.zc"
    return (
#line 213 "D:/zenc/std/string.zc"
String__compare_ignore_case(self, other) == zero);
    }
}

#line 216 "D:/zenc/std/string.zc"

bool String__eq_str(String* self, char* s)
{
    {

#line 217 "D:/zenc/std/string.zc"
    int zero = 0;

#line 218 "D:/zenc/std/string.zc"
    return (
#line 218 "D:/zenc/std/string.zc"
strcmp(String__c_str(self), s) == zero);
    }
}

#line 221 "D:/zenc/std/string.zc"

size_t String__length(String* self)
{
    {

#line 222 "D:/zenc/std/string.zc"
if ((self->vec.len == 0))     {

#line 222 "D:/zenc/std/string.zc"
    return 0;
    }

#line 223 "D:/zenc/std/string.zc"
    return (self->vec.len - 1);
    }
}

#line 226 "D:/zenc/std/string.zc"

String String__substring(String* self, size_t start, size_t len)
{
    {

#line 227 "D:/zenc/std/string.zc"
if (((start + len) > 
#line 227 "D:/zenc/std/string.zc"
String__length(self)))     {

#line 228 "D:/zenc/std/string.zc"
__zenc_panic("substring out of bounds");
    }

#line 230 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 230 "D:/zenc/std/string.zc"
Vec_char__new();

#line 231 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 232 "D:/zenc/std/string.zc"
Vec_char__push((&v), Vec_char__get((&self->vec), (start + i)));
    }

#line 234 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 236 "D:/zenc/std/string.zc"
    char* d = v.data;

#line 237 "D:/zenc/std/string.zc"
    size_t l = v.len;

#line 238 "D:/zenc/std/string.zc"
    size_t c = v.cap;

#line 239 "D:/zenc/std/string.zc"
Vec_char__forget((&v));

#line 241 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c}}; 
#line 230 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }

#line 230 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

#line 243 "D:/zenc/std/string.zc"

bool String__contains_str(String* self, char* target)
{
    {

#line 244 "D:/zenc/std/string.zc"
    return 
#line 244 "D:/zenc/std/string.zc"
Option_size_t__is_some((Option_size_t[]){String__find_str(self, target)});
    }
}

#line 247 "D:/zenc/std/string.zc"

Option_size_t String__find_str(String* self, char* target)
{
    {

#line 248 "D:/zenc/std/string.zc"
    ZC_AUTO t_len = 
#line 248 "D:/zenc/std/string.zc"
strlen(target);

#line 249 "D:/zenc/std/string.zc"
if ((t_len == 0))     {

#line 249 "D:/zenc/std/string.zc"
    return 
#line 249 "D:/zenc/std/string.zc"
Option_size_t__Some(0);
    }

#line 250 "D:/zenc/std/string.zc"
    size_t s_len = 
#line 250 "D:/zenc/std/string.zc"
String__length(self);

#line 251 "D:/zenc/std/string.zc"
if ((t_len > s_len))     {

#line 251 "D:/zenc/std/string.zc"
    return 
#line 251 "D:/zenc/std/string.zc"
Option_size_t__None();
    }

#line 253 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i <= (s_len - t_len); i = (i + 1))     {

#line 254 "D:/zenc/std/string.zc"
    bool is_match = true;

#line 255 "D:/zenc/std/string.zc"
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {

#line 256 "D:/zenc/std/string.zc"
if ((
#line 256 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + k)) != target[k]))     {

#line 257 "D:/zenc/std/string.zc"
(is_match = false);

#line 258 "D:/zenc/std/string.zc"
break;
    }
    }

#line 261 "D:/zenc/std/string.zc"
if (is_match)     {

#line 261 "D:/zenc/std/string.zc"
    return 
#line 261 "D:/zenc/std/string.zc"
Option_size_t__Some(i);
    }
    }

#line 263 "D:/zenc/std/string.zc"
    return 
#line 263 "D:/zenc/std/string.zc"
Option_size_t__None();
    }
}

#line 266 "D:/zenc/std/string.zc"

Vec_size_t String__find_all_str(String* self, char* target)
{
    {

#line 267 "D:/zenc/std/string.zc"
    int __z_drop_flag_indices = 1; Vec_size_t indices = 
#line 267 "D:/zenc/std/string.zc"
Vec_size_t__new();

#line 268 "D:/zenc/std/string.zc"
    ZC_AUTO t_len = 
#line 268 "D:/zenc/std/string.zc"
strlen(target);

#line 269 "D:/zenc/std/string.zc"
if ((t_len == 0))     {

#line 269 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = indices; memset(&indices, 0, sizeof(_z_ret_mv)); __z_drop_flag_indices = 0; 
#line 267 "D:/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec_size_t__Drop_glue(&indices);
_z_ret_mv; });
    }

#line 270 "D:/zenc/std/string.zc"
    size_t s_len = 
#line 270 "D:/zenc/std/string.zc"
String__length(self);

#line 271 "D:/zenc/std/string.zc"
if ((t_len > s_len))     {

#line 271 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = indices; memset(&indices, 0, sizeof(_z_ret_mv)); __z_drop_flag_indices = 0; 
#line 267 "D:/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec_size_t__Drop_glue(&indices);
_z_ret_mv; });
    }

#line 273 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i <= (s_len - t_len); i = (i + 1))     {

#line 274 "D:/zenc/std/string.zc"
    bool is_match = true;

#line 275 "D:/zenc/std/string.zc"
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {

#line 276 "D:/zenc/std/string.zc"
if ((
#line 276 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + k)) != target[k]))     {

#line 277 "D:/zenc/std/string.zc"
(is_match = false);

#line 278 "D:/zenc/std/string.zc"
break;
    }
    }

#line 281 "D:/zenc/std/string.zc"
if (is_match)     {

#line 282 "D:/zenc/std/string.zc"
Vec_size_t__push((&indices), i);
    }
    }

#line 286 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = indices; memset(&indices, 0, sizeof(_z_ret_mv)); __z_drop_flag_indices = 0; 
#line 267 "D:/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec_size_t__Drop_glue(&indices);
_z_ret_mv; });

#line 267 "D:/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec_size_t__Drop_glue(&indices);
    }
}

#line 289 "D:/zenc/std/string.zc"

String String__to_lowercase(String* self)
{
    {

#line 290 "D:/zenc/std/string.zc"
    size_t len = 
#line 290 "D:/zenc/std/string.zc"
String__length(self);

#line 291 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 291 "D:/zenc/std/string.zc"
Vec_char__new();

#line 292 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 293 "D:/zenc/std/string.zc"
    char c = 
#line 293 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i);

#line 294 "D:/zenc/std/string.zc"
if (((c >= 'A') && (c <= 'Z')))     {

#line 295 "D:/zenc/std/string.zc"
Vec_char__push((&v), ((char)((((int32_t)(c)) + 32))));
    }
 else     {

#line 297 "D:/zenc/std/string.zc"
Vec_char__push((&v), c);
    }
    }

#line 300 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 301 "D:/zenc/std/string.zc"
    char* d = v.data;

#line 302 "D:/zenc/std/string.zc"
    size_t l = v.len;

#line 303 "D:/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 304 "D:/zenc/std/string.zc"
Vec_char__forget((&v));

#line 305 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c_cap}}; 
#line 291 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }

#line 291 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

#line 308 "D:/zenc/std/string.zc"

String String__pad_right(String* self, size_t target_len, char pad_char)
{
    {

#line 309 "D:/zenc/std/string.zc"
    size_t current_len = 
#line 309 "D:/zenc/std/string.zc"
String__length(self);

#line 310 "D:/zenc/std/string.zc"
if ((current_len >= target_len))     {

#line 311 "D:/zenc/std/string.zc"
    return 
#line 311 "D:/zenc/std/string.zc"
String__from(String__c_str(self));
    }

#line 314 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 314 "D:/zenc/std/string.zc"
Vec_char__new();

#line 315 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < current_len; i = (i + 1))     {

#line 316 "D:/zenc/std/string.zc"
Vec_char__push((&v), Vec_char__get((&self->vec), i));
    }

#line 318 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(current_len); i < target_len; i = (i + 1))     {

#line 319 "D:/zenc/std/string.zc"
Vec_char__push((&v), pad_char);
    }

#line 321 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 323 "D:/zenc/std/string.zc"
    char* d = v.data;

#line 324 "D:/zenc/std/string.zc"
    size_t l = v.len;

#line 325 "D:/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 326 "D:/zenc/std/string.zc"
Vec_char__forget((&v));

#line 327 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c_cap}}; 
#line 314 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }

#line 314 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

#line 330 "D:/zenc/std/string.zc"

String String__pad_left(String* self, size_t target_len, char pad_char)
{
    {

#line 331 "D:/zenc/std/string.zc"
    size_t current_len = 
#line 331 "D:/zenc/std/string.zc"
String__length(self);

#line 332 "D:/zenc/std/string.zc"
if ((current_len >= target_len))     {

#line 333 "D:/zenc/std/string.zc"
    return 
#line 333 "D:/zenc/std/string.zc"
String__from(String__c_str(self));
    }

#line 336 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 336 "D:/zenc/std/string.zc"
Vec_char__new();

#line 337 "D:/zenc/std/string.zc"
    size_t diff = (target_len - current_len);

#line 338 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < diff; i = (i + 1))     {

#line 339 "D:/zenc/std/string.zc"
Vec_char__push((&v), pad_char);
    }

#line 341 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < current_len; i = (i + 1))     {

#line 342 "D:/zenc/std/string.zc"
Vec_char__push((&v), Vec_char__get((&self->vec), i));
    }

#line 344 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 346 "D:/zenc/std/string.zc"
    char* d = v.data;

#line 347 "D:/zenc/std/string.zc"
    size_t l = v.len;

#line 348 "D:/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 349 "D:/zenc/std/string.zc"
Vec_char__forget((&v));

#line 350 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c_cap}}; 
#line 336 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }

#line 336 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

#line 353 "D:/zenc/std/string.zc"

String String__to_uppercase(String* self)
{
    {

#line 354 "D:/zenc/std/string.zc"
    size_t len = 
#line 354 "D:/zenc/std/string.zc"
String__length(self);

#line 355 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 355 "D:/zenc/std/string.zc"
Vec_char__new();

#line 356 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 357 "D:/zenc/std/string.zc"
    char c = 
#line 357 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i);

#line 358 "D:/zenc/std/string.zc"
if (((c >= 'a') && (c <= 'z')))     {

#line 359 "D:/zenc/std/string.zc"
Vec_char__push((&v), ((char)((((int32_t)(c)) - 32))));
    }
 else     {

#line 361 "D:/zenc/std/string.zc"
Vec_char__push((&v), c);
    }
    }

#line 364 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 365 "D:/zenc/std/string.zc"
    char* d = v.data;

#line 366 "D:/zenc/std/string.zc"
    size_t l = v.len;

#line 367 "D:/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 368 "D:/zenc/std/string.zc"
Vec_char__forget((&v));

#line 369 "D:/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c_cap}}; 
#line 355 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }

#line 355 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

#line 372 "D:/zenc/std/string.zc"

Option_size_t String__find(String* self, char target)
{
    {

#line 373 "D:/zenc/std/string.zc"
    size_t len = 
#line 373 "D:/zenc/std/string.zc"
String__length(self);

#line 374 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 375 "D:/zenc/std/string.zc"
if ((
#line 375 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i) == target))     {

#line 376 "D:/zenc/std/string.zc"
    return 
#line 376 "D:/zenc/std/string.zc"
Option_size_t__Some(i);
    }
    }

#line 379 "D:/zenc/std/string.zc"
    return 
#line 379 "D:/zenc/std/string.zc"
Option_size_t__None();
    }
}

#line 382 "D:/zenc/std/string.zc"

void String__print(String* self)
{
    {

#line 383 "D:/zenc/std/string.zc"
printf("%s", String__c_str(self));

#line 384 "D:/zenc/std/string.zc"
fflush(stdout);
    }
}

#line 387 "D:/zenc/std/string.zc"

void String__println(String* self)
{
    {

#line 388 "D:/zenc/std/string.zc"
printf("%s\n", String__c_str(self));
    }
}

#line 391 "D:/zenc/std/string.zc"

bool String__is_empty(String* self)
{
    {

#line 392 "D:/zenc/std/string.zc"
    return (
#line 392 "D:/zenc/std/string.zc"
String__length(self) == 0);
    }
}

#line 395 "D:/zenc/std/string.zc"

bool String__contains(String* self, char target)
{
    {

#line 396 "D:/zenc/std/string.zc"
    return 
#line 396 "D:/zenc/std/string.zc"
Option_size_t__is_some((__typeof__((String__find(self, target)))[]){String__find(self, target)});
    }
}

#line 399 "D:/zenc/std/string.zc"

bool String__starts_with(String* self, char* prefix)
{
    {

#line 400 "D:/zenc/std/string.zc"
    ZC_AUTO plen = 
#line 400 "D:/zenc/std/string.zc"
strlen(prefix);

#line 401 "D:/zenc/std/string.zc"
if ((plen > 
#line 401 "D:/zenc/std/string.zc"
String__length(self)))     {

#line 401 "D:/zenc/std/string.zc"
    return false;
    }

#line 402 "D:/zenc/std/string.zc"
    int zero = 0;

#line 403 "D:/zenc/std/string.zc"
    return (
#line 403 "D:/zenc/std/string.zc"
strncmp(String__c_str(self), prefix, plen) == zero);
    }
}

#line 406 "D:/zenc/std/string.zc"

bool String__ends_with(String* self, char* suffix)
{
    {

#line 407 "D:/zenc/std/string.zc"
    ZC_AUTO slen = 
#line 407 "D:/zenc/std/string.zc"
strlen(suffix);

#line 408 "D:/zenc/std/string.zc"
    size_t len = 
#line 408 "D:/zenc/std/string.zc"
String__length(self);

#line 409 "D:/zenc/std/string.zc"
if ((slen > len))     {

#line 409 "D:/zenc/std/string.zc"
    return false;
    }

#line 410 "D:/zenc/std/string.zc"
    int32_t offset = ((int32_t)((len - slen)));

#line 411 "D:/zenc/std/string.zc"
    int zero = 0;

#line 412 "D:/zenc/std/string.zc"
    return (
#line 412 "D:/zenc/std/string.zc"
strcmp((String__c_str(self) + offset), suffix) == zero);
    }
}

#line 415 "D:/zenc/std/string.zc"

void String__reserve(String* self, size_t cap)
{
    {

#line 416 "D:/zenc/std/string.zc"
Vec_char__grow_to_fit((&self->vec), (cap + 1));
    }
}

#line 419 "D:/zenc/std/string.zc"

void String__free(String* self)
{
    {

#line 420 "D:/zenc/std/string.zc"
Vec_char__free((&self->vec));
    }
}

#line 423 "D:/zenc/std/string.zc"

size_t String___utf8_seq_len(char first_byte)
{
    {

#line 424 "D:/zenc/std/string.zc"
    int32_t b = ((int32_t)(first_byte));

#line 425 "D:/zenc/std/string.zc"
if (((b & 128) == 0))     {

#line 425 "D:/zenc/std/string.zc"
    return 1;
    }

#line 426 "D:/zenc/std/string.zc"
if (((b & 224) == 192))     {

#line 426 "D:/zenc/std/string.zc"
    return 2;
    }

#line 427 "D:/zenc/std/string.zc"
if (((b & 240) == 224))     {

#line 427 "D:/zenc/std/string.zc"
    return 3;
    }

#line 428 "D:/zenc/std/string.zc"
if (((b & 248) == 240))     {

#line 428 "D:/zenc/std/string.zc"
    return 4;
    }

#line 429 "D:/zenc/std/string.zc"
    return 1;
    }
}

#line 432 "D:/zenc/std/string.zc"

size_t String__utf8_len(String* self)
{
    {

#line 433 "D:/zenc/std/string.zc"
    size_t count = 0;

#line 434 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 435 "D:/zenc/std/string.zc"
    size_t len = 
#line 435 "D:/zenc/std/string.zc"
String__length(self);

#line 436 "D:/zenc/std/string.zc"
while ((i < len))     {

#line 437 "D:/zenc/std/string.zc"
    char c = 
#line 437 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i);

#line 438 "D:/zenc/std/string.zc"
(i = (i + 
#line 438 "D:/zenc/std/string.zc"
String___utf8_seq_len(c)));

#line 439 "D:/zenc/std/string.zc"
(count = (count + 1));
    }

#line 441 "D:/zenc/std/string.zc"
    return count;
    }
}

#line 444 "D:/zenc/std/string.zc"

String String__utf8_at(String* self, size_t idx)
{
    {

#line 445 "D:/zenc/std/string.zc"
    size_t count = 0;

#line 446 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 447 "D:/zenc/std/string.zc"
    size_t len = 
#line 447 "D:/zenc/std/string.zc"
String__length(self);

#line 448 "D:/zenc/std/string.zc"
while ((i < len))     {

#line 449 "D:/zenc/std/string.zc"
    char c = 
#line 449 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i);

#line 450 "D:/zenc/std/string.zc"
    size_t seq = 
#line 450 "D:/zenc/std/string.zc"
String___utf8_seq_len(c);

#line 452 "D:/zenc/std/string.zc"
if ((count == idx))     {

#line 453 "D:/zenc/std/string.zc"
    return 
#line 453 "D:/zenc/std/string.zc"
String__substring(self, i, seq);
    }

#line 456 "D:/zenc/std/string.zc"
(i = (i + seq));

#line 457 "D:/zenc/std/string.zc"
(count = (count + 1));
    }

#line 459 "D:/zenc/std/string.zc"
    return 
#line 459 "D:/zenc/std/string.zc"
String__new("");
    }
}

#line 462 "D:/zenc/std/string.zc"

int32_t String__utf8_get(String* self, size_t idx)
{
    {

#line 463 "D:/zenc/std/string.zc"
    size_t count = 0;

#line 464 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 465 "D:/zenc/std/string.zc"
    size_t len = 
#line 465 "D:/zenc/std/string.zc"
String__length(self);

#line 466 "D:/zenc/std/string.zc"
while ((i < len))     {

#line 467 "D:/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(
#line 467 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i)));

#line 468 "D:/zenc/std/string.zc"
    size_t seq = 
#line 468 "D:/zenc/std/string.zc"
String___utf8_seq_len(((char)(c)));

#line 470 "D:/zenc/std/string.zc"
if ((count == idx))     {

#line 471 "D:/zenc/std/string.zc"
if ((seq == 1))     {

#line 471 "D:/zenc/std/string.zc"
    return ((int32_t)(c));
    }

#line 472 "D:/zenc/std/string.zc"
if ((seq == 2))     {

#line 472 "D:/zenc/std/string.zc"
    return ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(
#line 472 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63))));
    }

#line 473 "D:/zenc/std/string.zc"
if ((seq == 3))     {

#line 473 "D:/zenc/std/string.zc"
    return ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(
#line 473 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 2)))))) & 63))));
    }

#line 474 "D:/zenc/std/string.zc"
if ((seq == 4))     {

#line 474 "D:/zenc/std/string.zc"
    return ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(
#line 474 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63) << 12)) | ((((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 2)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 3)))))) & 63))));
    }
    }

#line 477 "D:/zenc/std/string.zc"
(i = (i + seq));

#line 478 "D:/zenc/std/string.zc"
(count = (count + 1));
    }

#line 480 "D:/zenc/std/string.zc"
    return ((int32_t)(0));
    }
}

#line 483 "D:/zenc/std/string.zc"

Vec_int32_t String__runes(String* self)
{
    {

#line 484 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_int32_t v = 
#line 484 "D:/zenc/std/string.zc"
Vec_int32_t__new();

#line 485 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 486 "D:/zenc/std/string.zc"
    size_t len = 
#line 486 "D:/zenc/std/string.zc"
String__length(self);

#line 487 "D:/zenc/std/string.zc"
while ((i < len))     {

#line 488 "D:/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(
#line 488 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i)));

#line 489 "D:/zenc/std/string.zc"
    size_t seq = 
#line 489 "D:/zenc/std/string.zc"
String___utf8_seq_len(((char)(c)));

#line 490 "D:/zenc/std/string.zc"
    int32_t val = 0;

#line 491 "D:/zenc/std/string.zc"
if ((seq == 1))     {

#line 492 "D:/zenc/std/string.zc"
(val = ((int32_t)(c)));
    }

#line 493 "D:/zenc/std/string.zc"
 else if ((seq == 2))     {

#line 494 "D:/zenc/std/string.zc"
(val = ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(
#line 494 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63)))));
    }

#line 495 "D:/zenc/std/string.zc"
 else if ((seq == 3))     {

#line 496 "D:/zenc/std/string.zc"
(val = ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(
#line 496 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 2)))))) & 63)))));
    }

#line 497 "D:/zenc/std/string.zc"
 else if ((seq == 4))     {

#line 498 "D:/zenc/std/string.zc"
(val = ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(
#line 498 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63) << 12)) | ((((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 2)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 3)))))) & 63)))));
    }

#line 500 "D:/zenc/std/string.zc"
Vec_int32_t__push((&v), val);

#line 501 "D:/zenc/std/string.zc"
(i = (i + seq));
    }

#line 503 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 484 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_int32_t__Drop_glue(&v);
_z_ret_mv; });

#line 484 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_int32_t__Drop_glue(&v);
    }
}

#line 506 "D:/zenc/std/string.zc"

StringCharsIter String__iterator(String* self)
{
    {

#line 507 "D:/zenc/std/string.zc"
    return 
#line 507 "D:/zenc/std/string.zc"
String__chars(self);
    }
}

#line 510 "D:/zenc/std/string.zc"

StringCharsIter String__chars(String* self)
{
    {

#line 511 "D:/zenc/std/string.zc"
    return (struct StringCharsIter){.data = 
#line 512 "D:/zenc/std/string.zc"
String__c_str(self), .len = 
#line 513 "D:/zenc/std/string.zc"
String__length(self)};
    }
}

#line 518 "D:/zenc/std/string.zc"

String String__from_runes_vec(Vec_int32_t runes)
{

#line 518 "D:/zenc/std/string.zc"
    int __z_drop_flag_runes = 1;
    {

#line 519 "D:/zenc/std/string.zc"
    int __z_drop_flag_s = 1; String s = 
#line 519 "D:/zenc/std/string.zc"
String__new("");

#line 520 "D:/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < runes.len; i = (i + 1))     {

#line 521 "D:/zenc/std/string.zc"
String__push_rune((&s), Vec_int32_t__get((&runes), i));
    }

#line 523 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 519 "D:/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);

#line 518 "D:/zenc/std/string.zc"
    if (__z_drop_flag_runes) Vec_int32_t__Drop_glue(&runes);
_z_ret_mv; });

#line 519 "D:/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }

#line 518 "D:/zenc/std/string.zc"
    if (__z_drop_flag_runes) Vec_int32_t__Drop_glue(&runes);
}

#line 526 "D:/zenc/std/string.zc"

void String__insert_rune(String* self, size_t idx, int32_t r)
{
    {

#line 527 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 528 "D:/zenc/std/string.zc"
    size_t count = 0;

#line 529 "D:/zenc/std/string.zc"
    size_t len = 
#line 529 "D:/zenc/std/string.zc"
String__length(self);

#line 530 "D:/zenc/std/string.zc"
while (((i < len) && (count < idx)))     {

#line 531 "D:/zenc/std/string.zc"
(i = (i + 
#line 531 "D:/zenc/std/string.zc"
String___utf8_seq_len(Vec_char__get((&self->vec), i))));

#line 532 "D:/zenc/std/string.zc"
(count = (count + 1));
    }

#line 535 "D:/zenc/std/string.zc"
    uint32_t val = ((uint32_t)(r));

#line 536 "D:/zenc/std/string.zc"
if ((val < 128))     {

#line 537 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), i, ((char)(val)));
    }

#line 538 "D:/zenc/std/string.zc"
 else if ((val < 2048))     {

#line 539 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), i, ((char)((192 | (val >> 6)))));

#line 540 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), (i + 1), ((char)((128 | (val & 63)))));
    }

#line 541 "D:/zenc/std/string.zc"
 else if ((val < 65536))     {

#line 542 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), i, ((char)((224 | (val >> 12)))));

#line 543 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), (i + 1), ((char)((128 | ((val >> 6) & 63)))));

#line 544 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), (i + 2), ((char)((128 | (val & 63)))));
    }
 else     {

#line 546 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), i, ((char)((240 | (val >> 18)))));

#line 547 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), (i + 1), ((char)((128 | ((val >> 12) & 63)))));

#line 548 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), (i + 2), ((char)((128 | ((val >> 6) & 63)))));

#line 549 "D:/zenc/std/string.zc"
Vec_char__insert((&self->vec), (i + 3), ((char)((128 | (val & 63)))));
    }
    }
}

#line 553 "D:/zenc/std/string.zc"

int32_t String__remove_rune_at(String* self, size_t idx)
{
    {

#line 554 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 555 "D:/zenc/std/string.zc"
    size_t count = 0;

#line 556 "D:/zenc/std/string.zc"
    size_t len = 
#line 556 "D:/zenc/std/string.zc"
String__length(self);

#line 557 "D:/zenc/std/string.zc"
while (((i < len) && (count < idx)))     {

#line 558 "D:/zenc/std/string.zc"
(i = (i + 
#line 558 "D:/zenc/std/string.zc"
String___utf8_seq_len(Vec_char__get((&self->vec), i))));

#line 559 "D:/zenc/std/string.zc"
(count = (count + 1));
    }

#line 562 "D:/zenc/std/string.zc"
if ((i >= len))     {

#line 562 "D:/zenc/std/string.zc"
    return ((int32_t)(0));
    }

#line 564 "D:/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(
#line 564 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i)));

#line 565 "D:/zenc/std/string.zc"
    size_t seq = 
#line 565 "D:/zenc/std/string.zc"
String___utf8_seq_len(((char)(c)));

#line 566 "D:/zenc/std/string.zc"
    int32_t val = 0;

#line 567 "D:/zenc/std/string.zc"
if ((seq == 1))     {

#line 567 "D:/zenc/std/string.zc"
(val = ((int32_t)(c)));
    }

#line 568 "D:/zenc/std/string.zc"
 else if ((seq == 2))     {

#line 568 "D:/zenc/std/string.zc"
(val = ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(
#line 568 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63)))));
    }

#line 569 "D:/zenc/std/string.zc"
 else if ((seq == 3))     {

#line 569 "D:/zenc/std/string.zc"
(val = ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(
#line 569 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 2)))))) & 63)))));
    }

#line 570 "D:/zenc/std/string.zc"
 else if ((seq == 4))     {

#line 570 "D:/zenc/std/string.zc"
(val = ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(
#line 570 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + 1)))))) & 63) << 12)) | ((((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 2)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec_char__get((&self->vec), (i + 3)))))) & 63)))));
    }

#line 572 "D:/zenc/std/string.zc"
for (size_t j = (size_t)(0); j < seq; j = (j + 1))     {

#line 573 "D:/zenc/std/string.zc"
Vec_char__remove((&self->vec), i);
    }

#line 575 "D:/zenc/std/string.zc"
    return val;
    }
}

#line 578 "D:/zenc/std/string.zc"

String String__utf8_substr(String* self, size_t start_idx, size_t num_chars)
{
    {

#line 579 "D:/zenc/std/string.zc"
if ((num_chars == 0))     {

#line 579 "D:/zenc/std/string.zc"
    return 
#line 579 "D:/zenc/std/string.zc"
String__new("");
    }

#line 581 "D:/zenc/std/string.zc"
    size_t byte_start = 0;

#line 582 "D:/zenc/std/string.zc"
    size_t byte_len = 0;

#line 584 "D:/zenc/std/string.zc"
    size_t count = 0;

#line 585 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 586 "D:/zenc/std/string.zc"
    size_t len = 
#line 586 "D:/zenc/std/string.zc"
String__length(self);

#line 587 "D:/zenc/std/string.zc"
    bool found_start = false;

#line 589 "D:/zenc/std/string.zc"
while ((i < len))     {

#line 591 "D:/zenc/std/string.zc"
if (((!found_start) && (count == start_idx)))     {

#line 592 "D:/zenc/std/string.zc"
(byte_start = i);

#line 593 "D:/zenc/std/string.zc"
(found_start = true);

#line 595 "D:/zenc/std/string.zc"
(count = 0);
    }

#line 596 "D:/zenc/std/string.zc"
 else if ((!found_start))     {

#line 598 "D:/zenc/std/string.zc"
    char c = 
#line 598 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i);

#line 599 "D:/zenc/std/string.zc"
(i = (i + 
#line 599 "D:/zenc/std/string.zc"
String___utf8_seq_len(c)));

#line 600 "D:/zenc/std/string.zc"
(count = (count + 1));

#line 601 "D:/zenc/std/string.zc"
continue;
    }

#line 605 "D:/zenc/std/string.zc"
if ((count < num_chars))     {

#line 606 "D:/zenc/std/string.zc"
    char c = 
#line 606 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i);

#line 607 "D:/zenc/std/string.zc"
    size_t seq = 
#line 607 "D:/zenc/std/string.zc"
String___utf8_seq_len(c);

#line 608 "D:/zenc/std/string.zc"
(byte_len = (byte_len + seq));

#line 609 "D:/zenc/std/string.zc"
(i = (i + seq));

#line 610 "D:/zenc/std/string.zc"
(count = (count + 1));
    }
 else     {

#line 612 "D:/zenc/std/string.zc"
break;
    }
    }

#line 616 "D:/zenc/std/string.zc"
if ((!found_start))     {

#line 616 "D:/zenc/std/string.zc"
    return 
#line 616 "D:/zenc/std/string.zc"
String__new("");
    }

#line 618 "D:/zenc/std/string.zc"
    return 
#line 618 "D:/zenc/std/string.zc"
String__substring(self, byte_start, byte_len);
    }
}

#line 620 "D:/zenc/std/string.zc"

Vec_String String__split(String* self, char delim)
{
    {

#line 621 "D:/zenc/std/string.zc"
    int __z_drop_flag_parts = 1; Vec_String parts = 
#line 621 "D:/zenc/std/string.zc"
Vec_String__new();

#line 622 "D:/zenc/std/string.zc"
    size_t len = 
#line 622 "D:/zenc/std/string.zc"
String__length(self);

#line 623 "D:/zenc/std/string.zc"
if ((len == 0))     {

#line 623 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 621 "D:/zenc/std/string.zc"
    if (__z_drop_flag_parts) Vec_String__Drop_glue(&parts);
_z_ret_mv; });
    }

#line 625 "D:/zenc/std/string.zc"
    size_t start = 0;

#line 626 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 628 "D:/zenc/std/string.zc"
while ((i < len))     {

#line 629 "D:/zenc/std/string.zc"
if ((
#line 629 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), i) == delim))     {

#line 631 "D:/zenc/std/string.zc"
Vec_String__push((&parts), String__substring(self, start, (i - start)));

#line 632 "D:/zenc/std/string.zc"
(start = (i + 1));
    }

#line 634 "D:/zenc/std/string.zc"
(i = (i + 1));
    }

#line 638 "D:/zenc/std/string.zc"
if ((start <= len))     {

#line 639 "D:/zenc/std/string.zc"
Vec_String__push((&parts), String__substring(self, start, (len - start)));
    }

#line 642 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 621 "D:/zenc/std/string.zc"
    if (__z_drop_flag_parts) Vec_String__Drop_glue(&parts);
_z_ret_mv; });

#line 621 "D:/zenc/std/string.zc"
    if (__z_drop_flag_parts) Vec_String__Drop_glue(&parts);
    }
}

#line 645 "D:/zenc/std/string.zc"

String String__trim(String* self)
{
    {

#line 646 "D:/zenc/std/string.zc"
    size_t start = 0;

#line 647 "D:/zenc/std/string.zc"
    size_t len = 
#line 647 "D:/zenc/std/string.zc"
String__length(self);

#line 648 "D:/zenc/std/string.zc"
    size_t end = len;

#line 651 "D:/zenc/std/string.zc"
while ((start < len))     {

#line 652 "D:/zenc/std/string.zc"
    char c = 
#line 652 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), start);

#line 653 "D:/zenc/std/string.zc"
if (((((c != ' ') && (c != '\t')) && (c != '\n')) && (c != '\r')))     {

#line 654 "D:/zenc/std/string.zc"
break;
    }

#line 656 "D:/zenc/std/string.zc"
(start = (start + 1));
    }

#line 659 "D:/zenc/std/string.zc"
if ((start == len))     {

#line 660 "D:/zenc/std/string.zc"
    return 
#line 660 "D:/zenc/std/string.zc"
String__new("");
    }

#line 664 "D:/zenc/std/string.zc"
while ((end > start))     {

#line 665 "D:/zenc/std/string.zc"
    char c = 
#line 665 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (end - 1));

#line 666 "D:/zenc/std/string.zc"
if (((((c != ' ') && (c != '\t')) && (c != '\n')) && (c != '\r')))     {

#line 667 "D:/zenc/std/string.zc"
break;
    }

#line 669 "D:/zenc/std/string.zc"
(end = (end - 1));
    }

#line 672 "D:/zenc/std/string.zc"
    return 
#line 672 "D:/zenc/std/string.zc"
String__substring(self, start, (end - start));
    }
}

#line 675 "D:/zenc/std/string.zc"

String String__replace(String* self, char* target, char* replacement)
{
    {

#line 676 "D:/zenc/std/string.zc"
    ZC_AUTO t_len = 
#line 676 "D:/zenc/std/string.zc"
strlen(target);

#line 677 "D:/zenc/std/string.zc"
if ((t_len == 0))     {

#line 677 "D:/zenc/std/string.zc"
    return 
#line 677 "D:/zenc/std/string.zc"
String__substring(self, 0, String__length(self));
    }

#line 680 "D:/zenc/std/string.zc"
    size_t s_len = 
#line 680 "D:/zenc/std/string.zc"
String__length(self);

#line 681 "D:/zenc/std/string.zc"
    int __z_drop_flag_result = 1; String result = 
#line 681 "D:/zenc/std/string.zc"
String__new("");

#line 683 "D:/zenc/std/string.zc"
    size_t i = 0;

#line 684 "D:/zenc/std/string.zc"
while ((i < s_len))     {

#line 686 "D:/zenc/std/string.zc"
if (((i + t_len) <= s_len))     {

#line 687 "D:/zenc/std/string.zc"
    bool is_match = true;

#line 689 "D:/zenc/std/string.zc"
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {

#line 690 "D:/zenc/std/string.zc"
if ((
#line 690 "D:/zenc/std/string.zc"
Vec_char__get((&self->vec), (i + k)) != target[k]))     {

#line 691 "D:/zenc/std/string.zc"
(is_match = false);

#line 692 "D:/zenc/std/string.zc"
break;
    }
    }

#line 696 "D:/zenc/std/string.zc"
if (is_match)     {

#line 697 "D:/zenc/std/string.zc"
    int __z_drop_flag_r_str = 1; String r_str = 
#line 697 "D:/zenc/std/string.zc"
String__new(replacement);

#line 698 "D:/zenc/std/string.zc"
String__append((&result), (&r_str));

#line 699 "D:/zenc/std/string.zc"
(i = (i + t_len));

#line 700 "D:/zenc/std/string.zc"

#line 697 "D:/zenc/std/string.zc"
    if (__z_drop_flag_r_str) String__Drop_glue(&r_str);
continue;

#line 697 "D:/zenc/std/string.zc"
    if (__z_drop_flag_r_str) String__Drop_glue(&r_str);
    }
    }

#line 705 "D:/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec_char v = 
#line 705 "D:/zenc/std/string.zc"
Vec_char__new();

#line 706 "D:/zenc/std/string.zc"
Vec_char__push((&v), Vec_char__get((&self->vec), i));

#line 707 "D:/zenc/std/string.zc"
Vec_char__push((&v), 0);

#line 708 "D:/zenc/std/string.zc"
    int __z_drop_flag_ch_s = 1; String ch_s = 
#line 708 "D:/zenc/std/string.zc"
String__new(v.data);

#line 709 "D:/zenc/std/string.zc"
String__append((&result), (&ch_s));

#line 710 "D:/zenc/std/string.zc"
String__destroy((&ch_s));

#line 711 "D:/zenc/std/string.zc"
(i = (i + 1));

#line 708 "D:/zenc/std/string.zc"
    if (__z_drop_flag_ch_s) String__Drop_glue(&ch_s);

#line 705 "D:/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }

#line 713 "D:/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 681 "D:/zenc/std/string.zc"
    if (__z_drop_flag_result) String__Drop_glue(&result);
_z_ret_mv; });

#line 681 "D:/zenc/std/string.zc"
    if (__z_drop_flag_result) String__Drop_glue(&result);
    }
}

#line 18 "D:/zenc/std/string.zc"

Option_int32_t StringCharsIter__next(StringCharsIter* self)
{
    {

#line 19 "D:/zenc/std/string.zc"
if ((self->pos >= self->len))     {

#line 19 "D:/zenc/std/string.zc"
    return 
#line 19 "D:/zenc/std/string.zc"
Option_int32_t__None();
    }

#line 21 "D:/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(self->data[self->pos]));

#line 22 "D:/zenc/std/string.zc"
    size_t seq = 
#line 22 "D:/zenc/std/string.zc"
String___utf8_seq_len(((char)(c)));

#line 24 "D:/zenc/std/string.zc"
if (((self->pos + seq) > self->len))     {

#line 25 "D:/zenc/std/string.zc"
(self->pos = self->len);

#line 26 "D:/zenc/std/string.zc"
    return 
#line 26 "D:/zenc/std/string.zc"
Option_int32_t__None();
    }

#line 29 "D:/zenc/std/string.zc"
    int32_t val = 0;

#line 30 "D:/zenc/std/string.zc"
if ((seq == 1))     {

#line 31 "D:/zenc/std/string.zc"
(val = ((int32_t)(c)));
    }

#line 32 "D:/zenc/std/string.zc"
 else if ((seq == 2))     {

#line 33 "D:/zenc/std/string.zc"
(val = ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(self->data[(self->pos + 1)])))) & 63)))));
    }

#line 34 "D:/zenc/std/string.zc"
 else if ((seq == 3))     {

#line 35 "D:/zenc/std/string.zc"
(val = ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(self->data[(self->pos + 1)])))) & 63) << 6)) | (((int32_t)(((uint8_t)(self->data[(self->pos + 2)])))) & 63)))));
    }

#line 36 "D:/zenc/std/string.zc"
 else if ((seq == 4))     {

#line 37 "D:/zenc/std/string.zc"
(val = ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(self->data[(self->pos + 1)])))) & 63) << 12)) | ((((int32_t)(((uint8_t)(self->data[(self->pos + 2)])))) & 63) << 6)) | (((int32_t)(((uint8_t)(self->data[(self->pos + 3)])))) & 63)))));
    }

#line 40 "D:/zenc/std/string.zc"
(self->pos = (self->pos + seq));

#line 41 "D:/zenc/std/string.zc"
    return 
#line 41 "D:/zenc/std/string.zc"
Option_int32_t__Some(val);
    }
}

#line 44 "D:/zenc/std/string.zc"

StringCharsIter StringCharsIter__iterator(StringCharsIter* self)
{
    {

#line 45 "D:/zenc/std/string.zc"
    return (*self);
    }
}
