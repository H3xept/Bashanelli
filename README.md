# Bashanelli (`bnli`)

An exercise in finding out that bash was way more extensive than we first thought.

---
## Features

- External program support
- Shell script support
- A bunch of builtins
- Profile loading
- Custom PS1
- History
- Cycling through previous commands (up/down arrows)

<sub>also we implemented our own readline! 👍</sub>

<sub>we ran out of time to do redirection... :(</sub>

---

## Profiles

Profiles are shell scripts that are ran on launch of Bashanelli.

### Bash Profile

Bashanelli will attempt to load standard bash and sh profiles (`~/.profile`, `~/.bash_profile`, etc) upon launch.
<sub>Currently disabled as Bashanelli cannot handle conditions and loops, which bash profiles are full of.</sub>

### Bashanelli Profile

Bashanelli loads its own profile on launch.

This is loaded from `~/.bnli_profile`, if such a file exists.

---

## Custom PS1

Bashanelli parses its PS1 (prompt) string from the `$PS1` environment variable.

In order to set a custom PS1 you can add the following line to `~/.bnli_profile`:

```
export PS1=<custom PS1>
```

### Control Chars

Note that control characters must be double escaped as the slashes are escaped while parsing the `export` statement, as well as when parsing the PS1. 

- `\\h` - Hostname, up to the first appearance of `.`
- `\\H` - Hostname
- `\\u` - Login
- `\\s` - The name of the shell (Usually "bashanelli")
- `\\w` - The current working directory
- `\\n` - Newline char (`\n`)

#### ANSI Escape Sequences

ANSI escape codes can be used (for colour and formatting).

They must be written in the following form:

```
"\\033[<ANSI code>m"
```

#### Example

```
export PS1="\\033[1m\\033[32m\\u@\\h\\033[0m:\\033[1m\\033[34m\\w\\033[0m\\033[1m> \\033[0m"
```

With a user "bob" on a machine "myComputer", the PS1 will look as follows:

![prompt](https://user-images.githubusercontent.com/30571778/54396454-f0cf1a80-46aa-11e9-9885-d8d261d9515c.png)

---

### History

Bashanelli uses a bash-style history that keeps track of previously entered commands.

The history is stored in `~/.bnli_history`.

#### Invocations

- `!!` - Execute last command.
- `!<n>` - Executes the n<sup>th</sup> command from the history.
- `!<-n>` - Executes the n<sup>th</sup> last command from the history.

---

## Builtins

###`cd`
Changes the working directory.
Providing no args changes the working directory to $HOME.

---

###`export`

Sets an environment variable to a given value.

#### Flags

- `-p` - Prints all environment variables.

---

###`unset`

Unsets the value of a given environment variable.

---

### `alias`

Assigns an alias to a given command.

#### Flags

- `-p` - Prints all aliases.

---

### `unalias`

Removes a given alias.

---

### `builtin`

Executes a given command as a builtin.

---

### `source`

Executes a given command as a shell script.

---

### `exec`

Executes a given command as an executable program.

---

### `exit`

Quits the shell with exit code 0.

---
