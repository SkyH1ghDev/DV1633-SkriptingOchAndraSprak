project "Application"

    kind "ConsoleApp"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.hpp", "src/**.cpp"}
    includedirs{"../Library/include", targetBuildPath .. "/External/Include"}

    -- LibDirs doesn't work for some reason. MSVC doesn't find raylib
    --[[libdirs
    {
        targetBuildPath .. "/External/lib"
    }]]--

    dependson("RayLib", "LuaJIT", "Library", "EnTT")

    links
    {
        "Library",
        targetBuildPath .. "/External/lib/raylib", "winmm", -- Raylib required libraries
        targetBuildPath .. "/External/lib/luajit", targetBuildPath .. "/External/lib/lua51" -- LuaJIT required libraries
    }