package models

import "sync"

// The meta details of a language-agnostic serializable project.
type Project struct {
	nameLock        sync.RWMutex
	Name            string `json:"name,omitempty,omitzero" toml:"name,omitempty,omitzero"`
	authorLock      sync.RWMutex
	Author          *Author `json:"author,omitempty,omitzero" toml:"author,omitempty,omitzero"`
	descriptionLock sync.RWMutex
	Description     string `json:"description,omitempty,omitzero" toml:"description,omitempty,omitzero"`
	versionLock     sync.RWMutex
	Version         *Version `json:"version,omitempty,omitzero" toml:"version,omitempty,omitzero"`
}

// Construct a new project.
// Returns a new project with its given properties.
func NewProject(name string, author *Author, description string, version *Version) *Project {
	var project *Project = new(Project)
	project.Name = name
	project.Author = author
	project.Description = description
	project.Version = version
	return project
}

// Lock the internal mutex of the project writing.
func (project *Project) Lock() {
	project.nameLock.Lock()
	project.authorLock.Lock()
	project.descriptionLock.Lock()
	project.versionLock.Lock()
}

// Unlock the internal mutex of the project writing.
func (project *Project) Unlock() {
	project.nameLock.Unlock()
	project.authorLock.Unlock()
	project.descriptionLock.Unlock()
	project.versionLock.Unlock()
}

// Lock the internal mutex of the project for reading.
func (project *Project) RLock() {
	project.nameLock.RLock()
	project.authorLock.RLock()
	project.descriptionLock.RLock()
	project.versionLock.RLock()
}

// Unlock the internal mutex of the project for reading.
func (project *Project) RUnlock() {
	project.nameLock.RUnlock()
	project.authorLock.RUnlock()
	project.descriptionLock.RUnlock()
	project.versionLock.RUnlock()
}
