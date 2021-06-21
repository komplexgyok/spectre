workspace "spectre"
    architecture "x86_64"
    startproject "sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "spectre"
    location "spectre"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/source/**.h",
        "%{prj.name}/source/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/source"
    }

    filter "configurations:Debug"
        defines "SPECTRE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "SPECTRE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SPECTRE_PLATFORM_WINDOWS"
        }

project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/source/**.h",
        "%{prj.name}/source/**.cpp"
    }

    includedirs
    {
        "spectre/source"
    }

    links
    {
        "spectre"
    }

    filter "configurations:Debug"
        defines "SPECTRE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "SPECTRE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SPECTRE_PLATFORM_WINDOWS"
        }
