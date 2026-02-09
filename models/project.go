package models

// The meta details of a language-agnostic serializable project.
type Project struct {
	Name        string   `json:"name,omitempty,omitzero" toml:"name,omitempty,omitzero"`
	Author      string   `json:"author,omitempty,omitzero" toml:"author,omitempty,omitzero"`
	Description string   `json:"description,omitempty,omitzero" toml:"description,omitempty,omitzero"`
	Version     *Version `json:"version,omitempty,omitzero" toml:"version,omitempty,omitzero"`
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
