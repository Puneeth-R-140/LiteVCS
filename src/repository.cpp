#include "repository.h"
#include "utils.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <zlib.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <regex>

Repository::Repository(const std::string& rootPath)
    : root(rootPath),
      vcsDir(rootPath + "/.vcs"),
      indexFile(vcsDir + "/index") {}

bool Repository::isInitialized() const {
    return utils::exists(vcsDir);
}

void Repository::init() {
    if (isInitialized()) {
        std::cout << "Repository already initialized.\n";
        return;
    }

    utils::create_dir(vcsDir + "/objects/blobs");
    utils::create_dir(vcsDir + "/objects/trees");
    utils::create_dir(vcsDir + "/objects/commits");

    std::ofstream(indexFile).close();
    std::ofstream(vcsDir + "/HEAD") << "null";
    std::ofstream(vcsDir + "/config") << "version=1";

    std::cout << "Initialized empty LiteVCS repository.\n";
}

void Repository::trackFile(const std::string& filePath) {
    if (!isInitialized()) {
        std::cout << "Error: not a LiteVCS repository.\n";
        return;
    }

    std::filesystem::path fullPath = root + "/" + filePath;

    if (!utils::exists(fullPath.string())) {
        std::cout << "Error: file does not exist\n";
        return;
    }

    std::filesystem::path inputPath(filePath);
    std::filesystem::path absolute =
    std::filesystem::absolute(inputPath);

std::filesystem::path relative =
    std::filesystem::relative(absolute, root);

// normalize to generic form (uses /)
std::string normalized = relative.generic_string();

auto tracked = utils::read_lines(indexFile);
for (const auto& f : tracked) {
    if (f == normalized) {
        std::cout << "Already tracked: " << normalized << "\n";
        return;
    }
}

utils::append_line(indexFile, normalized);
std::cout << "Tracked: " << normalized << "\n";
}

void Repository::save(const std::string& message) {
    if (!isInitialized()) {
        std::cout << "Error: not a LiteVCS repository.\n";
        return;
    }
    std::ifstream idx(indexFile);
    if (!idx.peek()) {
    std::cout << "Error: no tracked files to commit.\n";
    return;
    }

    std::ifstream index(indexFile);
    std::string file;
    std::ostringstream tree;

    while (std::getline(index, file)) {
        std::string blobHash = createBlob(file);
        tree << file << " " << blobHash << "\n";
    }

    std::string treeContent = tree.str();
    std::string treeHash = utils::sha1(treeContent);

    std::string treePath = vcsDir + "/objects/trees/" + treeHash;
    if (!std::filesystem::exists(treePath)) {
    utils::write_binary(treePath, utils::compress(treeContent));
    }

    std::string parent = utils::read_file(vcsDir + "/HEAD");
    std::time_t now = std::time(nullptr);

    std::ostringstream commit;
    commit << "tree " << treeHash << "\n";
    commit << "parent " << parent << "\n";
    commit << "time " << now << "\n";
    commit << "message " << message << "\n";

    std::string commitData = commit.str();
    std::string commitHash = utils::sha1(commitData);

    std::string commitPath = vcsDir + "/objects/commits/" + commitHash;
    utils::write_binary(commitPath, utils::compress(commitData));

    utils::write_binary(vcsDir + "/HEAD", commitHash);

    std::cout << "Saved commit: " << commitHash.substr(0, 8) << "...\n";
}

std::string Repository::createBlob(const std::string& filePath) {
    std::string fullPath = root + "/" + filePath;
    std::string content = utils::read_file(fullPath);

    std::string hash = utils::sha1(content);
    std::string blobPath = vcsDir + "/objects/blobs/" + hash;

    if (!std::filesystem::exists(blobPath)) {
        utils::write_binary(blobPath, utils::compress(content));
    }

    return hash;
}

std::string Repository::readObject(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cout << "Error: object missing: " << path << "\n";
        return {};
    }

    std::vector<char> compressed((std::istreambuf_iterator<char>(in)),
                                  std::istreambuf_iterator<char>());

    // Start with a conservative cap; grow if needed
    uLongf outCap = compressed.size() * 6 + 64;
    std::vector<Bytef> out(outCap);

    int res = ::uncompress(out.data(), &outCap,
                            reinterpret_cast<Bytef*>(compressed.data()),
                            compressed.size());

    if (res != Z_OK) {
        std::cout << "Error: failed to decompress object.\n";
        return {};
    }

    return std::string(reinterpret_cast<char*>(out.data()), outCap);
}

