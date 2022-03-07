#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <filesystem>
#include <string>
#include <vector>

enum work_type {original, converted, both}; 

class Worker {
private:
    const std::filesystem::path src;
    const std::filesystem::path dest;

public:
    std::string raw_suffix = ".CR2";
    std::string converted_suffix = ".jpg";

    Worker(const std::filesystem::path &src, const std::filesystem::path &dest) : 
        src(src), dest(dest) {}

    bool sync_photos(work_type, bool, bool);

    bool rename_photos(const std::string &, work_type, bool, bool);

    bool rename_folder(const std::string &, work_type);

    void read_raw_files(std::vector<std::string> &);
    
    void read_converted_files(std::vector<std::string> & files);

    static void read_files(std::vector<std::string> &, const std::string &, const std::filesystem::path &);
};

#endif // WORKER_H_INCLUDED