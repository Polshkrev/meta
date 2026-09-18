package models

// The meta details of a language-agnostic serializable project.
//
// Deprecated: Due to a move to c++, this will be deleted.
type Project struct {
	Name        string   `json:"name,omitempty,omitzero" yaml:"name,omitempty,omitzero" toml:"name,omitempty,omitzero"`
	Author      *Author  `json:"author,omitempty,omitzero" yaml:"author,omitempty,omitzero" toml:"author,omitempty,omitzero"`
	Description string   `json:"description,omitempty,omitzero" yaml:"description,omitempty,omitzero" toml:"description,omitempty,omitzero"`
	Type        Type     `json:"type,omitempty,omitzero" yaml:"type,omitempty,omitzero" toml:"type,omitempty,omitzero"`
	Version     *Version `json:"version,omitempty,omitzero" yaml:"version,omitempty,omitzero" toml:"version,omitempty,omitzero"`
}

// Construct a new project.
// Returns a new project with its given properties.
//
// Deprecated: Due to a move to c++, this will be deleted.
func NewProject(name string, author *Author, description string, version *Version) *Project {
	var project *Project = new(Project)
	project.Name = name
	project.Author = author
	project.Description = description
	project.Version = version
	return project
}
