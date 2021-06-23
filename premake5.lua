workspace "spectre"
    architecture "x86_64"
    startproject "sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "dependencies"
	include "spectre/vendor/glad"
	include "spectre/vendor/glfw"
group ""

include "spectre"
include "sandbox"
