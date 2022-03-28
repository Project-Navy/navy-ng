#pragma once

#define DECLARE_LOCK(name) volatile int name ## _locked
#define LOCK(name) \
	while (!__sync_bool_compare_and_swap(& name ## _locked, 0, 1)); \
	__sync_synchronize();
#define UNLOCK(name) \
	__sync_synchronize(); \
	name ## _locked = 0;