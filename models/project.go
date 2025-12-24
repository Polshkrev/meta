package models

// The meta details of a language-agnostic serializable project.
type Project struct {
	Name        string   `json:"name" toml:"name"`
	Author      string   `json:"author" toml:"author"`
	Description string   `json:"description" toml:"description"`
	Version     *Version `json:"version" toml:"version"`
}

// Construct a new project.
// Returns a new project with its given properties.
func NewProject(name, author, description string, version *Version) *Project {
	var project *Project = new(Project)
	project.Name = name
	project.Author = author
	project.Description = description
	project.Version = version
	return project
}
