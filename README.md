## parse-git-porcelain-v1

This is a very simple C program to parse [git status Porcelain Format Version 1](https://git-scm.com/docs/git-status#_porcelain_format_version_1).

### How to compile

```sh
cc -o3 parse-git-porcelain.c -o parse-git-porcelain
```

### How to use

```sh
# In a git working directory
git status --porcelain --branch | ~/path/to/parse-git-porcelain
```

### Example of building a shell prompt segment

![A zsh prompt showing the current directory, git branch, and working directory status through emojis ( ✔ ⬇ )](https://github.com/seojangho/parse-git-porcelain-v1/blob/main/prompt-example.png?raw=true)

The output is optimized for priting a shell prompt segment summarizing the git working directory status.

Have a look at the provided [zshrc snippet](https://github.com/seojangho/parse-git-porcelain-v1/blob/main/zshrc), which is directly taken from my dotfiles.
