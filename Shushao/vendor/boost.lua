project "Boost"
    basedir ("boost")

    -- first time:
    --  in msvc from developer command prompt
    --  bootstrap.bat
    --  bjam headers

    kind 'StaticLib'
    targetdir ("%{prj.location}/stage/lib")
    objdir ("%{prj.location}/stage/lib")

    configuration { "gmake2" }
        buildcommands { "./b2 toolset=gcc --build-type=complete stage cxxflags=\"-std=c++17\" -mt --with-coroutine" }

    configuration { "vs2019" }
        prebuildcommands {
            (path.translate("bjam toolset=msvc link=static threading=multi address-model=64 runtime-link=static --with-coroutine stage"))
        }