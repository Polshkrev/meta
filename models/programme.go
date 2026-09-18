package models

import (
	"fmt"

	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/fayl"
)

// Meta information about a language-agnostic programme.
//
// Deprecated: Due to a move to c++, this will be deleted.
type Programme struct {
	Project      *Project            `json:"project,omitempty,omitzero" yaml:"project,omitempty,omitzero" toml:"project,omitempty,omitzero"`
	License      *License            `json:"license,omitempty,omitzero" yaml:"license,omitempty,omitzero" toml:"license,omitempty,omitzero"`
	Tags         []string            `json:"tags,omitempty,omitzero" yaml:"tags,omitempty,omitzero" toml:"tags,omitempty,omitzero"`
	Contributers []*Author           `json:"contributers,omitempty,omitzero" yaml:"contributers,omitempty,omitzero" toml:"contributers,omitempty,omitzero"`
	Paths        map[string]string   `json:"paths,omitempty,omitzero" yaml:"paths,omitempty,omitzero" toml:"paths,omitempty,omitzero"`
	Urls         map[string]string   `json:"urls,omitempty,omitzero" yaml:"urls,omitempty,omitzero" toml:"urls,omitempty,omitzero"`
	Commands     map[string][]string `json:"commands,omitempty,omitzero" yaml:"commands,omitempty,omitzero" toml:"commands,omitempty,omitzero"`
}

// Construct a new programme.
// Returns a new programme based on a given project.
//
// Deprecated: Due to a move to c++, this will be deleted.
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

// Obtain the names of each of the contributers for the project.
// Returns a slice of strings containing all of the names of each of the contributers.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AvailableContributers() []string {
	var result []string = make([]string, 0)
	var i int
	for i = range programme.Contributers {
		var contributer *Author = programme.Contributers[i]
		result = append(result, contributer.Name)
	}
	return result
}

// Obtain the available commands for the project.
// Returns a slice of strings containing all of the available commands.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AvailablePaths() []string {
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Paths {
		result = append(result, key)
	}
	return result
}

// Obtain the available urls for the project.
// Returns a slice of strings containing all of the available urls.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AvailableUrls() []string {
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Urls {
		result = append(result, key)
	}
	return result
}

// Obtain the available paths for the project.
// Returns a slice of strings containing all of the available paths.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AvailableCommands() []string {
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Commands {
		result = append(result, key)
	}
	return result
}

// Add a license to the programme.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AddLicense(license *License) {
	programme.License = license
}

// Add tags to the programme.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AddTags(tags ...string) {
	programme.Tags = append(programme.Tags, tags...)
}

// Add contributers to the programme.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AddContributers(contributers ...*Author) {
	programme.Contributers = append(programme.Contributers, contributers...)
}

// Add a key to a map only if the key is not already stored in the map.
// If the key is already found within the map, a [gopolutils.KeyError] is returned.
//
// Deprecated: Due to a move to c++, this will be deleted.
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
// If the path is already found within the programme, a [gopolutils.KeyError] is returned.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AddPath(key, value string) *gopolutils.Exception {
	return checkedAdd(&programme.Paths, key, value)
}

// Add a url to the programme.
// If the url is already found within the programme, a [gopolutils.KeyError] is returned.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AddUrl(key, value string) *gopolutils.Exception {
	return checkedAdd(&programme.Urls, key, value)
}

// Add a command to the programme.
// If the command is already found within the programme, a [gopolutils.KeyError] is returned.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) AddCommand(command Command, parts ...string) *gopolutils.Exception {
	return checkedAdd(&programme.Commands, command.String(), parts)
}

// Read a stored path within the programme.
// Returns the path stored at the given key.
// If the key is not found in the programme, a [gopolutils.KeyError] is returned with an empty string.
// If the absolute path of the file can not be obtained, or the file can not be read, an [gopolutils.IOError] is returned with an empty string.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) ReadPath(path string) (string, *gopolutils.Exception) {
	var ok bool
	var item string
	item, ok = programme.Paths[path]
	if !ok {
		return "", gopolutils.NewNamedException(gopolutils.KeyError, "Can not access path %s.", path)
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
// If the license is nil, an [gopolutils.IOError] is returned with an empty string.
// If the license is determined to be incomplete or empty, a [gopolutils.ValueError] is returned with an empty string.
// If the absolute path of the file can not be obtained, or the file can not be read, an [gopolutils.IOError] is returned with an empty string.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) ReadLicense() (string, *gopolutils.Exception) {
	if programme.License == nil {
		return "", gopolutils.NewNamedException(gopolutils.IOError, "No license has been provided for project '%s'.", programme)
	} else if programme.License.IsEmpty() {
		return "", gopolutils.NewNamedException(gopolutils.ValueError, "The license for project '%s' is either incomplete or empty.", programme)
	}
	return programme.License.Read()
}

// Represent a programme as a string.
// Returns a string representation of a programme.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (programme *Programme) String() string {
	return programme.Project.Name
}
