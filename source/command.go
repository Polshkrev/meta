package source

// The available command options.
type Command = string

const (
	BUILD  Command = "build"
	RUN    Command = "run"
	TEST   Command = "test"
	SERVE  Command = "serve"
	DOCS   Command = "docs"
	DIST   Command = "dist"
	SCRIPT Command = "script"
)
