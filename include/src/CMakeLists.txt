file(GLOB_RECURSE src_f CONFIGURE_DEPENDS
    "*.hpp"
    "*.h"
    "*.cpp"
    "*.c"
)

target_sources(texgui PRIVATE
    ${src_f}
)

target_sources(example1 PRIVATE
    ${src_f}
)
