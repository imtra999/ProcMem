#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <string_view>

class Memory {
private:
	std::uintptr_t ProcessId = 0;
	HANDLE ProcessHandle = nullptr;

public:
	// Constructor finds process id and opens handle
	Memory(std::string_view ProcessName) noexcept
	{
		::PROCESSENTRY32 entry;
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (::Process32Next(snapshot, &entry)) 
		{
			if (!ProcessName.compare(entry.szExeFile)) {
				ProcessId = entry.th32ProcessID;
				ProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
				break;
			}
		}

		// Free handle
		if (snapshot) {
			::CloseHandle(snapshot);
		}
	}

	// Destructor frees the opened handle
	~Memory()
	{
		if (ProcessHandle)
			::CloseHandle(ProcessHandle);
	}

	// Returns the base address of a module by name
	std::uintptr_t GetModuleAddress(std::string_view ModuleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

		std::uintptr_t result = 0;

		while (::Module32Next(snapshot, &entry))
		{
			if (!ModuleName.compare(entry.szModule))
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (snapshot)
			::CloseHandle(snapshot);

		return result;
	}

	// Read process memory
	template <typename T>
	const T Read(const std::uintptr_t address) const noexcept
	{
		T value = { };
		::ReadProcessMemory(ProcessHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	// Write process memory
	template <typename T>
	void Write(const std::uintptr_t address, const T& value) const noexcept
	{
		::WriteProcessMemory(ProcessHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};