project "assimp"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    warnings "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "assimp/code/CApi/**",
        "assimp/code/AssetLib/FBX/**",
        "assimp/code/AssetLib/Obj/**",
        "assimp/code/Common/**",
        "assimp/code/Material/**",
        "assimp/code/PostProcessing/**",
        "assimp/contrib/zlib/**.c"
    }

    removefiles
    {
        "assimp/code/AssetLib/FBX/FBXExporter.h",
        "assimp/code/AssetLib/FBX/FBXExporter.cpp",
        "assimp/code/AssetLib/Obj/ObjExporter.h",
        "assimp/code/AssetLib/Obj/ObjExporter.cpp",
        "assimp/contrib/zlib/contrib/inflate86/inffas86.c"
    }

    includedirs
    {
        "assimp",
        "assimp/include",
        "assimp/include/assimp",
        "assimp/code",
        "assimp/contrib/pugixml/src",
        "assimp/contrib/unzip",
        "assimp/contrib/zlib",
    }

    defines
    {
        "ASSIMP_BUILD_NO_X_IMPORTER",
        "ASSIMP_BUILD_NO_AMF_IMPORTER",
        "ASSIMP_BUILD_NO_3DS_IMPORTER",
        "ASSIMP_BUILD_NO_MD3_IMPORTER",
        "ASSIMP_BUILD_NO_MDL_IMPORTER",
        "ASSIMP_BUILD_NO_MD2_IMPORTER",
        "ASSIMP_BUILD_NO_PLY_IMPORTER",
        "ASSIMP_BUILD_NO_ASE_IMPORTER",
        -- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
        "ASSIMP_BUILD_NO_HMP_IMPORTER",
        "ASSIMP_BUILD_NO_SMD_IMPORTER",
        "ASSIMP_BUILD_NO_MDC_IMPORTER",
        "ASSIMP_BUILD_NO_MD5_IMPORTER",
        "ASSIMP_BUILD_NO_STL_IMPORTER",
        "ASSIMP_BUILD_NO_LWO_IMPORTER",
        "ASSIMP_BUILD_NO_DXF_IMPORTER",
        "ASSIMP_BUILD_NO_NFF_IMPORTER",
        "ASSIMP_BUILD_NO_RAW_IMPORTER",
        "ASSIMP_BUILD_NO_SIB_IMPORTER",
        "ASSIMP_BUILD_NO_OFF_IMPORTER",
        "ASSIMP_BUILD_NO_AC_IMPORTER",
        "ASSIMP_BUILD_NO_BVH_IMPORTER",
        "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
        "ASSIMP_BUILD_NO_IRR_IMPORTER",
        "ASSIMP_BUILD_NO_Q3D_IMPORTER",
        "ASSIMP_BUILD_NO_B3D_IMPORTER",
        "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
        "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
        "ASSIMP_BUILD_NO_CSM_IMPORTER",
        "ASSIMP_BUILD_NO_3D_IMPORTER",
        "ASSIMP_BUILD_NO_LWS_IMPORTER",
        "ASSIMP_BUILD_NO_OGRE_IMPORTER",
        "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
        "ASSIMP_BUILD_NO_MS3D_IMPORTER",
        "ASSIMP_BUILD_NO_COB_IMPORTER",
        "ASSIMP_BUILD_NO_BLEND_IMPORTER",
        "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
        "ASSIMP_BUILD_NO_NDO_IMPORTER",
        "ASSIMP_BUILD_NO_IFC_IMPORTER",
        "ASSIMP_BUILD_NO_XGL_IMPORTER",
        "ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
        "ASSIMP_BUILD_NO_GLTF_IMPORTER",
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
        "ASSIMP_BUILD_NO_3MF_IMPORTER",
        "ASSIMP_BUILD_NO_X3D_IMPORTER",
        "ASSIMP_BUILD_NO_MMD_IMPORTER",
        "ASSIMP_BUILD_NO_M3D_IMPORTER"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
    
    filter "system:windows"
        systemversion "latest"
