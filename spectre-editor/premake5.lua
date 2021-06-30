project "spectre-editor"
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
        "%{wks.location}/spectre/source",
        "%{wks.location}/spectre/vendor",
        "%{wks.location}/spectre/vendor/glm",
        "%{wks.location}/spectre/vendor/glad/include",
        "%{wks.location}/spectre/vendor/glfw/glfw/include",
        "%{wks.location}/spectre/vendor/imgui/imgui"
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
