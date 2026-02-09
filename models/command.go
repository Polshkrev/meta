package models

import "github.com/Polshkrev/gopolutils"

// The available command options.
type Command = gopolutils.StringEnum

const (
	Build  Command = "build"
	Run    Command = "run"
	Test   Command = "test"
	Serve  Command = "serve"
	Docs   Command = "docs"
	Dist   Command = "dist"
	Script Command = "script"
)
