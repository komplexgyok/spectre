workspace "spectre"
    architecture "x86"
    startproject "spectre-editor"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "dependencies"
	include "spectre/vendor/assimp"
	include "spectre/vendor/glad"
	include "spectre/vendor/glfw"
	include "spectre/vendor/imgui"
group ""

include "spectre"
include "spectre-editor"
include "sandbox"
