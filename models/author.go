package models

import "sync"

type Author struct {
	nameLock  sync.RWMutex
	Name      string `json:"name,omitempty" toml:"name,omitempty"`
	emailLock sync.RWMutex
	Email     string `json:"email,omitempty" toml:"email,omitempty"`
}

// Construct a new project author.
// Returns a new author with a given name and email.
func NewAuthor(name, email string) *Author {
	var author *Author = new(Author)
	author.Name = name
	author.Email = email
	return author
}

// Lock the internal mutex of the author writing.
func (author *Author) Lock() {
	author.nameLock.Lock()
	author.emailLock.Lock()
}

// Unlock the internal mutex of the author writing.
func (author *Author) Unlock() {
	author.nameLock.Unlock()
	author.emailLock.Unlock()
}

// Lock the internal mutex of the author for reading.
func (author *Author) RLock() {
	author.nameLock.RLock()
	author.emailLock.RLock()
}

// Unlock the internal mutex of the author for reading.
func (author *Author) RUnlock() {
	author.nameLock.RUnlock()
	author.emailLock.RUnlock()
}
