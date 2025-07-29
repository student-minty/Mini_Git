#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

const string MINIGIT_DIR = ".minigit";
const string COMMITS_FILE = MINIGIT_DIR + "/commits.txt";
const string BRANCHES_FILE = MINIGIT_DIR + "/branches.txt";
const string HEAD_FILE = MINIGIT_DIR + "/HEAD";
const string STAGING_DIR = MINIGIT_DIR + "/staging";

// Simple non-crypto hash for commit IDs
string simpleHash(const string &data) {
    unsigned long hash = 5381;
    for (char c : data) hash = ((hash << 5) + hash) + c;
    stringstream ss;
    ss << hex << hash;
    return ss.str().substr(0, 7);
}

// Get current HEAD (branch or commit)
string getHead() {
    ifstream in(HEAD_FILE);
    string head;
    getline(in, head);
    return head.empty() ? "master" : head;
}

void setHead(const string &head) {
    ofstream out(HEAD_FILE);
    out << head;
}

// Map branch to commit
unordered_map<string, string> loadBranches() {
    unordered_map<string, string> branches;
    ifstream in(BRANCHES_FILE);
    string line;
    while (getline(in, line)) {
        size_t sep = line.find('|');
        if (sep != string::npos)
            branches[line.substr(0, sep)] = line.substr(sep + 1);
    }
    return branches;
}

void saveBranches(const unordered_map<string, string> &branches) {
    ofstream out(BRANCHES_FILE);
    for (auto &b : branches)
        out << b.first << "|" << b.second << "\n";
}

// Initialize repo
void initRepo() {
    if (!fs::exists(MINIGIT_DIR)) {
        fs::create_directory(MINIGIT_DIR);
        ofstream(COMMITS_FILE).close();
        unordered_map<string, string> branches = {{"master", ""}};
        saveBranches(branches);
        setHead("master");
        cout << "Initialized empty MiniGit repository.\n";
    } else {
        cout << "Repository already exists.\n";
    }
}

// Add file to staging
void addFile(const string &filePath) {
    if (!fs::exists(filePath)) {
        cout << "File does not exist.\n";
        return;
    }
    if (!fs::exists(STAGING_DIR)) fs::create_directory(STAGING_DIR);
    fs::copy(filePath, STAGING_DIR + "/" + fs::path(filePath).filename().string(),
             fs::copy_options::overwrite_existing);
    cout << "Added " << filePath << " to staging area.\n";
}

// Commit staged changes
void commitChanges(const string &message) {
    if (!fs::exists(STAGING_DIR) || fs::is_empty(STAGING_DIR)) {
        cout << "No changes to commit.\n";
        return;
    }
    string commitHash = simpleHash(message + to_string(time(nullptr)));
    string commitDir = MINIGIT_DIR + "/" + commitHash;
    fs::copy(STAGING_DIR, commitDir, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    fs::remove_all(STAGING_DIR);

    ofstream commitsOut(COMMITS_FILE, ios::app);
    commitsOut << commitHash << "|" << message << "|" << time(nullptr) << "\n";
    commitsOut.close();

    auto branches = loadBranches();
    string head = getHead();
    if (branches.find(head) != branches.end()) {
        branches[head] = commitHash;
        saveBranches(branches);
    }
    cout << "Committed as " << commitHash << ": " << message << "\n";
}

// Show commit history
void showLog() {
    ifstream in(COMMITS_FILE);
    string line;
    vector<string> commits;
    while (getline(in, line)) commits.push_back(line);
    reverse(commits.begin(), commits.end());
    for (auto &c : commits) {
        stringstream ss(c);
        string hash, msg, ts;
        getline(ss, hash, '|');
        getline(ss, msg, '|');
        getline(ss, ts, '|');
        time_t t = stol(ts);
        cout << "Commit " << hash << " - " << ctime(&t) << "    " << msg << "\n";
    }
}

// Checkout branch or commit
void checkout(const string &target) {
    auto branches = loadBranches();
    string commitHash = target;
    if (branches.find(target) != branches.end()) commitHash = branches[target];

    string commitDir = MINIGIT_DIR + "/" + commitHash;
    if (!fs::exists(commitDir)) {
        cout << "Target not found.\n";
        return;
    }
    for (auto &p : fs::directory_iterator(commitDir)) {
        fs::copy(p.path(), p.path().filename(),
                 fs::copy_options::overwrite_existing);
    }
    setHead(target);
    cout << "Checked out " << target << "\n";
}

// Create branch
void createBranch(const string &name) {
    auto branches = loadBranches();
    string head = getHead();
    string commit = branches[head];
    if (branches.find(name) != branches.end()) {
        cout << "Branch already exists.\n";
        return;
    }
    branches[name] = commit;
    saveBranches(branches);
    cout << "Created branch " << name << " at " << commit << "\n";
}

// Merge branch (simple overwrite)
void mergeBranch(const string &name) {
    auto branches = loadBranches();
    if (branches.find(name) == branches.end()) {
        cout << "Branch not found.\n";
        return;
    }
    string commitHash = branches[name];
    string commitDir = MINIGIT_DIR + "/" + commitHash;
    if (!fs::exists(commitDir)) {
        cout << "Nothing to merge.\n";
        return;
    }
    for (auto &p : fs::directory_iterator(commitDir)) {
        fs::copy(p.path(), p.path().filename(),
                 fs::copy_options::overwrite_existing);
    }
    cout << "Merged branch " << name << " into current branch.\n";
}

int main() {
    cout << "MiniGit C++ (Branches & Merge)\n";
    string cmd;
    while (true) {
        cout << "minigit> ";
        getline(cin, cmd);
        stringstream ss(cmd);
        vector<string> tokens;
        string word;
        while (ss >> word) tokens.push_back(word);
        if (tokens.empty()) continue;

        if (tokens[0] == "init") initRepo();
        else if (tokens[0] == "add" && tokens.size() == 2) addFile(tokens[1]);
        else if (tokens[0] == "commit" && tokens.size() >= 2) {
            string msg;
            for (int i = 1; i < tokens.size(); ++i) msg += tokens[i] + " ";
            commitChanges(msg);
        }
        else if (tokens[0] == "log") showLog();
        else if (tokens[0] == "checkout" && tokens.size() == 2) checkout(tokens[1]);
        else if (tokens[0] == "branch" && tokens.size() == 2) createBranch(tokens[1]);
        else if (tokens[0] == "merge" && tokens.size() == 2) mergeBranch(tokens[1]);
        else if (tokens[0] == "exit") break;
        else cout << "Commands: init | add <file> | commit <msg> | log | checkout <branch/commit> | branch <name> | merge <branch> | exit\n";
    }
}
