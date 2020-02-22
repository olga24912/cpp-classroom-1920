#include <utility>

namespace cls_2 {
    template <typename T>
    class unique_ptr {
    private:
        T* value;
    public:
        unique_ptr() noexcept {
            value = nullptr;
        }

        explicit unique_ptr(T* value) : value(value) {}

        unique_ptr(const unique_ptr& /*u_ptr*/) = delete;
        unique_ptr(unique_ptr&& u_ptr) noexcept : value(u_ptr.release()) {}
        unique_ptr& operator=(const unique_ptr& /*u_ptr*/) = delete;
        unique_ptr& operator=(unique_ptr&& u_ptr) noexcept {
            delete value;
            value = u_ptr.release();
            return *this;
        }

        explicit operator bool() const {
            return value != nullptr;
        }

        T* get() const {
            return value;
        }


        T* operator -> () const {
            return value;
        }

        T& operator * () const {
            return *value;
        }

        T* release() {
            T* tmp_value = value;
            value = nullptr;
            return tmp_value;
        }

        void reset(T* new_value = nullptr) {
            delete value;
            value = new_value;
        }

        ~unique_ptr() {
            delete value;
        }
    };

    template <typename T, typename ... Args>
    unique_ptr<T> make_unique(Args&& ... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args) ...));
    }
}