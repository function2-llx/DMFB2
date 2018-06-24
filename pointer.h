// 本文件中提供了一些方法，用以检测指针是否被释放。
// 你不需要在意这个文件中的代码。
#include <cstdlib>

using namespace std;

const int MAX_RECORDS = 2000;
struct pointer_record {
	uintptr_t ptr;
	bool array, released;
} records[MAX_RECORDS];
int n_records = 0;

void add_record(void *ptr, bool array) {
	auto addr = reinterpret_cast<uintptr_t>(ptr);
	int idx = -1;
	for (int i = 0; i < n_records; ++i)
		if (records[i].ptr == addr) {
			idx = i;
			break;
		}
	if (idx == -1) idx = n_records++;
	records[idx] = {addr, array, false};
}

pointer_record &find_record(void *ptr) {
	auto addr = reinterpret_cast<uintptr_t>(ptr);
	for (int i = 0; i < n_records; ++i)
		if (records[i].ptr == addr) return records[i];
	throw runtime_error("Trying to free a pointer that wasn't allocated");
}

void *operator new(size_t sz) {
	void *ptr = malloc(sz);
	// cerr << "new " << sz << " " << hex << ptr << dec << endl;
	add_record(ptr, false);
	return ptr;
}

void *operator new[](size_t sz) {
	void *ptr = malloc(sz);
	// cerr << "new[] " << sz << " " << hex << ptr << dec << endl;
	add_record(ptr, true);
	return ptr;
}

void operator delete(void *ptr) noexcept {
	// cerr << "delete " << hex << ptr << dec << endl;
	if (ptr == nullptr) return;
	pointer_record &record = find_record(ptr);
	if (record.array)
		throw runtime_error("Trying to free an array pointer with `delete`");
	if (record.released)
		throw runtime_error("Trying to free a pointer that was already deallocated");
	free(ptr);
	record.released = true;
}

void operator delete[](void *ptr) noexcept {
	// cerr << "delete[] " << hex << ptr << dec << endl;
	if (ptr == nullptr) return;
	pointer_record &record = find_record(ptr);
	if (!record.array)
		throw runtime_error("Trying to free a non-array pointer with `delete[]`");
	if (record.released)
		throw runtime_error("Trying to free a pointer that was already deallocated");
	free(ptr);
	record.released = true;
}
