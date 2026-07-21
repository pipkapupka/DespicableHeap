#pragma once

#include "Config.hpp"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <type_traits>

namespace DH {
    template<typename ElementType, size_t Capacity>
    class StaticVector {
        alignas(ElementType) std::byte data_[sizeof(ElementType) * Capacity] = {};
        size_t size_ = 0;

    public:
        using reference = ElementType&;
        using const_reference = const ElementType&;
        using pointer = ElementType*;
        using const_pointer = const ElementType*;

        // ----------- Rule of Five --------------- //
        /**
         * @brief Constructor
         * @example StaticVector<int, 1000> vec1();
         */
        StaticVector() = default;

        /**
         * @brief Destructor
         * @example Out of scope
         */
        ~StaticVector() {
            clear();
        }

        /**
         * @brief Copy Constructor
         * @example StaticVector<int, 1000> vec2 = vec1;
         */
        StaticVector(const StaticVector& other) {
            if constexpr (std::is_trivially_copyable_v<ElementType>) {
                std::memcpy(this->data_, other.data_, other.size_ * sizeof(ElementType));
            } else {
                auto src = reinterpret_cast<const_pointer>(other.data_);
                auto dest = reinterpret_cast<pointer>(this->data_);
                for (size_t i = 0; i < other.size_; ++i) {
                    new (dest + i) ElementType(src[i]);
                }
            }
            this->size_ = other.size_;
        }

        /**
         * @brief Copy Assignment Operator
         * @example vec2 = vec1;
         */
        StaticVector& operator=(const StaticVector& other) {
            if (this == &other) {
                return *this;
            }

            this->clear();

            if constexpr (std::is_trivially_copyable_v<ElementType>) {
                std::memcpy(this->data_, other.data_, other.size_ * sizeof(ElementType));
            } else {
                auto src = reinterpret_cast<const_pointer>(other.data_);
                auto dest = reinterpret_cast<pointer>(this->data_);
                for (size_t i = 0; i < other.size_; ++i) {
                    new (dest + i) ElementType(src[i]);
                }
            }

            this->size_ = other.size_;
            return *this;
        }

        /**
         * @brief Move Constructor
         * @example StaticVector<int, 1000> vec2 = std::move(vec1);
         */
        StaticVector(StaticVector&& other) noexcept {
            if constexpr (std::is_trivially_copyable_v<ElementType>) {
                std::memcpy(this->data_, other.data_, other.size_ * sizeof(ElementType));
            } else {
                auto src = reinterpret_cast<pointer>(other.data_);
                auto dest = reinterpret_cast<pointer>(this->data_);
                for (size_t i = 0; i < other.size_; ++i) {
                    new (dest + i) ElementType(std::move(src[i]));
                }
            }

            this->size_ = other.size_;
            other.size_ = 0;
        }

        /**
         * @brief Move Assignment Operator
         * @example vec2 = std::move(vec1);
         */
        StaticVector& operator=(StaticVector&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->clear();

            if constexpr (std::is_trivially_copyable_v<ElementType>) {
                std::memcpy(this->data_, other.data_, other.size_ * sizeof(ElementType));
            } else {
                auto src = reinterpret_cast<pointer>(other.data_);
                auto dest = reinterpret_cast<pointer>(this->data_);
                for (size_t i = 0; i < other.size_; ++i) {
                    new (dest + i) ElementType(std::move(src[i]));
                }
            }

            this->size_ = other.size_;
            other.size_ = 0;

            return *this;
        }
        // ---------------------------------------- //

        // ------------ State Observers ----------- //
        [[nodiscard]] bool empty() const {
            return size_ == 0;
        }

        [[nodiscard]] size_t size() const {
            return size_;
        }

        static constexpr size_t capacity() {
            return Capacity;
        }

        [[nodiscard]] bool full() const {
            return size_ == Capacity;
        }
        // ---------------------------------------- //

        // ------ Random Access (Memories) -------- //
        // ------- Daft Punk Reference XD  -------- //
        reference operator[](const size_t index) {
            DH_ASSERT(index >= size_);
            return *reinterpret_cast<pointer>(data_ + index * sizeof(ElementType));
        }

        const_reference operator[](const size_t index) const {
            DH_ASSERT(index >= size_);
            return *reinterpret_cast<const_pointer>(data_ + index * sizeof(ElementType));
        }

        /**
         * @brief Something like default vector `at()` method without exception banger.
         * @param index Index of object.
         * @return Object or nullptr if there is nothing at given index.
         */
        reference at(const size_t index) {
            DH_ASSERT(index >= size_);
            return *reinterpret_cast<pointer>(data_ + index * sizeof(ElementType));
        }

        const_reference at(const size_t index) const {
            DH_ASSERT(index >= size_);
            return *reinterpret_cast<const_pointer>(data_ + index * sizeof(ElementType));
        }

        reference front() {
            return *reinterpret_cast<pointer>(data_);
        }

        const_reference front() const {
            return *reinterpret_cast<const_pointer>(data_);
        }

        reference back() {
            return *reinterpret_cast<pointer>(data_ + size_ * sizeof(ElementType));
        }

        const_reference back() const {
            return *reinterpret_cast<const_pointer>(data_ + size_ * sizeof(ElementType));
        }
        // ---------------------------------------- //

        // -------------- Iterators --------------- //
        pointer begin() {
            return reinterpret_cast<pointer>(data_);
        }

        pointer end() {
            return reinterpret_cast<pointer>(data_) + size_;
        }

        const_pointer begin() const {
            return reinterpret_cast<const_pointer>(data_);
        }

        const_pointer end() const {
            return reinterpret_cast<const_pointer>(data_) + size_;
        }

        const_pointer cbegin() const {
            return this->begin();
        }

        const_pointer cend() const {
            return this->end();
        }
        // ---------------------------------------- //

        // ------------ C-API Bridge -------------- //
        pointer data() {
            return reinterpret_cast<pointer>(data_);
        }

        const_pointer data() const {
            return reinterpret_cast<const_pointer>(data_);
        }
        // ---------------------------------------- //

        // --------- Size Management -------------- //
        void resize(size_t new_size) {

        }

        void push_back(const ElementType& element) {
            new (reinterpret_cast<pointer>(data_) + size_) ElementType(element);
            size_++;
        }

        void push_back(ElementType&& element) {
            new (reinterpret_cast<pointer>(data_) + size_) ElementType(std::move(element));
            size_++;
        }

        template<typename... Args>
        void emplace_back(Args&&... args) {
            new (reinterpret_cast<pointer>(data_) + size_) ElementType(std::forward<Args>(args)...);
            size_++;
        }

        void pop_back() {
            if (this->empty()) {
                return;
            }
            reinterpret_cast<pointer>(data_)[--size_].~ElementType();
        }

        void erase(size_t index) {
            if (index != size_ - 1) {
                auto ptr_to_element = reinterpret_cast<pointer>(data_);
                ptr_to_element[index] = std::move(ptr_to_element[size_ - 1]);
            }
            this->pop_back();
        }

        void clear() {
            if constexpr (std::is_trivially_destructible_v<ElementType>) {
                size_ = 0;
            } else {
                auto typed_ptr = reinterpret_cast<pointer>(data_);
                for (size_t i = 0; i < size_; ++i) {
                    typed_ptr[i].~ElementType();
                }
                size_ = 0;
            }
        }
        // ---------------------------------------- //
    };
}