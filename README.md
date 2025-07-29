# MiniGit – A Lightweight Version Control System

A simplified version control system built in **C++17**, inspired by Git.  
Supports **init**, **add**, **commit**, **log**, **checkout**, **branch**, **merge**, and **diff** commands, providing insight into how real version control works under the hood.

---

## Features
- **Repository Initialization**:  
  `init` – Create a `.minigit` directory to start tracking changes.
- **Staging & Committing**:  
  `add <file>` – Stage files for commit.  
  `commit "<message>"` – Save changes with a unique hash.  
- **History & Versioning**:  
  `log` – View commit history with messages and timestamps.  
  `checkout <branch/commit>` – Switch to a branch or commit.  
- **Branching & Merging**:  
  `branch <name>` – Create new branches.  
  `merge <branch>` – Merge another branch into the current branch.  
- **Diff**:  
  `diff <commit1> <commit2>` – Compare two commits line by line.

---
