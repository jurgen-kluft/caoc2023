package caoc2023

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cfile "github.com/jurgen-kluft/cfile/package"
	ctext "github.com/jurgen-kluft/ctext/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'caoc2023'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	ctextpkg := ctext.GetPackage()
	cfilepkg := cfile.GetPackage()
	cbasepkg := cbase.GetPackage()
	ccorepkg := ccore.GetPackage()

	// The main (ctext) package
	mainpkg := denv.NewPackage("ctext")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(ctextpkg)
	mainpkg.AddPackage(cfilepkg)
	mainpkg.AddPackage(cbasepkg)
	mainpkg.AddPackage(ccorepkg)

	// 'caoc2023' library
	mainlib := denv.SetupDefaultCppLibProject("caoc2023", "github.com\\jurgen-kluft\\caoc2023")
	mainlib.Dependencies = append(mainlib.Dependencies, ctextpkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, cfilepkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, cbasepkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, ccorepkg.GetMainLib())

	// 'caoc2023' unittest project
	maintest := denv.SetupDefaultCppTestProject("caoc2023_test", "github.com\\jurgen-kluft\\caoc2023")
	maintest.Dependencies = append(maintest.Dependencies, cunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, ctextpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, cfilepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, cbasepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, ccorepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
