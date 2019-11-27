#include "queue.hpp"

#include <cassert>
#include <string>
#include <list>
#include <stdexcept>

static void test_queue() {
    {
        cls_09::queue<int> q;
    }

    {
        cls_09::queue<std::string> initial;
        cls_09::queue<std::string> const other(initial);

        initial.push("1");
        assert(initial.size() == 1 && other.size() == 0);

        initial = static_cast<cls_09::queue<std::string> const>(initial);
        assert(initial.size() == 1 && other.size() == 0);

        initial = other;
        assert(initial.size() == 0 && other.size() == 0);
    }

    {
        cls_09::queue<int> queue;
        cls_09::queue<int> const& ref = queue;

        auto check_queue_state = [&](size_t size, int front) {
            assert(queue.size() == size && queue.front() == front); \
            assert(ref.size() == size && ref.front() == front);
        };

        queue.push(0);
        check_queue_state(1, 0);

        queue.push(1);
        check_queue_state(2, 0);

        queue.push(2);
        check_queue_state(3, 0);

        queue.pop();
        check_queue_state(2, 1);

#undef CHECK_QUEUE_STATE
    }

    {
        cls_09::queue<int> queue;
        std::list<int> original;

        auto push_to_both = [&](int i) {
            queue.push(i);
            original.push_back(i);
        };

        auto pop_from_both = [&]() {
            if (queue.size() != 0) {
                queue.pop();
            }

            if (!original.empty()) {
                original.pop_front();
            }
        };

        auto assert_consistent = [&]() {
            assert(queue.size() == original.size());
            if (queue.size() != 0) {
                assert(queue.front() == original.front());
            }
        };

        for (int i = 0; i < 1024; ++i) {
            if (i % 3 != 0) {
                push_to_both(i);
            } else {
                pop_from_both();
            }

            assert_consistent();
        }
    }

    {
        static size_t counter{0};

        struct throws_on_copy {
            throws_on_copy() = default;
            throws_on_copy(throws_on_copy const& other) {
                if (++counter % 3 == 0) {
                    throw std::runtime_error("Exception from copy constructor");
                }
            }
        };

        cls_09::queue<throws_on_copy> queue;

        for (int i = 0; i < 100; ++i) {
            throws_on_copy element;

            size_t previous_size = queue.size();
            try {
                queue.push(element);
            } catch (std::runtime_error const&) {
                assert(previous_size == queue.size());
            }
        }
    }
}

int main() {
    test_queue();
}