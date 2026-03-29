// valac tester.vala --pkg=gio-2.0
using GLib;

/*
Structure describing a single pipex test
Each test corresponds to:
./pipex infile cmd1 cmd2 outfile
*/
struct Test {
	public string infile;
	public string cmd1;
	public string cmd2;
	public string outfile;
}

// Color codes
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

/*
Run a command and capture stdout and stderr
*/
string run_command(string[] cmd, out string err_output)
{
	string output;
	int status;

	try {
		Process.spawn_sync(
			null,
			cmd,
			null,
			SpawnFlags.SEARCH_PATH,
			null,
			out output,
			out err_output,
			out status
		);
	}
	catch (SpawnError e) {
		stderr.printf("Execution error: %s\n", e.message);
		err_output = "";
		return "";
	}

	return output;
}

int print_valgrind_errors(string valgrind_log)
{
	//  string[] lines = valgrind_log.split("\n");
	int	error = 0;

	if (valgrind_log.contains("(below main)"))
	{
		stdout.printf("%sCommand leak detected.\n!!! To verify !!!%s\n", YELLOW, RESET);
		error = 2;
	}

	if (valgrind_log.contains("Open file descriptor"))
	{
		stdout.printf("%sFile descriptor open.%s", RED, RESET);
		error++;
	}

	if (valgrind_log.contains("invalid read") && error != 2)
	{
		error = 1;
		stdout.printf("Invalid read detected");

	}

	if (valgrind_log.contains("invalid write") && error != 2)
	{
		stdout.printf("Invalid write detected");
		error = 1;
	}

	if (valgrind_log.contains("Conditional jump") && error != 2)
	{
		stdout.printf("Uninitialised value detected");
		error = 1;
	}

	if (valgrind_log.contains("Definitely lost") && error != 2)
	{
		stdout.printf("Memory leak detected");
		error = 1;
	}

	if (valgrind_log.contains("children") && error != 2)
	{
		stdout.printf("File descriptor leak detected.");
		error = 1;
	}

	if (valgrind_log.contains("parent") && error != 2)
	{
		stdout.printf("File descriptor leak detected.");
		error = 1;
	}

	if (valgrind_log.contains("Invalid size value") && error != 2)
	{
		stdout.printf("Memory leak detected.");
		error = 1;
	}

	if (error == 1) {
		stdout.printf("\nValgrind report ---\n");
	}
	return (error);
}

public int test_number = 0;

async void run_test (Test t) {

	++test_number;
	stdout.printf("\n--- Test %d: %s %s | %s %s ---\n", test_number, t.infile, t.cmd1, t.cmd2, t.outfile);


	string bash_outfile  = "outfile_bash";

	/*
	Run pipex with valgrind
	*/

	string[] pipex_cmd = {
		"valgrind",
		"--leak-check=full",
		"--show-leak-kinds=all",
		"--track-origins=yes",
		"--track-fds=yes",
		//  "--trace-children=yes",
		"./pipex",
		t.infile,
		t.cmd1,
		t.cmd2,
		t.outfile,
		null
	};

	string valgrind_log;
	run_command(pipex_cmd, out valgrind_log);

	/*
	Analyse valgrind output
	*/

	bool valgrind_error = false;

	error = print_valgrind_errors(valgrind_log);

	if (error == 1)
		valgrind_error = true;
	
	/*
	Run equivalent bash pipeline
	*/

	bool outfile_exists = FileUtils.test(t.outfile, FileTest.EXISTS);

	//  string pipex_outfile = t.outfile;

	if (outfile_exists)
	{
		try {
			string content;
			FileUtils.get_contents(t.outfile, out content);
			FileUtils.set_contents(bash_outfile, content);
		}
		catch (Error e) {
			stderr.printf("Copy error\n");
		}
	}

	string shell_pipeline = "< %s %s | %s > %s".printf(t.infile, t.cmd1, t.cmd2, bash_outfile);

	string[] bash_cmd = {
		"bash",
		"-c",
		shell_pipeline,
		null
	};

	string bash_err;

	/*
	Read outputs
	*/

	run_command(bash_cmd, out bash_err);

	string expected = "";
	try {
		FileUtils.get_contents(bash_outfile, out expected);
	} catch (Error e) {
		stderr.printf("Read error\n");
	}

	string actual = "";
	try {
		FileUtils.get_contents(t.outfile, out actual);
	} catch (Error e) {
		stderr.printf("Read error\n");
	}

	/*
	Compare outputs
	*/

	if (expected == actual && !valgrind_error)
	{
		stdout.printf("%sOK\n%s", GREEN, RESET);
		passed++;
	}
	else
	{
		stdout.printf("Expected output:\n%s\n", expected);
		stdout.printf("Pipex output:\n%s\n", actual);

		stdout.printf("%sFAIL\n%s", RED, RESET);

		failed++;
	}

	FileUtils.remove(bash_outfile);
}

int passed = 0;
int failed = 0;
int error = 0;

public int nb = 0;

async int main () {

	stdout.printf("\n========== Pipex Tester ==========\n");

	Test[] tests = {
		{"infile.txt", "cat", "wc -l", "outfile.txt"},
		{"infile.txt", "grep hello", "wc", "outfile.txt"},
		{"infile.txt", "cat -e", "wc -l", "outfile.txt"},
		{"nofile", "cat", "wc -l", "outfile.txt"},
		{"nofile", "cat -e", "wc -l", "outfile.txt"},
		{"infile.txt", "cat", "wc", "nofile"},
		{"infile.txt", "cat -e", "wc -l", "nofile"},
		{"infile.txt", "ls", "c", "out"},
		{"infile.txt", "l", "cat", "out"},
		{"infile.txt", "ls", " cat", "out"},
		{"", "ls", "wc", "out"},
		{"", "", "cat", "out"},
		{"", "", "", ""},
		{"infile.txt", "cat", " wc", "outfile.txt"},
		{"infile.txt", "cat", "wc ", "outfile.txt"},
		{"infile.txt", "sleep 3", "sleep 5", "outfile.txt"},
		{"infile.txt", "sleep 5", "sleep 3", "outfile.txt"},
	};

	foreach (Test nb in tests) {
		//  ++nb;
		run_test(nb);
		yield;
	}
	
	stdout.printf("\n_____ Results _____\n");
	stdout.printf("Passed: %d\n", passed);
	stdout.printf("Failed: %d\n", failed);

	return 0;
}