package main

import (
	"cmp"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"strings"

	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/fayl"
	"github.com/Polshkrev/meta/models"
)

const (
	Folder   string      = "."
	FileName string      = "meta"
	FileType fayl.Suffix = fayl.Toml
)

func readMeta(path *fayl.Path) *models.Programme {
	if !path.Exists() {
		fmt.Fprintln(os.Stderr, gopolutils.NewNamedException(gopolutils.FileNotFoundError, fmt.Sprintf("File '%s' does not exist.", path.ToString())))
		os.Exit(1)
	}
	return gopolutils.Must(fayl.ReadObject[models.Programme](path))
}

func getOutput(command *exec.Cmd, outputChannel chan<- string, errorChannel chan<- error) {
	var output []byte
	var outputError error
	output, outputError = command.CombinedOutput()
	outputChannel <- string(output)
	errorChannel <- outputError
	defer close(outputChannel)
	defer close(errorChannel)
}

func handleScript(command []string, result **exec.Cmd) {
	var commandPath *fayl.Path = gopolutils.Must(fayl.PathFrom(command[0]).Absolute())
	*result = exec.Command(commandPath.ToString(), command[1:]...)
}

func availableCommands(programme *models.Programme, intent models.Command, command []string) {
	var availableCommands string = fmt.Sprintf("[%s]", strings.Join(programme.AvailableCommands(), ", "))
	if len(command) == 0 || command == nil {
		fmt.Fprintln(os.Stderr, gopolutils.NewException(fmt.Sprintf("No command '%s' has been defined for '%s'.\nAvailable Commands: %s", intent, programme.Project.Name, availableCommands)))
		os.Exit(1)
	}
}

func runCommand(programme *models.Programme, intent models.Command, outputChannel chan<- string, errorChannel chan<- error) {
	if len(intent) == 0 {
		fmt.Fprintln(os.Stderr, gopolutils.NewException("No arguments have been provided."))
		flag.Usage()
		os.Exit(1)
	}
	var command []string = programme.Commands[intent]
	availableCommands(programme, intent, command)
	var cmd *exec.Cmd = exec.Command(command[0], command[1:]...)
	if intent == models.Script {
		handleScript(command, &cmd)
	}
	go getOutput(cmd, outputChannel, errorChannel)
}

func main() {
	var programme *models.Programme = readMeta(fayl.PathFromParts(Folder, FileName, FileType))
	flag.Parse()
	var intent models.Command = cmp.Or(
		flag.Arg(0),
		models.Script,
	)
	var outputChannel chan string = make(chan string, 1)
	var errorChannel chan error = make(chan error, 1)
	go runCommand(programme, intent, outputChannel, errorChannel)
	var output string = <-outputChannel
	var outputError error = <-errorChannel
	if outputError != nil {
		panic(outputError)
	}
	fmt.Print(output) // TODO: Make this a redirect instead of simply printing.
}
