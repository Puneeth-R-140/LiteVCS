#include "cli.h"
#include "repository.h"
#include <iostream>
#include <filesystem>

void CLI::handle(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: vcs <command>\n";
        return;
    }

    Repository repo(std::filesystem::current_path().string());

    if (args[1] == "init") {
        repo.init();
    }
    else if (args[1] == "track") {
        if (args.size() < 3) {
            std::cout << "Usage: vcs track <file>\n";
            return;
        }
        repo.trackFile(args[2]);
    }
    else if (args[1] == "save") {
    if (args.size() < 3) {
        std::cout << "Usage: vcs save \"message\"\n";
        return;
    }
    repo.save(args[2]);
   }
   else if (args[1] == "history") {
    repo.showHistory();
}
else if (args[1] == "go") {
    if (args.size() < 3) {
        std::cout << "Usage: vcs go <commit_hash>\n";
        return;
    }
    repo.goToCommit(args[2]);
}
else if (args[1] == "diff") {
    repo.diff();
}
else {
        std::cout << "Unknown command: " << args[1] << "\n";
    }

}
