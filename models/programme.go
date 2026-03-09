package models

import (
	"fmt"
	"sync"

	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/fayl"
)

// Meta information about a language-agnostic programme.
type Programme struct {
	projectLock      sync.RWMutex
	Project          *Project `json:"project,omitempty,omitzero" toml:"project,omitempty,omitzero"`
	licenseLock      sync.RWMutex
	License          *License `json:"license,omitempty,omitzero" toml:"license,omitempty,omitzero"`
	tagsLock         sync.RWMutex
	Tags             []string `json:"tags,omitempty,omitzero" toml:"tags,omitempty,omitzero"`
	contributersLock sync.RWMutex
	Contributers     []*Author `json:"contributers,omitempty,omitzero" toml:"contributers,omitempty,omitzero"`
	pathsLock        sync.RWMutex
	Paths            map[string]string `json:"paths,omitempty,omitzero" toml:"paths,omitempty,omitzero"`
	urlsLock         sync.RWMutex
	Urls             map[string]string `json:"urls,omitempty,omitzero" toml:"urls,omitempty,omitzero"`
	commandsLock     sync.RWMutex
	Commands         map[string][]string `json:"commands,omitempty,omitzero" toml:"commands,omitempty,omitzero"`
}

// Construct a new programme.
// Returns a new programme based on a given project.
func NewProgramme(project *Project) *Programme {
	var programme *Programme = new(Programme)
	programme.Project = project
	programme.Tags = make([]string, 0)
	programme.Contributers = make([]*Author, 0)
	programme.Paths = make(map[string]string, 0)
	programme.Urls = make(map[string]string, 0)
	programme.Commands = make(map[string][]string, 0)
	return programme
}

// Obtain the available commands for the project.
// Returns a slice of strings containing all of the available commands.
func (programme *Programme) AvailablePaths() []string {
	programme.RLock()
	defer programme.RUnlock()
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Paths {
		result = append(result, key)
	}
	return result
}

// Obtain the available urls for the project.
// Returns a slice of strings containing all of the available urls.
func (programme *Programme) AvailableUrls() []string {
	programme.RLock()
	defer programme.RUnlock()
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Urls {
		result = append(result, key)
	}
	return result
}

// Obtain the available paths for the project.
// Returns a slice of strings containing all of the available paths.
func (programme *Programme) AvailableCommands() []string {
	programme.RLock()
	defer programme.RUnlock()
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Commands {
		result = append(result, key)
	}
	return result
}

// Add a license to the programme.
func (programme *Programme) AddLicense(license *License) {
	programme.Lock()
	defer programme.Unlock()
	programme.License = license
}

// Add tags to the programme.
func (programme *Programme) AddTags(tags ...string) {
	programme.Lock()
	defer programme.Unlock()
	programme.Tags = append(programme.Tags, tags...)
}

// Add contributers to the programme.
func (programme *Programme) AddContributers(contributers ...*Author) {
	programme.Lock()
	defer programme.Unlock()
	programme.Contributers = append(programme.Contributers, contributers...)
}

// Add a key to a map only if the key is not already stored in the map.
// If the key is already found within the map, a `KeyError` is returned.
func checkedAdd[Type any](mapping *map[string]Type, key string, value Type) *gopolutils.Exception {
	var ok bool
	_, ok = (*mapping)[key]
	if ok {
		return gopolutils.NewNamedException(gopolutils.KeyError, fmt.Sprintf("Can not add an already stored value '%s'.", key))
	}
	(*mapping)[key] = value
	return nil
}

// Add a path to the programme.
// If the path is already found within the programme, a `KeyError` is returned.
func (programme *Programme) AddPath(key, value string) *gopolutils.Exception {
	programme.Lock()
	defer programme.Unlock()
	return checkedAdd(&programme.Paths, key, value)
}

// Add a url to the programme.
// If the url is already found within the programme, a `KeyError` is returned.
func (programme *Programme) AddUrl(key, value string) *gopolutils.Exception {
	programme.Lock()
	defer programme.Unlock()
	return checkedAdd(&programme.Urls, key, value)
}

// Add a command to the programme.
// If the command is already found within the programme, a `KeyError` is returned.
func (programme *Programme) AddCommand(command Command, parts ...string) *gopolutils.Exception {
	programme.Lock()
	defer programme.Unlock()
	return checkedAdd(&programme.Commands, command, parts)
}

// Read a stored path within the programme.
// Returns the path stored at the given key.
// If the key is not found in the programme, a `KeyError` is returned with an empty string.
// If the absolute path of the file can not be obtained, or the file can not be read, an IOError is returned with an empty string.
func (programme *Programme) ReadPath(path string) (string, *gopolutils.Exception) {
	programme.RLock()
	defer programme.RUnlock()
	var ok bool
	var item string
	item, ok = programme.Paths[path]
	if !ok {
		return "", gopolutils.NewNamedException(gopolutils.KeyError, fmt.Sprintf("Can not access path %s.", path))
	}
	var raw []byte
	var except *gopolutils.Exception
	raw, except = fayl.Read(fayl.PathFrom(item))
	if except != nil {
		return "", except
	}
	return string(raw), nil
}

// Read the license file as a string.
// Returns the contents of the license file as string.
// If the absolute path of the file can not be obtained, or the file can not be read, an IOError is returned with an empty string.
func (programme *Programme) ReadLicense() (string, *gopolutils.Exception) {
	programme.RLock()
	defer programme.RUnlock()
	return programme.License.Read()
}

// Lock the internal mutex of the programme writing.
func (programme *Programme) Lock() {
	programme.projectLock.Lock()
	programme.licenseLock.Lock()
	programme.tagsLock.Lock()
	programme.contributersLock.Lock()
	programme.pathsLock.Lock()
	programme.urlsLock.Lock()
	programme.commandsLock.Lock()
}

// Unlock the internal mutex of the programme writing.
func (programme *Programme) Unlock() {
	programme.projectLock.Unlock()
	programme.licenseLock.Unlock()
	programme.tagsLock.Unlock()
	programme.contributersLock.Unlock()
	programme.pathsLock.Unlock()
	programme.urlsLock.Unlock()
	programme.commandsLock.Unlock()
}

// Lock the internal mutex of the programme for reading.
func (programme *Programme) RLock() {
	programme.projectLock.RLock()
	programme.licenseLock.RLock()
	programme.tagsLock.RLock()
	programme.contributersLock.RLock()
	programme.pathsLock.RLock()
	programme.urlsLock.RLock()
	programme.commandsLock.RLock()
}

// Unlock the internal mutex of the programme for reading.
func (programme *Programme) RUnlock() {
	programme.projectLock.RUnlock()
	programme.licenseLock.RUnlock()
	programme.tagsLock.RUnlock()
	programme.contributersLock.RUnlock()
	programme.pathsLock.RUnlock()
	programme.urlsLock.RUnlock()
	programme.commandsLock.RUnlock()
}