void Repository::showHistory() {
    if (!isInitialized()) {
        std::cout << "Error: not a LiteVCS repository.\n";
        return;
    }

    std::string current = utils::read_file(vcsDir + "/HEAD");

    while (current != "null" && !current.empty()) {
        std::string path = vcsDir + "/objects/commits/" + current;
        std::string data = readObject(path);

        std::istringstream iss(data);
        std::string line, parent, message, time;

        while (std::getline(iss, line)) {
            if (line.rfind("parent", 0) == 0) parent = line.substr(7);
            if (line.rfind("time", 0) == 0) time = line.substr(5);
            if (line.rfind("message", 0) == 0) message = line.substr(8);
        }

        std::cout << "commit " << current.substr(0, 8) << "\n";
        std::cout << "Date: " << time << "\n";
        std::cout << "Message: " << message << "\n\n";

        current = parent;
    }
}

void Repository::goToCommit(const std::string& commitHash) {
   std::string resolved = resolveCommitHash(commitHash);
    if (resolved.empty()) {
    std::cout << "Error: commit hash ambiguous or not found\n";
    return;
    }
    if (resolved.empty()) {
    std::cout << "Error: commit hash ambiguous or not found\n";
    return;
    }

std::string commitPath = vcsDir + "/objects/commits/" + resolved;

    if (!std::filesystem::exists(commitPath)) {
        std::cout << "Error: commit not found\n";
        return;
    }

    std::string commitData = readObject(commitPath);
    std::istringstream iss(commitData);
    std::string line, treeHash;

    while (std::getline(iss, line)) {
        if (line.rfind("tree", 0) == 0)
            treeHash = line.substr(5);
    }

    std::string treePath = vcsDir + "/objects/trees/" + treeHash;
    std::string treeData = readObject(treePath);

    std::istringstream treeStream(treeData);
    std::string file, blobHash;

    while (treeStream >> file >> blobHash) {
        std::string blobPath = vcsDir + "/objects/blobs/" + blobHash;
        std::string content = readObject(blobPath);

        std::ofstream(root + "/" + file, std::ios::binary) << content;
    }

    utils::write_binary(vcsDir + "/HEAD", commitHash);
    std::cout << "Moved to commit " << commitHash.substr(0, 8) << "\n";
}

std::string Repository::resolveCommitHash(const std::string& prefix) {
    std::string commitsDir = vcsDir + "/objects/commits";
    std::string match;

    for (const auto& entry : std::filesystem::directory_iterator(commitsDir)) {
        std::string filename = entry.path().filename().string();

        if (filename.rfind(prefix, 0) == 0) { // starts with prefix
            if (!match.empty()) {
                return ""; // ambiguous
            }
            match = filename;
        }
    }
    return match;
}

std::vector<std::string> Repository::splitLines(const std::string& content) {
    std::istringstream iss(content);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::pair<char, std::string>>
Repository::lcsDiff(const std::vector<std::string>& a,
                    const std::vector<std::string>& b) {

    int n = static_cast<int>(a.size());
    int m = static_cast<int>(b.size());

    // DP table
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Build LCS length table
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (a[i] == b[j])
                dp[i][j] = 1 + dp[i + 1][j + 1];
            else
                dp[i][j] = std::max(dp[i + 1][j], dp[i][j + 1]);
        }
    }

    // Reconstruct diff
    std::vector<std::pair<char, std::string>> result;
    int i = 0, j = 0;

    while (i < n && j < m) {
        if (a[i] == b[j]) {
            // unchanged line → skip
            i++;
            j++;
        }
        else if (dp[i + 1][j] >= dp[i][j + 1]) {
            result.push_back({ '-', a[i] });
            i++;
        }
        else {
            result.push_back({ '+', b[j] });
            j++;
        }
    }

    // Remaining deletions
    while (i < n) {
        result.push_back({ '-', a[i++] });
    }

    // Remaining insertions
    while (j < m) {
        result.push_back({ '+', b[j++] });
    }

    return result;
}

bool Repository::isIgnorableLine(const std::string& line,
                                 bool ignoreEmpty,
                                 bool ignoreWhitespace) {
    if (ignoreEmpty && line.empty()) return true;

    if (ignoreWhitespace) {
        for (char c : line) {
            if (!isspace(static_cast<unsigned char>(c)))
                return false;
        }
        return true; // all whitespace
    }
    return false;
}

