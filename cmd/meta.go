package main

import (
	"cmp"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"strings"

	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/collections"
	"github.com/Polshkrev/gopolutils/fayl"
	"github.com/Polshkrev/meta/models"
)

const (
	Folder   string      = "."
	FileName string      = "meta"
	FileType fayl.Suffix = fayl.Toml
)

var (
	mapping collections.Mapping[string, []string] = collections.NewMap[string, []string]()
)

func initializeMapping(mapping collections.Mapping[string, []string], programme *models.Programme) {
	var except *gopolutils.Exception = mapping.Insert("tags", programme.Tags)
	if except != nil {
		panic(except)
	}
	except = mapping.Insert("contributers", programme.AvailableContributers())
	if except != nil {
		panic(except)
	}
	except = mapping.Insert("paths", programme.AvailablePaths())
	if except != nil {
		panic(except)
	}
	except = mapping.Insert("urls", programme.AvailableUrls())
	if except != nil {
		panic(except)
	}
	except = mapping.Insert("commands", programme.AvailableCommands())
	if except != nil {
		panic(except)
	}
}

func getAvailableKeys(key string, mapping collections.Mapping[string, []string]) ([]string, *gopolutils.Exception) {
	var keys *[]string
	var except *gopolutils.Exception
	keys, except = mapping.At(key)
	if except != nil {
		return nil, except
	}
	return *keys, nil
}

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
		fmt.Fprintln(os.Stderr, gopolutils.NewNamedException(gopolutils.RuntimeError, "No arguments have been provided."))
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

func run(programme *models.Programme, intent models.Command) string {
	var outputChannel chan string = make(chan string, 20)
	var errorChannel chan error = make(chan error, 20)
	go runCommand(programme, intent, outputChannel, errorChannel)
	var output string = <-outputChannel
	var outputError error = <-errorChannel
	if outputError != nil {
		panic(gopolutils.NewNamedException(gopolutils.IOError, outputError.Error()))
	}
	return output
}

func main() {
	var programme *models.Programme = readMeta(fayl.PathFromParts(Folder, FileName, FileType))
	var versionFlag *bool = flag.Bool("v", false, "Display the version of the programme.")
	var licenseFlag *bool = flag.Bool("l", false, "Display the license of the programme.")
	var listFlag *string = flag.String("a", "", "Display all available keys at the given label.")
	flag.Parse()
	if *versionFlag {
		fmt.Printf("%s - %s", programme.Project.Name, programme.Project.Version.ToString())
		os.Exit(0)
	} else if *licenseFlag {
		fmt.Print(gopolutils.Must(programme.ReadLicense()))
		os.Exit(0)
	} else if *listFlag != "" {
		initializeMapping(mapping, programme)
		var keys []string = gopolutils.Must(getAvailableKeys(*listFlag, mapping))
		var i int
		for i = range keys {
			var key string = keys[i]
			fmt.Println(key)
		}
		os.Exit(0)
	}
	var intent models.Command = cmp.Or(
		flag.Arg(0),
		models.Script,
	)
	var output string = run(programme, intent)
	fmt.Print(output) // TODO: Make this a redirect instead of simply printing.
}
