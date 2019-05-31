project 'Freetype'
    basedir ("freetype")
    kind 'StaticLib'
    targetdir ("%{prj.location}/objs")
    objdir ("%{prj.location}/objs")

    configuration { "gmake2" }
        buildcommands { "../../../vendor/ftjam/jam -sJAM_TOOLSET=MINGW" }

    configuration { "vs2019" }
        prebuildcommands { path.translate("../../../vendor/ftjam/jam -sJAM_TOOLSET=VISUALC") }