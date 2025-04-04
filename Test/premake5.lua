project "Test"

    kind "ConsoleApp"
    location(rootPath .. "/Generated")

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.h", "src/**.cpp"}
    includedirs{"../Library/include", targetBuildPath .. "/External/include"}

    libdirs{targetBuildPath .. "/External/lib"}

    -- Although this consoleApp does not link to the "googletest" project per say, it is still dependant on its build to finish
    dependson {"googletest"}

    links{"Library", "gtest"}