void Repository::diff(bool ignoreEmpty, bool ignoreWhitespace) {
    if (!isInitialized()) {
        std::cout << "Error: not a LiteVCS repository.\n";
        return;
    }

    std::string head = utils::read_file(vcsDir + "/HEAD");
    if (head == "null" || head.empty()) {
        std::cout << "No commits to compare against.\n";
        return;
    }

    std::string commitData =
        readObject(vcsDir + "/objects/commits/" + head);

    std::istringstream commitStream(commitData);
    std::string line, treeHash;

    while (std::getline(commitStream, line)) {
        if (line.rfind("tree", 0) == 0)
            treeHash = line.substr(5);
    }

    std::string treeData =
        readObject(vcsDir + "/objects/trees/" + treeHash);

    std::istringstream treeStream(treeData);
    std::string filePath, blobHash;
    bool anyChange = false;

    while (treeStream >> filePath >> blobHash) {
        std::filesystem::path wp =
        std::filesystem::path(root) / filePath;

    if (!std::filesystem::exists(wp)) {
        std::cout << "diff -- " << filePath << "\n";
        std::cout << "- [file deleted]\n\n";
        anyChange = true;
        continue;  
    }

        auto oldLines = splitLines(
            readObject(vcsDir + "/objects/blobs/" + blobHash));
        auto newLines = splitLines(
            utils::read_file(root + "/" + filePath));

        auto changes = lcsDiff(oldLines, newLines);

        bool printedHeader = false;
        for (auto& [type, text] : changes) {

            if (isIgnorableLine(text, ignoreEmpty, ignoreWhitespace))
                continue;

            if (!printedHeader) {
                std::cout << "diff -- " << filePath << "\n";
                printedHeader = true;
                anyChange = true;
            }

            std::cout << type << " " << text << "\n";
        }

        if (printedHeader) std::cout << "\n";
    }

    if (!anyChange) {
        std::cout << "No changes detected.\n";
    }
}

std::string Repository::normalizeWhitespace(const std::string& s) {
    std::string out;
    bool inSpace = false;
    for (char c : s) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!inSpace) out.push_back(' ');
            inSpace = true;
        } else {
            out.push_back(c);
            inSpace = false;
        }
    }
    // trim
    while (!out.empty() && out.front() == ' ') out.erase(out.begin());
    while (!out.empty() && out.back() == ' ') out.pop_back();
    return out;
}

std::string Repository::extractFunction(const std::string& line) {
    // Matches: return_type name(args) {   OR   name(args){
    static std::regex fn(R"(([A-Za-z_][A-Za-z0-9_]*)\s*\([^;]*\)\s*\{)");
    std::smatch m;
    if (std::regex_search(line, m, fn)) {
        return m[1];
    }
    return "";
}

void Repository::diffSmart(bool ignoreEmpty, bool ignoreWhitespace) {
    if (!isInitialized()) {
        std::cout << "Error: not a LiteVCS repository.\n";
        return;
    }

    std::string head = utils::read_file(vcsDir + "/HEAD");
    if (head == "null" || head.empty()) {
        std::cout << "No commits to compare against.\n";
        return;
    }

    std::string commitData =
        readObject(vcsDir + "/objects/commits/" + head);

    std::istringstream commitStream(commitData);
    std::string line, treeHash;

    while (std::getline(commitStream, line)) {
        if (line.rfind("tree", 0) == 0)
            treeHash = line.substr(5);
    }

    std::string treeData =
        readObject(vcsDir + "/objects/trees/" + treeHash);

    std::istringstream treeStream(treeData);
    std::string filePath, blobHash;
    bool anyMeaningful = false;

    while (treeStream >> filePath >> blobHash) {
         std::filesystem::path wp =
        std::filesystem::path(root) / filePath;

    if (!std::filesystem::exists(wp)) {
        std::cout << "diff -- " << filePath << "\n";
        std::cout << "- [file deleted]\n\n";
        anyMeaningful = true;
        continue;  
    }
        auto oldLines = splitLines(
            readObject(vcsDir + "/objects/blobs/" + blobHash));
        auto newLines = splitLines(
            utils::read_file(root + "/" + filePath));

        std::unordered_map<std::string, std::vector<std::string>> oldFns, newFns;
        std::string current;

        for (const auto& l : oldLines) {
            if (isIgnorableLine(l, ignoreEmpty, ignoreWhitespace)) continue;
            std::string fn = extractFunction(l);
            if (!fn.empty()) current = fn;
            if (!current.empty())
                oldFns[current].push_back(normalizeWhitespace(l));
        }

        current.clear();
        for (const auto& l : newLines) {
            if (isIgnorableLine(l, ignoreEmpty, ignoreWhitespace)) continue;
            std::string fn = extractFunction(l);
            if (!fn.empty()) current = fn;
            if (!current.empty())
                newFns[current].push_back(normalizeWhitespace(l));
        }

        for (const auto& [fn, oldBody] : oldFns) {
            auto it = newFns.find(fn);
            if (it == newFns.end()) continue;
            if (oldBody != it->second) {
                if (!anyMeaningful) {
                    std::cout << "smart-diff -- " << filePath << "\n\n";
                }
                anyMeaningful = true;
                std::cout << "Modified function: " << fn << "()\n";
            }
        }
    }

    if (!anyMeaningful) {
        std::cout << "No meaningful changes detected.\n";
    }
}


