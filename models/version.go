package models

import (
	"fmt"

	"github.com/Polshkrev/gopolutils"
)

// Representation of a semantic versioning object.
type Version struct {
	Name        string `json:"name,omitempty,omitzero" toml:"name,omitempty,omitzero"`
	Description string `json:"description,omitempty,omitzero" toml:"description,omitempty,omitzero"`
	Major       uint8  `json:"major,omitempty,omitzero" toml:"major,omitempty,omitzero"`
	Minor       uint8  `json:"minor,omitempty,omitzero" toml:"minor,omitempty,omitzero"`
	Patch       uint8  `json:"patch,omitempty,omitzero" toml:"patch,omitempty,omitzero"`
}

// Construct a new zero-initialized version object.
// All the string properties are empty and the numeric properties are set to zero.
// Returns a pointer to a new version object.
func NewVersion() *Version {
	return new(Version)
}

// Construct a new version object with given numeric values.
// The string properties of the version object are empty.
// Returns a pointer to a new version object with each of the numeric properties initialized with the given parametres.
func VersionConvert(major, minor, patch uint8) *Version {
	var version *Version = NewVersion()
	version.Major = major
	version.Minor = minor
	version.Patch = patch
	return version
}

// Construct a new version object initialized with a given name.
// Returns a pointer to a new version object initialized with the given name.
func NewNamedVersion(name string) *Version {
	var version *Version = NewVersion()
	version.Name = name
	return version
}

// Construct a new version object with each of its string properties initialized.
// Returns a pointer to a new version object initialized with the given string parametres.
func NewStringVersion(name, description string) *Version {
	var version *Version = NewNamedVersion(name)
	version.Description = description
	return version
}

// Construct a full initialized version object.
// Returns a pointer to a new fully initialized version object.
func NewFullVersion(name, description string, major, minor, patch uint8) *Version {
	var version *Version = VersionConvert(major, minor, patch)
	version.Name = name
	version.Description = description
	return version
}

// Determine if the version object's major property is greater than or equal to the given operand.
// Returns true if the version object's major property is greater than or equal to the given operand.
func (version Version) CompareMajor(major uint8) bool {
	return version.Major >= major
}

// Determine if the version object's minor property is greater than or equal to the given operand.
// Returns true if the version object's minor property is greater than or equal to the given operand.
func (version Version) CompareMinor(minor uint8) bool {
	return version.Minor >= minor
}

// Determine if the version object's patch property is greater than or equal to the given operand.
// Returns true if the version object's patch property is greater than or equal to the given operand.
func (version Version) ComparePatch(patch uint8) bool {
	return version.Patch >= patch
}

// Compare each of the numeric properties of the version object to a given operand.
// Returns true if each of the version object's properties are greater than or equal to the given operand's numeric properties.
func (version Version) Compare(operand Version) bool {
	return version.CompareMajor(operand.Major) && version.CompareMinor(operand.Minor) && version.ComparePatch(operand.Patch)
}

// Determine if the version object is equal to zero.
// Returns true if each of the version object's numeric properties are equal to zero, else false.
func (version Version) IsZero() bool {
	return version.Major == 0 && version.Minor == 0 && version.Patch == 0
}

// Determine if the version object is public.
// Returns true if the version object's major property is evaluated greater than or equal to 1.
func (version Version) IsPublic() bool {
	return version.CompareMajor(1)
}

// Publish a version object.
// Set the version object's major property to 1.
// Zero-out all other numeric properties.
// If the version object is evaluated to have already been published, a ValueError is returned and no properties are modified.
func (version *Version) Publish() *gopolutils.Exception {
	if version.IsPublic() {
		return gopolutils.NewNamedException(gopolutils.ValueError, "Version is already public.")
	}
	version.Major = 1
	version.Minor = 0
	version.Patch = 0
	return nil
}

// Increment the version object's major property.
// The version object's minor and patch properties are set to 0.
func (version *Version) Release() {
	version.Major++
	version.Minor = 0
	version.Patch = 0
}

// Increment the version object's minor property.
// The version object's patch property is set to 0.
// The version object's major version is not modified.
func (version *Version) Update() {
	version.Minor++
	version.Patch = 0
}

// Increment the version object's patch property.
// The version object's major and minor property are not modified.
func (version *Version) Fix() {
	version.Patch++
}

// Render a string representation of the version object.
// Returns a version object represented as a string.
func (version Version) ToString() string {
	if len(version.Name) == 0 && len(version.Description) == 0 {
		return version.NumberString()
	} else if len(version.Name) == 0 && len(version.Description) != 0 {
		return fmt.Sprintf("%s - %s", version.NumberString(), version.Description)
	} else if len(version.Name) != 0 && len(version.Description) == 0 {
		return fmt.Sprintf("%s: %s", version.Name, version.NumberString())
	}
	return fmt.Sprintf("%s: %s - %s", version.Name, version.NumberString(), version.Description)
}

// Render a string representation of the version object's numeric properties.
// Returns the version object's numeric properties represented as a string.
func (version Version) NumberString() string {
	return fmt.Sprintf("%d.%d.%d", version.Major, version.Minor, version.Patch)
}
