//#ifndef HIGH_PERF_QUEUE_H
//#define HIGH_PERF_QUEUE_H

//#include <stdbool.h>
//#include <stddef.h>
//#include <string.h>

//// 计算不小于size的最小2的幂次方
//#define NEXT_POWER_OF_2(size) \
//    ((size) <= 1 ? 1 : \
//     (size) <= 2 ? 2 : \
//     (size) <= 4 ? 4 : \
//     (size) <= 8 ? 8 : \
//     (size) <= 16 ? 16 : \
//     (size) <= 32 ? 32 : \
//     (size) <= 64 ? 64 : \
//     (size) <= 128 ? 128 : \
//     (size) <= 256 ? 256 : \
//     (size) <= 512 ? 512 : \
//     (size) <= 1024 ? 1024 : \
//     1024)

//// 定义队列结构
//#define GENERIC_QUEUE(NAME, TYPE, SIZE) \
//typedef struct { \
//    TYPE data[NEXT_POWER_OF_2(SIZE)]; \
//    size_t front; \
//    size_t rear; \
//    size_t count; \
//    size_t mask; \
//}NAME##_queue_t;

//// 初始化队列
//#define QUEUE_INIT(TYPE, SIZE) \
//static inline void NAME##_queue_init(NAME##_queue_t* q) { \
//    q->front = 0; \
//    q->rear = 0; \
//    q->count = 0; \
//    q->mask = NEXT_POWER_OF_2(SIZE) - 1; \
//}

//// 检查队列是否为空
//#define QUEUE_EMPTY(TYPE) \
//static inline bool NAME##_queue_empty(const TYPE##_queue_t* q) { \
//    return q->count == 0; \
//}

//// 检查队列是否已满
//#define QUEUE_FULL(TYPE) \
//static inline bool TYPE##_queue_full(const TYPE##_queue_t* q) { \
//    return q->count == (q->mask + 1); \
//}

//// 获取队列元素数量
//#define QUEUE_SIZE(TYPE) \
//static inline size_t TYPE##_queue_size(const TYPE##_queue_t* q) { \
//    return q->count; \
//}

//// 入队操作（分支预测优化）
//#define QUEUE_ENQUEUE(TYPE) \
//static inline bool __attribute__((always_inline)) \
//TYPE##_queue_enqueue(TYPE##_queue_t* q, const TYPE* value) { \
//    if (__builtin_expect(q->count == (q->mask + 1), 0)) \
//        return false; \
//    memcpy(&q->data[q->rear], value, sizeof(TYPE)); \
//    q->rear = (q->rear + 1) & q->mask; \
//    q->count++; \
//    return true; \
//}

//// 查看队首元素
//#define QUEUE_FRONT_CONST(TYPE) \
//static inline const TYPE* __attribute__((always_inline)) \
//TYPE##_queue_front_const(const TYPE##_queue_t* q) { \
//    return __builtin_expect(q->count == 0, 0) ? NULL : &q->data[q->front]; \
//}

////// 查看队首元素
//#define QUEUE_FRONT(TYPE) \
//static inline TYPE* __attribute__((always_inline)) \
//TYPE##_queue_front(TYPE##_queue_t* q) { \
//    return __builtin_expect(q->count == 0, 0) ? NULL : &q->data[q->front]; \
//}

//// 出队操作（分支预测优化）
//#define QUEUE_DEQUEUE(TYPE) \
//static inline bool __attribute__((always_inline)) \
//TYPE##_queue_dequeue(TYPE##_queue_t* q, TYPE* value) { \
//    if (__builtin_expect(q->count == 0, 0)) \
//        return false; \
//    if (value) \
//        memcpy(value, &q->data[q->front], sizeof(TYPE)); \
//    q->front = (q->front + 1) & q->mask; \
//    q->count--; \
//    return true; \
//}

//// 批量出队操作
//#define QUEUE_DEQUEUE_BATCH(TYPE) \
//static inline size_t __attribute__((always_inline)) \
//TYPE##_queue_dequeue_batch(TYPE##_queue_t* q, TYPE* values, size_t n) { \
//    size_t actual = q->count < n ? q->count : n; \
//    if (__builtin_expect(actual == 0, 0)) \
//        return 0; \
//    \
//    size_t capacity = q->mask + 1; \
//    if (q->front + actual <= capacity) { \
//        if (values) \
//            memcpy(values, &q->data[q->front], actual * sizeof(TYPE)); \
//    } else { \
//        size_t first_part = capacity - q->front; \
//        if (values) { \
//            memcpy(values, &q->data[q->front], first_part * sizeof(TYPE)); \
//            memcpy(values + first_part, &q->data[0], (actual - first_part) * sizeof(TYPE)); \
//        } \
//    } \
//    \
//    q->front = (q->front + actual) & q->mask; \
//    q->count -= actual; \
//    return actual; \
//}
////批量入队
//#define QUEUE_ENQUEUE_BATCH(TYPE) \
//static inline bool __attribute__((always_inline)) \
//TYPE##_enqueue_batch(TYPE##_queue_t* q, const TYPE* values, size_t n) { \
//    if (__builtin_expect((q->count + n) > (q->mask + 1), 0)) \
//        return false; \
//    \
//    size_t capacity = q->mask + 1; \
//    if (q->rear + n <= capacity) { \
//        memcpy(&q->data[q->rear], values, n * sizeof(TYPE)); \
//    } else { \
//        size_t first_part = capacity - q->rear; \
//        memcpy(&q->data[q->rear], values, first_part * sizeof(TYPE)); \
//        memcpy(&q->data[0], values + first_part, (n - first_part) * sizeof(TYPE)); \
//    } \
//    \
//    q->rear = (q->rear + n) & q->mask; \
//    q->count += n; \
//    return true; \
//}


