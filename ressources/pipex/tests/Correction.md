# Correction

Comparer avec le résultat bash si nécessaire

AVANT LA CORRECTION avoir les flags de valgrind creer un .valgrindrc

- [ ] norminette

Creer le infile et outfile puis faire les tests sans

#### Basics

explicite path
`valgrind ./pipex infile.txt ./cat wc outfile.txt`
\-> bash :
`< infile.txt ./cat | wc > outfile.txt`
\-> expected : create outfile.txt = 3 colonnes avec des valeurs != 0, sauf si infile vide

- [ ] `valgrind ./pipex infile.txt cat "wc -l" outfile.txt`
\-> expected : outfile.txt : \[nb lines\]
\-> bash :
`< infile.txt cat | wc -l > outfile.txt`

- [ ] `valgrind ./pipex infile.txt nocmd wc outfile.txt`
\-> commande not found + excute out\_cmd
\-> expected : outfile.txt : "0 0 0"
\-> bash :
`< infile.txt nocmd | wc > outfile.txt`

- [ ] `valgrind ./pipex infile.txt cat nocmd outfile.txt`
\-> expected :commande not found + create outfile.txt
\-> bash :
`< infile.txt cat | nocmd > outfile.txt`

- [ ] `./pipex infile.txt " cat" wc outfile.txt`
\-> command not found + excute out\_cmd
\-> expected : outfile.txt : "0 0 0"
\-> bash :
`< infile.txt " cat" | wc > outfile.txt`

- [ ] `valgrind ./pipex infile.txt cat " wc" outfile.txt`
- [ ] `valgrind ./pipex infile.txt cat "wc " outfile.txt`
\-> command not found : wc + create outfile
\-> expected : create a empty outfile.txt
\-> bash :
`< infile.txt cat | " wc" > outfile.txt`
`< infile.txt cat | "wc " > outfile.txt`

#### Empty Arguments
- [ ] `valgrind ./pipex "" cat wc nofile`
\-> error no such file + create nofile + execute cmd2
\-> bash :
`< "" cat | wc > nofile`

```plain
  no such file
```

*   expected : nofile = " 0 0 0"

- [ ] `valgrind ./pipex infile.txt cat wc ""`
- [ ] `valgrind ./pipex nofile cat wc ""`
\-> expected : error no such file
\-> bash :
`< infile.txt cat | wc > ""`
`< nofile cat | wc > ""`

- [ ] `valgrind ./pipex "" "" "cat" outfile.txt`
\-> error + creer un outfile vide
\-> bash :
`< "" "" | cat > outfile.txt`

- [ ] `valgrind ./pipex infile.txt cat "" ""`
\-> no such file
\-> bash :
`< infile.txt cat | "" > ""`

- [ ] `valgrind ./pipex nofile "cat" "" ""`
\-> bash
`< nofile cat | "" > ""`
\-> expected :

```bash
no such file or directory: nofile
no such file or directory:
```

- [ ] `valgrind ./pipex "" "" "cat" "nofile"`
\-> bash
`< "" "" | cat > nofile`
\-> expected : + create nofile

```plain
  no such file
```

- [ ] `.``/valgrind pipex "" "" "" ""`
\-> bash :
`< "" "" | "" > ""`

```perl
no such file or directory: 
no such file or directory:
```

`a``l`
- [ ] `valind ./pipex infile.txt "sleep 3" "wc -l" outfile.txt`
\-> expected : wait 3 sec + outfile.txt = "0"
\-> bash :
`< < infile.txt sleep 3 | wc -l > outfile.txt`

`lgrind ./pipex infile.txt "sleep 3" "sleep 5" outfile.txt`G
- [ ] 
\-> expectd : wait 5 sec -> faire sans valgrind (il ralentit)

EGet CrazyMultiple options
\->`valgrind ./pipex infile.txt "echo coucou les" "wc -l" outfile.txt`\>-> bash :
`< infile.txt echo coucou les | wc -l > outfile.txt`
expected : compare values

Explicit Path
1. avoir cat dans le dossier actuel (le chercher dans user/bin)
2. `valgrind ./pipex infile.txt ./cat wc outfile.txt`
\-> bash
`< infile.txt ./cat | wc > outfile.txt`
*   expected : outfile.txt = 3 colonnes avec des valeurs != 0, sauf si infile vide

*