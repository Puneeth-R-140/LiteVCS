/**
 * LiteVCS CLI Header
 * 
 * Command-line interface for LiteVCS.
 * 
 * @author Puneeth R (GHOST)
 * @license MIT
 */

#pragma once
#include <vector>
#include <string>

/**
 * CLI class - Handles command-line argument parsing and execution
 */
class CLI {
public:
    static void handle(const std::vector<std::string>& args);
};