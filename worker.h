#include <filesystem>
#include <string>

enum work_type {original, converted, both}; 

class Worker {
private:
    const std::filesystem::path src;
    const std::filesystem::path dest;

public:
    Worker(const std::filesystem::path &src, const std::filesystem::path &dest) : 
        src(src), dest(dest) {}

    bool sync_photos(work_type, bool, bool);

    bool rename_photos(const std::string &, work_type, bool, bool);

    bool rename_folder(const std::string &, work_type);
};