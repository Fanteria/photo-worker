#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <filesystem>
#include <string>
#include <vector>
#include <set>

enum work_type {original, converted, both}; 

class Worker {
private:
    const std::filesystem::path src;
    const std::filesystem::path dest;

    static std::filesystem::path new_path(const std::filesystem::path &, const std::string &);
public:
    std::string raw_suffix = ".CR2";
    std::string converted_suffix = ".jpg";

    Worker(const std::filesystem::path &src, const std::filesystem::path &dest) : 
        src(src), dest(dest) {}

    void sync_photos(work_type, bool, bool);

    void rename_photos(const std::string &, work_type, bool, bool);

    void rename_folder(const std::string &, work_type type = original);

    void read_raw_files(std::vector<std::string> &);
    
    void read_converted_files(std::vector<std::string> &);

    static void read_files(std::vector<std::string> &, const std::string &, const std::filesystem::path &);

    static void rename_files_in_folder(const std::filesystem::path &, const std::string &, const std::string &, bool, bool, bool);
};

#endif // WORKER_H_INCLUDED