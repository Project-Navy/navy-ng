#pragma once 

void simd_init(void);
void simd_ctx_init(void *ptr);
void simd_ctx_save(void *ptr);
void simd_ctx_load(void *ptr);