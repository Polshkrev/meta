package models

// Meta information about a language-agnostic programme.
type Programme struct {
	Project      *Project            `json:"project" toml:"project"`
	License      License             `json:"license" toml:"license"`
	Tags         []string            `json:"tags" toml:"tags"`
	Contributers []string            `json:"contributers" toml:"contributers"`
	Paths        map[string]string   `json:"paths" toml:"paths"`
	Urls         map[string]string   `json:"urls" toml:"urls"`
	Commands     map[string][]string `json:"commands" toml:"commands"`
}

// Construct a new programme.
// Returns a new programme based on a given project.
func NewProgramme(project *Project) *Programme {
	var programme *Programme = new(Programme)
	programme.Project = project
	return programme
}

// Obtain the available commands for the project.
// Returns a slice of strings containing all of the available commands.
func (programme Programme) AvailableCommands() []string {
	var result []string = make([]string, 0)
	var key string
	for key = range programme.Commands {
		result = append(result, key)
	}
	return result
}
