*This project has been created as part of the 42 curriculum by pswirgie.*
<p id="top"></p>

# Pipex Project

- [Pipex Project](#pipex-project)
	- [1. Descriptions](#1-descriptions)
	- [2. Architecture](#2-architecture)
		- [2.1. Structure](#21-structure)
	- [3. Instructions](#3-instructions)
		- [Exemples](#exemples)
	- [4. Resources](#4-resources)
		- [Online ressources :](#online-ressources-)

<!-- new lines -->
<br><br><br> 

## 1. Descriptions

 Pipex is a programm in C.<br>The purpose is imitate the pipe comportement with commands and file.
<br>
```bash
./pipex infile.txt command1 command2 outfile.txt
```
exemple for : 
```bash
./pipex infile.txt "cat -e" "wc -l" outfile.txt
```
we expect the same than : 
```bash
< infile.txt cat -e | wc -l > outfile.txt
```
It's handled with an child-parent architecture and a structure.<br>
Infile is the children. Outfile is the parent.<br>
If no children exist or we don't have a properly permission, the program will return and print an error.<br>
If no parent exist, the program create one.

The bonus part is absent due to a lack of time.
It following this requierements :
- handle multiple pipes
- fist parameter here_doc << >>

<!-- new lines -->
<br><br><br>

## 2. Architecture

1. Main
2. Parsing :
- initialisation structure values : 0 or NULL
- give to structure for infile and outfile: command, all command options (we can have more than one option), number of options, filename.
char **parse->type.args_execve
args_execve = path command, eg.{"grep", "-a", NULL};
3. Initialisation char ** tab{cmd, option 1, options 2, [...], NULL} to give to execve, one for infile and another for outfile.
4. Execute commands with all options.
With check command path, validation command, validation options.
5. Free entire structure.

.

### 2.1. Structure
```C
typedef struct s_parse
{
	t_element	in; -> all the t_element for infile, children
	t_element	out; -> all the t_element for outfile, parent
	int			error; -> number for the return
}		t_parse;
```

|error| |
|-----|-|
|1|permission denied|
|2|no such file or directory|
|127| command not found|

```C
typedef struct s_element
{
	char		*file; -> file name
	char		*cmd; -> command
	char		*cmd_path; -> path comman for execute
	char		*path_explicit; -> start NULL, filled if we have "./command"
	char		**options; -> a char ** with all options
	char		**args_execve; -> for execute command, we need execve, it need a char ** {cmd, option 1, options 2, [...], NULL}
	int			nb_opt; -> nb of options
	int			is_file; -> is a filename
	int			is_cmd; -> is command (valid or not)
	int			fd; -> fd file
	int			close; -> start at 0, 1 = fd is close
}				t_element;

```
[back to top](#top)

<!-- new lines -->
<br><br><br>

## 3. Instructions

1. git clone [url]
2. cd ./directoryname
3. In terminal of the project, type "Make" then press Enter.
4. To use pipex, this is the syntax :<br> `./pipex infile.txt command1 command2 outfile.txt`
   1. If you want to add flag in your commands use <br> `./pipex infile.txt "command1 -flag" "command2 -flag" outfile.txt`<br>
(We can give invalid infile or invalid outfile. The programm return an error.)
<!-- new lines -->
<br><br>

`Tests basics`

```bash
./pipex infile.txt "cat a l o" wc outfile.txt
./pipex infile.txt "cat infile.txt" wc outfile.txt
./pipex infile.txt "cat lol" wc outfile.txt
```
<!-- new line -->
<br>

`Tests valgrind`
```bash
valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full --track-fds=yes --trace-children=yes -s [test basic]
```
<!-- new line -->
<br>

### Exemples

Input :

```bash
infile.txt : "coucou"
./pipex infile.txt cat wc outfile.txt
```

Output : 
```bash
outfile.txt : "1	1	7"
```
[back to top](#top)


<!-- new lines -->
<br><br><br>

## 4. Resources
Many discussions with my peers.
Many tests and explanation by AI.
Ai tools were used for :
- review the code structure and impove some bad habits
- assist in understanding the global execution of the pipex project
- help detect memory leaks and debugging issues

README has been improved by tips from nbaudoin ! Thanks you !

### Online ressources : 
- [medium-pipex-explanation](https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901)
- [exit-code-terminal](https://www.geeksforgeeks.org/linux-unix/how-to-use-exit-code-to-read-from-terminal-from-script-and-with-logical-operators/)
- [french-pipe-explanation](https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/)
- [french-child-process-explanation](https://www.codequoi.com/creer-et-tuer-des-processus-fils-en-c/)

[back to top](#top)