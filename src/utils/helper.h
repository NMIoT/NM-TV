#ifndef _HELPER_H
#define _HELPER_H
#include <Arduino.h>
#include "storage.h"

#define IN
#define OUT


void psram_init();

void disable_usb_uart();

void enable_usb_uart();

double diff_calculate(void *target);

int str_to_byte_array(const char *in, size_t in_size, uint8_t *out);

void hex_byte_array_to_str(const uint8_t* hexArray, size_t length, char* charArray);

bool checkValid(uint8_t* hash, uint8_t* target);

void logo_print() ;

String get_last_reboot_reason();

String formatNumber(float num, uint8_t bits);

String convert_time_to_local(uint32_t timestamp);

String convert_uptime_to_string(uint32_t timecnt);

float parseHashRateStr(const String& hashRateStr);

float parseDiffStr(const String& diffStr);

void* psramAllocator(size_t size);

void psramDeallocator(void* pointer);

int compareVersions(const String& version1, const String& version2);

template <typename T>
struct PsramAllocator {
    using value_type = T;

    PsramAllocator() = default;

    template <typename U>
    constexpr PsramAllocator(const PsramAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        T* ptr = static_cast<T*>(heap_caps_malloc(n * sizeof(T), MALLOC_CAP_SPIRAM));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    void deallocate(T* p, std::size_t) noexcept {
        heap_caps_free(p);
    }
};

template <typename T, typename U>
bool operator==(const PsramAllocator<T>&, const PsramAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const PsramAllocator<T>&, const PsramAllocator<U>&) { return false; }



#endif 