//#endif // HIGH_PERF_QUEUE_H


#ifndef HIGH_PERF_QUEUE_H
#define HIGH_PERF_QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define NEXT_POW2(size) \
    ((size) <= 1 ? 1 : \
     (size) <= 2 ? 2 : \
     (size) <= 4 ? 4 : \
     (size) <= 8 ? 8 : \
     (size) <= 16 ? 16 : \
     (size) <= 32 ? 32 : \
     (size) <= 64 ? 64 : \
     (size) <= 128 ? 128 : \
     (size) <= 256 ? 256 : \
     (size) <= 512 ? 512 : \
     1024)

// 核心队列宏
#define GENERIC_QUEUE(NAME, TYPE, SIZE) \
typedef struct { \
    TYPE data[NEXT_POW2(SIZE)]; \
    size_t front, rear, count; \
    size_t mask; \
} NAME##_t; \
\
static inline void NAME##_init(NAME##_t* q) { \
    q->front = q->rear = q->count = 0; \
    *(size_t*)&q->mask = NEXT_POW2(SIZE) - 1; \
} \
\
static inline bool NAME##_empty(const NAME##_t* q) { \
    return !q->count; \
} \
\
static inline bool NAME##_full(const NAME##_t* q) { \
    return q->count == (q->mask + 1); \
} \
\
static inline size_t NAME##_capacity(const NAME##_t* q) { \
    return q->mask + 1; \
} \
\
/* 单元素入队 */ \
static inline bool __attribute__((always_inline, hot)) \
NAME##_enqueue(NAME##_t* q, const TYPE* val) { \
    if (__builtin_expect(NAME##_full(q), 0)) return false; \
    __builtin_prefetch(&q->data[q->rear], 1, 3); \
    memcpy(&q->data[q->rear], val, sizeof(TYPE)); \
    q->rear = (q->rear + 1) & q->mask; \
    q->count++; \
    return true; \
} \
\
/* 批量入队*/ \
static inline bool __attribute__((always_inline, hot)) \
NAME##_enqueue_batch(NAME##_t* q, const TYPE* batch, size_t n) { \
    if (__builtin_expect(n == 0, 0)) return true; \
    size_t free_space = NAME##_capacity(q) - q->count; \
    if (__builtin_expect(n > free_space, 0)) return false; \
    \
    size_t mask = q->mask; \
    size_t rear = q->rear; \
    \
    /* 处理缓冲区环绕 */ \
    if (rear + n <= mask + 1) { \
        memcpy(&q->data[rear], batch, n * sizeof(TYPE)); \
    } else { \
        size_t first_part = mask + 1 - rear; \
        memcpy(&q->data[rear], batch, first_part * sizeof(TYPE)); \
        memcpy(q->data, batch + first_part, (n - first_part) * sizeof(TYPE)); \
    } \
    \
    q->rear = (rear + n) & mask; \
    q->count += n; \
    return true; \
} \
\
/* 单元素出队 */ \
static inline bool __attribute__((always_inline, hot)) \
NAME##_dequeue(NAME##_t* q, TYPE* val) { \
    if (__builtin_expect(NAME##_empty(q), 0)) return false; \
    if (val) memcpy(val, &q->data[q->front], sizeof(TYPE)); \
    q->front = (q->front + 1) & q->mask; \
    q->count--; \
    return true; \
} \
\
/* 批量出队 */ \
static inline size_t __attribute__((always_inline, hot)) \
NAME##_dequeue_batch(NAME##_t* q, TYPE* buf, size_t n) { \
    size_t cnt = q->count; \
    if (__builtin_expect(n == 0 || cnt == 0, 0)) return 0; \
    n = n < cnt ? n : cnt; \
    size_t mask = q->mask; \
    size_t idx = q->front; \
    \
    if (idx + n <= mask + 1) { \
        if (buf) memcpy(buf, &q->data[idx], n * sizeof(TYPE)); \
    } else { \
        size_t first = mask + 1 - idx; \
        if (buf) { \
            memcpy(buf, &q->data[idx], first * sizeof(TYPE)); \
            memcpy(buf + first, q->data, (n - first) * sizeof(TYPE)); \
        } \
    } \
    \
    q->front = (idx + n) & mask; \
    q->count = cnt - n; \
    return n; \
}

#endif // HIGH_PERF_QUEUE_H

