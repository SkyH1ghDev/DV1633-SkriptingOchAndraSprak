project "Application"

    kind "ConsoleApp"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.hpp", "src/**.cpp"}
    includedirs{"../Library/include", targetBuildPath .. "/External/Include"}

    dependson("RayLib")

    links
    {
        "Library",
        targetBuildPath .. "/External/lib/raylib", "winmm" -- Raylib required libraries
    }