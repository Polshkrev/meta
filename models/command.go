package models

import "github.com/Polshkrev/gopolutils"

// The available command options.
type Command gopolutils.StringEnum

const (
	Build  Command = "build"
	Run    Command = "run"
	Test   Command = "test"
	Serve  Command = "serve"
	Docs   Command = "docs"
	Dist   Command = "dist"
	Help   Command = "help"
	Script Command = "script"
)

// Determine if the command is valid.
// Returns true if the command has been defined in the command enum, else false.
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
func (command Command) String() string {
	return string(command)
}
