#pragma once
#include <string>
#include <vector>

class Repository {
public:
    explicit Repository(const std::string& rootPath);

    bool isInitialized() const;
    void init();
    void trackFile(const std::string& filePath);
    void save(const std::string& message);
    void showHistory();
    void goToCommit(const std::string& commitHash);
    void diff();
   
private:
    std::string root;
    std::string vcsDir;
    std::string indexFile;

    std::string createBlob(const std::string& filePath);

    std::string readObject(const std::string& path);
    std::string resolveCommitHash(const std::string& prefix);
    std::vector<std::string> splitLines(const std::string& content);
   
};
