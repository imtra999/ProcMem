# ProcMem
A lightweight C++ library for reading and writing memory of Windows processes for learning and debugging.

## Usage Example
```cpp
#include "procmem.h"

int main() {
	// Offsets
	constexpr std::ptrdiff_t Offset1 = 0x12;
	constexpr std::ptrdiff_t Offset2 = 0x24;

	// Open process by name
	auto mem = Memory{ "notepad.exe" };

    // Get base address of module
    const auto base = mem.GetModuleAddress("notepad.dll");

    // Example: read from memory
    auto address1 = mem.Read<std::uintptr_t>(base + Offset1);

    auto valueAddress = address1 + Offset2;

    INT32 value = mem.Read<INT32>(valueAddress);

    // Example: write to memory
    mem.Write<INT32>(valueAddress, 16);
}
```
