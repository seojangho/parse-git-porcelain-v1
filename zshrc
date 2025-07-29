prompt_git() {
	if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
		return
	fi

	# ref
	local ref=?
	ref=$(git symbolic-ref HEAD 2>/dev/null) || ref=$(git rev-parse --short HEAD 2>/dev/null)
	echo -n "%F{black}%K{white} ${ref#refs/heads/} "

	# print status
	local should_print_dirty=yes
	local git_status
	git_status=$(git status --porcelain --branch 2>/dev/null)
	if [[ $? -ne 0 ]]; then
		echo -n '???(git) '
		return
	fi
	git_status=$(echo "${git_status}" | ~/path/to/parse-git-porcelain 2>/dev/null)
	if [[ $? -ne 0 ]]; then
		echo -n '???(git parse) '
		return
	fi
	if [[ "${git_status}" =~ C ]]; then
		echo -n '%F{green}✔ '
		should_print_dirty=no
	fi
	if [[ "${git_status}" =~ u ]]; then
		echo -n '%F{yellow}✭ '
		should_print_dirty=no
	fi
	if [[ "${git_status}" =~ a ]]; then
		echo -n '%F{green}✚ '
		should_print_dirty=no
	fi
	if [[ "${git_status}" =~ m ]]; then
		echo -n '%F{black}• '
		should_print_dirty=no
	fi
	if [[ "${git_status}" =~ 'r' ]]; then
		echo -n '%F{black}➜ '
		should_print_dirty=no
	fi
	if [[ "${git_status}" =~ d ]]; then
		echo -n '%F{red}✖ '
		should_print_dirty=no
	fi
	if [[ "${git_status}" =~ n ]]; then
		echo -n '%F{red}🗲 '
		should_print_dirty=no
	fi
	if [[ "${should_print_dirty}" = yes && "${git_status}" =~ X ]]; then
		echo -n '%F{red}✘ '
	fi
	if git rev-parse --verify refs/stash >/dev/null 2>&1; then
		echo -n '%F{black}≡ '
	fi
	if [[ "${git_status}" =~ A ]]; then
		echo -n '%F{black}⬆ '
	fi
	if [[ "${git_status}" =~ B ]]; then
		echo -n '%F{black}⬇ '
	fi
	if [[ "${git_status}" =~ D ]]; then
		echo -n '%F{black}⬆⬇ '
	fi
}

prompt() {
	# Red ✗ by exit status of the last command
	echo -n "%(0?..%F{white}%K{red} ✗ %? )"

	# jobs
	echo -n "%(1j.%F{white}%K{red} ⧗ .)"

	# dir
	echo -n "%F{black}%K{cyan} %~ "

	# vcs
	echo -n '$(prompt_git)'

	# prompt
	echo -e "%k\n%(!.%F{red}.%F{cyan})❯%f "
}

PROMPT=$(prompt)
