local libDirectory

project "GoogleTest"

    kind "StaticLib"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    libDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"

    filter "system:windows"
        kind "Utility"
        prebuildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        prebuildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "RayLib"

    kind "StaticLib"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    libDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"

    filter "system:windows"
        kind "Utility"
        prebuildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_INSTALL_INCLUDEDIR=include/raylib -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "Entt"
    kind "StaticLib"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    libDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "/%{prj.name}\""

    filter "system:windows"
        kind "Utility"
        prebuildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DENTT_USE_LIBCPP=true -DENTT_INSTALL=true -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "LuaJIT"

    kind "StaticLib"
    location(rootPath .. "/Generated/Projects")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    libDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "/%{prj.name}"
    local includeCopyPath = "\"" .. targetBuildPath .. "/External/include/%{prj.name}\""
    local libCopyPath = "\"" .. targetBuildPath .. "/External/lib\""

    filter "system:windows"
        kind "Utility"

        prebuildcommands {
            "{CHDIR} " .. rootPath .. "/External/LuaJIT/src",
            "start /wait /b msvcbuild static",

            "{MKDIR} " .. includeCopyPath,
            "{MKDIR} " .. libCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/lua.hpp\" " .. includeCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/lua.h\" " .. includeCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/luaconf.h\" " .. includeCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/lauxlib.h\" " .. includeCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/lualib.h\" " .. includeCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/luajit.h\" " .. includeCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/lua51.lib\" " .. libCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/luajit.lib\" " .. libCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/minilua.lib\" " .. libCopyPath,
            "{COPY} \"" .. rootPath .. "/External/LuaJIT/src/luajit.pdb\" " .. libCopyPath,
            
            -- Remove because of gitignore thingy
            "{DELETE} \"" .. rootPath .. "/External/LuaJIT/src/luajit.pdb\"",
            "{DELETE} \"" .. rootPath .. "/External/LuaJIT/src/vc140.pdb\""
        }

    --[[filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }]]--
