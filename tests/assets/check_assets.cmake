file(READ "${PROJECT_ROOT}/docs/assets/asset-sources.md" manifest)
file(GLOB_RECURSE assets RELATIVE "${PROJECT_ROOT}"
     "${PROJECT_ROOT}/assets/*.jpg"
     "${PROJECT_ROOT}/assets/*.mp3"
     "${PROJECT_ROOT}/assets/*.png"
     "${PROJECT_ROOT}/assets/*.wav")
foreach(asset IN LISTS assets)
  string(FIND "${manifest}" "${asset}" found)
  if(found EQUAL -1)
    message(FATAL_ERROR "Asset is missing provenance row: ${asset}")
  endif()
endforeach()
if(NOT assets)
  message(FATAL_ERROR "No traced visual assets found")
endif()
message(STATUS "Asset provenance passed for ${assets}")
