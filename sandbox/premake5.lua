project "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "source/**.h",
        "source/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/spectre/source"
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
