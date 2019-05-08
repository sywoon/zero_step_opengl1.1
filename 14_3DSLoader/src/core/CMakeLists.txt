
project (Core)

include_directories(
  ../external/opengl1.1/include
  .
  base
  framework
  include
  texture
)

include(base/CMakeLists.txt)
include(framework/CMakeLists.txt)
include(texture/CMakeLists.txt)


set(CORE_SRC Core.cpp
    ${CORE_BASE_SRC}
    ${CORE_FRAMEWORK_SRC}
    ${CORE_TEXTURE_SRC}
    )

add_library(Core ${CORE_SRC})




