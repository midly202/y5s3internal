#include <exception>
#include <type_traits>
#include <Windows.h>

class ptr_invalid : std::exception {
    uintptr_t ptr;

public:
    ptr_invalid(uintptr_t ptr) : std::exception("A pointer was invalid."), ptr(ptr) {}

    uintptr_t what_ptr() const noexcept { return ptr; }
};

template<typename T>
    requires(!std::is_same<void, T>::value) // Remove this line if you do not compile with /std:c++20
T read(uintptr_t address) {
    if (!address) throw ptr_invalid(address);
    return *reinterpret_cast<T*>(address);
}

template<typename T>
    requires(!std::is_same<void, T>::value) // Remove this line if you do not compile with /std:c++20
T read_seh(uintptr_t address) {
    if (!address) throw ptr_invalid(address);

    __try {
        return *reinterpret_cast<T*>(address);
    }
    __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        throw ptr_invalid(address);
    }
}

template<typename T>
    requires(!std::is_same<void, T>::value) // Remove this line if you do not compile with /std:c++20
void write(uintptr_t address, T value) {
    if (!address) throw ptr_invalid(address);

    *reinterpret_cast<T*>(address) = value;
}

template<typename T>
    requires(!std::is_same<void, T>::value) // Remove this line if you do not compile with /std:c++20
void write_seh(uintptr_t address, T value) {
    if (!address) throw ptr_invalid(address);

    __try {
        *reinterpret_cast<T*>(address) = value;
    }
    __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        throw ptr_invalid(address);
    }
}