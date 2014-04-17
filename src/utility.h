#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cstdio>

namespace {

const int OpenFlags   = O_RDONLY;
const mode_t OpenMode = S_IRUSR | S_IWUSR;

}

std::string readFile(const std::string& path) {
	int descriptor(
		open(path.data(), OpenFlags, OpenMode)
	);

	if ( descriptor == -1 ) {
		close(descriptor);

		return "io error";
	} else {
		struct stat info;
		fstat(descriptor, &info);
		const std::size_t size(info.st_size);

		char* buffer(new char[size]);

		ssize_t readSize(read(
			descriptor,
			reinterpret_cast<void*>(buffer),
			size
		));

		close(descriptor);

		std::string content(
			buffer,
			readSize
		);

		delete[] buffer;

		return content;
	}
}
