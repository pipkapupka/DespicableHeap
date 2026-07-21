#pragma once

namespace DH::Test {
    struct TrivialData {
        int x;
        float y;
    };

    struct SpyObject {
        static inline int default_constructions = 0;
        static inline int copy_constructions = 0;
        static inline int move_constructions = 0;
        static inline int destructions = 0;
        static inline int copy_assignments = 0;
        static inline int move_assignments = 0;

        static void reset() {
            default_constructions = 0;
            copy_constructions = 0;
            move_constructions = 0;
            destructions = 0;
            copy_assignments = 0;
            move_assignments = 0;
        }

        int object_id = 0;

        // ----------- Rule of Five --------------- //
        explicit SpyObject(const int init_id = 0) : object_id(init_id) {
            default_constructions++;
        }

        ~SpyObject() {
            destructions++;
        }

        SpyObject(const SpyObject& other) : object_id(other.object_id) {
            copy_constructions++;
        }

        SpyObject& operator=(const SpyObject& other) {
            if (this == &other) return *this;
            object_id = other.object_id;
            copy_assignments++;
            return *this;
        }

        SpyObject(SpyObject&& other) noexcept : object_id(other.object_id) {
            move_constructions++;
            other.object_id = -1;
        }

        SpyObject& operator=(SpyObject&& other) noexcept {
            if (this == &other) return *this;
            object_id = other.object_id;
            other.object_id = -1;
            move_assignments++;
            return *this;
        }
        // ---------------------------------------- //
    };
}