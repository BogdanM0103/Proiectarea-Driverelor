/*

Sã se creeze o aplica?ie în C sau C++ care cite?te de la intrarea standard un numãr N 
si alocã N pagini folosind apelul sistem VirtualAlloc(https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc) . 
De asemenea, sã se afi?eze dimensiunea unei pagini.

*/
#include <iostream>
#include <windows.h>

int main() {
    // Get system page size
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    SIZE_T pageSize = sysInfo.dwPageSize;
    std::cout << "Page size: " << pageSize << " bytes" << std::endl;

    // Read number of pages to allocate
    int N;
    std::cout << "Enter number of pages to allocate: ";
    std::cin >> N;

    if (N <= 0) {
        std::cerr << "Invalid number of pages." << std::endl;
        return 1;
    }

    // Allocate memory using VirtualAlloc
    void* allocatedMemory = VirtualAlloc(nullptr, N * pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (allocatedMemory == nullptr) {
        std::cerr << "Memory allocation failed." << std::endl;
        return 1;
    }

    std::cout << "Successfully allocated " << N << " pages (" << (N * pageSize) << " bytes) at address: " << allocatedMemory << std::endl;

    // Free allocated memory
    if (VirtualFree(allocatedMemory, 0, MEM_RELEASE)) {
        std::cout << "Memory successfully freed." << std::endl;
    }
    else {
        std::cerr << "Memory deallocation failed." << std::endl;
    }

    return 0;
}
