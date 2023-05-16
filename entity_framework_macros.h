#ifndef ENTITY_FRAMEWORK_DEF_H
#define ENTITY_FRAMEWORK_DEF_H

#if !(defined(__unix__) || defined(__APPLE__) || defined(_WIN32))
    #error "Unsupported platform"
#endif

#if defined(__unix__) || defined(__APPLE__)
    #define ENTITY_FRAMEWORK_IS_UNIX 1
    #define ENTITY_FRAMEWORK_IS_WINDOWS 0
#elif defined(_WIN32)
    #define ENTITY_FRAMEWORK_IS_UNIX 0
    #define ENTITY_FRAMEWORK_IS_WINDOWS 1
#endif

#if defined(ENTITY_FRAMEWORK_DEBUG)
    #define ENTITY_FRAMEWORK_CHECK(code) do { code } while (0)
#else
    #define ENTITY_FRAMEWORK_CHECK(code) do { } while (0)
#endif

#if defined(ENTITY_FRAMEWORK_ALLOC_DEBUG)
    #define ENTITY_FRAMEWORK_ALLOC_CHECK(code) do { code } while (0)
#else
    #define ENTITY_FRAMEWORK_ALLOC_CHECK(code) do { } while (0)
#endif

#ifndef ENTITY_ALIGNMENT
    #define ENTITY_ALIGNMENT 32
#endif

#define ENTITY_ALIGN(size) (((size) + (ENTITY_ALIGNMENT) - 1) & ~((ENTITY_ALIGNMENT) - 1))

#define _ENTITY_STRUCT_DECL(n, t) typedef struct n t;

#if ENTITY_FRAMEWORK_IS_UNIX == 1
    #define ENTITY_STRUCT_DECL(name, _typename, body) \
        _ENTITY_STRUCT_DECL(name, _typename) \
        struct name body __attribute__((aligned(ENTITY_ALIGNMENT)))
#else
    #define ENTITY_STRUCT_DECL(name, _typename, body) \
        _ENTITY_STRUCT_DECL(name, _typename) \
        __declspec(align(ENTITY_ALIGNMENT)) struct name body
#endif

#if ENTITY_FRAMEWORK_IS_UNIX == 1
    #define ENTITY_ALIGNED_VAR(type, name) type name __attribute__((aligned(ENTITY_ALIGNMENT)))
#else
    #define ENTITY_ALIGNED_VAR(type, name) __declspec(align(ENTITY_ALIGNMENT)) type name
#endif

#if defined(ENTITY_FRAMEWORK_ALLOW_UNALIGNED_REALLOC)
    #if ENTITY_FRAMEWORK_IS_UNIX == 1
        #warn "Unaligned realloc is enabled - this may cause memory leaks, memory slow down, and/or corruption"
    #else
        #warn "Unaligned realloc is enabled but should not be used on Windows"
    #endif
#endif

#endif // ENTITY_FRAMEWORK_DEF_H
