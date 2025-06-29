project "Library"

    kind "StaticLib"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"include/**.hpp", "src/**.cpp"}
    includedirs{"include/", targetBuildPath .. "/External/Include", targetBuildPath .. "/External/Include/LuaJIT"}

    dependson{"Sol", "Lua"}