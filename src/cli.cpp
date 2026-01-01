/**
 * LiteVCS Command-Line Interface
 * 
 * Handles user commands and delegates to Repository class.
 * 
 * @author Puneeth R (GHOST)
 * @license MIT
 */

#include "cli.h"
#include "repository.h"
#include <iostream>
#include <filesystem>

/**
 * Handle command-line arguments and execute appropriate repository actions
 * @param args Command-line arguments vector
 */
void CLI::handle(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: vcs <command>\n";
        std::cout << "\nAvailable commands:\n";
        std::cout << "  init                     - Initialize a new repository\n";
        std::cout << "  track <file>             - Track a file\n";
        std::cout << "  save \"message\"           - Commit changes\n";
        std::cout << "  history                  - Show commit history\n";
        std::cout << "  go <commit_hash>         - Checkout a commit\n";
        std::cout << "  diff [options]           - Show changes\n";
        std::cout << "\nDiff options:\n";
        std::cout << "  --smart                  - Smart/semantic diff\n";
        std::cout << "  --ignore-empty           - Ignore empty lines\n";
        std::cout << "  --ignore-whitespace      - Ignore whitespace\n";
        return;
    }

    try {
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
            // Security: Validate commit hash length (SHA-1 is 40 hex characters)
            if (args[2].length() > 40) {
                std::cout << "Error: invalid commit hash (too long)\n";
                return;
            }
            // Validate hex characters
            for (char c : args[2]) {
                if (!isxdigit(static_cast<unsigned char>(c))) {
                    std::cout << "Error: commit hash must contain only hexadecimal characters\n";
                    return;
                }
            }
            repo.goToCommit(args[2]);
        }
        else if (args[1] == "diff") {
            bool smart = false, ignoreEmpty = false, ignoreWhitespace = false;

            for (size_t i = 2; i < args.size(); ++i) {
                if (args[i] == "--smart") smart = true;
                else if (args[i] == "--ignore-empty") ignoreEmpty = true;
                else if (args[i] == "--ignore-whitespace") ignoreWhitespace = true;
                else {
                    std::cout << "Warning: unknown option " << args[i] << "\n";
                }
            }

            if (smart)
                repo.diffSmart(ignoreEmpty, ignoreWhitespace);
            else
                repo.diff(ignoreEmpty, ignoreWhitespace);
        }
        else {
            std::cout << "Unknown command: " << args[1] << "\n";
            std::cout << "Run 'vcs' without arguments to see available commands.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}
