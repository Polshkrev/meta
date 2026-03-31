package models

// Representation of an author of a project.
type Author struct {
	Name  string `json:"name,omitempty" yaml:"name,omitempty" toml:"name,omitempty"`
	Email string `json:"email,omitempty" yaml:"email,omitempty" toml:"email,omitempty"`
}

// Construct a new project author.
// Returns a new author with a given name and email.
func NewAuthor(name, email string) *Author {
	var author *Author = new(Author)
	author.Name = name
	author.Email = email
	return author
}
