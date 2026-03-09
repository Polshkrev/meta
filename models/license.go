package models

import (
	"sync"

	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/fayl"
)

// A license a programme uses.
type License struct {
	typeLock sync.RWMutex
	Type     string `json:"type" toml:"type"`
	pathLock sync.RWMutex
	Path     string `json:"path" toml:"path"`
}

// Construct a new license.
// Returns a new license based on its given properties.
func NewLicense(kind, path string) *License {
	var license *License = new(License)
	license.Type = kind
	license.Path = path
	return license
}

// Read the license file as a string.
// Returns the contents of the license file as string.
// If the absolute path of the file can not be obtained, or the file can not be read, an [gopolutils.IOError] is returned with an empty string.
func (license *License) Read() (string, *gopolutils.Exception) {
	license.RLock()
	defer license.RUnlock()
	var raw []byte
	var except *gopolutils.Exception
	raw, except = fayl.Read(fayl.PathFrom(license.Path))
	if except != nil {
		return "", except
	}
	return string(raw), nil
}

// Lock the internal mutex of the license writing.
func (license *License) Lock() {
	license.typeLock.Lock()
	license.pathLock.Lock()
}

// Unlock the internal mutex of the license writing.
func (license *License) Unlock() {
	license.typeLock.Unlock()
	license.pathLock.Unlock()
}

// Lock the internal mutex of the license for reading.
func (license *License) RLock() {
	license.typeLock.RLock()
	license.pathLock.RLock()
}

// Unlock the internal mutex of the license for reading.
func (license *License) RUnlock() {
	license.typeLock.RUnlock()
	license.pathLock.RUnlock()
}
