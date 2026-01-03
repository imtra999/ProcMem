# ProcMem
A lightweight C++ library for reading and writing memory of Windows processes for learning and debugging.

## Usage Example
```cpp
#include "procmem.h"

int main() {
	// Offsets
	constexpr std::ptrdiff_t Offset = 0x24;

	// Open process by name
	auto mem = Memory{ "notepad.exe" };

    // Get base address of module
    const auto base = mem.GetModuleAddress("notepad.dll");

    // Example: read from memory
    auto valueAddress = mem.Read<std::uintptr_t>(base + Offset);

    INT32 value = mem.Read<INT32>(valueAddress);

    // Example: write to memory
    mem.Write<INT32>(valueAddress, 16);
}
```
