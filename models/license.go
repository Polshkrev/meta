package source

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
