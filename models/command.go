package models

import "github.com/Polshkrev/gopolutils"

// The available command options.
//
// Deprecated: Due to a move to c++, this will be deleted.
type Command gopolutils.StringEnum

const (
	// Deprecated: Due to a move to c++, this will be deleted.
	Build Command = "build"
	// Deprecated: Due to a move to c++, this will be deleted.
	Run Command = "run"
	// Deprecated: Due to a move to c++, this will be deleted.
	Test Command = "test"
	// Deprecated: Due to a move to c++, this will be deleted.
	Serve Command = "serve"
	// Deprecated: Due to a move to c++, this will be deleted.
	Docs Command = "docs"
	// Deprecated: Due to a move to c++, this will be deleted.
	Dist Command = "dist"
	// Deprecated: Due to a move to c++, this will be deleted.
	Help Command = "help"
	// Deprecated: Due to a move to c++, this will be deleted.
	Script Command = "script"
)

// Determine if the command is valid.
// Returns true if the command has been defined in the command enum, else false.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (command Command) IsValid() bool {
	switch command {
	case Build:
		return true
	case Run:
		return true
	case Test:
		return true
	case Serve:
		return true
	case Docs:
		return true
	case Dist:
		return true
	case Help:
		return true
	case Script:
		return true
	default:
		return false
	}
}

// Represent a command as a string.
// Returns a string representation of a command.
//
// Deprecated: Due to a move to c++, this will be deleted.
func (command Command) String() string {
	return string(command)
}
