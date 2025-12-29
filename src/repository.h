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
    void diff(bool ignoreEmpty, bool ignoreWhitespace);
    void diffSmart(bool ignoreEmpty, bool ignoreWhitespace);

   
private:
    std::string root;
    std::string vcsDir;
    std::string indexFile;

    std::string createBlob(const std::string& filePath);

    std::string readObject(const std::string& path);
    std::string resolveCommitHash(const std::string& prefix);
    std::vector<std::string> splitLines(const std::string& content);
    
    
    std::string normalizeWhitespace(const std::string& s);
    std::string extractFunction(const std::string& line);
    bool isIgnorableLine(const std::string& line, bool ignoreEmpty, bool ignoreWhitespace);
    std::vector<std::pair<char, std::string>>
    lcsDiff(const std::vector<std::string>& a,
        const std::vector<std::string>& b);
};
