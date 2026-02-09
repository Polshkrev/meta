package models

import (
	"github.com/Polshkrev/gopolutils"
	"github.com/Polshkrev/gopolutils/fayl"
)

// A license a programme uses.
type License struct {
	Type string `json:"type" toml:"type"`
	Path string `json:"path" toml:"path"`
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
// If the absolute path of the file can not be obtained, or the file can not be read, an IOError is returned with an empty string.
func (license License) Read() (string, *gopolutils.Exception) {
	var raw []byte
	var except *gopolutils.Exception
	raw, except = fayl.Read(fayl.PathFrom(license.Path))
	if except != nil {
		return "", except
	}
	return string(raw), nil
}
