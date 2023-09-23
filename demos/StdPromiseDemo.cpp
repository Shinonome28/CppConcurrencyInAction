#include "CommonUtils.h"
#include <cstdio>
#include <future>

std::future<void> ReadFileAsync(const char *filename, std::promise<std::shared_ptr<char[]>> *promise)
{
	using namespace std;

	return async(launch::async, [=]()
				 {
					 FILE *file;
					 fopen_s(&file, filename, "r");
					 if (file == 0)
					 {
						 promise->set_exception(make_exception_ptr(exception("can not open size")));
						 return;
					 }
					 if (fseek(file, 0, SEEK_END))
					 {
						 promise->set_exception(make_exception_ptr(exception("can not get file size")));
						 return;
					 }
					 size_t size = ftell(file);
					 rewind(file);
					 std::shared_ptr<char[]> buffer(new char[size + 1]);
					 size_t bytes_read = fread(buffer.get(), sizeof(char), size, file);
					 if (ferror(file))
					 {
						 promise->set_exception(make_exception_ptr(exception("read error")));
						 return;
					 }
					 buffer[bytes_read] = '\n';
					 if (fclose(file))
					 {
						 promise->set_exception(make_exception_ptr(exception("can not read")));
						 return;
					 }
					 promise->set_value(buffer); });
}

void StdPromiseDemo_()
{
	using namespace std;

	promise<shared_ptr<char[]>> promise;
	// MODIFY THIS
	future<void> future = ReadFileAsync("C:\\Users\\shinonome\\Desktop\\test.txt", &promise);
	try
	{
		future.get();
		shared_ptr<char[]> buffer = promise.get_future().get();
		printf("%s", buffer.get());
	}
	catch (exception &e)
	{
		printf("%s", e.what());
	}
	fflush(stdout);
}

void main()
{
	MeasureRuntime(StdPromiseDemo_);
}
