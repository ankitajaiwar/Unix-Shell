To run the program type following commands in your shell:
	make
	./myshell



1. The shell supports four built-in commands: "exit", "cd", "pwd", "set" and all external commands.

2. For setting MYPATH use following command without any whitespaces.
	set MYPATH="path"


3. The shell supports one externally implemented "myls" commandThe. myls executable is made when you run make command. The executable is placed in the same folder as myshell. You can use 'pwd' command to get current directory while running myshell and set is as MYPATH to run command 'myls' succesfully. If environment variable MYPATH is not set, the shell gives an error. 

4. The shell supports multiple pipelines. There is no upper limit.Eg. command1 | command2 | command3 | command4

5. The shell supports Input-Output Redirection of all forms such as command1 < file1, or command1 > file1, or command1 < file1 > file2.

6. As per project specificaions, the shell does not support Pipe/Input-Output Redirection/Background together.

7. The shell supports simple background process if the command is followed by ampersand(&). If a command is followed by & , its execution happens in the background and you can use the screen to enter another command. The $ sign may not appear when you send the process to the background but you can still enter new commands.

8. As per project specifications, the shell does not support any other special characters.

9. The shell terminates if CTRL-D(end of file character) is encountered.

