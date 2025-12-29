#include "repository.h"
#include "utils.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <zlib.h>
#include <vector>
#include <algorithm>


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
    std::vector<char> compressed((std::istreambuf_iterator<char>(in)),
                                  std::istreambuf_iterator<char>());

    uLongf decompressedSize = 10 * compressed.size();
    std::vector<Bytef> buffer(decompressedSize);

    ::uncompress(buffer.data(), &decompressedSize,
                 reinterpret_cast<Bytef*>(compressed.data()),
                 compressed.size());

    return std::string(reinterpret_cast<char*>(buffer.data()), decompressedSize);
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

void Repository::diff() {
    if (!isInitialized()) {
        std::cout << "Error: not a LiteVCS repository.\n";
        return;
    }

    std::string head = utils::read_file(vcsDir + "/HEAD");
    if (head == "null" || head.empty()) {
        std::cout << "No commits to compare against.\n";
        return;
    }

    // Load commit → tree
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

        std::string blobData =
            readObject(vcsDir + "/objects/blobs/" + blobHash);

        std::string workingData =
            utils::read_file(root + "/" + filePath);

        auto oldLines = splitLines(blobData);
        auto newLines = splitLines(workingData);

        if (oldLines == newLines)
            continue;

        anyChange = true;
        std::cout << "diff -- " << filePath << "\n";

        size_t i = 0, j = 0;
        while (i < oldLines.size() || j < newLines.size()) {
            if (i < oldLines.size() && j < newLines.size()
                && oldLines[i] == newLines[j]) {
                i++; j++;
            }
            else {
                if (i < oldLines.size()) {
                    std::cout << "- " << oldLines[i] << "\n";
                    i++;
                }
                if (j < newLines.size()) {
                    std::cout << "+ " << newLines[j] << "\n";
                    j++;
                }
            }
        }
        std::cout << "\n";
    }

    if (!anyChange) {
        std::cout << "No changes detected.\n";
    }
}