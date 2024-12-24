#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <sstream>
#include <set>

namespace fs = std::filesystem;

// Repository State Struct
struct Repository {
    bool initialized = false;
    std::set<std::string> stagedFiles;
    std::set<std::string> committedFiles;
    std::string currentBranch = "main";
    std::map<std::string, std::vector<std::string>> branches; // Branch name -> Commit history
};

// Function to display repository status
void displayRepoStatus(const Repository& repo) {
    std::cout << "==== Repository Status ====\n";
    std::cout << "Current Branch: " << repo.currentBranch << "\n";
    std::cout << "Staged Files: ";
    for (const auto& file : repo.stagedFiles) {
        std::cout << file << " ";
    }
    std::cout << "\nCommitted Files: ";
    for (const auto& file : repo.committedFiles) {
        std::cout << file << " ";
    }
    std::cout << "\n===========================\n";
}

// Command Handling
bool processCommand(const std::string& command, Repository& repo) {
    std::istringstream iss(command);
    std::string gitCommand, arg1, arg2;
    iss >> gitCommand;

    if (gitCommand == "git") {
        iss >> arg1;
        if (arg1 == "init") {
            if (!repo.initialized) {
                repo.initialized = true;
                repo.branches[repo.currentBranch] = {};
                std::cout << "Repository initialized.\n";
                return true;
            } else {
                std::cout << "Repository already initialized.\n";
                return false;
            }
        } else if (arg1 == "add") {
            iss >> arg2;
            if (fs::exists(arg2)) {
                repo.stagedFiles.insert(arg2);
                std::cout << "File " << arg2 << " added to staging area.\n";
                return true;
            } else {
                std::cout << "File " << arg2 << " does not exist.\n";
                return false;
            }
        } else if (arg1 == "commit") {
            std::string message;
            iss >> arg2;
            if (arg2 == "-m") {
                std::getline(iss, message);
                message = message.substr(1, message.length() - 2); // Remove quotes
                if (!repo.stagedFiles.empty()) {
                    for (const auto& file : repo.stagedFiles) {
                        repo.committedFiles.insert(file);
                    }
                    repo.branches[repo.currentBranch].push_back(message);
                    repo.stagedFiles.clear();
                    std::cout << "Committed changes: \"" << message << "\"\n";
                    return true;
                } else {
                    std::cout << "No changes to commit.\n";
                    return false;
                }
            } else {
                std::cout << "Invalid commit syntax. Use: git commit -m \"message\"\n";
                return false;
            }
        } else if (arg1 == "branch") {
            iss >> arg2;
            if (repo.branches.find(arg2) == repo.branches.end()) {
                repo.branches[arg2] = repo.branches[repo.currentBranch]; // Copy commit history
                std::cout << "Branch " << arg2 << " created.\n";
                return true;
            } else {
                std::cout << "Branch " << arg2 << " already exists.\n";
                return false;
            }
        } else if (arg1 == "checkout") {
            iss >> arg2;
            if (repo.branches.find(arg2) != repo.branches.end()) {
                repo.currentBranch = arg2;
                std::cout << "Switched to branch " << arg2 << ".\n";
                return true;
            } else {
                std::cout << "Branch " << arg2 << " does not exist.\n";
                return false;
            }
        } else {
            std::cout << "Unknown Git command.\n";
            return false;
        }
    } else {
        std::cout << "Invalid command. All commands must start with 'git'.\n";
        return false;
    }
}

// Level Implementation
void level(Repository& repo, const std::string& objective, const std::vector<std::string>& hints) {
    std::cout << "==== Level Objective ====\n";
    std::cout << objective << "\n";
    std::cout << "=========================\n";

    int hintIndex = 0;
    while (true) {
        displayRepoStatus(repo);

        std::cout << "Enter your command: ";
        std::string command;
        std::getline(std::cin, command);

        if (processCommand(command, repo)) {
            std::cout << "Objective complete!\n\n";
            break;
        } else {
            if (hintIndex < hints.size()) {
                std::cout << "Hint: " << hints[hintIndex++] << "\n";
            } else {
                std::cout << "No more hints available. Keep trying!\n";
            }
        }
    }
}

int main() {
    Repository repo;

    // Define levels
    std::vector<std::string> objectives = {
        "Initialize a Git repository using `git init`.",
        "Add a file named 'file.txt' to the staging area using `git add file.txt`.",
        "Commit your changes with the message 'Initial commit' using `git commit -m \"Initial commit\"`.",
        "Create a new branch named 'feature' using `git branch feature`.",
        "Switch to the 'feature' branch using `git checkout feature`."
    };

    std::vector<std::vector<std::string>> hints = {
        {"Use the `git init` command."},
        {"Check if the file 'file.txt' exists.", "Use the `git add <filename>` syntax."},
        {"Make sure you have staged files first.", "Use the `git commit -m \"message\"` syntax."},
        {"Use the `git branch <branchname>` syntax."},
        {"Use the `git checkout <branchname>` syntax."}
    };

    for (size_t i = 0; i < objectives.size(); ++i) {
        level(repo, objectives[i], hints[i]);
    }

    std::cout << "Congratulations! You've completed all the levels and learned Git basics and branching!\n";
    return 0;
}
