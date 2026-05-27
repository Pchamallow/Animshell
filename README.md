*This project has been created as part of the 42 curriculum by stkloutz and pswirgie.*
<p id="top"></p>

<!-- new lines -->
<br>
<div align="center">

# Minishell Project
</div>

- [Minishell Project](#minishell-project)
	- [1. Descriptions](#1-descriptions)
	- [2. Architecture](#2-architecture)
	- [2.1 Shell execution cycle](#21-shell-execution-cycle)
		- [1. Initialization](#1-initialization)
		- [2. Main Loop](#2-main-loop)
		- [3. Parsing](#3-parsing)
		- [4. Command Execution](#4-command-execution)
			- [1. Token reading (pipes)](#1-token-reading-pipes)
				- [2. Command validation (non-builtin)](#2-command-validation-non-builtin)
			- [3. Pipe management](#3-pipe-management)
			- [4. Built-in commands](#4-built-in-commands)
			- [5. Forking process](#5-forking-process)
			- [6. Status and signal handling](#6-status-and-signal-handling)
		- [5. Freeing and Reinitialization of the Structure](#5-freeing-and-reinitialization-of-the-structure)
	- [2.2 Structure typedef](#22-structure-typedef)
	- [2.3 Structure folder](#23-structure-folder)
	- [2.4 Fonctions utils](#24-fonctions-utils)
	- [3. Instructions](#3-instructions)
			- [Valgrind Flags](#valgrind-flags)
			- [Example basic tests](#example-basic-tests)
	- [4. Resources](#4-resources)

<!-- new lines -->
<br><br><br>

## 1. Descriptions
This program is a minimal shell inspired by Bash, written in C.<br>
It reads a prompt, e.g.:
```bash
echo coucou | wc
```
The goal is to create a program that:
- Parses the given prompt according to Bash rules:
	- Single quotes and double quotes behave differently
	- Depending on the context, environment variables are expanded or not (according to the subject requirements)
- Uses `fork`:
	- Creates a child process to execute commands while keeping the main program running as an interactive shell
- Uses `pipefd`:
	- Combined with `fork`, it allows communication between processes by capturing the output of commands
- Implements built-in commands:
	- `echo` with option `-n` 
	- `cd` with only a relative or absolute path
	- `pwd` with no options
	- `export` with no options
	- `unset` with no options
	- `env` with no options or arguments
	- `exit` with no options
- Supports pipes (`|`)
- Implements heredoc (`<<`)
- Handles input and output redirections (`<`, `>`, `>>`)
- Manages environment variables
- Handles signals:
	- `CTRL+C`
	- `CTRL+D`
	- `CTRL+\`
	- in: in prompt, commands, heredoc

[back to top](#top)
<!-- new lines -->
<br><br><br><br><br>

## 2. Architecture

All along the project:

- A structure is used to store multiple variable types (int, char, char *) but also custom sub-types (builtin_kind).
- We handle several signal functions to receive and manage signals.
- If a malloc fails, we free the entire structure and exit either the child process or the main program. In both cases, we ensure no memory leaks.
- The return error code is updated throughout the program:
  - If a command succeeds → return 0
  - If infile/outfile fails → return the appropriate error code
  - And many other cases depending on the context

[back to top](#top)
<!-- new lines -->
<br>

## 2.1 Shell execution cycle

### 1. Initialization

We initialize values that will be used for the first prompt:
- Main structure
- Copy of `envp` into the structure
- Current working directory (`pwd`)


### 2. Main Loop

Inside the loop, we:

- Print the prompt (e.g. `minishell$: `), unless modified by a command that affects the prompt behavior
- Initialisation of sub-part of structure(exec, pipe);


### 3. Parsing
// STEPH


### 4. Command Execution
- This process runs in a loop.
---
#### 1. Token reading (pipes)
- Read pipe tokens
- Define input and output (infile, outfile, pipe, heredoc)
- Validate the command
- Parse arguments
- Update the current pipe index and move to the next one
---
##### 2. Command validation (non-builtin)
- If the command is valid and is not a builtin:
  - Create the `char **` array for `execve`
  - Include the command path and its arguments
---
#### 3. Pipe management
- Pipes are used to retrieve the output of commands
- Data is passed between processes using `pipefd`
---
#### 4. Built-in commands
  - Execute it in the appropriate process (parent or child depending on the case)
  - If the output must be piped, or if there are no pipes involved → execute in the child process
  - If the result must affect the next prompt → execute in the parent process
  - Example: `echo -n coucou` modifies the shell output directly, so it must be executed in the parent process to affect the next prompt
---
#### 5. Forking process
- At this stage, we have both a parent and a child process
- We store the `last_pid` of the last child to retrieve the final exit status
- child process
	- If input/output is not standard (= terminal), apply redirections using `dup2`
	- Close all unnecessary file descriptors (infile, outfile, heredoc, pipefds)
	- Execute the command (some builtins are not executed in child)
	- Free allocated memory
	- Exit the process
- parent process
	- Read data from `pipefd[0]` and pass it to the next child if needed
	- Close `pipefd[1]`
	- Free and close unused resources:
		- Prompt (reinitialized at each loop)
		- Heredoc (if not closed)
		- Infile / outfile
	- Move to the next command in the pipeline
---
At this point, all pipes have been processed.

---
#### 6. Status and signal handling
- Use a `waitpid` loop to wait for all child processes
- Handle signals if any occurred
- Retrieve the exit status of the last executed command
---
<!-- new lines -->
<br>

### 5. Freeing and Reinitialization of the Structure

The program returns to the main loop.

- We only free the necessary parts of the structure, not everything:
  - heredoc
  - prompt
  - the list of tokens and their contents (e.g. `char *value`)
  - the `char *result` from `cd`
---
Finally, the loop starts again.

---
[back to top](#top)
<!-- new lines -->
<br>

## 2.2 Structure typedef

```C
typedef struct s_minishell
{
	t_exec		exec;		// execution context (see below)
	t_token		*token;		// first token of the command line
	t_token		*here_doc; 	// single active heredoc (fd + value)

	t_builtin	builtin;	// builtin state (result, prompt flag, error code)
	char		*prompt;	// current prompt string
}				t_minishell;
```
| `t_exec exec` | Description |
|------|-------------|
| `char **paths_for_search_cmd` | Paths extracted from `PATH` environment variable, split into possible directories to locate and execute commands |
| `char **envp` | Copy of the environment variables (`envp`), which may be modified during execution (e.g. after `unset PATH`) |
| `long long error` | Error code of the last command or pipeline execution, used as the program return value |
| `long long error_old` | Previous error code, kept when needed for restoration or comparison |
| `int index_pipe` | Index of the current pipe in the token list |
| `int index_prev_pipe` | Index of the previous pipe in the token list |
| `int nb_pipes` | Total number of pipes in the current command line |
| `t_pipe *pipe_lst` | List of pipeline commands. <br> Each node contains: <br> - input/output tokens (`infile/outfile` with value and file descriptor), <br> - first token of the command, <br> - command token, <br> - command metadata (`is_cmd`, `nb_args`, `error`, `infile_error`, `outfile_error`), <br> - input/output type (`ERROR`, `TERMINAL`, `IS_FILE`, `IS_HEREDOC`, `IS_PIPE`), <br> - builtin type (`t_builtin_kind`), <br> - pointer to next pipe |
| `t_token *last_pipe` | First token of the next pipe (i.e. last token of current pipe + 1) |
| `t_token **first_token` | Pointer to the first token of the entire command line |

// STEPH ?? tableau similaire pour parse expand ou non ?

[back to top](#top)
<br><br>

## 2.3 Structure folder
```
.
├── Makefile
├── README.md
├── includes
│   └── minishell.h
├── lib
│   └── libft
├── readline.supp
└── src
	├── execute
	│   ├── built_in
	│   ├── cmd_path
	│	├── exec_cmds.c
	│   ├── execute.c
	│   ├── heredoc.c
	│   ├── init
	│   ├── read_token.c
	│   └── utils
	├── main.c
	├── outfile.txt
	├── parsing
	│   ├── expand_line.c
	│   ├── expand_line_count.c
	│   ├── expand_line_strlcat_add_quotes.c
	│   ├── expand_line_utils.c
	│   ├── ft_token_list.c
	│   ├── handle_quote_type.c
	│   ├── handle_token_types.c
	│   ├── parsing.c
	│   ├── parsing_errors_free.c
	│   ├── parsing_utils.c
	│   └── separate_into_tokens.c
	└── signals
		├── signals_default.c
		├── signals_heredoc.c
		└── signals_interactive.c
```
[back to top](#top)


<!-- new lines -->
<br>

## 2.4 Fonctions utils

|Utils||
|-|-|
|TOKEN|--------------------------------------------------------|
|lst_size| size of token list|
|UTILS_CHAR|--------------------------------------------------------|
|count_chr|count the occurrence(s) of a chr in a str|
|UTILS_STR|--------------------------------------------------------|
|ft_strcmp| = strlcmp, choose the longest len between src and dst|
|is_space| index of the last space in a str|
|ft_strcpy| = = strlcpy, choose the longest len between src and dst|
|safe_join|strjoin, return NULL if malloc fail|
|UTILS_STRV|--------------------------------------------------------|
|strv_dup| malloc a char **str and copy from a char **str |
|free_strv / free_strv_len| free a char **str |
|strvlen| len of a strv|
|memcpy_strv|copy memory strv|
[back to top](#top)
<!-- new lines -->
<br><br><br><br><br>

## 3. Instructions

1. `git clone [url]`
2. `cd directory_name`
3. In the project terminal, run `make` and press Enter.
4. Write any prompt according to the subject requirements and limitations.

#### Valgrind Flags
(for .valgrindrc)
```
--leak-check=full
--show-leak-kinds=all
--trace-children=yes
--track-fds=yes
--track-origins=yes
--suppressions=readline.supp
```

#### Example basic tests
```bash
echo -n coucou
echo coucou | wc -l
echo "1""2""3"
<<a cat | wc <<b
jkkhjkh | echo coucou
ls | exit 42
```

[back to top](#top)
<!-- new lines -->
<br><br><br>


## 4. Resources
- Many discussions with ours peers
- the linux man
- Ai tools were used for :
	- review the code structure and impove some bad habits
	- assist in understanding the global execution of the minishell project
	- help detect memory leaks and debugging issues
- [using readline](https://www.codestudy.net/blog/where-is-the-readline-function-located-in-c/)
- [termcap](https://www.gnu.org/software/termutils/manual/termcap-1.3/html_chapter/termcap_2.html)
- [exit-code-terminal](https://www.geeksforgeeks.org/linux-unix/how-to-use-exit-code-to-read-from-terminal-from-script-and-with-logical-operators/)
- Thanks to Gauthier(gaasseli) and lucnicol for their documentation : [minishell_tests](https://docs.google.com/document/d/1r0yE7gK12J6lW79mzY8ELyKJEiDqkOeRfXZ6DWSKZVE/edit?tab=t.0)
- Thanks to our colleagues who shared their Minishell test suites to help us : 
	- Nathan (nda-cunh) : [minishell_tester](https://gitlab.com/nda-cunh/minishell_tester)
	- Aliexei (aprivalo)
- Thanks to Amy and Seb who tested our Minishell during an intense duel.
//STEPH

[back to top](#top)