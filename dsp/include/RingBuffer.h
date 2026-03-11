#pragma once

// https://sartech.substack.com/p/spsc-queue-part-1-ditch-the-lock
// https://www.reddit.com/r/cpp/comments/1ivlv7e/building_a_fast_spsc_queue_atomics_memory/

#include <cassert>
#include <atomic>
#include <array>
#include <iostream>


namespace ModalSynth {

template<typename T, std::size_t size>
class RingBuffer
{
    // Make sure size is a power of two
    static_assert((size != 0) && ((size & (size - 1)) == 0), 
            "Size must be a power of two!\n");

private:
    std::atomic<std::size_t> m_readIndex{};
    std::atomic<std::size_t> m_writeIndex{};
    std::array<T, size> m_buffer;
    std::size_t m_mask{size - 1};

public:
    bool push(const T& item)
    {
        std::size_t writeIndex = m_writeIndex.load();
        std::size_t nextWriteIndex = (writeIndex + 1) & m_mask;

        if (nextWriteIndex == m_readIndex)
            return false;

        m_buffer[writeIndex] = item;
        m_writeIndex = nextWriteIndex;
        return true;
    }

    bool pop(T& item)
    {
        std::size_t readIndex = m_readIndex.load();
        if (readIndex == m_writeIndex)
            return false;

        item = m_buffer[m_readIndex];
        m_readIndex = (readIndex + 1) & m_mask;
        return true;
    }
};
};
