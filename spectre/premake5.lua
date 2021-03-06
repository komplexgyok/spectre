project "spectre"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "source/**.h",
        "source/**.cpp",
        "vendor/stb_image/**.cpp"
    }

    includedirs
    {
        "source",
        "vendor/assimp/assimp/include",
        "vendor/entt/include",
        "vendor/glad/include",
        "vendor/glfw/glfw/include",
        "vendor/glm",
        "vendor/imgui/imgui",
        "vendor/stb_image"
    }

    links
    {
        "assimp",
        "glad",
        "glfw",
        "imgui"
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
            "SPECTRE_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }
