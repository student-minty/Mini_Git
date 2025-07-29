MiniGit – A Lightweight Version Control System
A simplified version control system built in C++17, inspired by Git.
Supports init, add, commit, log, checkout, branch, merge, and diff commands, providing insight into how real version control works under the hood.
Features
Repository Initialization:
init – Create a .minigit directory to start tracking changes.

Staging & Committing:
add <file> – Stage files for commit.
commit "<message>" – Save changes with a unique hash.

History & Versioning:
log – View commit history with messages and timestamps.
checkout <branch/commit> – Switch to a branch or commit.

Branching & Merging:
branch <name> – Create new branches.
merge <branch> – Merge another branch into the current branch.

Diff:
diff <commit1> <commit2> – Compare two commits line by line.

Project Structure

.minigit/              # Repository data
    commits.txt        # Commit history (hash|message|timestamp)
    branches.txt       # Branch pointers
    HEAD               # Current branch or commit
    <commit_hash>/     # Snapshot of files for each commit
    staging/           # Temporary area for added files
Usage
1. Compile

g++ -std=c++17 minigit.cpp -o minigit

2. Run

./minigit

3. Commands

init                           # Initialize repository
add <file>                     # Stage a file
commit "Commit message"        # Save staged changes
log                            # View commit history
checkout <branch/commit>       # Switch to branch or commit
branch <name>                  # Create new branch
merge <branch>                 # Merge branch into current
diff <commit1> <commit2>       # Show changes between commits
exit                           # Exit MiniGit

Example Session

minigit> init
Initialized empty MiniGit repository.

minigit> add file1.txt
Added file1.txt to staging area.

minigit> commit "Initial commit"
Committed as 8c3f7a1: Initial commit

minigit> branch feature
Created branch feature at 8c3f7a1

minigit> checkout feature
Checked out feature

minigit> add file2.txt
Added file2.txt to staging area.

minigit> commit "Added new feature"
Committed as a2b8cde: Added new feature

minigit> checkout master
Checked out master

minigit> merge feature
Merged branch feature into current branch.

minigit> log
Commit a2b8cde - Fri Jul 25 18:10:42 2025
    Added new feature

Commit 8c3f7a1 - Fri Jul 25 18:09:31 2025
    Initial commit
    
Architecture
(Add a diagram here)

Command Handler → Parses user input & calls respective functions.

File Manager → Manages snapshots & staging using C++17 filesystem.

Metadata Manager → Stores commits, branches, and HEAD pointer.

Diff Engine → Compares commits to show line‑level changes.

Why This Project?
Understand core Git internals (commits, branches, diffs).

Demonstrate C++17 filesystem & data structure usage.

Show system‑level design skills (branching & merging).

Future Enhancements
Conflict detection in merges.

Compressed commit storage (zlib).

User configuration (author info).

Better colored diff output.
