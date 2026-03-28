package main

import (
	"cmp"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"strings"

	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/collections/safe"
	"github.com/Polshkrev/gopolutils/fayl"
	"github.com/Polshkrev/meta/models"
)

const (
	Folder   string      = "."       // Parent folder where the target file is stored.
	FileName string      = "meta"    // Name of the target file.
	FileType fayl.Suffix = fayl.Toml // Suffix of the target file.
)

var (
	// Mapping of each of the available keys within a given tag.
	mapping safe.Mapping[string, []string] = safe.NewMap[string, []string]()
)

// Intialize a given mapping with each of the programme's properties.
// If any of the insertions fails, the function panics.
func initializeMapping(mapping safe.Mapping[string, []string], programme *models.Programme) {
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

// Obtain the available keys within a given mapping at a given tag.
// Returns the available keys within the mapping at a given tag.
// If the mapping is empty, a [gopolutils.ValueError] is returned with a nil data pointer.
// If the key is not in the mapping, a [gopolutils.KeyError] is returned with a nil data pointer.
func getAvailableKeys(key string, mapping safe.Mapping[string, []string]) ([]string, *gopolutils.Exception) {
	var keys *[]string
	var except *gopolutils.Exception
	keys, except = mapping.At(key)
	if except != nil {
		return nil, except
	}
	return *keys, nil
}

// Read the given target file.
// Returns a new programme based on the given [fayl.Path].
// If the given [fayl.Path] does not exist, the function panics with a [gopolutils.FileNotFoundError].
func readMeta(path *fayl.Path) *models.Programme {
	if !path.Exists() {
		panic(gopolutils.NewNamedException(gopolutils.FileNotFoundError, "File '%s' does not exist.", path))
	}
	return gopolutils.Must(fayl.ReadObject[models.Programme](path))
}

// Obtain the output of the given command.
func getOutput(command *exec.Cmd, outputChannel chan<- string, errorChannel chan<- error) {
	defer close(outputChannel)
	defer close(errorChannel)
	var output []byte
	var outputError error
	output, outputError = command.CombinedOutput()
	outputChannel <- string(output)
	errorChannel <- outputError
}

// Handle the script option given to the given command result.
func handleScript(command []string, result **exec.Cmd) {
	var commandPath *fayl.Path = gopolutils.Must(fayl.PathFrom(command[0]).Absolute())
	*result = exec.Command(commandPath.String(), command[1:]...)
}

// Obtain the available commands of the given programme.
func availableCommands(programme *models.Programme, intent models.Command, command []string) {
	var availableCommands string = fmt.Sprintf("[%s]", strings.Join(programme.AvailableCommands(), ", "))
	if len(command) != 0 || command != nil {
		return
	}
	fmt.Fprintln(os.Stderr, gopolutils.NewException(fmt.Sprintf("No command '%s' has been defined for '%s'.\nAvailable Commands: %s", intent, programme.Project.Name, availableCommands)))
	os.Exit(1)
}

// Run a specified programme with a given intent.
func runCommand(programme *models.Programme, intent models.Command, outputChannel chan<- string, errorChannel chan<- error) {
	if len(intent) == 0 {
		fmt.Fprintln(os.Stderr, gopolutils.NewNamedException(gopolutils.RuntimeError, "No arguments have been provided."))
		flag.Usage()
		os.Exit(1)
	}
	var command []string = programme.Commands[intent.String()]
	availableCommands(programme, intent, command)
	var cmd *exec.Cmd = exec.Command(command[0], command[1:]...)
	if intent == models.Script {
		handleScript(command, &cmd)
	}
	go getOutput(cmd, outputChannel, errorChannel)
}

// Run a given programme at a given intent.
// Returns the string representation of the executed command.
func run(programme *models.Programme, intent models.Command) string {
	var outputChannel chan string = make(chan string, 1)
	var errorChannel chan error = make(chan error, 1)
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
		fmt.Printf("%s - %s", programme.Project.Name, programme.Project.Version)
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
	var intent string = cmp.Or(
		flag.Arg(0),
		models.Script.String(),
	)
	var command models.Command = models.Command(intent)
	if !command.IsValid() {
		panic(gopolutils.NewNamedException(gopolutils.ValueError, "'%s' is not a valid command.", intent))
	}
	var output string = run(programme, command)
	fmt.Print(output) // TODO: Make this a redirect instead of simply printing.
}
