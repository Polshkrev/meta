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
	"github.com/Polshkrev/goserialize"
	"github.com/Polshkrev/meta/source"
)

const (
	FOLDER   string = "."
	FILENAME string = "meta"
	FILETYPE string = goserialize.TOMLType
)

func readMeta(path *fayl.Path) *source.Programme {
	if !path.Exists() {
		fmt.Fprintln(os.Stderr, gopolutils.NewNamedException("IOError", fmt.Sprintf("File '%s' does not exist.", path.ToString())))
		os.Exit(1)
	}
	return gopolutils.Must(fayl.ReadObject[source.Programme](path))
}

func getOutput(command *exec.Cmd, outputChannel chan<- string) {
	var output []byte
	output, _ = command.CombinedOutput()
	outputChannel <- string(output)
	defer close(outputChannel)
}

func handleScript(command []string, result **exec.Cmd) {
	var commandPath *fayl.Path = gopolutils.Must(fayl.PathFrom(command[0]).Absolute())
	*result = exec.Command(commandPath.ToString(), command[1:]...)
}

func availableCommands(programme *source.Programme, intent source.Command, command []string) {
	var availableCommands string = fmt.Sprintf("[%s]", strings.Join(programme.AvailableCommands(), ", "))
	if len(command) == 0 || command == nil {
		fmt.Fprintln(os.Stderr, gopolutils.NewException(fmt.Sprintf("No command '%s' has been defined for '%s'.\nAvailable Commands: %s", intent, programme.Project.Name, availableCommands)))
		os.Exit(1)
	}
}

func main() {
	var programme *source.Programme = readMeta(fayl.PathFromParts(FOLDER, FILENAME, FILETYPE))
	var intent source.Command = cmp.Or(
		flag.Arg(0),
		source.SCRIPT,
	)
	var outputChannel chan string = make(chan string, 1)
	go runCommand(programme, intent, outputChannel)
	var output string = <-outputChannel
	fmt.Print(output) // TODO: Make this a redirect instead of simply printing.
}
