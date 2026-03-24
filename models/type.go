package models

import "github.com/Polshkrev/gopolutils"

// Type of a programme.
type Type gopolutils.StringEnum

const (
	None          Type = ""
	Api           Type = "api"
	Bot           Type = "bot"
	Documentation Type = "documentation"
	Game          Type = "game"
	Library       Type = "library"
	List          Type = "list"
	Application   Type = "application"
)

// Represent a type of a programme as a string.
// Returns a type of a programme as a string.
func (kind Type) String() string {
	return string(kind)
}
